/* tilemap88m72.c

	When the videoram for a tile changes, call tilemap_mark_tile_dirty
	with the appropriate memory offset.

	In the video driver, follow these steps:

	1)	Set each tilemap's scroll registers.

	2)	Call tilemap_draw to draw the tilemaps to the screen, from back to front.

	Notes:
	-	You can currently configure a tilemap as xscroll + scrolling columns or
		yscroll + scrolling rows, but not both types of scrolling simultaneously.
*/

#if !defined(DECLARE) && !defined(TRANSP)



#include "driver.h"
//#include "osinline.h"
#include "tile_m72.h"
#include "state.h"

//#define MAX_TILESIZE 8

#define TILE_FLAG_DIRTY (0x80)

typedef enum { eWHOLLY_TRANSPARENT, eWHOLLY_OPAQUE, eMASKED } trans_t;

//typedef void (*tilemap_draw_func)( struct tilemap *tilemap, int xpos, int ypos, int mask, int value );

struct tilemap
{
	struct mame_bitmap *pixmap;	/* cached color data */
	struct mame_bitmap *transparency_bitmap;	/* cached color data */

	/* callback to interpret video RAM for the tilemap */
	void (*tile_get_info)( int memory_offset );

	int *memory_offset_to_cached_indx;
	UINT32 *cached_indx_to_memory_offset;
//	UINT32 (*get_memory_offset)( UINT32 col, UINT32 row, UINT32 num_cols, UINT32 num_rows );


	//void *user_data;

	UINT32 max_memory_offset;


	UINT32 pixmap_pitch_line;	/* cached color data */
	UINT32 pixmap_pitch_row;	/* cached color data */


	UINT32 transparency_bitmap_pitch_line;	/* cached color data */
	UINT32 transparency_bitmap_pitch_row;	/* cached color data */
	UINT8 *transparency_data, **transparency_data_row;	/* cached color data */


//	int dx, dx_if_flipped;
//	int dy, dy_if_flipped;
//	int scrollx_delta, scrolly_delta;

	int enable;
	int attributes;

//	int type;	/*TILEMAP_SPLIT*/
//	int transparent_pen;
	UINT32 fgmask[4], bgmask[4]; /* for TILEMAP_SPLIT */

//	UINT32 *pPenToPixel[4];

//	UINT8 (*draw_tile)( struct tilemap *tilemap, UINT16 col, UINT16 row, UINT8 flags );

//	int cached_scroll_rows, cached_scroll_cols;
	int *cached_rowscroll, *cached_colscroll;

//	int logical_scroll_rows, logical_scroll_cols;
	int logical_rowscroll, logical_colscroll;

	int orientation;
	int palette_offset;

//	UINT16 tile_depth;
//	UINT16 tile_granularity;
	UINT8 *tile_dirty_map;
	UINT8 all_tiles_dirty;
	UINT8 all_tiles_clean;

//	UINT32 num_tiles;
//	UINT32 num_pens;

//	UINT32 num_logical_rows, num_logical_cols;
//	UINT32 num_cached_rows, num_cached_cols;

//	int logical_flip_to_cached_flip[4];

//	UINT32 logical_tile_width, logical_tile_height;
//	UINT32 cached_tile_width, cached_tile_height;

//	UINT32 cached_width, cached_height;


	struct tilemap *next; /* resource tracking */
};

struct mame_bitmap *	priority_bitmap;
UINT32					priority_bitmap_pitch_line;
UINT32					priority_bitmap_pitch_row;

static struct tilemap * first_tilemap; /* resource tracking */
static UINT32			screen_width, screen_height;
struct tile_info		tile_info;

//static UINT32 g_mask32[32];

typedef void (*blitmask_t)( void *dest, const void *source, const UINT8 *pMask, int mask, int value, int count, UINT8 *pri, UINT32 pcode );
typedef void (*blitopaque_t)( void *dest, const void *source, int count, UINT8 *pri, UINT32 pcode );

/* the following parameters are constant across tilemap_draw calls */
static struct
{
	blitmask_t draw_masked;
	blitopaque_t draw_opaque;
	int clip_left, clip_top, clip_right, clip_bottom;
	UINT32					tilemap_priority_code;
	struct	mame_bitmap *	screen_bitmap;
	UINT32					screen_bitmap_pitch_line;
	UINT32					screen_bitmap_pitch_row;
} blit;

/***********************************************************************************/

static int mappings_create( struct tilemap *tilemap );
static void mappings_dispose( struct tilemap *tilemap );
static void mappings_update( struct tilemap *tilemap );
static void recalculate_scroll( struct tilemap *tilemap );

static void tilemap_reset(void);

static void update_tile_info( struct tilemap *tilemap, UINT32 cached_indx, UINT16 cached_col, UINT16 cached_row );

/***********************************************************************************/

//static void InitMask32(void)
//{
//	int i;
//	for (i=0;i<16;i++)
//	{
//		UINT32 p1 = (i&1) ? 0xFFFF : 0;
//		UINT32 p2 = (i&2) ? 0xFFFF : 0;
//		UINT32 p3 = (i&4) ? 0xFFFF : 0;
//		UINT32 p4 = (i&8) ? 0xFFFF : 0;
//		g_mask32[i*2  ] = (p2 << 16) | p1;
//		g_mask32[i*2+1] = (p4 << 16) | p3;
//	}
//}


void tilemap_set_transparent_pen( struct tilemap *tilemap, int pen )
{
//	tilemap->transparent_pen = pen;
}

void tilemap_set_transmask( struct tilemap *tilemap, int which, UINT32 fgmask, UINT32 bgmask )
{
	if( tilemap->fgmask[which] != fgmask || tilemap->bgmask[which] != bgmask )
	{
		tilemap->fgmask[which] = fgmask;
		tilemap->bgmask[which] = bgmask;
		tilemap_mark_all_tiles_dirty( tilemap );
	}
}

//void tilemap_set_depth( struct tilemap *tilemap, int tile_depth, int tile_granularity )
//{
//	if( tilemap->tile_dirty_map )
//	{
//		free( tilemap->tile_dirty_map);
//	}
//	tilemap->tile_dirty_map = malloc( Machine->drv->total_colors >> tile_granularity );
//	if( tilemap->tile_dirty_map )
//	{
//		tilemap->tile_depth = tile_depth;
//		tilemap->tile_granularity = tile_granularity;
//	}
//}

