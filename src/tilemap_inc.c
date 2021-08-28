
#if (0==PSP_FORCE16)

#define ROZ_PLOT_PIXEL(INPUT_VAL)										\
	if (blit.draw_masked == (blitmask_t)pbt32)							\
	{																	\
		clut = &Machine->remapped_colortable[priority >> 16] ;			\
		*dest = alpha_blend32(*dest, clut[INPUT_VAL]);					\
		*pri = (*pri & priority_mask) | priority;						\
	}																	\
	else if (blit.draw_masked == (blitmask_t)pdt32) 					\
	{																	\
		clut = &Machine->remapped_colortable[priority >> 16] ;			\
		*dest = clut[INPUT_VAL] ;										\
		*pri = (*pri & priority_mask) | priority;						\
	}																	\
	else if (blit.draw_masked == (blitmask_t)npbt32)					\
	{																	\
		clut = &Machine->remapped_colortable[priority >> 16] ;			\
		*dest = alpha_blend32(*dest, clut[INPUT_VAL]) ; 				\
/*		logerror("PARTIALLY IMPLEMENTED ROZ VIDEO MODE - npbt32\n") ;*/ \
	}																	\
	else if (blit.draw_masked == (blitmask_t)npdt32)					\
	{																	\
		clut = &Machine->remapped_colortable[priority >> 16] ;			\
		*dest = clut[INPUT_VAL] ;										\
/*		logerror("PARTIALLY IMPLEMENTED ROZ VIDEO MODE - npbt32\n") ;*/ \
	}																	\
	else if (blit.draw_masked == (blitmask_t)pbt15) 					\
	{																	\
		clut = &Machine->remapped_colortable[priority >> 16] ;			\
		*dest = alpha_blend16(*dest, clut[INPUT_VAL]) ; 				\
		*pri = (*pri & priority_mask) | priority;						\
	}																	\
	else if (blit.draw_masked == (blitmask_t)pdt15) 					\
	{																	\
		clut = &Machine->remapped_colortable[priority >> 16] ;			\
		*dest = clut[INPUT_VAL] ;										\
		*pri = (*pri & priority_mask) | priority;						\
	}																	\
	else if (blit.draw_masked == (blitmask_t)pdt16pal)					\
	{																	\
		*dest = (INPUT_VAL) + (priority >> 16) ;						\
		*pri = (*pri & priority_mask) | priority;						\
	}																	\
	else if (blit.draw_masked == (blitmask_t)pdt16) 					\
	{																	\
		*dest = INPUT_VAL ; 											\
		*pri = (*pri & priority_mask) | priority;						\
	}																	\
	else if (blit.draw_masked == (blitmask_t)pdt16np)					\
	{																	\
		*dest = INPUT_VAL ; 											\
	}

#else //(0==PSP_FORCE16)

#define ROZ_PLOT_PIXEL(INPUT_VAL)										\
	if (blit.draw_masked == (blitmask_t)pbt15) 							\
	{																	\
		clut = &Machine->remapped_colortable[priority >> 16] ;			\
		*dest = alpha_blend16(*dest, clut[INPUT_VAL]) ; 				\
		*pri = (*pri & priority_mask) | priority;						\
	}																	\
	else if (blit.draw_masked == (blitmask_t)pdt15) 					\
	{																	\
		clut = &Machine->remapped_colortable[priority >> 16] ;			\
		*dest = clut[INPUT_VAL] ;										\
		*pri = (*pri & priority_mask) | priority;						\
	}																	\
	else if (blit.draw_masked == (blitmask_t)pdt16pal)					\
	{																	\
		*dest = (INPUT_VAL) + (priority >> 16) ;						\
		*pri = (*pri & priority_mask) | priority;						\
	}																	\
	else if (blit.draw_masked == (blitmask_t)pdt16) 					\
	{																	\
		*dest = INPUT_VAL ; 											\
		*pri = (*pri & priority_mask) | priority;						\
	}																	\
	else if (blit.draw_masked == (blitmask_t)pdt16np)					\
	{																	\
		*dest = INPUT_VAL ; 											\
	}

#endif //(0==PSP_FORCE16)


#ifdef DECLARE

