/* tilemap88n.c

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

// [for F2(chr have roz)]F2の場合、但しCHRダケROZとゆー変なフラグ。
#if (0==PSP_F2_ROZ_TILE)
/* 設定を反映 */
	#if (0==PSP_FORCE_NO_ROZ)
		#define MMM_FORCE_NO_ROZ 0
	#else //(0==PSP_FORCE_NO_ROZ)
		#define MMM_FORCE_NO_ROZ 1
	#endif //(0==PSP_FORCE_NO_ROZ)
//
	#if (0==PSP_FORCE_NO_ZOOM)
		#define MMM_FORCE_NO_ZOOM 0
	#else //(0==PSP_FORCE_NO_ZOOM)
		#define MMM_FORCE_NO_ZOOM 1
	#endif //(0==PSP_FORCE_NO_ZOOM)
#else //(0==PSP_F2_ROZ_TILE)
/* F2はROZ有効 */
	#define MMM_FORCE_NO_ROZ 0
	#define MMM_FORCE_NO_ZOOM 0
#endif //(0==PSP_F2_ROZ_TILE)


#if (1==namcos1)
/* need 36x28 tilemaps */
	  #define CACHED_WIDTH_WAS_2N 0
	  #define CACHED_HEIGHT_WAS_2N 0
#elif (1==namcos86)
	/* 64x32 only */
	  #define CACHED_WIDTH_WAS_2N 1
	  #define CACHED_HEIGHT_WAS_2N 1
#elif (1==namcoNA)
	/* 64x64 only */
	  #define CACHED_WIDTH_WAS_2N 1
	  #define CACHED_HEIGHT_WAS_2N 1
#elif (1==taitoAX)
/* 註：taitoのsystem系は例外なく割り切れる。(Asuka X B H L F1 F2 F3 S Z AIR SJ) */
	  #define CACHED_WIDTH_WAS_2N 1
	  #define CACHED_HEIGHT_WAS_2N 1
#elif (1==taitoF2)
/* 註：taitoのsystem系は例外なく割り切れる。(Asuka X B H L F1 F2 F3 S Z AIR SJ) */
	  #define CACHED_WIDTH_WAS_2N 1
	  #define CACHED_HEIGHT_WAS_2N 1
#elif (1==taitoZ)
/* 註：taitoのsystem系は例外なく割り切れる。(Asuka X B H L F1 F2 F3 S Z AIR SJ) */
	  #define CACHED_WIDTH_WAS_2N 1
	  #define CACHED_HEIGHT_WAS_2N 1
#elif (1==nightstr)
/* 註：taitoのsystem系は例外なく割り切れる。(Asuka X B H L F1 F2 F3 S Z AIR SJ) */
	  #define CACHED_WIDTH_WAS_2N 1
	  #define CACHED_HEIGHT_WAS_2N 1
#else
/* デフォルトは念の為安全側 */
	/* 1: tilemapx が2のｎ乗で割り切れるなら１．割れないなら０． */
	//#define CACHED_WIDTH_WAS_2N 1
	  #define CACHED_WIDTH_WAS_2N 0
	/* 0: tilemapy が2のｎ乗で割り切れるなら１．割れないなら０． */
	//#define CACHED_HEIGHT_WAS_2N 1
	  #define CACHED_HEIGHT_WAS_2N 0
#endif //


#include "driver.h"
//#include "osinline.h"
#include "tilemap.h"
#include "state.h"

#define SWAP(X,Y) { UINT32 temp=X; X=Y; Y=temp; }
#define MAX_TILESIZE 8

#define TILE_FLAG_DIRTY (0x80)

typedef enum { eWHOLLY_TRANSPARENT, eWHOLLY_OPAQUE, eMASKED } trans_t;

typedef void (*tilemap_draw_func)( struct tilemap *tilemap, int xpos, int ypos, int mask, int value );

struct tilemap
{
	struct mame_bitmap *pixmap;	/* cached color data */
	struct mame_bitmap *transparency_bitmap;	/* cached color data */

	/* callback to interpret video RAM for the tilemap */
	void (*tile_get_info)( int memory_offset );

	int *memory_offset_to_cached_indx;
	UINT32 *cached_indx_to_memory_offset;
	UINT32 (*get_memory_offset)( UINT32 col, UINT32 row, UINT32 num_cols, UINT32 num_rows );

//	void *user_data;

	UINT32 max_memory_offset;


	UINT32 pixmap_pitch_line;	/* cached color data */
	UINT32 pixmap_pitch_row;	/* cached color data */

	UINT32 transparency_bitmap_pitch_line;	/* cached color data */
	UINT32 transparency_bitmap_pitch_row;	/* cached color data */
	UINT8 *transparency_data, **transparency_data_row;	/* cached color data */


	UINT32 num_tiles;
//	UINT32 num_pens;

	UINT32 num_logical_rows, num_logical_cols;
	UINT32 num_cached_rows, num_cached_cols;

	int logical_flip_to_cached_flip[4];

//	UINT32 logical_tile_width, logical_tile_height;
//	UINT32 cached_tile_width, cached_tile_height;

	UINT32 cached_width, cached_height;



	int enable;
	int attributes;

	int type;
	int transparent_pen;
	UINT32 fgmask[4], bgmask[4]; /* for TILEMAP_SPLIT */

	UINT32 *pPenToPixel[4];

	UINT8 (*draw_tile)( struct tilemap *tilemap, UINT32 col, UINT32 row, UINT32 flags );

	int cached_scroll_rows, cached_scroll_cols;
	int *cached_rowscroll, *cached_colscroll;

	int logical_scroll_rows, logical_scroll_cols;
	int *logical_rowscroll, *logical_colscroll;

	int orientation;
	int palette_offset;

	UINT16 tile_depth, tile_granularity;
	UINT8 *tile_dirty_map;
	UINT8 all_tiles_dirty;
	UINT8 all_tiles_clean;


	int dx, dx_if_flipped;
	int dy, dy_if_flipped;
	int scrollx_delta, scrolly_delta;


	struct tilemap *next; /* resource tracking */
};

struct mame_bitmap *		priority_bitmap;
UINT32					priority_bitmap_pitch_line;
UINT32					priority_bitmap_pitch_row;

static struct tilemap * first_tilemap; /* resource tracking */
static UINT32			screen_width, screen_height;
struct tile_info		tile_info;

static UINT32 g_mask32[32];

typedef void (*blitmask_t)( void *dest, const void *source, const UINT8 *pMask, int mask, int value, int count, UINT8 *pri, UINT32 pcode );
typedef void (*blitopaque_t)( void *dest, const void *source, int count, UINT8 *pri, UINT32 pcode );