/***********************************************************************************/
/* some common mappings */

//UINT32 tilemap_scan_rows( UINT32 col, UINT32 row, UINT32 num_cols, UINT32 num_rows )/*m72*/
//{
//	/* logical (col,row) -> memory offset */
//	return row*num_cols + col;
//}

//UINT32 tilemap_scan_rows_flip_x( UINT32 col, UINT32 row, UINT32 num_cols, UINT32 num_rows )
//{
//	/* logical (col,row) -> memory offset */
//	return row*num_cols + (num_cols-col-1);
//}
//UINT32 tilemap_scan_rows_flip_y( UINT32 col, UINT32 row, UINT32 num_cols, UINT32 num_rows )
//{
//	/* logical (col,row) -> memory offset */
//	return (num_rows-row-1)*num_cols + col;
//}
//UINT32 tilemap_scan_rows_flip_xy( UINT32 col, UINT32 row, UINT32 num_cols, UINT32 num_rows )
//{
//	/* logical (col,row) -> memory offset */
//	return (num_rows-row-1)*num_cols + (num_cols-col-1);
//}
//UINT32 tilemap_scan_cols( UINT32 col, UINT32 row, UINT32 num_cols, UINT32 num_rows )
//{
//	/* logical (col,row) -> memory offset */
//	return col*num_rows + row;
//}
//UINT32 tilemap_scan_cols_flip_x( UINT32 col, UINT32 row, UINT32 num_cols, UINT32 num_rows )
//{
//	/* logical (col,row) -> memory offset */
//	return (num_cols-col-1)*num_rows + row;
//}
//UINT32 tilemap_scan_cols_flip_y( UINT32 col, UINT32 row, UINT32 num_cols, UINT32 num_rows )
//{
//	/* logical (col,row) -> memory offset */
//	return col*num_rows + (num_rows-row-1);
//}
//UINT32 tilemap_scan_cols_flip_xy( UINT32 col, UINT32 row, UINT32 num_cols, UINT32 num_rows )
//{
//	/* logical (col,row) -> memory offset */
//	return (num_cols-col-1)*num_rows + (num_rows-row-1);
//}
/***********************************************************************************/

static int mappings_create( struct tilemap *tilemap )
{
	int max_memory_offset = 0;
	UINT32 col,row;
//	UINT32 num_logical_rows = tilemap->num_logical_rows;
//	UINT32 num_logical_cols = tilemap->num_logical_cols;
	/* count offsets (might be larger than num_tiles) */
	for( row=0; row<64/*num_logical_rows*/; row++ )
	{
		for( col=0; col<64/*num_logical_cols*/; col++ )
		{
		//	UINT32 memory_offset = /*tilemap->get_memory_offset*/tilemap_scan_rows( col, row, 64,64/*num_logical_cols, num_logical_rows*/ );
			UINT32 memory_offset = (row*64 + col);
			if( memory_offset>max_memory_offset ) max_memory_offset = memory_offset;
		}
	}
	max_memory_offset++;
	tilemap->max_memory_offset = max_memory_offset;
	/* logical to cached (tilemap_mark_dirty) */
	tilemap->memory_offset_to_cached_indx = malloc( sizeof(int)*max_memory_offset );
	if( tilemap->memory_offset_to_cached_indx )
	{
		/* cached to logical (get_tile_info) */
		tilemap->cached_indx_to_memory_offset = malloc( sizeof(UINT32)*(64*64)/*tilemap->num_tiles*/ );
		if( tilemap->cached_indx_to_memory_offset ) return 0; /* no error */
		free( tilemap->memory_offset_to_cached_indx );
	}
	return -1; /* error */
}

static void mappings_dispose( struct tilemap *tilemap )
{
	free( tilemap->cached_indx_to_memory_offset );
	free( tilemap->memory_offset_to_cached_indx );
}

static void mappings_update( struct tilemap *tilemap )
{
//	int logical_flip;
	UINT32 logical_indx, cached_indx;
//	UINT32 num_cached_rows = tilemap->num_cached_rows;
//	UINT32 num_cached_cols = tilemap->num_cached_cols;
//	UINT32 num_logical_rows = tilemap->num_logical_rows;
//	UINT32 num_logical_cols = tilemap->num_logical_cols;
	for( logical_indx=0; logical_indx<tilemap->max_memory_offset; logical_indx++ )
	{
		tilemap->memory_offset_to_cached_indx[logical_indx] = -1;
	}

	for( logical_indx=0; logical_indx<(64*64)/*tilemap->num_tiles*/; logical_indx++ )
	{
		UINT32 logical_col = logical_indx%64/*num_logical_cols*/;
		UINT32 logical_row = logical_indx/64/*num_logical_cols*/;
//		UINT32 logical_col = logical_indx&63/*num_logical_cols*/;
//		UINT32 logical_row = logical_indx/64/*num_logical_cols*/;
	//	int memory_offset = /*tilemap->get_memory_offset*/tilemap_scan_rows( logical_col, logical_row, 64,64/*num_logical_cols, num_logical_rows*/ );
		UINT32 memory_offset = (logical_row*64 + logical_col);
		UINT32 cached_col = logical_col;
		UINT32 cached_row = logical_row;
		if( tilemap->orientation & ORIENTATION_FLIP_X ) cached_col = (/*num_cached_cols*/64-1)-cached_col;
		if( tilemap->orientation & ORIENTATION_FLIP_Y ) cached_row = (/*num_cached_rows*/64-1)-cached_row;
		cached_indx = cached_row*64/*num_cached_cols*/+cached_col;
		tilemap->memory_offset_to_cached_indx[memory_offset] = cached_indx;
		tilemap->cached_indx_to_memory_offset[cached_indx] = memory_offset;
	}
//	for( logical_flip = 0; logical_flip<4; logical_flip++ )
//	{
//		int cached_flip = logical_flip;
//		if( tilemap->attributes&TILEMAP_FLIPX ) cached_flip ^= TILE_FLIPX;
//		if( tilemap->attributes&TILEMAP_FLIPY ) cached_flip ^= TILE_FLIPY;
//		tilemap->logical_flip_to_cached_flip[logical_flip] = cached_flip;
//	}
}

/***********************************************************************************/

static void pio( void *dest, const void *source, int count, UINT8 *pri, UINT32 pcode )
{
	int i;

	if (pcode)
		for( i=0; i<count; i++ )
		{
			pri[i] = (pri[i] & (pcode >> 8)) | pcode;
		}
}