#if (0==MMM_FORCE_NO_ROZ)

DECLARE(copyroz_core,(struct mame_bitmap *bitmap,struct tilemap *tilemap,
		UINT32 startx,UINT32 starty,int incxx,int incxy,int incyx,int incyy,int wraparound,
		const struct rectangle *clip,
		int mask,int value,
		UINT32 priority,UINT32 priority_mask,UINT32 palette_offset),
{
	UINT32 cx;
	UINT32 cy;
	int x;
	int sx;
	int sy;
	int ex;
	int ey;
	struct mame_bitmap *srcbitmap = tilemap->pixmap;
	struct mame_bitmap *transparency_bitmap = tilemap->transparency_bitmap;
	const int xmask = srcbitmap->width-1;
	const int ymask = srcbitmap->height-1;
	const int widthshifted = srcbitmap->width << 16;
	const int heightshifted = srcbitmap->height << 16;
	DATA_TYPE *dest;
	UINT8 *pri;
	const UINT16 *src;
	const UINT8 *pMask;

	pen_t *clut ;

	if (clip)
	{
		startx += clip->min_x * incxx + clip->min_y * incyx;
		starty += clip->min_x * incxy + clip->min_y * incyy;

		sx = clip->min_x;
		sy = clip->min_y;
		ex = clip->max_x;
		ey = clip->max_y;
	}
	else
	{
		sx = 0;
		sy = 0;
		ex = bitmap->width-1;
		ey = bitmap->height-1;
	}


	if (incxy == 0 && incyx == 0 && !wraparound)
	{
		/* optimized loop for the not rotated case */

		if (incxx == 0x10000)
		{
			/* optimized loop for the not zoomed case */

			/* startx is unsigned */
			startx = ((INT32)startx) >> 16;

			if (startx >= srcbitmap->width)
			{
				sx += -startx;
				startx = 0;
			}

			if (sx <= ex)
			{
				while (sy <= ey)
				{
					if (starty < heightshifted)
					{
						x = sx;
						cx = startx;
						cy = starty >> 16;
						dest = ((DATA_TYPE *)bitmap->line[sy]) + sx;

						pri = ((UINT8 *)priority_bitmap->line[sy]) + sx;
						src = (UINT16 *)srcbitmap->line[cy];
						pMask = (UINT8 *)transparency_bitmap->line[cy];

						while (x <= ex && cx < srcbitmap->width)
						{
							if ( (pMask[cx]&mask) == value )
							{
								ROZ_PLOT_PIXEL((src[cx]+palette_offset)) ;
							}
							cx++;
							x++;
							dest++;
							pri++;
						}
					}
					starty += incyy;
					sy++;
				}
			}
		}
		else
		{
			while (startx >= widthshifted && sx <= ex)
			{
				startx += incxx;
				sx++;
			}

			if (sx <= ex)
			{
				while (sy <= ey)
				{
					if (starty < heightshifted)
					{
						x = sx;
						cx = startx;
						cy = starty >> 16;
						dest = ((DATA_TYPE *)bitmap->line[sy]) + sx;

						pri = ((UINT8 *)priority_bitmap->line[sy]) + sx;
						src = (UINT16 *)srcbitmap->line[cy];
						pMask = (UINT8 *)transparency_bitmap->line[cy];

						while (x <= ex && cx < widthshifted)
						{
							if ( (pMask[cx>>16]&mask) == value )
							{
								ROZ_PLOT_PIXEL((src[cx >> 16]+palette_offset)) ;
							}
							cx += incxx;
							x++;
							dest++;
							pri++;
						}
					}
					starty += incyy;
					sy++;
				}
			}
		}
	}
	else
	{
		if (wraparound)
		{
			/* plot with wraparound */
			while (sy <= ey)
			{
				x = sx;
				cx = startx;
				cy = starty;
				dest = ((DATA_TYPE *)bitmap->line[sy]) + sx;
				pri = ((UINT8 *)priority_bitmap->line[sy]) + sx;
				while (x <= ex)
				{
					if( (((UINT8 *)transparency_bitmap->line[(cy>>16)&ymask])[(cx>>16)&xmask]&mask) == value )
					{
						ROZ_PLOT_PIXEL(((((UINT16 *)srcbitmap->line[(cy >> 16) & ymask])[(cx >> 16) & xmask]+palette_offset))) ;
					}
					cx += incxx;
					cy += incxy;
					x++;
					dest++;
					pri++;
				}
				startx += incyx;
				starty += incyy;
				sy++;
			}
		}
		else
		{
			while (sy <= ey)
			{
				x = sx;
				cx = startx;
				cy = starty;
				dest = ((DATA_TYPE *)bitmap->line[sy]) + sx;
				pri = ((UINT8 *)priority_bitmap->line[sy]) + sx;
				while (x <= ex)
				{
					if (cx < widthshifted && cy < heightshifted)
					{
						if( (((UINT8 *)transparency_bitmap->line[cy>>16])[cx>>16]&mask)==value )
						{
							ROZ_PLOT_PIXEL((((UINT16 *)srcbitmap->line[cy >> 16])[cx >> 16]+palette_offset)) ;
						}
					}
					cx += incxx;
					cy += incxy;
					x++;
					dest++;
					pri++;
				}
				startx += incyx;
				starty += incyy;
				sy++;
			}
		}
	}
})