/* the following parameters are constant across tilemap_draw calls */
static struct
{
	blitmask_t draw_masked;
	blitopaque_t draw_opaque;
	int clip_left, clip_top, clip_right, clip_bottom;
	UINT32 tilemap_priority_code;
	struct mame_bitmap *	screen_bitmap;
	UINT32				screen_bitmap_pitch_line;
	UINT32				screen_bitmap_pitch_row;
} blit;

/***********************************************************************************/

static int PenToPixel_Init( struct tilemap *tilemap );
static void PenToPixel_Term( struct tilemap *tilemap );
static int mappings_create( struct tilemap *tilemap );
static void mappings_dispose( struct tilemap *tilemap );
static void mappings_update( struct tilemap *tilemap );
static void recalculate_scroll( struct tilemap *tilemap );

static void install_draw_handlers( struct tilemap *tilemap );
static void tilemap_reset(void);

static void update_tile_info( struct tilemap *tilemap, UINT32 cached_indx, UINT32 cached_col, UINT32 cached_row );

/***********************************************************************************/

static int PenToPixel_Init( struct tilemap *tilemap )
{
	/*
		Construct a table for all tile orientations in advance.
		This simplifies drawing tiles and masks tremendously.
		If performance is an issue, we can always (re)introduce
		customized code for each case and forgo tables.
	*/
	int i,x,y,tx,ty;
	UINT32 *pPenToPixel;
	int lError;

	lError = 0;
	for( i=0; i<4; i++ )
	{
		pPenToPixel = malloc( /*tilemap->num_pens*/(8*8)*sizeof(UINT32) );
		if( pPenToPixel==NULL )
		{
			lError = 1;
		}
		else
		{
			tilemap->pPenToPixel[i] = pPenToPixel;
			for( ty=0; ty</*h*/8; ty++ )
			{
				for( tx=0; tx</*w*/8; tx++ )
				{
					x = tx;
					y = ty;
					if( i&TILE_FLIPX ) x = /*w*/8-1-x;
					if( i&TILE_FLIPY ) y = /*h*/8-1-y;
					*pPenToPixel++ = x+y*MAX_TILESIZE;
				}
			}
		}
	}
	return lError;
}

static void PenToPixel_Term( struct tilemap *tilemap )
{
	int i;
	for( i=0; i<4; i++ )
	{
		free( tilemap->pPenToPixel[i] );
	}
}

static void InitMask32(void)
{
	int i;

	for (i=0;i<16;i++)
	{
		UINT32 p1 = (i&1) ? 0xFFFF : 0;
		UINT32 p2 = (i&2) ? 0xFFFF : 0;
		UINT32 p3 = (i&4) ? 0xFFFF : 0;
		UINT32 p4 = (i&8) ? 0xFFFF : 0;

		g_mask32[i*2] = (p2 << 16) | p1;
		g_mask32[i*2+1] = (p4 << 16) | p3;
	}
}