static void pit( void *dest, const void *source, const UINT8 *pMask, int mask, int value, int count, UINT8 *pri, UINT32 pcode )
{
	int i;

	if (pcode)
		for( i=0; i<count; i++ )
		{
			if( (pMask[i]&mask)==value )
			{
				pri[i] = (pri[i] & (pcode >> 8)) | pcode;
			}
		}
}

/***********************************************************************************/

//#ifndef pdo16
//static void pdo16( UINT16 *dest, const UINT16 *source, int count, UINT8 *pri, UINT32 pcode )
//{
//	int i;
//	memcpy( dest,source,count*sizeof(UINT16) );
//	for( i=0; i<count; i++ )
//	{
//		pri[i] = (pri[i] & (pcode >> 8)) | pcode;
//	}
//}
//#endif

#ifndef pdo16pal
static void pdo16pal( UINT16 *dest, const UINT16 *source, int count, UINT8 *pri, UINT32 pcode )
{
	int pal = pcode >> 16;
	int i;
	for( i=0; i<count; i++ )
	{
		dest[i] = source[i] + pal;
		pri[i] = (pri[i] & (pcode >> 8)) | pcode;
	}
}
#endif

#ifndef pdo16np
static void pdo16np( UINT16 *dest, const UINT16 *source, int count, UINT8 *pri, UINT32 pcode )
{
	memcpy( dest,source,count*sizeof(UINT16) );
}
#endif



/***********************************************************************************/

//#ifndef pdt16
//static void pdt16( UINT16 *dest, const UINT16 *source, const UINT8 *pMask, int mask, int value, int count, UINT8 *pri, UINT32 pcode )
//{
//	int i;
//	for( i=0; i<count; i++ )
//	{
//		if( (pMask[i]&mask)==value )
//		{
//			dest[i] = source[i];
//			pri[i] = (pri[i] & (pcode >> 8)) | pcode;
//		}
//	}
//}
//#endif

//#ifndef pdt16pal
//static void pdt16pal( UINT16 *dest, const UINT16 *source, const UINT8 *pMask, int mask, int value, int count, UINT8 *pri, UINT32 pcode )
//{
//	int pal = pcode >> 16;
//	int i;
//	for( i=0; i<count; i++ )
//	{
//		if( (pMask[i]&mask)==value )
//		{
//			dest[i] = source[i] + pal;
//			pri[i] = (pri[i] & (pcode >> 8)) | pcode;
//		}
//	}
//}
//#endif

#ifndef pdt16np
static void pdt16np( UINT16 *dest, const UINT16 *source, const UINT8 *pMask, int mask, int value, int count, UINT8 *pri, UINT32 pcode )
{
	int i;

	for( i=0; i<count; i++ )
	{
		if( (pMask[i]&mask)==value )
			dest[i] = source[i];
	}
}
#endif


/***********************************************************************************/

//#define DEPTH 16
//#define DATA_TYPE UINT16
static void draw16BPP(struct tilemap *tilemap, int xpos, int ypos, int mask, int value )
{
	trans_t transPrev;
	trans_t transCur;
	const UINT8 *pTrans;
	UINT32 cached_indx;
	struct mame_bitmap *screen = blit.screen_bitmap;
	int tilemap_priority_code = blit.tilemap_priority_code;
	int x1 = xpos;
	int y1 = ypos;
	int x2 = xpos+8*64;//tilemap->cached_width;
	int y2 = ypos+8*64;//tilemap->cached_height;
	/*DATA_TYPE*/UINT16 *dest_baseaddr = NULL;
	/*DATA_TYPE*/UINT16 *dest_next;
	int dy;
	int count;
	const UINT16 *source0;
	/*DATA_TYPE*/UINT16 *dest0;
	UINT8 *pmap0;
	int i;

	int c1; /* leftmost visible column in source tilemap */
	int c2; /* rightmost visible column in source tilemap */
	int y; /* current screen line to render */
	int y_next;
	UINT8 *priority_bitmap_baseaddr;
	UINT8 *priority_bitmap_next;
	const UINT16 *source_baseaddr;
	const UINT16 *source_next;
	const UINT8 *mask0;
	const UINT8 *mask_baseaddr;
	const UINT8 *mask_next;

	/* clip source coordinates */
	if( x1<blit.clip_left   ) x1 = blit.clip_left;
	if( x2>blit.clip_right  ) x2 = blit.clip_right;
	if( y1<blit.clip_top    ) y1 = blit.clip_top;
	if( y2>blit.clip_bottom ) y2 = blit.clip_bottom;

	if( x1<x2 && y1<y2 ) /* do nothing if totally clipped */
	{
		priority_bitmap_baseaddr = xpos + (UINT8 *)priority_bitmap->line[y1];
		if( screen )
		{
			dest_baseaddr = xpos + (/*DATA_TYPE*/UINT16 *)screen->line[y1];
		}

		/* convert screen coordinates to source tilemap coordinates */
		x1 -= xpos;
		y1 -= ypos;
		x2 -= xpos;
		y2 -= ypos;

		source_baseaddr = (UINT16 *)tilemap->pixmap->line[y1];
		mask_baseaddr = tilemap->transparency_bitmap->line[y1];

		c1 = x1/ /*w*/8; /* round down */
		c2 = (x2+/*w*/8-1)/ /*w*/8; /* round up */

		y = y1;
		y_next = /*h*/8*(y1/ /*h*/8) + /*h*/8;
		if( y_next>y2 ) y_next = y2;

		dy = y_next-y;
		dest_next = dest_baseaddr + dy*blit.screen_bitmap_pitch_line;
		priority_bitmap_next = priority_bitmap_baseaddr + dy*priority_bitmap_pitch_line;
		source_next = source_baseaddr + dy*tilemap->pixmap_pitch_line;
		mask_next = mask_baseaddr + dy*tilemap->transparency_bitmap_pitch_line;
		for(;;)
		{
		UINT16 column;
		UINT16 row;
		UINT16 x_start;
		UINT16 x_end;
			row = y/ /*h*/8;
			x_start = x1;

			transPrev = eWHOLLY_TRANSPARENT;
			pTrans = mask_baseaddr + x_start;

			cached_indx = row*64/*tilemap->num_cached_cols*/ + c1;
			for( column=c1; column<=c2; column++ )
			{
				if( column == c2 )
				{
					transCur = eWHOLLY_TRANSPARENT;
					goto L_Skip;
				}

				if( tilemap->transparency_data[cached_indx]==TILE_FLAG_DIRTY )
				{
					update_tile_info( tilemap, cached_indx, column, row );
				}

				if( (tilemap->transparency_data[cached_indx]&mask)!=0 )
				{
					transCur = eMASKED;
				}
				else
				{
					transCur = (((*pTrans)&mask) == value)?eWHOLLY_OPAQUE:eWHOLLY_TRANSPARENT;
				}
				pTrans += /*w*/8;

			L_Skip:
				if( transCur!=transPrev )
				{
					x_end = column*/*w*/8;
					if( x_end<x1 ) x_end = x1;
					if( x_end>x2 ) x_end = x2;

					if( transPrev != eWHOLLY_TRANSPARENT )
					{
						count = x_end - x_start;
						source0 = source_baseaddr + x_start;
						dest0 = dest_baseaddr + x_start;
						pmap0 = priority_bitmap_baseaddr + x_start;

						if( transPrev == eWHOLLY_OPAQUE )
						{
							i = y;
							for(;;)
							{
								blit.draw_opaque( dest0, source0, count, pmap0, tilemap_priority_code );
								if( ++i == y_next ) break;

								dest0 += blit.screen_bitmap_pitch_line;
								source0 += tilemap->pixmap_pitch_line;
								pmap0 += priority_bitmap_pitch_line;
							}
						} /* transPrev == eWHOLLY_OPAQUE */
						else /* transPrev == eMASKED */
						{
							mask0 = mask_baseaddr + x_start;
							i = y;
							for(;;)
							{
								blit.draw_masked( dest0, source0, mask0, mask, value, count, pmap0, tilemap_priority_code );
								if( ++i == y_next ) break;

								dest0 += blit.screen_bitmap_pitch_line;
								source0 += tilemap->pixmap_pitch_line;
								mask0 += tilemap->transparency_bitmap_pitch_line;
								pmap0 += priority_bitmap_pitch_line;
							}
						} /* transPrev == eMASKED */
					} /* transPrev != eWHOLLY_TRANSPARENT */
					x_start = x_end;
					transPrev = transCur;
				}
				cached_indx++;
			}
			if( y_next==y2 ) break; /* we are done! */

			priority_bitmap_baseaddr = priority_bitmap_next;
			dest_baseaddr = dest_next;
			source_baseaddr = source_next;
			mask_baseaddr = mask_next;
			y = y_next;
			y_next += /*h*/8;

			if( y_next>=y2 )
			{
				y_next = y2;
			}
			else
			{
				dest_next += blit.screen_bitmap_pitch_row;
				priority_bitmap_next += priority_bitmap_pitch_row;
				source_next += tilemap->pixmap_pitch_row;
				mask_next += tilemap->transparency_bitmap_pitch_row;
			}
		} /* process next row */
	} /* not totally clipped */

	//osd_pend();
}