#endif //(0==MMM_FORCE_NO_ROZ)

#ifndef osd_pend
#define osd_pend() do { } while (0)
#endif

DECLARE( draw, (struct tilemap *tilemap, int xpos, int ypos, int mask, int value ),
{
	trans_t transPrev;
	trans_t transCur;
	const UINT8 *pTrans;
	UINT32 cached_indx;
	struct mame_bitmap *screen = blit.screen_bitmap;
	int tilemap_priority_code = blit.tilemap_priority_code;
	int x1 = xpos;
	int y1 = ypos;
	int x2 = xpos+tilemap->cached_width;
	int y2 = ypos+tilemap->cached_height;
	DATA_TYPE *dest_baseaddr = NULL;
	DATA_TYPE *dest_next;
	int dy;
	int count;
	const UINT16 *source0;
	DATA_TYPE *dest0;
	UINT8 *pmap0;
	int i;
	int row;
	int x_start;
	int x_end;
	int column;
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
	if( x1<blit.clip_left ) x1 = blit.clip_left;
	if( x2>blit.clip_right ) x2 = blit.clip_right;
	if( y1<blit.clip_top ) y1 = blit.clip_top;
	if( y2>blit.clip_bottom ) y2 = blit.clip_bottom;

	if( x1<x2 && y1<y2 ) /* do nothing if totally clipped */
	{
		priority_bitmap_baseaddr = xpos + (UINT8 *)priority_bitmap->line[y1];
		if( screen )
		{
			dest_baseaddr = xpos + (DATA_TYPE *)screen->line[y1];
		}

		/* convert screen coordinates to source tilemap coordinates */
		x1 -= xpos;
		y1 -= ypos;
		x2 -= xpos;
		y2 -= ypos;

		source_baseaddr = (UINT16 *)tilemap->pixmap->line[y1];
		mask_baseaddr = tilemap->transparency_bitmap->line[y1];

		c1 = x1/tilemap->cached_tile_width; /* round down */
		c2 = (x2+tilemap->cached_tile_width-1)/tilemap->cached_tile_width; /* round up */

		y = y1;
		y_next = tilemap->cached_tile_height*(y1/tilemap->cached_tile_height) + tilemap->cached_tile_height;
		if( y_next>y2 ) y_next = y2;

		dy = y_next-y;
		dest_next = dest_baseaddr + dy*blit.screen_bitmap_pitch_line;
		priority_bitmap_next = priority_bitmap_baseaddr + dy*priority_bitmap_pitch_line;
		source_next = source_baseaddr + dy*tilemap->pixmap_pitch_line;
		mask_next = mask_baseaddr + dy*tilemap->transparency_bitmap_pitch_line;
		for(;;)
		{
			row = y/tilemap->cached_tile_height;
			x_start = x1;

			transPrev = eWHOLLY_TRANSPARENT;
			pTrans = mask_baseaddr + x_start;

			cached_indx = row*tilemap->num_cached_cols + c1;
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
				pTrans += tilemap->cached_tile_width;

			L_Skip:
				if( transCur!=transPrev )
				{
					x_end = column*tilemap->cached_tile_width;
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
			y_next += tilemap->cached_tile_height;

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

	osd_pend();
})

#undef DATA_TYPE
#undef DEPTH
#undef DECLARE
#endif /* DECLARE */

#ifdef TRANSP
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

static UINT8 TRANSP(HandleTransparencyBitmask)(struct tilemap *tilemap, UINT32 x0, UINT32 y0, UINT32 flags)
{
	UINT32 tile_width = tilemap->cached_tile_width;
	UINT32 tile_height = tilemap->cached_tile_height;
	struct mame_bitmap *pixmap = tilemap->pixmap;
	struct mame_bitmap *transparency_bitmap = tilemap->transparency_bitmap;
	int pitch = tile_width + tile_info.skip;
	PAL_INIT;
	UINT32 *pPenToPixel;
	const UINT8 *pPenData = tile_info.pen_data;
	const UINT8 *pSource;
	UINT32 code_transparent = tile_info.priority;
	UINT32 code_opaque = code_transparent | TILE_FLAG_FG_OPAQUE;
	UINT32 tx;
	UINT32 ty;
	UINT32 data;
	UINT32 yx;
	UINT32 x;
	UINT32 y;
	UINT32 pen;
	UINT8 *pBitmask = tile_info.mask_data;
	UINT32 bitoffs;
	int bWhollyOpaque;
	int bWhollyTransparent;
	int bDontIgnoreTransparency = !(flags&TILE_IGNORE_TRANSPARENCY);

	bWhollyOpaque = 1;
	bWhollyTransparent = 1;

	pPenToPixel = tilemap->pPenToPixel[flags&(TILE_FLIPY|TILE_FLIPX)];

	if( flags&TILE_4BPP )
	{
		for( ty=tile_height; ty!=0; ty-- )
		{
			pSource = pPenData;
			for( tx=tile_width/2; tx!=0; tx-- )
			{
				data = *pSource++;

				pen = data&0xf;
				yx = *pPenToPixel++;
				x = x0+(yx%MAX_TILESIZE);
				y = y0+(yx/MAX_TILESIZE);
				*(x+(UINT16 *)pixmap->line[y]) = PAL_GET(pen);

				pen = data>>4;
				yx = *pPenToPixel++;
				x = x0+(yx%MAX_TILESIZE);
				y = y0+(yx/MAX_TILESIZE);
				*(x+(UINT16 *)pixmap->line[y]) = PAL_GET(pen);
			}
			pPenData += pitch/2;
		}
	}
	else
	{
		for( ty=tile_height; ty!=0; ty-- )
		{
			pSource = pPenData;
			for( tx=tile_width; tx!=0; tx-- )
			{
				pen = *pSource++;
				yx = *pPenToPixel++;
				x = x0+(yx%MAX_TILESIZE);
				y = y0+(yx/MAX_TILESIZE);
				*(x+(UINT16 *)pixmap->line[y]) = PAL_GET(pen);
			}
			pPenData += pitch;
		}
	}

	pPenToPixel = tilemap->pPenToPixel[flags&(TILE_FLIPY|TILE_FLIPX)];
	bitoffs = 0;
	for( ty=tile_height; ty!=0; ty-- )
	{
		for( tx=tile_width; tx!=0; tx-- )
		{
			yx = *pPenToPixel++;
			x = x0+(yx%MAX_TILESIZE);
			y = y0+(yx/MAX_TILESIZE);
			if( bDontIgnoreTransparency && (pBitmask[bitoffs/8]&(0x80>>(bitoffs&7))) == 0 )
			{
				((UINT8 *)transparency_bitmap->line[y])[x] = code_transparent;
				bWhollyOpaque = 0;
			}
			else
			{
				((UINT8 *)transparency_bitmap->line[y])[x] = code_opaque;
				bWhollyTransparent = 0;
			}
			bitoffs++;
		}
	}

	return (bWhollyOpaque || bWhollyTransparent)?0:TILE_FLAG_FG_OPAQUE;
}

static UINT8 TRANSP(HandleTransparencyColor)(struct tilemap *tilemap, UINT32 x0, UINT32 y0, UINT32 flags)
{
	UINT32 tile_width = tilemap->cached_tile_width;
	UINT32 tile_height = tilemap->cached_tile_height;
	struct mame_bitmap *pixmap = tilemap->pixmap;
	struct mame_bitmap *transparency_bitmap = tilemap->transparency_bitmap;
	int pitch = tile_width + tile_info.skip;
	PAL_INIT;
	UINT32 *pPenToPixel = tilemap->pPenToPixel[flags&(TILE_FLIPY|TILE_FLIPX)];
	const UINT8 *pPenData = tile_info.pen_data;
	const UINT8 *pSource;
	UINT32 code_transparent = tile_info.priority;
	UINT32 code_opaque = code_transparent | TILE_FLAG_FG_OPAQUE;
	UINT32 tx;
	UINT32 ty;
	UINT32 data;
	UINT32 yx;
	UINT32 x;
	UINT32 y;
	UINT32 pen;
	UINT32 transparent_color = tilemap->transparent_pen;
	int bWhollyOpaque;
	int bWhollyTransparent;

	bWhollyOpaque = 1;
	bWhollyTransparent = 1;

	if( flags&TILE_4BPP )
	{
		for( ty=tile_height; ty!=0; ty-- )
		{
			pSource = pPenData;
			for( tx=tile_width/2; tx!=0; tx-- )
			{
				data = *pSource++;

				pen = data&0xf;
				yx = *pPenToPixel++;
				x = x0+(yx%MAX_TILESIZE);
				y = y0+(yx/MAX_TILESIZE);
				*(x+(UINT16 *)pixmap->line[y]) = PAL_GET(pen);
				if( PAL_GET(pen)==transparent_color )
				{
					((UINT8 *)transparency_bitmap->line[y])[x] = code_transparent;
					bWhollyOpaque = 0;
				}
				else
				{
					((UINT8 *)transparency_bitmap->line[y])[x] = code_opaque;
					bWhollyTransparent = 0;
				}

				pen = data>>4;
				yx = *pPenToPixel++;
				x = x0+(yx%MAX_TILESIZE);
				y = y0+(yx/MAX_TILESIZE);
				*(x+(UINT16 *)pixmap->line[y]) = PAL_GET(pen);
				if( PAL_GET(pen)==transparent_color )
				{
					((UINT8 *)transparency_bitmap->line[y])[x] = code_transparent;
					bWhollyOpaque = 0;
				}
				else
				{
					((UINT8 *)transparency_bitmap->line[y])[x] = code_opaque;
					bWhollyTransparent = 0;
				}
			}
			pPenData += pitch/2;
		}
	}
	else
	{
		for( ty=tile_height; ty!=0; ty-- )
		{
			pSource = pPenData;
			for( tx=tile_width; tx!=0; tx-- )
			{
				pen = *pSource++;
				yx = *pPenToPixel++;
				x = x0+(yx%MAX_TILESIZE);
				y = y0+(yx/MAX_TILESIZE);
				*(x+(UINT16 *)pixmap->line[y]) = PAL_GET(pen);
				if( PAL_GET(pen)==transparent_color )
				{
					((UINT8 *)transparency_bitmap->line[y])[x] = code_transparent;
					bWhollyOpaque = 0;
				}
				else
				{
					((UINT8 *)transparency_bitmap->line[y])[x] = code_opaque;
					bWhollyTransparent = 0;
				}
			}
			pPenData += pitch;
		}
	}
	return (bWhollyOpaque || bWhollyTransparent)?0:TILE_FLAG_FG_OPAQUE;
}

static UINT8 TRANSP(HandleTransparencyPen)(struct tilemap *tilemap, UINT32 x0, UINT32 y0, UINT32 flags)
{
	UINT32 tile_width = tilemap->cached_tile_width;
	UINT32 tile_height = tilemap->cached_tile_height;
	struct mame_bitmap *pixmap = tilemap->pixmap;
	struct mame_bitmap *transparency_bitmap = tilemap->transparency_bitmap;
	int pitch = tile_width + tile_info.skip;
	PAL_INIT;
	UINT32 *pPenToPixel = tilemap->pPenToPixel[flags&(TILE_FLIPY|TILE_FLIPX)];
	const UINT8 *pPenData = tile_info.pen_data;
	const UINT8 *pSource;
	UINT32 code_transparent = tile_info.priority;
	UINT32 code_opaque = code_transparent | TILE_FLAG_FG_OPAQUE;
	UINT32 tx;
	UINT32 ty;
	UINT32 data;
	UINT32 yx;
	UINT32 x;
	UINT32 y;
	UINT32 pen;
	UINT32 transparent_pen = tilemap->transparent_pen;
	int bWhollyOpaque;
	int bWhollyTransparent;

	bWhollyOpaque = 1;
	bWhollyTransparent = 1;

	if( flags&TILE_IGNORE_TRANSPARENCY )
	{
		transparent_pen = ~0;
	}

	if( flags&TILE_4BPP )
	{
		for( ty=tile_height; ty!=0; ty-- )
		{
			pSource = pPenData;
			for( tx=tile_width/2; tx!=0; tx-- )
			{
				data = *pSource++;

				pen = data&0xf;
				yx = *pPenToPixel++;
				x = x0+(yx%MAX_TILESIZE);
				y = y0+(yx/MAX_TILESIZE);
				*(x+(UINT16 *)pixmap->line[y]) = PAL_GET(pen);
				if( pen==transparent_pen )
				{
					((UINT8 *)transparency_bitmap->line[y])[x] = code_transparent;
					bWhollyOpaque = 0;
				}
				else
				{
					((UINT8 *)transparency_bitmap->line[y])[x] = code_opaque;
					bWhollyTransparent = 0;
				}

				pen = data>>4;
				yx = *pPenToPixel++;
				x = x0+(yx%MAX_TILESIZE);
				y = y0+(yx/MAX_TILESIZE);
				*(x+(UINT16 *)pixmap->line[y]) = PAL_GET(pen);
				((UINT8 *)transparency_bitmap->line[y])[x] = (pen==transparent_pen)?code_transparent:code_opaque;
			}
			pPenData += pitch/2;
		}
	}
	else
	{
		for( ty=tile_height; ty!=0; ty-- )
		{
			pSource = pPenData;
			for( tx=tile_width; tx!=0; tx-- )
			{
				pen = *pSource++;
				yx = *pPenToPixel++;
				x = x0+(yx%MAX_TILESIZE);
				y = y0+(yx/MAX_TILESIZE);
				*(x+(UINT16 *)pixmap->line[y]) = PAL_GET(pen);
				if( pen==transparent_pen )
				{
					((UINT8 *)transparency_bitmap->line[y])[x] = code_transparent;
					bWhollyOpaque = 0;

				}
				else
				{
					((UINT8 *)transparency_bitmap->line[y])[x] = code_opaque;
					bWhollyTransparent = 0;
				}
			}
			pPenData += pitch;
		}
	}

	return (bWhollyOpaque || bWhollyTransparent)?0:TILE_FLAG_FG_OPAQUE;
}

static UINT8 TRANSP(HandleTransparencyPenBit)(struct tilemap *tilemap, UINT32 x0, UINT32 y0, UINT32 flags)
{
	UINT32 tile_width = tilemap->cached_tile_width;
	UINT32 tile_height = tilemap->cached_tile_height;
	struct mame_bitmap *pixmap = tilemap->pixmap;
	struct mame_bitmap *transparency_bitmap = tilemap->transparency_bitmap;
	int pitch = tile_width + tile_info.skip;
	PAL_INIT;
	UINT32 *pPenToPixel = tilemap->pPenToPixel[flags&(TILE_FLIPY|TILE_FLIPX)];
	const UINT8 *pPenData = tile_info.pen_data;
	const UINT8 *pSource;
	UINT32 tx;
	UINT32 ty;
	UINT32 data;
	UINT32 yx;
	UINT32 x;
	UINT32 y;
	UINT32 pen;
	UINT32 penbit = tilemap->transparent_pen;
	UINT32 code_front = tile_info.priority | TILE_FLAG_FG_OPAQUE;
	UINT32 code_back = tile_info.priority | TILE_FLAG_BG_OPAQUE;
	int code;
	int and_flags = ~0;
	int or_flags = 0;

	if( flags&TILE_4BPP )
	{
		for( ty=tile_height; ty!=0; ty-- )
		{
			pSource = pPenData;
			for( tx=tile_width/2; tx!=0; tx-- )
			{
				data = *pSource++;

				pen = data&0xf;
				yx = *pPenToPixel++;
				x = x0+(yx%MAX_TILESIZE);
				y = y0+(yx/MAX_TILESIZE);
				*(x+(UINT16 *)pixmap->line[y]) = PAL_GET(pen);
				code = ((pen&penbit)==penbit)?code_front:code_back;
				and_flags &= code;
				or_flags |= code;
				((UINT8 *)transparency_bitmap->line[y])[x] = code;

				pen = data>>4;
				yx = *pPenToPixel++;
				x = x0+(yx%MAX_TILESIZE);
				y = y0+(yx/MAX_TILESIZE);
				*(x+(UINT16 *)pixmap->line[y]) = PAL_GET(pen);
				code = ((pen&penbit)==penbit)?code_front:code_back;
				and_flags &= code;
				or_flags |= code;
				((UINT8 *)transparency_bitmap->line[y])[x] = code;
			}
			pPenData += pitch/2;
		}
	}
	else
	{
		for( ty=tile_height; ty!=0; ty-- )
		{
			pSource = pPenData;
			for( tx=tile_width; tx!=0; tx-- )
			{
				pen = *pSource++;
				yx = *pPenToPixel++;
				x = x0+(yx%MAX_TILESIZE);
				y = y0+(yx/MAX_TILESIZE);
				*(x+(UINT16 *)pixmap->line[y]) = PAL_GET(pen);
				code = ((pen&penbit)==penbit)?code_front:code_back;
				and_flags &= code;
				or_flags |= code;
				((UINT8 *)transparency_bitmap->line[y])[x] = code;
			}
			pPenData += pitch;
		}
	}
	return or_flags ^ and_flags;
}

static UINT8 TRANSP(HandleTransparencyPens)(struct tilemap *tilemap, UINT32 x0, UINT32 y0, UINT32 flags)
{
	UINT32 tile_width = tilemap->cached_tile_width;
	UINT32 tile_height = tilemap->cached_tile_height;
	struct mame_bitmap *pixmap = tilemap->pixmap;
	struct mame_bitmap *transparency_bitmap = tilemap->transparency_bitmap;
	int pitch = tile_width + tile_info.skip;
	PAL_INIT;
	UINT32 *pPenToPixel = tilemap->pPenToPixel[flags&(TILE_FLIPY|TILE_FLIPX)];
	const UINT8 *pPenData = tile_info.pen_data;
	const UINT8 *pSource;
	UINT32 code_transparent = tile_info.priority;
	UINT32 tx;
	UINT32 ty;
	UINT32 data;
	UINT32 yx;
	UINT32 x;
	UINT32 y;
	UINT32 pen;
	UINT32 fgmask = tilemap->fgmask[(flags>>TILE_SPLIT_OFFSET)&3];
	UINT32 bgmask = tilemap->bgmask[(flags>>TILE_SPLIT_OFFSET)&3];
	UINT32 code;
	int and_flags = ~0;
	int or_flags = 0;

	if( flags&TILE_4BPP )
	{
		for( ty=tile_height; ty!=0; ty-- )
		{
			pSource = pPenData;
			for( tx=tile_width/2; tx!=0; tx-- )
			{
				data = *pSource++;

				pen = data&0xf;
				yx = *pPenToPixel++;
				x = x0+(yx%MAX_TILESIZE);
				y = y0+(yx/MAX_TILESIZE);
				*(x+(UINT16 *)pixmap->line[y]) = PAL_GET(pen);
				code = code_transparent;
				if( !((1<<pen)&fgmask) ) code |= TILE_FLAG_FG_OPAQUE;
				if( !((1<<pen)&bgmask) ) code |= TILE_FLAG_BG_OPAQUE;
				and_flags &= code;
				or_flags |= code;
				((UINT8 *)transparency_bitmap->line[y])[x] = code;

				pen = data>>4;
				yx = *pPenToPixel++;
				x = x0+(yx%MAX_TILESIZE);
				y = y0+(yx/MAX_TILESIZE);
				*(x+(UINT16 *)pixmap->line[y]) = PAL_GET(pen);
				code = code_transparent;
				if( !((1<<pen)&fgmask) ) code |= TILE_FLAG_FG_OPAQUE;
				if( !((1<<pen)&bgmask) ) code |= TILE_FLAG_BG_OPAQUE;
				and_flags &= code;
				or_flags |= code;
				((UINT8 *)transparency_bitmap->line[y])[x] = code;
			}
			pPenData += pitch/2;
		}
	}
	else
	{
		for( ty=tile_height; ty!=0; ty-- )
		{
			pSource = pPenData;
			for( tx=tile_width; tx!=0; tx-- )
			{
				pen = *pSource++;
				yx = *pPenToPixel++;
				x = x0+(yx%MAX_TILESIZE);
				y = y0+(yx/MAX_TILESIZE);
				*(x+(UINT16 *)pixmap->line[y]) = PAL_GET(pen);
				code = code_transparent;
				if( !((1<<pen)&fgmask) ) code |= TILE_FLAG_FG_OPAQUE;
				if( !((1<<pen)&bgmask) ) code |= TILE_FLAG_BG_OPAQUE;
				and_flags &= code;
				or_flags |= code;
				((UINT8 *)transparency_bitmap->line[y])[x] = code;
			}
			pPenData += pitch;
		}
	}
	return and_flags ^ or_flags;
}

static UINT8 TRANSP(HandleTransparencyNone)(struct tilemap *tilemap, UINT32 x0, UINT32 y0, UINT32 flags)
{
	UINT32 tile_width = tilemap->cached_tile_width;
	UINT32 tile_height = tilemap->cached_tile_height;
	struct mame_bitmap *pixmap = tilemap->pixmap;
	struct mame_bitmap *transparency_bitmap = tilemap->transparency_bitmap;
	int pitch = tile_width + tile_info.skip;
	PAL_INIT;
	UINT32 *pPenToPixel = tilemap->pPenToPixel[flags&(TILE_FLIPY|TILE_FLIPX)];
	const UINT8 *pPenData = tile_info.pen_data;
	const UINT8 *pSource;
	UINT32 code_opaque = tile_info.priority;
	UINT32 tx;
	UINT32 ty;
	UINT32 data;
	UINT32 yx;
	UINT32 x;
	UINT32 y;
	UINT32 pen;

	if( flags&TILE_4BPP )
	{
		for( ty=tile_height; ty!=0; ty-- )
		{
			pSource = pPenData;
			for( tx=tile_width/2; tx!=0; tx-- )
			{
				data = *pSource++;

				pen = data&0xf;
				yx = *pPenToPixel++;
				x = x0+(yx%MAX_TILESIZE);
				y = y0+(yx/MAX_TILESIZE);
				*(x+(UINT16 *)pixmap->line[y]) = PAL_GET(pen);
				((UINT8 *)transparency_bitmap->line[y])[x] = code_opaque;

				pen = data>>4;
				yx = *pPenToPixel++;
				x = x0+(yx%MAX_TILESIZE);
				y = y0+(yx/MAX_TILESIZE);
				*(x+(UINT16 *)pixmap->line[y]) = PAL_GET(pen);
				((UINT8 *)transparency_bitmap->line[y])[x] = code_opaque;
			}
			pPenData += pitch/2;
		}
	}
	else
	{
		for( ty=tile_height; ty!=0; ty-- )
		{
			pSource = pPenData;
			for( tx=tile_width; tx!=0; tx-- )
			{
				pen = *pSource++;
				yx = *pPenToPixel++;
				x = x0+(yx%MAX_TILESIZE);
				y = y0+(yx/MAX_TILESIZE);
				*(x+(UINT16 *)pixmap->line[y]) = PAL_GET(pen);
				((UINT8 *)transparency_bitmap->line[y])[x] = code_opaque;
			}
			pPenData += pitch;
		}
	}
	return 0;
}

#undef TRANSP
#undef PAL_INIT
#undef PAL_GET
#endif // TRANSP