void tilemap_set_transparent_pen( struct tilemap *tilemap, int pen )
{
	tilemap->transparent_pen = pen;
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

void tilemap_set_depth( struct tilemap *tilemap, int tile_depth, int tile_granularity )
{
	if( tilemap->tile_dirty_map )
	{
		free( tilemap->tile_dirty_map);
	}
	tilemap->tile_dirty_map = malloc( Machine->drv->total_colors >> tile_granularity );
	if( tilemap->tile_dirty_map )
	{
		tilemap->tile_depth = tile_depth;
		tilemap->tile_granularity = tile_granularity;
	}
}

/***********************************************************************************/
/* some common mappings */

UINT32 tilemap_scan_rows( UINT32 col, UINT32 row, UINT32 num_cols, UINT32 num_rows )
{
	/* logical (col,row) -> memory offset */
	return row*num_cols + col;
}
UINT32 tilemap_scan_rows_flip_x( UINT32 col, UINT32 row, UINT32 num_cols, UINT32 num_rows )
{
	/* logical (col,row) -> memory offset */
	return row*num_cols + (num_cols-col-1);
}
UINT32 tilemap_scan_rows_flip_y( UINT32 col, UINT32 row, UINT32 num_cols, UINT32 num_rows )
{
	/* logical (col,row) -> memory offset */
	return (num_rows-row-1)*num_cols + col;
}
UINT32 tilemap_scan_rows_flip_xy( UINT32 col, UINT32 row, UINT32 num_cols, UINT32 num_rows )
{
	/* logical (col,row) -> memory offset */
	return (num_rows-row-1)*num_cols + (num_cols-col-1);
}

UINT32 tilemap_scan_cols( UINT32 col, UINT32 row, UINT32 num_cols, UINT32 num_rows )
{
	/* logical (col,row) -> memory offset */
	return col*num_rows + row;
}
UINT32 tilemap_scan_cols_flip_x( UINT32 col, UINT32 row, UINT32 num_cols, UINT32 num_rows )
{
	/* logical (col,row) -> memory offset */
	return (num_cols-col-1)*num_rows + row;
}
UINT32 tilemap_scan_cols_flip_y( UINT32 col, UINT32 row, UINT32 num_cols, UINT32 num_rows )
{
	/* logical (col,row) -> memory offset */
	return col*num_rows + (num_rows-row-1);
}
UINT32 tilemap_scan_cols_flip_xy( UINT32 col, UINT32 row, UINT32 num_cols, UINT32 num_rows )
{
	/* logical (col,row) -> memory offset */
	return (num_cols-col-1)*num_rows + (num_rows-row-1);
}

/***********************************************************************************/

static int mappings_create( struct tilemap *tilemap )
{
	int max_memory_offset = 0;
	UINT32 col,row;
	UINT32 num_logical_rows = tilemap->num_logical_rows;
	UINT32 num_logical_cols = tilemap->num_logical_cols;
	/* count offsets (might be larger than num_tiles) */
	for( row=0; row<num_logical_rows; row++ )
	{
		for( col=0; col<num_logical_cols; col++ )
		{
			UINT32 memory_offset = tilemap->get_memory_offset( col, row, num_logical_cols, num_logical_rows );
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
		tilemap->cached_indx_to_memory_offset = malloc( sizeof(UINT32)*tilemap->num_tiles );
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
	int logical_flip;
	UINT32 logical_indx, cached_indx;
	UINT32 num_cached_rows = tilemap->num_cached_rows;
	UINT32 num_cached_cols = tilemap->num_cached_cols;
	UINT32 num_logical_rows = tilemap->num_logical_rows;
	UINT32 num_logical_cols = tilemap->num_logical_cols;
	for( logical_indx=0; logical_indx<tilemap->max_memory_offset; logical_indx++ )
	{
		tilemap->memory_offset_to_cached_indx[logical_indx] = -1;
	}

	for( logical_indx=0; logical_indx<tilemap->num_tiles; logical_indx++ )
	{
		UINT32 logical_col = logical_indx%num_logical_cols;
		UINT32 logical_row = logical_indx/num_logical_cols;
		int memory_offset = tilemap->get_memory_offset( logical_col, logical_row, num_logical_cols, num_logical_rows );
		UINT32 cached_col = logical_col;
		UINT32 cached_row = logical_row;
		if( tilemap->orientation & ORIENTATION_FLIP_X ) cached_col = (num_cached_cols-1)-cached_col;
		if( tilemap->orientation & ORIENTATION_FLIP_Y ) cached_row = (num_cached_rows-1)-cached_row;
		cached_indx = cached_row*num_cached_cols+cached_col;
		tilemap->memory_offset_to_cached_indx[memory_offset] = cached_indx;
		tilemap->cached_indx_to_memory_offset[cached_indx] = memory_offset;
	}
	for( logical_flip = 0; logical_flip<4; logical_flip++ )
	{
		int cached_flip = logical_flip;
		if( tilemap->attributes&TILEMAP_FLIPX ) cached_flip ^= TILE_FLIPX;
		if( tilemap->attributes&TILEMAP_FLIPY ) cached_flip ^= TILE_FLIPY;
		tilemap->logical_flip_to_cached_flip[logical_flip] = cached_flip;
	}
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

#ifndef pdo16
static void pdo16( UINT16 *dest, const UINT16 *source, int count, UINT8 *pri, UINT32 pcode )
{
	int i;
	memcpy( dest,source,count*sizeof(UINT16) );
	for( i=0; i<count; i++ )
	{
		pri[i] = (pri[i] & (pcode >> 8)) | pcode;
	}
}
#endif

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

static void pdo15( UINT16 *dest, const UINT16 *source, int count, UINT8 *pri, UINT32 pcode )
{
	int i;
	pen_t *clut = &Machine->remapped_colortable[pcode >> 16];
	for( i=0; i<count; i++ )
	{
		dest[i] = clut[source[i]];
		pri[i] = (pri[i] & (pcode >> 8)) | pcode;
	}
}
#if (0==PSP_FORCE16)
#ifndef pdo32
static void pdo32( UINT32 *dest, const UINT16 *source, int count, UINT8 *pri, UINT32 pcode )
{
	int i;
	pen_t *clut = &Machine->remapped_colortable[pcode >> 16];
	for( i=0; i<count; i++ )
	{
		dest[i] = clut[source[i]];
		pri[i] = (pri[i] & (pcode >> 8)) | pcode;
	}
}
#endif

#ifndef npdo32
static void npdo32( UINT32 *dest, const UINT16 *source, int count, UINT8 *pri, UINT32 pcode )
{
	int oddcount = count & 3;
	int unrcount = count & ~3;
	int i;
	pen_t *clut = &Machine->remapped_colortable[pcode >> 16];
	for( i=0; i<oddcount; i++ )
	{
		dest[i] = clut[source[i]];
	}
	source += count; dest += count;
	for( i=-unrcount; i; i+=4 )
	{
		UINT32 eax, ebx;
		eax = source[i	];
		ebx = source[i+1];
		eax = clut[eax];
		ebx = clut[ebx];
		dest[i	] = eax;
		eax = source[i+2];
		dest[i+1] = ebx;
		ebx = source[i+3];
		eax = clut[eax];
		ebx = clut[ebx];
		dest[i+2] = eax;
		dest[i+3] = ebx;
	}
}
#endif
#endif //(0==PSP_FORCE16)
/***********************************************************************************/

#ifndef pdt16
static void pdt16( UINT16 *dest, const UINT16 *source, const UINT8 *pMask, int mask, int value, int count, UINT8 *pri, UINT32 pcode )
{
	int i;

	for( i=0; i<count; i++ )
	{
		if( (pMask[i]&mask)==value )
		{
			dest[i] = source[i];
			pri[i] = (pri[i] & (pcode >> 8)) | pcode;
		}
	}
}
#endif

#ifndef pdt16pal
static void pdt16pal( UINT16 *dest, const UINT16 *source, const UINT8 *pMask, int mask, int value, int count, UINT8 *pri, UINT32 pcode )
{
	int pal = pcode >> 16;
	int i;

	for( i=0; i<count; i++ )
	{
		if( (pMask[i]&mask)==value )
		{
			dest[i] = source[i] + pal;
			pri[i] = (pri[i] & (pcode >> 8)) | pcode;
		}
	}
}
#endif

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

static void pdt15( UINT16 *dest, const UINT16 *source, const UINT8 *pMask, int mask, int value, int count, UINT8 *pri, UINT32 pcode )
{
	int i;
	pen_t *clut = &Machine->remapped_colortable[pcode >> 16];
	for( i=0; i<count; i++ )
	{
		if( (pMask[i]&mask)==value )
		{
			dest[i] = clut[source[i]];
			pri[i] = (pri[i] & (pcode >> 8)) | pcode;
		}
	}
}
#if (0==PSP_FORCE16)
#ifndef pdt32
static void pdt32( UINT32 *dest, const UINT16 *source, const UINT8 *pMask, int mask, int value, int count, UINT8 *pri, UINT32 pcode )
{
	int i;
	pen_t *clut = &Machine->remapped_colortable[pcode >> 16];
	for( i=0; i<count; i++ )
	{
		if( (pMask[i]&mask)==value )
		{
			dest[i] = clut[source[i]];
			pri[i] = (pri[i] & (pcode >> 8)) | pcode;
		}
	}
}
#endif

#ifndef npdt32
static void npdt32( UINT32 *dest, const UINT16 *source, const UINT8 *pMask, int mask, int value, int count, UINT8 *pri, UINT32 pcode )
{
	int oddcount = count & 3;
	int unrcount = count & ~3;
	int i;
	pen_t *clut = &Machine->remapped_colortable[pcode >> 16];

	for( i=0; i<oddcount; i++ )
	{
		if( (pMask[i]&mask)==value ) dest[i] = clut[source[i]];
	}
	pMask += count, source += count; dest += count;
	for( i=-unrcount; i; i+=4 )
	{
		if( (pMask[i  ]&mask)==value ) dest[i  ] = clut[source[i  ]];
		if( (pMask[i+1]&mask)==value ) dest[i+1] = clut[source[i+1]];
		if( (pMask[i+2]&mask)==value ) dest[i+2] = clut[source[i+2]];
		if( (pMask[i+3]&mask)==value ) dest[i+3] = clut[source[i+3]];
	}
}
#endif
#endif //(0==PSP_FORCE16)
/***********************************************************************************/

static void pbo15( UINT16 *dest, const UINT16 *source, int count, UINT8 *pri, UINT32 pcode )
{
	int i;
	pen_t *clut = &Machine->remapped_colortable[pcode >> 16];
	for( i=0; i<count; i++ )
	{
		dest[i] = alpha_blend16(dest[i], clut[source[i]]);
		pri[i] = (pri[i] & (pcode >> 8)) | pcode;
	}
}
#if (0==PSP_FORCE16)
#ifndef pbo32
static void pbo32( UINT32 *dest, const UINT16 *source, int count, UINT8 *pri, UINT32 pcode )
{
	int i;
	pen_t *clut = &Machine->remapped_colortable[pcode >> 16];
	for( i=0; i<count; i++ )
	{
		dest[i] = alpha_blend32(dest[i], clut[source[i]]);
		pri[i] = (pri[i] & (pcode >> 8)) | pcode;
	}
}
#endif

#ifndef npbo32
static void npbo32( UINT32 *dest, const UINT16 *source, int count, UINT8 *pri, UINT32 pcode )
{
	int oddcount = count & 3;
	int unrcount = count & ~3;
	int i;
	pen_t *clut = &Machine->remapped_colortable[pcode >> 16];
	for( i=0; i<oddcount; i++ )
	{
		dest[i] = alpha_blend32(dest[i], clut[source[i]]);
	}
	source += count; dest += count;
	for( i=-unrcount; i; i+=4 )
	{
		dest[i	] = alpha_blend32(dest[i  ], clut[source[i	]]);
		dest[i+1] = alpha_blend32(dest[i+1], clut[source[i+1]]);
		dest[i+2] = alpha_blend32(dest[i+2], clut[source[i+2]]);
		dest[i+3] = alpha_blend32(dest[i+3], clut[source[i+3]]);
	}
}
#endif
#endif //(0==PSP_FORCE16)
/***********************************************************************************/

static void pbt15( UINT16 *dest, const UINT16 *source, const UINT8 *pMask, int mask, int value, int count, UINT8 *pri, UINT32 pcode )
{
	int i;
	pen_t *clut = &Machine->remapped_colortable[pcode >> 16];
	for( i=0; i<count; i++ )
	{
		if( (pMask[i]&mask)==value )
		{
			dest[i] = alpha_blend16(dest[i], clut[source[i]]);
			pri[i] = (pri[i] & (pcode >> 8)) | pcode;
		}
	}
}
#if (0==PSP_FORCE16)
#ifndef pbt32
static void pbt32( UINT32 *dest, const UINT16 *source, const UINT8 *pMask, int mask, int value, int count, UINT8 *pri, UINT32 pcode )
{
	int i;
	pen_t *clut = &Machine->remapped_colortable[pcode >> 16];
	for( i=0; i<count; i++ )
	{
		if( (pMask[i]&mask)==value )
		{
			dest[i] = alpha_blend32(dest[i], clut[source[i]]);
			pri[i] = (pri[i] & (pcode >> 8)) | pcode;
		}
	}
}
#endif

#ifndef npbt32
static void npbt32( UINT32 *dest, const UINT16 *source, const UINT8 *pMask, int mask, int value, int count, UINT8 *pri, UINT32 pcode )
{
	int oddcount = count & 3;
	int unrcount = count & ~3;
	int i;
	pen_t *clut = &Machine->remapped_colortable[pcode >> 16];

	for( i=0; i<oddcount; i++ )
	{
		if( (pMask[i]&mask)==value ) dest[i] = alpha_blend32(dest[i], clut[source[i]]);
	}
	pMask += count, source += count; dest += count;
	for( i=-unrcount; i; i+=4 )
	{
		if( (pMask[i  ]&mask)==value ) dest[i  ] = alpha_blend32(dest[i  ], clut[source[i  ]]);
		if( (pMask[i+1]&mask)==value ) dest[i+1] = alpha_blend32(dest[i+1], clut[source[i+1]]);
		if( (pMask[i+2]&mask)==value ) dest[i+2] = alpha_blend32(dest[i+2], clut[source[i+2]]);
		if( (pMask[i+3]&mask)==value ) dest[i+3] = alpha_blend32(dest[i+3], clut[source[i+3]]);
	}
}
#endif
#endif //(0==PSP_FORCE16)
/***********************************************************************************/

#define DEPTH 16
#define DATA_TYPE UINT16
#define DECLARE(function,args,body) static void function##16BPP args body
#include "tilemap88n_inc.c"

#if (0==PSP_FORCE16)
#define DEPTH 32
#define DATA_TYPE UINT32
#define DECLARE(function,args,body) static void function##32BPP args body
#include "tilemap88n_inc.c"
#endif //(0==PSP_FORCE16)

#define PAL_INIT const pen_t *pPalData = tile_info.pal_data
#define PAL_GET(pen) pPalData[pen]
#define TRANSP(f) f ## _ind
#include "tilemap88n_inc.c"

#define PAL_INIT int palBase = tile_info.pal_data - Machine->remapped_colortable
#define PAL_GET(pen) (palBase + (pen))
#define TRANSP(f) f ## _raw
#include "tilemap88n_inc.c"

/*********************************************************************************/

static void install_draw_handlers( struct tilemap *tilemap )
{
	if( Machine->game_colortable )
	{
		if( tilemap->type & TILEMAP_BITMASK )
			tilemap->draw_tile = HandleTransparencyBitmask_ind;
		else if( tilemap->type & TILEMAP_SPLIT_PENBIT )
			tilemap->draw_tile = HandleTransparencyPenBit_ind;
		else if( tilemap->type & TILEMAP_SPLIT )
			tilemap->draw_tile = HandleTransparencyPens_ind;
		else if( tilemap->type==TILEMAP_TRANSPARENT )
			tilemap->draw_tile = HandleTransparencyPen_ind;
		else if( tilemap->type==TILEMAP_TRANSPARENT_COLOR )
			tilemap->draw_tile = HandleTransparencyColor_ind;
		else
			tilemap->draw_tile = HandleTransparencyNone_ind;
	}
	else
	{
		if( tilemap->type & TILEMAP_BITMASK )
			tilemap->draw_tile = HandleTransparencyBitmask_raw;
		else if( tilemap->type & TILEMAP_SPLIT_PENBIT )
			tilemap->draw_tile = HandleTransparencyPenBit_raw;
		else if( tilemap->type & TILEMAP_SPLIT )
			tilemap->draw_tile = HandleTransparencyPens_raw;
		else if( tilemap->type==TILEMAP_TRANSPARENT )
			tilemap->draw_tile = HandleTransparencyPen_raw;
		else if( tilemap->type==TILEMAP_TRANSPARENT_COLOR )
			tilemap->draw_tile = HandleTransparencyColor_raw;
		else
			tilemap->draw_tile = HandleTransparencyNone_raw;
	}
}
#if (0==PSP_FORCE16)
INLINE tilemap_draw_func pick_draw_func( struct mame_bitmap *dest )
{
	switch (dest ? dest->depth : Machine->scrbitmap->depth)
	{
		case 32:
			return draw32BPP;

		case 16:
		case 15:
			return draw16BPP;
	}
	exit(1);
	return NULL;
}
#endif //(0==PSP_FORCE16)


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
	InitMask32();
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

struct tilemap *tilemap_create(
	void (*tile_get_info)( int memory_offset ),
	UINT32 (*get_memory_offset)( UINT32 col, UINT32 row, UINT32 num_cols, UINT32 num_rows ),
	int type,
	int tile_width_is8,
	int tile_height_is8,
	int num_cols, int num_rows )
{
	struct tilemap *tilemap;
	UINT32 row;
	int num_tiles;

	tilemap = calloc( 1,sizeof( struct tilemap ) );
	if( tilemap )
	{
		num_tiles = num_cols*num_rows;
		tilemap->num_logical_cols = num_cols;
		tilemap->num_logical_rows = num_rows;
	//	/*w*/8 = tile_width;
	//	tilemap->logical_tile_height = tile_height;
		tilemap->logical_colscroll = calloc(num_cols*8/*tile_width*/,sizeof(int));
		tilemap->logical_rowscroll = calloc(num_rows*8/*tile_height*/,sizeof(int));
		tilemap->num_cached_cols = num_cols;
		tilemap->num_cached_rows = num_rows;
		tilemap->num_tiles = num_tiles;
	//	tilemap->num_pens = /*tile_width*/8*8/*tile_height*/;
	//	/*w*/8 = tile_width;
	//	/*h*/8 = tile_height;
		tilemap->cached_width  = /*tile_width*/ 8*num_cols;
		tilemap->cached_height = /*tile_height*/8*num_rows;
		tilemap->tile_get_info = tile_get_info;
		tilemap->get_memory_offset = get_memory_offset;
		tilemap->orientation = ROT0;

		/* various defaults */
		tilemap->enable = 1;
		tilemap->type = type;
		tilemap->logical_scroll_rows = tilemap->cached_scroll_rows = 1;
		tilemap->logical_scroll_cols = tilemap->cached_scroll_cols = 1;
		tilemap->transparent_pen = -1;
		tilemap->tile_depth = 0;
		tilemap->tile_granularity = 0;
		tilemap->tile_dirty_map = 0;

		tilemap->cached_rowscroll	= calloc(tilemap->cached_height,sizeof(int));
		tilemap->cached_colscroll	= calloc(tilemap->cached_width, sizeof(int));

		tilemap->transparency_data = malloc( num_tiles );
		tilemap->transparency_data_row = malloc( sizeof(UINT8 *)*num_rows );

		tilemap->pixmap = bitmap_alloc_depth( tilemap->cached_width, tilemap->cached_height, -16 );
		tilemap->transparency_bitmap = bitmap_alloc_depth( tilemap->cached_width, tilemap->cached_height, -8 );

		if( tilemap->logical_rowscroll && tilemap->cached_rowscroll &&
			tilemap->logical_colscroll && tilemap->cached_colscroll &&
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

			for( row=0; row<num_rows; row++ )
			{
				tilemap->transparency_data_row[row] = tilemap->transparency_data+num_cols*row;
			}
			install_draw_handlers( tilemap );
			mappings_update( tilemap );
			memset( tilemap->transparency_data, TILE_FLAG_DIRTY, num_tiles );
			tilemap->next = first_tilemap;
			first_tilemap = tilemap;
			if( PenToPixel_Init( tilemap ) == 0 )
			{
				recalculate_scroll(tilemap);
				return tilemap;
			}
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
	PenToPixel_Term( tilemap );
	free( tilemap->logical_rowscroll );
	free( tilemap->cached_rowscroll );
	free( tilemap->logical_colscroll );
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
}

/***********************************************************************************/

void tilemap_set_scroll_cols( struct tilemap *tilemap, int n )
{
	tilemap->logical_scroll_cols = n;
	tilemap->cached_scroll_cols = n;
}

void tilemap_set_scroll_rows( struct tilemap *tilemap, int n )
{
	tilemap->logical_scroll_rows = n;
	tilemap->cached_scroll_rows = n;
}

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

static void update_tile_info( struct tilemap *tilemap, UINT32 cached_indx, UINT32 col, UINT32 row )
{
	UINT32 x0;
	UINT32 y0;
	UINT32 memory_offset;
	UINT32 flags;

profiler_mark(PROFILER_TILEMAP_UPDATE);

	memory_offset = tilemap->cached_indx_to_memory_offset[cached_indx];
	tilemap->tile_get_info( memory_offset );
	flags = tile_info.flags;
	flags = (flags&0xfc)|tilemap->logical_flip_to_cached_flip[flags&0x3];
	x0 = /*w*/8*col;
	y0 = /*h*/8*row;

	tilemap->transparency_data[cached_indx] = tilemap->draw_tile(tilemap,x0,y0,flags );

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
			memset( tilemap->transparency_data, TILE_FLAG_DIRTY, tilemap->num_tiles );
			tilemap->all_tiles_dirty = 0;
		}

		memset( &tile_info, 0x00, sizeof(tile_info) ); /* initialize defaults */
	//	tile_info.user_data = tilemap->user_data;

		/* walk over cached rows/cols (better to walk screen coords) */
		for( row=0; row<tilemap->num_cached_rows; row++ )
		{
			for( col=0; col<tilemap->num_cached_cols; col++ )
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

static void
recalculate_scroll( struct tilemap *tilemap )
{
	int i;

	tilemap->scrollx_delta = (tilemap->attributes & TILEMAP_FLIPX )?tilemap->dx_if_flipped:tilemap->dx;
	tilemap->scrolly_delta = (tilemap->attributes & TILEMAP_FLIPY )?tilemap->dy_if_flipped:tilemap->dy;

	for( i=0; i<tilemap->logical_scroll_rows; i++ )
	{
		tilemap_set_scrollx( tilemap, i, tilemap->logical_rowscroll[i] );
	}
	for( i=0; i<tilemap->logical_scroll_cols; i++ )
	{
		tilemap_set_scrolly( tilemap, i, tilemap->logical_colscroll[i] );
	}
}

void
tilemap_set_scrolldx( struct tilemap *tilemap, int dx, int dx_if_flipped )
{
	tilemap->dx = dx;
	tilemap->dx_if_flipped = dx_if_flipped;
	recalculate_scroll( tilemap );
}

void
tilemap_set_scrolldy( struct tilemap *tilemap, int dy, int dy_if_flipped )
{
	tilemap->dy = dy;
	tilemap->dy_if_flipped = dy_if_flipped;
	recalculate_scroll( tilemap );
}

void tilemap_set_scrollx( struct tilemap *tilemap, int which, int value )
{
	tilemap->logical_rowscroll[which] = value;
	value = tilemap->scrollx_delta-value; /* adjust */

	if( tilemap->orientation & ORIENTATION_FLIP_Y )
	{
		/* adjust affected row */
		which = tilemap->cached_scroll_rows-1 - which;
	}
	if( tilemap->orientation & ORIENTATION_FLIP_X )
	{
		/* adjust scroll amount */
		value = screen_width-tilemap->cached_width-value;
	}
	tilemap->cached_rowscroll[which] = value;
}

void tilemap_set_scrolly( struct tilemap *tilemap, int which, int value )
{
	tilemap->logical_colscroll[which] = value;
	value = tilemap->scrolly_delta - value; /* adjust */

	if( tilemap->orientation & ORIENTATION_FLIP_X )
	{
		/* adjust affected col */
		which = tilemap->cached_scroll_cols-1 - which;
	}
	if( tilemap->orientation & ORIENTATION_FLIP_Y )
	{
		/* adjust scroll amount */
		value = screen_height-tilemap->cached_height-value;
	}
	tilemap->cached_colscroll[which] = value;
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

void tilemap_draw( struct mame_bitmap *dest, const struct rectangle *cliprect, struct tilemap *tilemap, UINT32 flags, UINT32 priority )
{
	tilemap_draw_primask( dest, cliprect, tilemap, flags, priority, 0xff );
}

void tilemap_draw_primask( struct mame_bitmap *dest, const struct rectangle *cliprect, struct tilemap *tilemap, UINT32 flags, UINT32 priority, UINT32 priority_mask )
{
#if (0==PSP_FORCE16)
	tilemap_draw_func drawfunc = pick_draw_func(dest);
#endif //(0==PSP_FORCE16)
	int xpos,ypos,mask,value;
	int rows, cols;
	const int *rowscroll, *colscroll;
	int left, right, top, bottom;

profiler_mark(PROFILER_TILEMAP_DRAW);
	if( tilemap->enable )
	{
		/* scroll registers */
		rows		= tilemap->cached_scroll_rows;
		cols		= tilemap->cached_scroll_cols;
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
			right	= tilemap->cached_width;
			bottom	= tilemap->cached_height;
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
			memset( tilemap->transparency_data, TILE_FLAG_DIRTY, tilemap->num_tiles );
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
			blit.screen_bitmap_pitch_line = ((UINT8 *)dest->line[1]) - ((UINT8 *)dest->line[0]);
			switch( dest->depth )
			{
#if (0==PSP_FORCE16)
			case 32:
				if (priority)
				{
					if( flags&TILEMAP_ALPHA )
					{
						blit.draw_masked = (blitmask_t)pbt32;
						blit.draw_opaque = (blitopaque_t)pbo32;
					}
					else
					{
						blit.draw_masked = (blitmask_t)pdt32;
						blit.draw_opaque = (blitopaque_t)pdo32;
					}
				}
				else
				{
					//* AAT APR2003: added 32-bit no-priority counterpart
					if( flags&TILEMAP_ALPHA )
					{
						blit.draw_masked = (blitmask_t)npbt32;
						blit.draw_opaque = (blitopaque_t)npbo32;
					}
					else
					{
						blit.draw_masked = (blitmask_t)npdt32;
						blit.draw_opaque = (blitopaque_t)npdo32;
					}
				}
				blit.screen_bitmap_pitch_line /= 4;
				break;
#endif //(0==PSP_FORCE16)
			case 15:
				if( flags&TILEMAP_ALPHA )
				{
					blit.draw_masked = (blitmask_t)pbt15;
					blit.draw_opaque = (blitopaque_t)pbo15;
				}
				else
				{
					blit.draw_masked = (blitmask_t)pdt15;
					blit.draw_opaque = (blitopaque_t)pdo15;
				}
				blit.screen_bitmap_pitch_line /= 2;
				break;

			case 16:
				if (tilemap->palette_offset)
				{
					blit.draw_masked = (blitmask_t)pdt16pal;
					blit.draw_opaque = (blitopaque_t)pdo16pal;
				}
				else if (priority)
				{
					blit.draw_masked = (blitmask_t)pdt16;
					blit.draw_opaque = (blitopaque_t)pdo16;
				}
				else
				{
					blit.draw_masked = (blitmask_t)pdt16np;
					blit.draw_opaque = (blitopaque_t)pdo16np;
				}
				blit.screen_bitmap_pitch_line /= 2;
				break;

			default:
				exit(1);
				break;
			}
			blit.screen_bitmap_pitch_row = blit.screen_bitmap_pitch_line*/*h*/8;
		} /* dest == bitmap */

		if( !(tilemap->type==TILEMAP_OPAQUE || (flags&TILEMAP_IGNORE_TRANSPARENCY)) )
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

		blit.tilemap_priority_code = (priority & 0xff) | ((priority_mask & 0xff) << 8) | (tilemap->palette_offset << 16);

		if( rows == 1 && cols == 1 )
		{ /* XY scrolling playfield */
			int scrollx = rowscroll[0];
			int scrolly = colscroll[0];

#if (0==CACHED_WIDTH_WAS_2N)
/* 本格版？？ */
			if( scrollx < 0 )
			{
				scrollx = tilemap->cached_width - (-scrollx) % tilemap->cached_width;
			}
			else
			{
				scrollx = scrollx % tilemap->cached_width;
			}
#else
/* 簡易版 */
			scrollx &= ((tilemap->cached_width )-1);
#endif
#if (0==CACHED_HEIGHT_WAS_2N)
/* 本格版？？ */
			if( scrolly < 0 )
			{
				scrolly = tilemap->cached_height - (-scrolly) % tilemap->cached_height;
			}
			else
			{
				scrolly = scrolly % tilemap->cached_height;
			}
#else
/* 簡易版 */
			scrolly &= ((tilemap->cached_height)-1);
#endif

			blit.clip_left		= left;
			blit.clip_top		= top;
			blit.clip_right 	= right;
			blit.clip_bottom	= bottom;

			for(
				ypos = scrolly - tilemap->cached_height;
				ypos < blit.clip_bottom;
				ypos += tilemap->cached_height )
			{
				for(
					xpos = scrollx - tilemap->cached_width;
					xpos < blit.clip_right;
					xpos += tilemap->cached_width )
				{
#if (0==PSP_FORCE16)
					drawfunc( tilemap, xpos, ypos, mask, value );
#else
					draw16BPP( tilemap, xpos, ypos, mask, value );
#endif //(0==PSP_FORCE16)
				}
			}
		}
		else if( rows == 1 )
		{ /* scrolling columns + horizontal scroll */
			int col = 0;
			int colwidth = tilemap->cached_width / cols;
			int scrollx = rowscroll[0];
#if (0==CACHED_WIDTH_WAS_2N)
/* 本格版？？ */
			if( scrollx < 0 )
			{
				scrollx = tilemap->cached_width - (-scrollx) % tilemap->cached_width;
			}
			else
			{
				scrollx = scrollx % tilemap->cached_width;
			}
#else
/* 簡易版 */
			scrollx &= ((tilemap->cached_width )-1);
#endif
			blit.clip_top		= top;
			blit.clip_bottom	= bottom;

			while( col < cols )
			{
				int cons	= 1;
				int scrolly = colscroll[col];

				/* count consecutive columns scrolled by the same amount */
				if( scrolly != TILE_LINE_DISABLED )
				{
					while( col + cons < cols && colscroll[col + cons] == scrolly ) cons++;
#if (0==CACHED_HEIGHT_WAS_2N)
/* 本格版？？ */
					if( scrolly < 0 )
					{
						scrolly = tilemap->cached_height - (-scrolly) % tilemap->cached_height;
					}
					else
					{
						scrolly %= tilemap->cached_height;
					}
#else
/* 簡易版 */
					scrolly &= ((tilemap->cached_height)-1);
#endif
					blit.clip_left = col * colwidth + scrollx;
					if (blit.clip_left < left) blit.clip_left = left;
					blit.clip_right = (col + cons) * colwidth + scrollx;
					if (blit.clip_right > right) blit.clip_right = right;

					for(
						ypos = scrolly - tilemap->cached_height;
						ypos < blit.clip_bottom;
						ypos += tilemap->cached_height )
					{
#if (0==PSP_FORCE16)
						drawfunc( tilemap, scrollx, ypos, mask, value );
#else
						draw16BPP( tilemap, scrollx, ypos, mask, value );
#endif //(0==PSP_FORCE16)
					}

					blit.clip_left = col * colwidth + scrollx - tilemap->cached_width;
					if (blit.clip_left < left) blit.clip_left = left;
					blit.clip_right = (col + cons) * colwidth + scrollx - tilemap->cached_width;
					if (blit.clip_right > right) blit.clip_right = right;

					for(
						ypos = scrolly - tilemap->cached_height;
						ypos < blit.clip_bottom;
						ypos += tilemap->cached_height )
					{
#if (0==PSP_FORCE16)
						drawfunc( tilemap, scrollx - tilemap->cached_width, ypos, mask, value );
#else
						draw16BPP( tilemap, scrollx - tilemap->cached_width, ypos, mask, value );
#endif //(0==PSP_FORCE16)
					}
				}
				col += cons;
			}
		}
		else if( cols == 1 )
		{ /* scrolling rows + vertical scroll */
			int row = 0;
			int rowheight = tilemap->cached_height / rows;
			int scrolly = colscroll[0];
#if (0==CACHED_HEIGHT_WAS_2N)
/* 本格版？？ */
			if( scrolly < 0 )
			{
				scrolly = tilemap->cached_height - (-scrolly) % tilemap->cached_height;
			}
			else
			{
				scrolly = scrolly % tilemap->cached_height;
			}
#else
/* 簡易版 */
			scrolly &= ((tilemap->cached_height)-1);
#endif
			blit.clip_left = left;
			blit.clip_right = right;
			while( row < rows )
			{
				int cons = 1;
				int scrollx = rowscroll[row];
				/* count consecutive rows scrolled by the same amount */
				if( scrollx != TILE_LINE_DISABLED )
				{
					while( row + cons < rows && rowscroll[row + cons] == scrollx ) cons++;
#if (0==CACHED_WIDTH_WAS_2N)
/* 本格版？？ */
					if( scrollx < 0)
					{
						scrollx = tilemap->cached_width - (-scrollx) % tilemap->cached_width;
					}
					else
					{
						scrollx %= tilemap->cached_width;
					}
#else
/* 簡易版 */
					scrollx &= ((tilemap->cached_width )-1);
#endif
					blit.clip_top = row * rowheight + scrolly;
					if (blit.clip_top < top) blit.clip_top = top;
					blit.clip_bottom = (row + cons) * rowheight + scrolly;
					if (blit.clip_bottom > bottom) blit.clip_bottom = bottom;
					for(
						xpos = scrollx - tilemap->cached_width;
						xpos < blit.clip_right;
						xpos += tilemap->cached_width )
					{
#if (0==PSP_FORCE16)
						drawfunc( tilemap, xpos, scrolly, mask, value );
#else
						draw16BPP( tilemap, xpos, scrolly, mask, value );
#endif //(0==PSP_FORCE16)
					}
					blit.clip_top = row * rowheight + scrolly - tilemap->cached_height;
					if (blit.clip_top < top) blit.clip_top = top;
					blit.clip_bottom = (row + cons) * rowheight + scrolly - tilemap->cached_height;
					if (blit.clip_bottom > bottom) blit.clip_bottom = bottom;
					for(
						xpos = scrollx - tilemap->cached_width;
						xpos < blit.clip_right;
						xpos += tilemap->cached_width )
					{
#if (0==PSP_FORCE16)
						drawfunc( tilemap, xpos, scrolly - tilemap->cached_height, mask, value );
#else
						draw16BPP( tilemap, xpos, scrolly - tilemap->cached_height, mask, value );
#endif //(0==PSP_FORCE16)
					}
				}
				row += cons;
			}
		}
	}
profiler_mark(PROFILER_END);
}


#if (0==MMM_FORCE_NO_ROZ)

void tilemap_draw_roz_primask( struct mame_bitmap *dest,const struct rectangle *cliprect,struct tilemap *tilemap,
		UINT32 startx,UINT32 starty,int incxx,int incxy,int incyx,int incyy,
		int wraparound,
		UINT32 flags, UINT32 priority, UINT32 priority_mask )
{
	if( (incxx == 1<<16) && !incxy & !incyx && (incyy == 1<<16) && wraparound )
	{
		tilemap_set_scrollx( tilemap, 0, startx >> 16 );
		tilemap_set_scrolly( tilemap, 0, starty >> 16 );
		tilemap_draw( dest, cliprect, tilemap, flags, priority );
	}
	else
	{
		int mask,value;

profiler_mark(PROFILER_TILEMAP_DRAW_ROZ);
		if( tilemap->enable )
		{
			/* tile priority */
			mask		= TILE_FLAG_TILE_PRIORITY;
			value		= TILE_FLAG_TILE_PRIORITY&flags;

			tilemap_get_pixmap( tilemap ); /* force update */

			if( !(tilemap->type==TILEMAP_OPAQUE || (flags&TILEMAP_IGNORE_TRANSPARENCY)) )
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

			switch( dest->depth )
			{
#if (0==PSP_FORCE16)
			case 32:
				/* Opaque drawing routines not present due to difficulty with
				optimization using current ROZ methods
				*/
				if (priority)
				{
					if( flags&TILEMAP_ALPHA )
						blit.draw_masked = (blitmask_t)pbt32;
					else
						blit.draw_masked = (blitmask_t)pdt32;
				}
				else
				{
					if( flags&TILEMAP_ALPHA )
						blit.draw_masked = (blitmask_t)npbt32;
					else
						blit.draw_masked = (blitmask_t)npdt32;
				}

				copyroz_core32BPP(dest,tilemap,startx,starty,incxx,incxy,incyx,incyy,
					wraparound,cliprect,mask,value,priority,priority_mask,tilemap->palette_offset);
				break;
#endif //(0==PSP_FORCE16)
			case 15:
				if( flags&TILEMAP_ALPHA )
					blit.draw_masked = (blitmask_t)pbt15;
				else
					blit.draw_masked = (blitmask_t)pdt15;

				copyroz_core16BPP(dest,tilemap,startx,starty,incxx,incxy,incyx,incyy,
					wraparound,cliprect,mask,value,priority,priority_mask,tilemap->palette_offset);
				break;

			case 16:
				if (tilemap->palette_offset)
					blit.draw_masked = (blitmask_t)pdt16pal;
				else if (priority)
					blit.draw_masked = (blitmask_t)pdt16;
				else
					blit.draw_masked = (blitmask_t)pdt16np;

				copyroz_core16BPP(dest,tilemap,startx,starty,incxx,incxy,incyx,incyy,
					wraparound,cliprect,mask,value,priority,priority_mask,tilemap->palette_offset);
				break;

			default:
				exit(1);
			}
		} /* tilemap->enable */
profiler_mark(PROFILER_END);
	}
}

/* notes:
   - startx and starty MUST be UINT32 for calculations to work correctly
   - srcbitmap->width and height are assumed to be a power of 2 to speed up wraparound
   */
void tilemap_draw_roz( struct mame_bitmap *dest,const struct rectangle *cliprect,struct tilemap *tilemap,
		UINT32 startx,UINT32 starty,int incxx,int incxy,int incyx,int incyy,
		int wraparound,
		UINT32 flags, UINT32 priority )
{
	tilemap_draw_roz_primask( dest,cliprect,tilemap,startx,starty,incxx,incxy,incyx,incyy,wraparound,flags,priority, 0xff );
}
#endif //(0==MMM_FORCE_NO_ROZ)

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
	struct tilemap *tilemap = tilemap_nb_find( number );
	*width	= tilemap->cached_width;
	*height = tilemap->cached_height;
}

void tilemap_nb_draw( struct mame_bitmap *dest, UINT32 number, UINT32 scrollx, UINT32 scrolly )
{
#if (0==PSP_FORCE16)
	tilemap_draw_func drawfunc = pick_draw_func(dest);
#endif //(0==PSP_FORCE16)
	int xpos,ypos;
	struct tilemap *tilemap = tilemap_nb_find( number );

	blit.screen_bitmap = dest;
	blit.screen_bitmap_pitch_line = ((UINT8 *)dest->line[1]) - ((UINT8 *)dest->line[0]);
	switch( dest->depth )
	{
#if (0==PSP_FORCE16)
	case 32:
		blit.draw_opaque = (blitopaque_t)pdo32;
		blit.screen_bitmap_pitch_line /= 4;
		break;
#endif //(0==PSP_FORCE16)
	case 15:
		blit.draw_opaque = (blitopaque_t)pdo15;
		blit.screen_bitmap_pitch_line /= 2;
		break;

	case 16:
		blit.draw_opaque = (blitopaque_t)pdo16pal;
		blit.screen_bitmap_pitch_line /= 2;
		break;

	default:
		exit(1);
		break;
	}
	priority_bitmap_pitch_row = priority_bitmap_pitch_line*/*h*/8;
	blit.screen_bitmap_pitch_row = blit.screen_bitmap_pitch_line*/*h*/8;
	blit.tilemap_priority_code = (tilemap->palette_offset << 16);
	scrollx = tilemap->cached_width  - scrollx % tilemap->cached_width;
	scrolly = tilemap->cached_height - scrolly % tilemap->cached_height;

	blit.clip_left		= 0;
	blit.clip_top		= 0;
	blit.clip_right 	= (dest->width < tilemap->cached_width) ? dest->width : tilemap->cached_width;
	blit.clip_bottom	= (dest->height < tilemap->cached_height) ? dest->height : tilemap->cached_height;

	for(
		ypos = scrolly - tilemap->cached_height;
		ypos < blit.clip_bottom;
		ypos += tilemap->cached_height )
	{
		for(
			xpos = scrollx - tilemap->cached_width;
			xpos < blit.clip_right;
			xpos += tilemap->cached_width )
		{
#if (0==PSP_FORCE16)
			drawfunc( tilemap, xpos, ypos, 0, 0 );
#else
			draw16BPP( tilemap, xpos, ypos, 0, 0 );
#endif //(0==PSP_FORCE16)
		}
	}
}


/***********************************************************************************/

#endif // !DECLARE && !TRANSP