//#define PAL_INIT const pen_t *pPalData = tile_info.pal_data
//#define PAL_GET(pen) pPalData[pen]
//#define TRANSP(f) f ## _ind

#define PAL_INIT int palBase = tile_info.pal_data - Machine->remapped_colortable
#define PAL_GET(pen) (palBase + (pen))

/*************************************************************************************************/

/* Each of the following routines draws pixmap and transarency data for a single tile.
 *
 * This function returns a per-tile code.  Each bit of this code is 0 if the corresponding
 * bit is zero in every byte of transparency data in the tile, or 1 if that bit is not
 * consistant within the tile.
 *
 * This precomputed value allows us for any particular tile and mask, to determine if all pixels
 * in that tile have the same masked transparency value.
 */
/*
static UINT8 TRANSP(HandleTransparencyBitmask)(	struct tilemap *tilemap, UINT32 x0, UINT32 y0, UINT32 flags){	return 0;}
static UINT8 TRANSP(HandleTransparencyColor)(	struct tilemap *tilemap, UINT32 x0, UINT32 y0, UINT32 flags){	return 0;}
static UINT8 TRANSP(HandleTransparencyPen)(		struct tilemap *tilemap, UINT32 x0, UINT32 y0, UINT32 flags){	return 0;}
static UINT8 TRANSP(HandleTransparencyPenBit)(	struct tilemap *tilemap, UINT32 x0, UINT32 y0, UINT32 flags){	return 0;}
static UINT8 TRANSP(HandleTransparencyNone)(	struct tilemap *tilemap, UINT32 x0, UINT32 y0, UINT32 flags){	return 0;}
*/

/*m72==HandleTransparencyPens*/
static UINT8 HandleTransparencyPens_raw(struct tilemap *tilemap, UINT16 x0, UINT16 y0, UINT8 flags)
{
	struct mame_bitmap *pixmap = tilemap->pixmap;
	struct mame_bitmap *transparency_bitmap = tilemap->transparency_bitmap;
/*	int pitch = w 8 +0 tile_info.skip;*/
	PAL_INIT;
	const UINT8 *pPenData = tile_info.pen_data;
	const UINT8 *pSource;
	UINT32 code_transparent = tile_info.priority;
	UINT8 tx;
	UINT8 ty;
	UINT16 x;
	UINT16 y;

	UINT32 pen;
	UINT32 fgmask = tilemap->fgmask[(flags>>TILE_SPLIT_OFFSET)&3];
	UINT32 bgmask = tilemap->bgmask[(flags>>TILE_SPLIT_OFFSET)&3];
	UINT32 code;
	int and_flags = ~0;
	int or_flags = 0;

	x0<<=3;
	y0<<=3;
/*	if( flags&TILE_4BPP )	{;	}	else*/
	{
		switch(flags&(TILE_FLIPY|TILE_FLIPX)){
		case 0:
			for( ty= 0; ty</*h*/8; ty++ )
			{
				pSource = pPenData;
				for( tx= 0; tx</*w*/8; tx++ )
				{
					pen = *pSource++;

					x = x0+((tx));
					y = y0+((ty));
					*(x+(UINT16 *)pixmap->line[y]) = PAL_GET(pen);
					code = code_transparent;
					if( !((1<<pen)&fgmask) ) code |= TILE_FLAG_FG_OPAQUE;
					if( !((1<<pen)&bgmask) ) code |= TILE_FLAG_BG_OPAQUE;
					and_flags &= code;
					or_flags |= code;
					((UINT8 *)transparency_bitmap->line[y])[x] = code;
				}
				pPenData += 8/*pitch*/;
			}
		break;
		case (TILE_FLIPX):
			for( ty= 0; ty</*h*/8; ty++ )
			{
				pSource = pPenData;
				for( tx= 0; tx</*w*/8; tx++ )
				{
					pen = *pSource++;

					x = x0+((7-tx));
					y = y0+((ty));
					*(x+(UINT16 *)pixmap->line[y]) = PAL_GET(pen);
					code = code_transparent;
					if( !((1<<pen)&fgmask) ) code |= TILE_FLAG_FG_OPAQUE;
					if( !((1<<pen)&bgmask) ) code |= TILE_FLAG_BG_OPAQUE;
					and_flags &= code;
					or_flags |= code;
					((UINT8 *)transparency_bitmap->line[y])[x] = code;
				}
				pPenData += 8/*pitch*/;
			}
		break;
		case (TILE_FLIPY):
			for( ty= 0; ty</*h*/8; ty++ )
			{
				pSource = pPenData;
				for( tx= 0; tx</*w*/8; tx++ )
				{
					pen = *pSource++;

					x = x0+((tx));
					y = y0+((7-ty));
					*(x+(UINT16 *)pixmap->line[y]) = PAL_GET(pen);
					code = code_transparent;
					if( !((1<<pen)&fgmask) ) code |= TILE_FLAG_FG_OPAQUE;
					if( !((1<<pen)&bgmask) ) code |= TILE_FLAG_BG_OPAQUE;
					and_flags &= code;
					or_flags |= code;
					((UINT8 *)transparency_bitmap->line[y])[x] = code;
				}
				pPenData += 8/*pitch*/;
			}
		break;
		case (TILE_FLIPY|TILE_FLIPX):
			for( ty= 0; ty</*h*/8; ty++ )
			{
				pSource = pPenData;
				for( tx= 0; tx</*w*/8; tx++ )
				{
					pen = *pSource++;

					x = x0+((7-tx));
					y = y0+((7-ty));
					*(x+(UINT16 *)pixmap->line[y]) = PAL_GET(pen);
					code = code_transparent;
					if( !((1<<pen)&fgmask) ) code |= TILE_FLAG_FG_OPAQUE;
					if( !((1<<pen)&bgmask) ) code |= TILE_FLAG_BG_OPAQUE;
					and_flags &= code;
					or_flags |= code;
					((UINT8 *)transparency_bitmap->line[y])[x] = code;
				}
				pPenData += 8/*pitch*/;
			}
		break;
		}
	}
	return and_flags ^ or_flags;
}
#undef PAL_INIT
#undef PAL_GET


/*********************************************************************************/

/***********************************************************************************/

static void tilemap_reset(void)
{
	tilemap_mark_all_tiles_dirty(ALL_TILEMAPS);
}

int tilemap_init( void )
{
	screen_width	= Machine->scrbitmap->width;
	screen_height	= Machine->scrbitmap->height;
	first_tilemap	= NULL;

	state_save_register_func_postload(tilemap_reset);
	priority_bitmap = bitmap_alloc_depth( screen_width, screen_height, -8 );
	if( priority_bitmap )
	{
		priority_bitmap_pitch_line = ((UINT8 *)priority_bitmap->line[1]) - ((UINT8 *)priority_bitmap->line[0]);
		return 0;
	}
	//InitMask32();
	return -1;
}

void tilemap_close( void )
{
	struct tilemap *next;

	while( first_tilemap )
	{
		next = first_tilemap->next;
		tilemap_dispose( first_tilemap );
		first_tilemap = next;
	}
	bitmap_free( priority_bitmap );
}

/***********************************************************************************/

static struct tilemap *tilemap_create(
	void (*tile_get_info)( int memory_offset )
//,
//	UINT32 (*dummy_get_memory_offset)( UINT32 col, UINT32 row, UINT32 num_cols, UINT32 num_rows ),/*tilemap_scan_rows*/
//	int type,/*TILEMAP_SPLIT*/
//	int tile_width, int tile_height,/*8,8*/
//	int num_cols, int num_rows /*64,64*/
)
{
	struct tilemap *tilemap;
	UINT32 row;
//	int num_tiles;

	tilemap = calloc( 1,sizeof( struct tilemap ) );
	if( tilemap )
	{
//		num_tiles = 64*64;//num_cols*num_rows;
//		tilemap->num_logical_cols = 64;//num_cols;
//		tilemap->num_logical_rows = 64;//num_rows;
	//	/*w*/8 = tile_width;
	//	tilemap->logical_tile_height = tile_height;
		//tilemap->logical_colscroll = calloc(/*num_cols*tile_width*/ 64*8,sizeof(int));
		//tilemap->logical_rowscroll = calloc(/*num_rows*tile_height*/64*8,sizeof(int));
//		tilemap->num_cached_cols = 64;//num_cols;
//		tilemap->num_cached_rows = 64;//num_rows;
	//	tilemap->num_tiles = num_tiles;
	//	tilemap->num_pens = (8*8);//tile_width*tile_height;
	//	/*w*/8 = tile_width;
	//	/*h*/8 = tile_height;
	//	tilemap->cached_width  = 8*64;//tile_width*num_cols;
	//	tilemap->cached_height = 8*64;//tile_height*num_rows;
		tilemap->tile_get_info = tile_get_info;
//		tilemap->get_memory_offset = get_memory_offset;
		tilemap->orientation = ROT0;

		/* various defaults */
		tilemap->enable = 1;
	//	tilemap->type = type;	/*TILEMAP_SPLIT*/
//		tilemap->logical_scroll_rows = tilemap->cached_scroll_rows = 1;
//		tilemap->logical_scroll_cols = tilemap->cached_scroll_cols = 1;
	//	tilemap->transparent_pen = -1;
//		tilemap->tile_depth = 0;
//		tilemap->tile_granularity = 0;
		tilemap->tile_dirty_map = 0;

		tilemap->cached_rowscroll	= calloc(/*tilemap->cached_height*/8*64, sizeof(int));
		tilemap->cached_colscroll	= calloc(/*tilemap->cached_width*/ 8*64, sizeof(int));

		tilemap->transparency_data = malloc( (64*64)/*num_tiles*/ );
		tilemap->transparency_data_row = malloc( sizeof(UINT8 *)*64/*num_rows*/ );

		tilemap->pixmap = bitmap_alloc_depth( /*tilemap->cached_width*/8*64, 8*64/*tilemap->cached_height*/, -16 );
		tilemap->transparency_bitmap = bitmap_alloc_depth( /*tilemap->cached_width*/8*64, 8*64/*tilemap->cached_height*/, -8 );

		if( /*tilemap->logical_rowscroll &&*/ tilemap->cached_rowscroll &&
			/*tilemap->logical_colscroll &&*/ tilemap->cached_colscroll &&
			tilemap->pixmap &&
			tilemap->transparency_data &&
			tilemap->transparency_data_row &&
			tilemap->transparency_bitmap &&
			(mappings_create( tilemap )==0) )
		{
			tilemap->pixmap_pitch_line = tilemap->pixmap->rowpixels;
			tilemap->pixmap_pitch_row = tilemap->pixmap_pitch_line*8/*tile_height*/;

			tilemap->transparency_bitmap_pitch_line = tilemap->transparency_bitmap->rowpixels;
			tilemap->transparency_bitmap_pitch_row = tilemap->transparency_bitmap_pitch_line*8/*tile_height*/;

			for( row=0; row<64/*num_rows*/; row++ )
			{
				tilemap->transparency_data_row[row] = tilemap->transparency_data+/*num_cols*/64*row;
			}
			mappings_update( tilemap );
			memset( tilemap->transparency_data, TILE_FLAG_DIRTY, (64*64)/*num_tiles*/ );
			tilemap->next = first_tilemap;
			first_tilemap = tilemap;
			recalculate_scroll(tilemap);
			return tilemap;
		}
		tilemap_dispose( tilemap );
	}
	return 0;
}

void tilemap_dispose( struct tilemap *tilemap )
{
	struct tilemap *prev;

	if( tilemap==first_tilemap )
	{
		first_tilemap = tilemap->next;
	}
	else
	{
		prev = first_tilemap;
		while( prev && prev->next != tilemap ) prev = prev->next;
		if( prev ) prev->next =tilemap->next;
	}
//	free( tilemap->logical_rowscroll );
	free( tilemap->cached_rowscroll );
//	free( tilemap->logical_colscroll );
	free( tilemap->cached_colscroll );
	free( tilemap->transparency_data );
	free( tilemap->transparency_data_row );
	bitmap_free( tilemap->transparency_bitmap );
	bitmap_free( tilemap->pixmap );
	mappings_dispose( tilemap );
	free( tilemap );
}

/***********************************************************************************/

void tilemap_set_enable( struct tilemap *tilemap, int enable )
{
	tilemap->enable = enable?1:0;
}


void tilemap_set_flip( struct tilemap *tilemap, int attributes )
{
#if 00
	if( tilemap==ALL_TILEMAPS )
	{
		tilemap = first_tilemap;
		while( tilemap )
		{
			tilemap_set_flip( tilemap, attributes );
			tilemap = tilemap->next;
		}
	}
	else if( tilemap->attributes!=attributes )
	{
		tilemap->attributes = attributes;
		tilemap->orientation = ROT0;
		if( attributes&TILEMAP_FLIPY )
		{
			tilemap->orientation ^= ORIENTATION_FLIP_Y;
		}

		if( attributes&TILEMAP_FLIPX )
		{
			tilemap->orientation ^= ORIENTATION_FLIP_X;
		}

		mappings_update( tilemap );
		recalculate_scroll( tilemap );
		tilemap_mark_all_tiles_dirty( tilemap );
	}
#endif
}

/***********************************************************************************/

//void tilemap_set_scroll_cols( struct tilemap *tilemap, int n )
//{
//	tilemap->logical_scroll_cols = n;
//	tilemap->cached_scroll_cols = n;
//}

//void tilemap_set_scroll_rows( struct tilemap *tilemap, int n )
//{
//	tilemap->logical_scroll_rows = n;
//	tilemap->cached_scroll_rows = n;
//}

/***********************************************************************************/

void tilemap_mark_tile_dirty( struct tilemap *tilemap, int memory_offset )
{
	if( memory_offset<tilemap->max_memory_offset )
	{
		int cached_indx = tilemap->memory_offset_to_cached_indx[memory_offset];
		if( cached_indx>=0 )
		{
			tilemap->transparency_data[cached_indx] = TILE_FLAG_DIRTY;
			tilemap->all_tiles_clean = 0;
		}
	}
}

void tilemap_mark_all_tiles_dirty( struct tilemap *tilemap )
{
	if( tilemap==ALL_TILEMAPS )
	{
		tilemap = first_tilemap;
		while( tilemap )
		{
			tilemap_mark_all_tiles_dirty( tilemap );
			tilemap = tilemap->next;
		}
	}
	else
	{
		tilemap->all_tiles_dirty = 1;
		tilemap->all_tiles_clean = 0;
	}
}

/***********************************************************************************/

static void update_tile_info( struct tilemap *tilemap, UINT32 cached_indx, UINT16 col, UINT16 row )
{
	UINT32 memory_offset;
	UINT8 flags;

profiler_mark(PROFILER_TILEMAP_UPDATE);

	memory_offset = tilemap->cached_indx_to_memory_offset[cached_indx];
	tilemap->tile_get_info( memory_offset );
	flags = tile_info.flags;
	//flags = (flags&0xfc)|tilemap->logical_flip_to_cached_flip[flags&0x3];

//	tilemap->transparency_data[cached_indx] = tilemap->draw_tile(tilemap,x0,y0,flags );
	tilemap->transparency_data[cached_indx] = HandleTransparencyPens_raw(tilemap,col,row,flags );

//HandleTransparencyPens_raw
profiler_mark(PROFILER_END);
}

struct mame_bitmap *tilemap_get_pixmap( struct tilemap * tilemap )
{
	UINT32 cached_indx = 0;
	UINT32 row,col;

	if (!tilemap)
		return 0;

	if (tilemap->all_tiles_clean == 0)
	{
profiler_mark(PROFILER_TILEMAP_DRAW);

		/* if the whole map is dirty, mark it as such */
		if (tilemap->all_tiles_dirty)
		{
			memset( tilemap->transparency_data, TILE_FLAG_DIRTY, (64*64)/*tilemap->num_tiles*/ );
			tilemap->all_tiles_dirty = 0;
		}

		memset( &tile_info, 0x00, sizeof(tile_info) ); /* initialize defaults */
		//tile_info.user_data = tilemap->user_data;

		/* walk over cached rows/cols (better to walk screen coords) */
		for( row=0; row<64/*tilemap->num_cached_rows*/; row++ )
		{
			for( col=0; col<64/*tilemap->num_cached_cols*/; col++ )
			{
				if( tilemap->transparency_data[cached_indx] == TILE_FLAG_DIRTY )
				{
					update_tile_info( tilemap, cached_indx, col, row );
				}
				cached_indx++;
			} /* next col */
		} /* next row */

		tilemap->all_tiles_clean = 1;

profiler_mark(PROFILER_END);
	}

	return tilemap->pixmap;
}

struct mame_bitmap *tilemap_get_transparency_bitmap( struct tilemap * tilemap )
{
	return tilemap->transparency_bitmap;
}

UINT8 *tilemap_get_transparency_data( struct tilemap * tilemap ) //*
{
	return tilemap->transparency_data;
}

/***********************************************************************************/


//void
//tilemap_set_scrolldx( struct tilemap *tilemap, int dx, int dx_if_flipped )
//{
//	tilemap->dx = dx;
//	tilemap->dx_if_flipped = dx_if_flipped;
//	recalculate_scroll( tilemap );
//}

//void
//tilemap_set_scrolldy( struct tilemap *tilemap, int dy, int dy_if_flipped )
//{
//	tilemap->dy = dy;
//	tilemap->dy_if_flipped = dy_if_flipped;
//	recalculate_scroll( tilemap );
//}

static void z_tilemap_set_scrollx( struct tilemap *tilemap, /*int which_000,*/ int value )
{
	tilemap->logical_rowscroll/*[0 which]*/ = value;
	value = /*tilemap->scrollx_delta*/-value; /* adjust */

	if( tilemap->orientation & ORIENTATION_FLIP_X )
	{
		/* adjust scroll amount */
		value = screen_width-/*tilemap->cached_width*/(8*64)-value;
	}
	tilemap->cached_rowscroll[0/*which*/] = value;
}

static void z_tilemap_set_scrolly( struct tilemap *tilemap, /*int which_000,*/ int value )
{
	tilemap->logical_colscroll/*[0 which ]*/ = value;
	value = /*tilemap->scrolly_delta*/ - value; /* adjust */

	if( tilemap->orientation & ORIENTATION_FLIP_Y )
	{
		/* adjust scroll amount */
		value = screen_height - (8*64)/*tilemap->cached_height*/ -value;
	}
	tilemap->cached_colscroll[0/*which*/] = value;
}

static void
recalculate_scroll( struct tilemap *tilemap )
{
//	int i;

	//tilemap->scrollx_delta = (tilemap->attributes & TILEMAP_FLIPX )?tilemap->dx_if_flipped:tilemap->dx;
	//tilemap->scrolly_delta = (tilemap->attributes & TILEMAP_FLIPY )?tilemap->dy_if_flipped:tilemap->dy;

//	for( i=0; i<tilemap->logical_scroll_rows; i++ )
	{
		z_tilemap_set_scrollx( tilemap, /*0 i,*/ tilemap->logical_rowscroll/*[0 i]*/ );
	}
//	for( i=0; i<tilemap->logical_scroll_cols; i++ )
	{
		z_tilemap_set_scrolly( tilemap, /*0 i,*/ tilemap->logical_colscroll/*[0 i]*/ );
	}
}
/***********************************************************************************/

void tilemap_set_palette_offset( struct tilemap *tilemap, int offset )
{
	tilemap->palette_offset = offset;
}

/***********************************************************************************/

//void tilemap_set_user_data( struct tilemap *tilemap, void *user_data )
//{
//	tilemap->user_data = user_data;
//}

/***********************************************************************************/

void tilemap_draw(
struct mame_bitmap *dest,
const struct rectangle *cliprect,
struct tilemap *tilemap,
UINT32 flags,
UINT32 priority )
//{
//	tilemap_draw_primask( dest, cliprect, tilemap, flags, priority, 0xff );
//}
//void tilemap_draw_primask(
//struct mame_bitmap *dest,
//const struct rectangle *cliprect,
//struct tilemap *tilemap,
//UINT32 flags,
//UINT32 priority,
//UINT32 priority_mask )
{
	int xpos,ypos,mask,value;
//	int rows, cols;
	const int *rowscroll, *colscroll;
	int left, right, top, bottom;

profiler_mark(PROFILER_TILEMAP_DRAW);
	if( tilemap->enable )
	{
		/* scroll registers */
	//	rows		= 1/*tilemap->cached_scroll_rows*/;
	//	cols		= 1/*tilemap->cached_scroll_cols*/;
		rowscroll	= tilemap->cached_rowscroll;
		colscroll	= tilemap->cached_colscroll;

		/* clipping */
		if( cliprect )
		{
			left	= cliprect->min_x;
			top 	= cliprect->min_y;
			right	= cliprect->max_x+1;
			bottom	= cliprect->max_y+1;
		}
		else
		{
			left	= 0;
			top 	= 0;
			right	= 8*64/*tilemap->cached_width*/;
			bottom	= 8*64/*tilemap->cached_height*/;
		}

		/* tile priority */
		mask		= TILE_FLAG_TILE_PRIORITY;
		value		= TILE_FLAG_TILE_PRIORITY&flags;

		/* initialize defaults */
		memset( &tile_info, 0x00, sizeof(tile_info) );
		//tile_info.user_data = tilemap->user_data;

		/* if the whole map is dirty, mark it as such */
		if (tilemap->all_tiles_dirty)
		{
			memset( tilemap->transparency_data, TILE_FLAG_DIRTY, (64*64)/*tilemap->num_tiles*/ );
			tilemap->all_tiles_dirty = 0;
		}

		/* priority_bitmap_pitch_row is tilemap-specific */
		priority_bitmap_pitch_row = priority_bitmap_pitch_line*/*h*/8;

		blit.screen_bitmap = dest;
		if( dest == NULL )
		{
			blit.draw_masked = (blitmask_t)pit;
			blit.draw_opaque = (blitopaque_t)pio;
		}
		else
		{
	//			if (tilemap->palette_offset)
	//			{
	//				blit.draw_masked = (blitmask_t)pdt16pal;
	//				blit.draw_opaque = (blitopaque_t)pdo16pal;
	//			}
	//			else if (priority)
	//			{
	//				blit.draw_masked = (blitmask_t)pdt16;
	//				blit.draw_opaque = (blitopaque_t)pdo16;
	//			}
	//			else
	//			{
					blit.draw_masked = (blitmask_t)pdt16np;
					blit.draw_opaque = (blitopaque_t)pdo16np;
	//			}

			blit.screen_bitmap_pitch_line = ((UINT8 *)dest->line[1]) - ((UINT8 *)dest->line[0]);
			blit.screen_bitmap_pitch_line /= 2;
			blit.screen_bitmap_pitch_row = blit.screen_bitmap_pitch_line*/*h*/8;
		} /* dest == bitmap */

		if( !(
				//		/*tilemap->type*/TILEMAP_SPLIT==TILEMAP_OPAQUE ||
			(flags&TILEMAP_IGNORE_TRANSPARENCY)) )
		{
			if( flags&TILEMAP_BACK )
			{
				mask	|= TILE_FLAG_BG_OPAQUE;
				value	|= TILE_FLAG_BG_OPAQUE;
			}
			else
			{
				mask	|= TILE_FLAG_FG_OPAQUE;
				value	|= TILE_FLAG_FG_OPAQUE;
			}
		}

		blit.tilemap_priority_code = (priority & 0xff) | ((/*priority_mask &*/ 0xff) << 8) | (tilemap->palette_offset << 16);

		//if( rows == 1 && cols == 1 )
		{ /* XY scrolling playfield */
			int scrollx = rowscroll[0];
			int scrolly = colscroll[0];

/*
-3  01  01 1==(4-3)
-2  10  10 2==(4-2)
-1  11  11 3==(4-1)
 0  00  00 (0)
 1  01  01 (1)
 2  10  10 (2)
 3  11  11 (3)
*/
		//	if( scrollx < 0 )
		//	{
		//		//scrollx = /*tilemap->cached_width*/(8*64) - ((-scrollx) % (8*64))/*tilemap->cached_width*/;
		//	}
		//	else
		//	{
		//	//	scrollx %= (8*64)/*tilemap->cached_width*/;
		//	}
			scrollx &= (8*64-1)/*tilemap->cached_width*/;

		//	if( scrolly < 0 )
		//	{
		//		//scrolly = (8*64)/*tilemap->cached_height*/ - ((-scrolly) % (8*64))/*tilemap->cached_height*/;
		//		scrolly--;
		//	}
		//	else
		//	{
		//	//	scrolly %= (8*64)/*tilemap->cached_height*/;
		//	}
			scrolly &= (8*64-1)/*tilemap->cached_height*/;

			blit.clip_left		= left;
			blit.clip_top		= top;
			blit.clip_right 	= right;
			blit.clip_bottom	= bottom;

			for(
				ypos = scrolly - (8*64)/*tilemap->cached_height*/;
				ypos < blit.clip_bottom;
				ypos += (8*64)/*tilemap->cached_height*/ )
			{
				for(
					xpos = scrollx - (8*64)/*tilemap->cached_width*/;
					xpos < blit.clip_right;
					xpos += (8*64)/*tilemap->cached_width*/ )
				{
					draw16BPP( tilemap, xpos, ypos, mask, value );
				}
			}
		}
	}
profiler_mark(PROFILER_END);
}


UINT32 tilemap_count( void )
{
	UINT32 count = 0;
	struct tilemap *tilemap = first_tilemap;
	while( tilemap )
	{
		count++;
		tilemap = tilemap->next;
	}
	return count;
}

static struct tilemap *tilemap_nb_find( int number )
{
	int count = 0;
	struct tilemap *tilemap;

	tilemap = first_tilemap;
	while( tilemap )
	{
		count++;
		tilemap = tilemap->next;
	}

	number = (count-1)-number;

	tilemap = first_tilemap;
	while( number-- )
	{
		tilemap = tilemap->next;
	}
	return tilemap;
}

void tilemap_nb_size( UINT32 number, UINT32 *width, UINT32 *height )
{
//	struct tilemap *tilemap = tilemap_nb_find( number );
	*width	= (8*64)/*tilemap->cached_width*/;
	*height = (8*64)/*tilemap->cached_height*/;
}

void tilemap_nb_draw( struct mame_bitmap *dest, UINT32 number, UINT32 scrollx, UINT32 scrolly )
{
	int xpos,ypos;
	struct tilemap *tilemap = tilemap_nb_find( number );

//	switch( dest->depth )
//	{
//	case 15:	blit.draw_opaque = (blitopaque_t)pdo15;		break;
//	case 16:
	blit.draw_opaque = (blitopaque_t)pdo16pal;	//break;
//	default:	exit(1);		break;
//	}
	blit.screen_bitmap = dest;
	blit.screen_bitmap_pitch_line = ((UINT8 *)dest->line[1]) - ((UINT8 *)dest->line[0]);
	blit.screen_bitmap_pitch_line /= 2;
	blit.screen_bitmap_pitch_row = blit.screen_bitmap_pitch_line*/*h*/8;
	priority_bitmap_pitch_row = priority_bitmap_pitch_line*/*h*/8;
	blit.tilemap_priority_code = (tilemap->palette_offset << 16);
	scrollx = (8*64)/*tilemap->cached_width*/  - (scrollx % (8*64))/*tilemap->cached_width*/;
	scrolly = (8*64)/*tilemap->cached_height*/ - (scrolly % (8*64))/*tilemap->cached_height*/;

	blit.clip_left		= 0;
	blit.clip_top		= 0;
	blit.clip_right 	= (dest->width  < (8*64)/*tilemap->cached_width*/)  ? dest->width  : (8*64)/*tilemap->cached_width*/;
	blit.clip_bottom	= (dest->height < (8*64)/*tilemap->cached_height*/) ? dest->height : (8*64)/*tilemap->cached_height*/;

	for(
		ypos = scrolly - (8*64)/*tilemap->cached_height*/;
		ypos < blit.clip_bottom;
		ypos += (8*64)/*tilemap->cached_height*/ )
	{
		for(
			xpos = scrollx - (8*64)/*tilemap->cached_width*/;
			xpos < blit.clip_right;
			xpos += (8*64)/*tilemap->cached_width*/ )
		{
			draw16BPP( tilemap, xpos, ypos, 0, 0 );
		}
	}
}


/***********************************************************************************/

#endif // !DECLARE && !TRANSP

