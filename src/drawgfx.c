

#ifndef DECLARE

#include "driver.h"


#ifdef LSB_FIRST
#define SHIFT0 0
#define SHIFT1 8
#define SHIFT2 16
#define SHIFT3 24
#else
#define SHIFT3 0
#define SHIFT2 8
#define SHIFT1 16
#define SHIFT0 24
#endif


typedef void (*plot_pixel_proc)(struct mame_bitmap *bitmap,int x,int y,pen_t pen);
typedef pen_t (*read_pixel_proc)(struct mame_bitmap *bitmap,int x,int y);
typedef void (*plot_box_proc)(struct mame_bitmap *bitmap,int x,int y,int width,int height,pen_t pen);


UINT8 gfx_drawmode_table[256];
UINT8 gfx_alpharange_table[256];

static UINT8 is_raw[TRANSPARENCY_MODES];


#ifdef ALIGN_INTS /* GSL 980108 read/write nonaligned dword routine for ARM processor etc */

INLINE UINT32 read_dword(void *address)
{
	if ((long)address & 3)
	{
		return	(*((UINT8 *)address  ) << SHIFT0) +
				(*((UINT8 *)address+1) << SHIFT1) +
				(*((UINT8 *)address+2) << SHIFT2) +
				(*((UINT8 *)address+3) << SHIFT3);
	}
	else
		return *(UINT32 *)address;
}


INLINE void write_dword(void *address, UINT32 data)
{
	if ((long)address & 3)
	{
		*((UINT8 *)address)   = (data>>SHIFT0);
		*((UINT8 *)address+1) = (data>>SHIFT1);
		*((UINT8 *)address+2) = (data>>SHIFT2);
		*((UINT8 *)address+3) = (data>>SHIFT3);
		return;
	}
	else
		*(UINT32 *)address = data;
}
#else
#define read_dword(address) *(int *)address
#define write_dword(address,data) *(int *)address=data
#endif



INLINE int readbit(const UINT8 *src,int bitnum)
{
	return src[bitnum / 8] & (0x80 >> (bitnum % 8));
}

struct _alpha_cache alpha_cache;
int alpha_active;

void alpha_init(void)
{
	int lev, byte;
	for(lev=0; lev<257; lev++)
		for(byte=0; byte<256; byte++)
			alpha_cache.alpha[lev][byte] = (byte*lev) >> 8;
	alpha_set_level(255);
}


static void calc_penusage(struct GfxElement *gfx,int num)
{
	int x,y;
	UINT8 *dp;

	if (!gfx->pen_usage) return;

	/* fill the pen_usage array with info on the used pens */
	gfx->pen_usage[num] = 0;

	dp = gfx->gfxdata + num * gfx->char_modulo;

	if (gfx->flags & GFX_PACKED)
	{
		for (y = 0;y < gfx->height;y++)
		{
			for (x = 0;x < gfx->width/2;x++)
			{
				gfx->pen_usage[num] |= 1 << (dp[x] & 0x0f);
				gfx->pen_usage[num] |= 1 << (dp[x] >> 4);
			}
			dp += gfx->line_modulo;
		}
	}
	else
	{
		for (y = 0;y < gfx->height;y++)
		{
			for (x = 0;x < gfx->width;x++)
			{
				gfx->pen_usage[num] |= 1 << dp[x];
			}
			dp += gfx->line_modulo;
		}
	}
}

void decodechar(struct GfxElement *gfx,int num,const UINT8 *src,const struct GfxLayout *gl)
{
	int plane,x,y;
	UINT8 *dp;
	int baseoffs;
	const UINT32 *xoffset,*yoffset;


	xoffset = gl->xoffset;
	yoffset = gl->yoffset;

	dp = gfx->gfxdata + num * gfx->char_modulo;
	memset(dp,0,gfx->char_modulo);

	baseoffs = num * gl->charincrement;

	if (gfx->flags & GFX_PACKED)
	{
		for (plane = 0;plane < gl->planes;plane++)
		{
			int shiftedbit = 1 << (gl->planes-1-plane);
			int offs = baseoffs + gl->planeoffset[plane];

			dp = gfx->gfxdata + num * gfx->char_modulo + (gfx->height-1) * gfx->line_modulo;

			y = gfx->height;
			while (--y >= 0)
			{
				int offs2 = offs + yoffset[y];

				x = gfx->width/2;
				while (--x >= 0)
				{
					if (readbit(src,offs2 + xoffset[2*x+1]))
						dp[x] |= shiftedbit << 4;
					if (readbit(src,offs2 + xoffset[2*x]))
						dp[x] |= shiftedbit;
				}
				dp -= gfx->line_modulo;
			}
		}
	}
	else
	{
		for (plane = 0;plane < gl->planes;plane++)
		{
			int shiftedbit = 1 << (gl->planes-1-plane);
			int offs = baseoffs + gl->planeoffset[plane];

			dp = gfx->gfxdata + num * gfx->char_modulo + (gfx->height-1) * gfx->line_modulo;

#ifdef PREROTATE_GFX
			y = gfx->height;
			while (--y >= 0)
			{
				int yoffs;

				yoffs = y;
				x = gfx->width;
				while (--x >= 0)
				{
					int xoffs;

					xoffs = x;
					if (readbit(src,offs + xoffset[xoffs] + yoffset[yoffs]))
						dp[x] |= shiftedbit;
				}
				dp -= gfx->line_modulo;
			}
#else
			y = gfx->height;
			while (--y >= 0)
			{
				int offs2 = offs + yoffset[y];

				x = gfx->width;
				while (--x >= 0)
				{
					if (readbit(src,offs2 + xoffset[x]))
						dp[x] |= shiftedbit;
				}
				dp -= gfx->line_modulo;
			}
#endif
		}
	}

	calc_penusage(gfx,num);
}


struct GfxElement *decodegfx(const UINT8 *src,const struct GfxLayout *gl)
{
	int c;
	struct GfxElement *gfx;


	if ((gfx = malloc(sizeof(struct GfxElement))) == 0)
		return 0;
	memset(gfx,0,sizeof(struct GfxElement));

	gfx->width = gl->width;
	gfx->height = gl->height;

	gfx->total_elements = gl->total;
	gfx->color_granularity = 1 << gl->planes;

	gfx->pen_usage = 0; /* need to make sure this is NULL if the next test fails) */
	if (gfx->color_granularity <= 32)	/* can't handle more than 32 pens */
		gfx->pen_usage = malloc(gfx->total_elements * sizeof(int));
		/* no need to check for failure, the code can work without pen_usage */

	if (gl->planeoffset[0] == GFX_RAW)
	{
		if (gl->planes <= 4) gfx->flags |= GFX_PACKED;

		gfx->line_modulo = gl->yoffset[0] / 8;
		gfx->char_modulo = gl->charincrement / 8;

		gfx->gfxdata = (UINT8 *)src + gl->xoffset[0] / 8;
		gfx->flags |= GFX_DONT_FREE_GFXDATA;

		for (c = 0;c < gfx->total_elements;c++)
			calc_penusage(gfx,c);
	}
	else
	{
		if (0 && gl->planes <= 4 && !(gfx->width & 1))
//		if (gl->planes <= 4 && !(gfx->width & 1))
		{
			gfx->flags |= GFX_PACKED;
			gfx->line_modulo = gfx->width/2;
		}
		else
			gfx->line_modulo = gfx->width;
		gfx->char_modulo = gfx->line_modulo * gfx->height;

		if ((gfx->gfxdata = malloc(gfx->total_elements * gfx->char_modulo * sizeof(UINT8))) == 0)
		{
			free(gfx->pen_usage);
			free(gfx);
			return 0;
		}

		for (c = 0;c < gfx->total_elements;c++)
			decodechar(gfx,c,src,gl);
	}

	return gfx;
}


void freegfx(struct GfxElement *gfx)
{
	if (gfx)
	{
		free(gfx->pen_usage);
		if (!(gfx->flags & GFX_DONT_FREE_GFXDATA))
			free(gfx->gfxdata);
		free(gfx);
	}
}




INLINE void blockmove_NtoN_transpen_noremap8(
		const UINT8 *srcdata,int srcwidth,int srcheight,int srcmodulo,
		UINT8 *dstdata,int dstmodulo,
		int transpen)
{
	UINT8 *end;
	int trans4;
	UINT32 *sd4;

	srcmodulo -= srcwidth;
	dstmodulo -= srcwidth;

	trans4 = transpen * 0x01010101;

	while (srcheight)
	{
		end = dstdata + srcwidth;
		while (((long)srcdata & 3) && dstdata < end)	/* longword align */
		{
			int col;

			col = *(srcdata++);
			if (col != transpen) *dstdata = col;
			dstdata++;
		}
		sd4 = (UINT32 *)srcdata;
		while (dstdata <= end - 4)
		{
			UINT32 col4;

			if ((col4 = *(sd4++)) != trans4)
			{
				UINT32 xod4;

				xod4 = col4 ^ trans4;
				if( (xod4&0x000000ff) && (xod4&0x0000ff00) &&
					(xod4&0x00ff0000) && (xod4&0xff000000) )
				{
					write_dword((UINT32 *)dstdata,col4);
				}
				else
				{
					if (xod4 & (0xff<<SHIFT0)) dstdata[0] = col4>>SHIFT0;
					if (xod4 & (0xff<<SHIFT1)) dstdata[1] = col4>>SHIFT1;
					if (xod4 & (0xff<<SHIFT2)) dstdata[2] = col4>>SHIFT2;
					if (xod4 & (0xff<<SHIFT3)) dstdata[3] = col4>>SHIFT3;
				}
			}
			dstdata += 4;
		}
		srcdata = (UINT8 *)sd4;
		while (dstdata < end)
		{
			int col;

			col = *(srcdata++);
			if (col != transpen) *dstdata = col;
			dstdata++;
		}

		srcdata += srcmodulo;
		dstdata += dstmodulo;
		srcheight--;
	}
}

INLINE void blockmove_NtoN_transpen_noremap_flipx8(
		const UINT8 *srcdata,int srcwidth,int srcheight,int srcmodulo,
		UINT8 *dstdata,int dstmodulo,
		int transpen)
{
	UINT8 *end;
	int trans4;
	UINT32 *sd4;

	srcmodulo += srcwidth;
	dstmodulo -= srcwidth;
	//srcdata += srcwidth-1;
	srcdata -= 3;

	trans4 = transpen * 0x01010101;

	while (srcheight)
	{
		end = dstdata + srcwidth;
		while (((long)srcdata & 3) && dstdata < end)	/* longword align */
		{
			int col;

			col = srcdata[3];
			srcdata--;
			if (col != transpen) *dstdata = col;
			dstdata++;
		}
		sd4 = (UINT32 *)srcdata;
		while (dstdata <= end - 4)
		{
			UINT32 col4;

			if ((col4 = *(sd4--)) != trans4)
			{
				UINT32 xod4;

				xod4 = col4 ^ trans4;
				if (xod4 & (0xff<<SHIFT0)) dstdata[3] = (col4>>SHIFT0);
				if (xod4 & (0xff<<SHIFT1)) dstdata[2] = (col4>>SHIFT1);
				if (xod4 & (0xff<<SHIFT2)) dstdata[1] = (col4>>SHIFT2);
				if (xod4 & (0xff<<SHIFT3)) dstdata[0] = (col4>>SHIFT3);
			}
			dstdata += 4;
		}
		srcdata = (UINT8 *)sd4;
		while (dstdata < end)
		{
			int col;

			col = srcdata[3];
			srcdata--;
			if (col != transpen) *dstdata = col;
			dstdata++;
		}

		srcdata += srcmodulo;
		dstdata += dstmodulo;
		srcheight--;
	}
}


INLINE void blockmove_NtoN_transpen_noremap16(
		const UINT16 *srcdata,int srcwidth,int srcheight,int srcmodulo,
		UINT16 *dstdata,int dstmodulo,
		int transpen)
{
	UINT16 *end;

	srcmodulo -= srcwidth;
	dstmodulo -= srcwidth;

	while (srcheight)
	{
		end = dstdata + srcwidth;
		while (dstdata < end)
		{
			int col;

			col = *(srcdata++);
			if (col != transpen) *dstdata = col;
			dstdata++;
		}

		srcdata += srcmodulo;
		dstdata += dstmodulo;
		srcheight--;
	}
}

INLINE void blockmove_NtoN_transpen_noremap_flipx16(
		const UINT16 *srcdata,int srcwidth,int srcheight,int srcmodulo,
		UINT16 *dstdata,int dstmodulo,
		int transpen)
{
	UINT16 *end;

	srcmodulo += srcwidth;
	dstmodulo -= srcwidth;
	//srcdata += srcwidth-1;

	while (srcheight)
	{
		end = dstdata + srcwidth;
		while (dstdata < end)
		{
			int col;

			col = *(srcdata--);
			if (col != transpen) *dstdata = col;
			dstdata++;
		}

		srcdata += srcmodulo;
		dstdata += dstmodulo;
		srcheight--;
	}
}

INLINE void blockmove_NtoN_transpen_noremap32(
		const UINT32 *srcdata,int srcwidth,int srcheight,int srcmodulo,
		UINT32 *dstdata,int dstmodulo,
		int transpen)
{
	UINT32 *end;

	srcmodulo -= srcwidth;
	dstmodulo -= srcwidth;

	while (srcheight)
	{
		end = dstdata + srcwidth;
		while (dstdata < end)
		{
			int col;

			col = *(srcdata++);
			if (col != transpen) *dstdata = col;
			dstdata++;
		}

		srcdata += srcmodulo;
		dstdata += dstmodulo;
		srcheight--;
	}
}

INLINE void blockmove_NtoN_transpen_noremap_flipx32(
		const UINT32 *srcdata,int srcwidth,int srcheight,int srcmodulo,
		UINT32 *dstdata,int dstmodulo,
		int transpen)
{
	UINT32 *end;

	srcmodulo += srcwidth;
	dstmodulo -= srcwidth;
	//srcdata += srcwidth-1;

	while (srcheight)
	{
		end = dstdata + srcwidth;
		while (dstdata < end)
		{
			int col;

			col = *(srcdata--);
			if (col != transpen) *dstdata = col;
			dstdata++;
		}

		srcdata += srcmodulo;
		dstdata += dstmodulo;
		srcheight--;
	}
}



static int afterdrawmask = 31;
int pdrawgfx_shadow_lowpri = 0;


/* 8-bit version */
#define DATA_TYPE UINT8
#define DEPTH 8

#define DECLARE(function,args,body)
#define DECLAREG(function,args,body)

#define HMODULO 1
#define VMODULO dstmodulo
#define COMMON_ARGS 													\
		const UINT8 *srcdata,int srcwidth,int srcheight,int srcmodulo,	\
		int leftskip,int topskip,int flipx,int flipy,					\
		DATA_TYPE *dstdata,int dstwidth,int dstheight,int dstmodulo

#define COLOR_ARG unsigned int colorbase,UINT8 *pridata,UINT32 pmask
#define INCREMENT_DST(n) {dstdata+=(n);pridata += (n);}
#define LOOKUP(n) (colorbase + (n))
#define SETPIXELCOLOR(dest,n) { if (((1 << (pridata[dest] & 0x1f)) & pmask) == 0) { if (pridata[dest] & 0x80) { dstdata[dest] = palette_shadow_table[n];} else { dstdata[dest] = (n);} } pridata[dest] = (pridata[dest] & 0x7f) | afterdrawmask; }
#define DECLARE_SWAP_RAW_PRI(function,args,body) void function##_raw_pri8 args body
#include "drawgfx_inc.c"
#undef DECLARE_SWAP_RAW_PRI
#undef COLOR_ARG
#undef LOOKUP
#undef SETPIXELCOLOR

#define COLOR_ARG const pen_t *paldata,UINT8 *pridata,UINT32 pmask
#define LOOKUP(n) (paldata[n])
#define SETPIXELCOLOR(dest,n) { if (((1 << (pridata[dest] & 0x1f)) & pmask) == 0) { if (pridata[dest] & 0x80) { dstdata[dest] = palette_shadow_table[n];} else { dstdata[dest] = (n);} } pridata[dest] = (pridata[dest] & 0x7f) | afterdrawmask; }
#define DECLARE_SWAP_RAW_PRI(function,args,body) void function##_pri8 args body
#include "drawgfx_inc.c"
#undef DECLARE_SWAP_RAW_PRI
#undef COLOR_ARG
#undef LOOKUP
#undef INCREMENT_DST
#undef SETPIXELCOLOR

#define COLOR_ARG unsigned int colorbase
#define INCREMENT_DST(n) {dstdata+=(n);}
#define LOOKUP(n) (colorbase + (n))
#define SETPIXELCOLOR(dest,n) {dstdata[dest] = (n);}
#define DECLARE_SWAP_RAW_PRI(function,args,body) void function##_raw8 args body
#include "drawgfx_inc.c"
#undef DECLARE_SWAP_RAW_PRI
#undef COLOR_ARG
#undef LOOKUP
#undef SETPIXELCOLOR

#define COLOR_ARG const pen_t *paldata
#define LOOKUP(n) (paldata[n])
#define SETPIXELCOLOR(dest,n) {dstdata[dest] = (n);}
#define DECLARE_SWAP_RAW_PRI(function,args,body) void function##8 args body
#include "drawgfx_inc.c"
#undef DECLARE_SWAP_RAW_PRI
#undef COLOR_ARG
#undef LOOKUP
#undef INCREMENT_DST
#undef SETPIXELCOLOR

#undef HMODULO
#undef VMODULO
#undef COMMON_ARGS
#undef DECLARE
#undef DECLAREG

#define DECLARE(function,args,body) void function##8 args body
#define DECLAREG(function,args,body) void function##8 args body
#define DECLARE_SWAP_RAW_PRI(function,args,body)
#define BLOCKMOVE(function,flipx,args) \
	if (flipx) blockmove_##function##_flipx##8 args ; \
	else blockmove_##function##8 args
#define BLOCKMOVELU(function,args) \
	blockmove_##function##8 args
#define BLOCKMOVERAW(function,args) \
	blockmove_##function##_raw##8 args
#define BLOCKMOVEPRI(function,args) \
	blockmove_##function##_pri##8 args
#define BLOCKMOVERAWPRI(function,args) \
	blockmove_##function##_raw_pri##8 args
#include "drawgfx_inc.c"
#undef DECLARE
#undef DECLARE_SWAP_RAW_PRI
#undef DECLAREG
#undef BLOCKMOVE
#undef BLOCKMOVELU
#undef BLOCKMOVERAW
#undef BLOCKMOVEPRI
#undef BLOCKMOVERAWPRI

#undef DEPTH
#undef DATA_TYPE

/* 16-bit version */
#define DATA_TYPE UINT16
#define DEPTH 16
#define alpha_blend_r alpha_blend_r16
#define alpha_blend alpha_blend16

#define DECLARE(function,args,body)
#define DECLAREG(function,args,body)

#define HMODULO 1
#define VMODULO dstmodulo
#define COMMON_ARGS 													\
		const UINT8 *srcdata,int srcwidth,int srcheight,int srcmodulo,	\
		int leftskip,int topskip,int flipx,int flipy,					\
		DATA_TYPE *dstdata,int dstwidth,int dstheight,int dstmodulo

#define COLOR_ARG unsigned int colorbase,UINT8 *pridata,UINT32 pmask
#define INCREMENT_DST(n) {dstdata+=(n);pridata += (n);}
#define LOOKUP(n) (colorbase + (n))
#define SETPIXELCOLOR(dest,n) { if (((1 << (pridata[dest] & 0x1f)) & pmask) == 0) { if (pridata[dest] & 0x80) { dstdata[dest] = palette_shadow_table[n];} else { dstdata[dest] = (n);} } pridata[dest] = (pridata[dest] & 0x7f) | afterdrawmask; }
#define DECLARE_SWAP_RAW_PRI(function,args,body) void function##_raw_pri16 args body
#include "drawgfx_inc.c"
#undef DECLARE_SWAP_RAW_PRI
#undef COLOR_ARG
#undef LOOKUP
#undef SETPIXELCOLOR

#define COLOR_ARG const pen_t *paldata,UINT8 *pridata,UINT32 pmask
#define LOOKUP(n) (paldata[n])
#define SETPIXELCOLOR(dest,n) { if (((1 << (pridata[dest] & 0x1f)) & pmask) == 0) { if (pridata[dest] & 0x80) { dstdata[dest] = palette_shadow_table[n];} else { dstdata[dest] = (n);} } pridata[dest] = (pridata[dest] & 0x7f) | afterdrawmask; }
#define DECLARE_SWAP_RAW_PRI(function,args,body) void function##_pri16 args body
#include "drawgfx_inc.c"
#undef DECLARE_SWAP_RAW_PRI
#undef COLOR_ARG
#undef LOOKUP
#undef INCREMENT_DST
#undef SETPIXELCOLOR

#define COLOR_ARG unsigned int colorbase
#define INCREMENT_DST(n) {dstdata+=(n);}
#define LOOKUP(n) (colorbase + (n))
#define SETPIXELCOLOR(dest,n) {dstdata[dest] = (n);}
#define DECLARE_SWAP_RAW_PRI(function,args,body) void function##_raw16 args body
#include "drawgfx_inc.c"
#undef DECLARE_SWAP_RAW_PRI
#undef COLOR_ARG
#undef LOOKUP
#undef SETPIXELCOLOR

#define COLOR_ARG const pen_t *paldata
#define LOOKUP(n) (paldata[n])
#define SETPIXELCOLOR(dest,n) {dstdata[dest] = (n);}
#define DECLARE_SWAP_RAW_PRI(function,args,body) void function##16 args body
#include "drawgfx_inc.c"
#undef DECLARE_SWAP_RAW_PRI
#undef COLOR_ARG
#undef LOOKUP
#undef INCREMENT_DST
#undef SETPIXELCOLOR

#undef HMODULO
#undef VMODULO
#undef COMMON_ARGS
#undef DECLARE
#undef DECLAREG

#define DECLARE(function,args,body) void function##16 args body
#define DECLAREG(function,args,body) void function##16 args body
#define DECLARE_SWAP_RAW_PRI(function,args,body)
#define BLOCKMOVE(function,flipx,args) \
	if (flipx) blockmove_##function##_flipx##16 args ; \
	else blockmove_##function##16 args
#define BLOCKMOVELU(function,args) \
	blockmove_##function##16 args
#define BLOCKMOVERAW(function,args) \
	blockmove_##function##_raw##16 args
#define BLOCKMOVEPRI(function,args) \
	blockmove_##function##_pri##16 args
#define BLOCKMOVERAWPRI(function,args) \
	blockmove_##function##_raw_pri##16 args
#include "drawgfx_inc.c"
#undef DECLARE
#undef DECLARE_SWAP_RAW_PRI
#undef DECLAREG
#undef BLOCKMOVE
#undef BLOCKMOVELU
#undef BLOCKMOVERAW
#undef BLOCKMOVEPRI
#undef BLOCKMOVERAWPRI

#undef DEPTH
#undef DATA_TYPE
#undef alpha_blend_r
#undef alpha_blend

#if (0==PSP_FORCE16)
/* 32-bit version */
//* AAT032503: added limited 32-bit shadow and highlight support
INLINE UINT32 SHADOW32(UINT32 c) {
	c = (c>>9&0x7c00) | (c>>6&0x03e0) | (c>>3&0x001f);
	return(((UINT32*)palette_shadow_table)[c]); }

#define DATA_TYPE UINT32
#define DEPTH 32
#define alpha_blend_r alpha_blend_r32
#define alpha_blend alpha_blend32

#define DECLARE(function,args,body)
#define DECLAREG(function,args,body)

#define HMODULO 1
#define VMODULO dstmodulo
#define COMMON_ARGS 													\
		const UINT8 *srcdata,int srcwidth,int srcheight,int srcmodulo,	\
		int leftskip,int topskip,int flipx,int flipy,					\
		DATA_TYPE *dstdata,int dstwidth,int dstheight,int dstmodulo

#define COLOR_ARG unsigned int colorbase,UINT8 *pridata,UINT32 pmask
#define INCREMENT_DST(n) {dstdata+=(n);pridata += (n);}
#define LOOKUP(n) (colorbase + (n))
#define SETPIXELCOLOR(dest,n) { UINT8 r8=pridata[dest]; if(!(1<<(r8&0x1f)&pmask)){ if(afterdrawmask){ r8&=0x7f; r8|=0x1f; dstdata[dest]=(n); pridata[dest]=r8; } else if(!(r8&0x80)){ dstdata[dest]=SHADOW32(n); pridata[dest]|=0x80; } } }
#define DECLARE_SWAP_RAW_PRI(function,args,body) void function##_raw_pri32 args body
#include "drawgfx_inc.c"
#undef DECLARE_SWAP_RAW_PRI
#undef COLOR_ARG
#undef LOOKUP
#undef SETPIXELCOLOR

#define COLOR_ARG const pen_t *paldata,UINT8 *pridata,UINT32 pmask
#define LOOKUP(n) (paldata[n])
#define SETPIXELCOLOR(dest,n) { UINT8 r8=pridata[dest]; if(!(1<<(r8&0x1f)&pmask)){ if(afterdrawmask){ r8&=0x7f; r8|=0x1f; dstdata[dest]=(n); pridata[dest]=r8; } else if(!(r8&0x80)){ dstdata[dest]=SHADOW32(n); pridata[dest]|=0x80; } } }
#define DECLARE_SWAP_RAW_PRI(function,args,body) void function##_pri32 args body
#include "drawgfx_inc.c"
#undef DECLARE_SWAP_RAW_PRI
#undef COLOR_ARG
#undef LOOKUP
#undef INCREMENT_DST
#undef SETPIXELCOLOR

#define COLOR_ARG unsigned int colorbase
#define INCREMENT_DST(n) {dstdata+=(n);}
#define LOOKUP(n) (colorbase + (n))
#define SETPIXELCOLOR(dest,n) {dstdata[dest] = (n);}
#define DECLARE_SWAP_RAW_PRI(function,args,body) void function##_raw32 args body
#include "drawgfx_inc.c"
#undef DECLARE_SWAP_RAW_PRI
#undef COLOR_ARG
#undef LOOKUP
#undef SETPIXELCOLOR

#define COLOR_ARG const pen_t *paldata
#define LOOKUP(n) (paldata[n])
#define SETPIXELCOLOR(dest,n) {dstdata[dest] = (n);}
#define DECLARE_SWAP_RAW_PRI(function,args,body) void function##32 args body
#include "drawgfx_inc.c"
#undef DECLARE_SWAP_RAW_PRI
#undef COLOR_ARG
#undef LOOKUP
#undef INCREMENT_DST
#undef SETPIXELCOLOR

#undef HMODULO
#undef VMODULO
#undef COMMON_ARGS
#undef DECLARE
#undef DECLAREG

#define DECLARE(function,args,body) void function##32 args body
#define DECLAREG(function,args,body) void function##32 args body
#define DECLARE_SWAP_RAW_PRI(function,args,body)
#define BLOCKMOVE(function,flipx,args) \
	if (flipx) blockmove_##function##_flipx##32 args ; \
	else blockmove_##function##32 args
#define BLOCKMOVELU(function,args)		blockmove_##function##32 args
#define BLOCKMOVERAW(function,args) 	blockmove_##function##_raw##32 args
#define BLOCKMOVEPRI(function,args) 	blockmove_##function##_pri##32 args
#define BLOCKMOVERAWPRI(function,args)	blockmove_##function##_raw_pri##32 args
#include "drawgfx_inc.c"
#undef DECLARE
#undef DECLARE_SWAP_RAW_PRI
#undef DECLAREG
#undef BLOCKMOVE
#undef BLOCKMOVELU
#undef BLOCKMOVERAW
#undef BLOCKMOVEPRI
#undef BLOCKMOVERAWPRI

#undef DEPTH
#undef DATA_TYPE
#undef alpha_blend_r
#undef alpha_blend
#endif //(PSP_FORCE16)


/***************************************************************************

  Draw graphic elements in the specified bitmap.

  transparency == TRANSPARENCY_NONE - no transparency.
  transparency == TRANSPARENCY_PEN - bits whose _original_ value is == transparent_color
									 are transparent. This is the most common kind of
									 transparency.
  transparency == TRANSPARENCY_PENS - as above, but transparent_color is a mask of
									 transparent pens.
  transparency == TRANSPARENCY_COLOR - bits whose _remapped_ palette index (taken from
									 Machine->game_colortable) is == transparent_color

  transparency == TRANSPARENCY_PEN_TABLE - the transparency condition is same as TRANSPARENCY_PEN
					A special drawing is done according to gfx_drawmode_table[source pixel].
					DRAWMODE_NONE	   transparent
					DRAWMODE_SOURCE    normal, draw source pixel.
					DRAWMODE_SHADOW    destination is changed through palette_shadow_table[]

***************************************************************************/

INLINE void common_drawgfx(struct mame_bitmap *dest,const struct GfxElement *gfx,
		unsigned int code,unsigned int color,int flipx,int flipy,int sx,int sy,
		const struct rectangle *clip,int transparency,int transparent_color,
		struct mame_bitmap *pri_buffer,UINT32 pri_mask)
{
	if (!gfx)
	{
		usrintf_showmessage("drawgfx() gfx == 0");
		return;
	}
	if (!gfx->colortable && !is_raw[transparency])
	{
		usrintf_showmessage("drawgfx() gfx->colortable == 0");
		return;
	}

	code %= gfx->total_elements;
	if (!is_raw[transparency])
		color %= gfx->total_colors;

	if (!alpha_active && (transparency == TRANSPARENCY_ALPHAONE || transparency == TRANSPARENCY_ALPHA || transparency == TRANSPARENCY_ALPHARANGE))
	{
		if (transparency == TRANSPARENCY_ALPHAONE && (cpu_getcurrentframe() & 1))
		{
			transparency = TRANSPARENCY_PENS;
			transparent_color = (1 << (transparent_color & 0xff))|(1 << (transparent_color >> 8));
		}
		else
		{
			transparency = TRANSPARENCY_PEN;
			transparent_color &= 0xff;
		}
	}

	if (gfx->pen_usage && (transparency == TRANSPARENCY_PEN || transparency == TRANSPARENCY_PENS))
	{
		int transmask = 0;

		if (transparency == TRANSPARENCY_PEN)
		{
			transmask = 1 << (transparent_color & 0xff);
		}
		else	/* transparency == TRANSPARENCY_PENS */
		{
			transmask = transparent_color;
		}

		if ((gfx->pen_usage[code] & ~transmask) == 0)
			/* character is totally transparent, no need to draw */
			return;
		else if ((gfx->pen_usage[code] & transmask) == 0)
			/* character is totally opaque, can disable transparency */
			transparency = TRANSPARENCY_NONE;
	}

	if (dest->depth == 8)
		drawgfx_core8(dest,gfx,code,color,flipx,flipy,sx,sy,clip,transparency,transparent_color,pri_buffer,pri_mask);
#if (0==PSP_FORCE16)
	else if(dest->depth == 15 || dest->depth == 16)
		drawgfx_core16(dest,gfx,code,color,flipx,flipy,sx,sy,clip,transparency,transparent_color,pri_buffer,pri_mask);
	else
		drawgfx_core32(dest,gfx,code,color,flipx,flipy,sx,sy,clip,transparency,transparent_color,pri_buffer,pri_mask);
#else
	else
		drawgfx_core16(dest,gfx,code,color,flipx,flipy,sx,sy,clip,transparency,transparent_color,pri_buffer,pri_mask);
#endif //(PSP_FORCE16)
}

void drawgfx(struct mame_bitmap *dest,const struct GfxElement *gfx,
		unsigned int code,unsigned int color,int flipx,int flipy,int sx,int sy,
		const struct rectangle *clip,int transparency,int transparent_color)
{
	profiler_mark(PROFILER_DRAWGFX);
	common_drawgfx(dest,gfx,code,color,flipx,flipy,sx,sy,clip,transparency,transparent_color,NULL,0);
	profiler_mark(PROFILER_END);
}

void pdrawgfx(struct mame_bitmap *dest,const struct GfxElement *gfx,
		unsigned int code,unsigned int color,int flipx,int flipy,int sx,int sy,
		const struct rectangle *clip,int transparency,int transparent_color,UINT32 priority_mask)
{
	profiler_mark(PROFILER_DRAWGFX);
	common_drawgfx(dest,gfx,code,color,flipx,flipy,sx,sy,clip,transparency,transparent_color,priority_bitmap,priority_mask | (1<<31));
	profiler_mark(PROFILER_END);
}

void mdrawgfx(struct mame_bitmap *dest,const struct GfxElement *gfx,
		unsigned int code,unsigned int color,int flipx,int flipy,int sx,int sy,
		const struct rectangle *clip,int transparency,int transparent_color,UINT32 priority_mask)
{
	profiler_mark(PROFILER_DRAWGFX);
	common_drawgfx(dest,gfx,code,color,flipx,flipy,sx,sy,clip,transparency,transparent_color,priority_bitmap,priority_mask);
	profiler_mark(PROFILER_END);
}


/***************************************************************************

  Use drawgfx() to copy a bitmap onto another at the given position.
  This function will very likely change in the future.

***************************************************************************/
void copybitmap(struct mame_bitmap *dest,struct mame_bitmap *src,int flipx,int flipy,int sx,int sy,
		const struct rectangle *clip,int transparency,int transparent_color)
{
	/* translate to proper transparency here */
	if (transparency == TRANSPARENCY_NONE)
		transparency = TRANSPARENCY_NONE_RAW;
	else if (transparency == TRANSPARENCY_PEN)
		transparency = TRANSPARENCY_PEN_RAW;
	else if (transparency == TRANSPARENCY_COLOR)
	{
		transparent_color = Machine->pens[transparent_color];
		transparency = TRANSPARENCY_PEN_RAW;
	}

	copybitmap_remap(dest,src,flipx,flipy,sx,sy,clip,transparency,transparent_color);
}


void copybitmap_remap(struct mame_bitmap *dest,struct mame_bitmap *src,int flipx,int flipy,int sx,int sy,
		const struct rectangle *clip,int transparency,int transparent_color)
{
	profiler_mark(PROFILER_COPYBITMAP);

	if (dest->depth == 8)
		copybitmap_core8(dest,src,flipx,flipy,sx,sy,clip,transparency,transparent_color);
#if (0==PSP_FORCE16)
	else if(dest->depth == 15 || dest->depth == 16)
		copybitmap_core16(dest,src,flipx,flipy,sx,sy,clip,transparency,transparent_color);
	else
		copybitmap_core32(dest,src,flipx,flipy,sx,sy,clip,transparency,transparent_color);
#else
	else
		copybitmap_core16(dest,src,flipx,flipy,sx,sy,clip,transparency,transparent_color);
#endif //(PSP_FORCE16)
	profiler_mark(PROFILER_END);
}



/***************************************************************************

  Copy a bitmap onto another with scroll and wraparound.
  This function supports multiple independently scrolling rows/columns.
  "rows" is the number of indepentently scrolling rows. "rowscroll" is an
  array of integers telling how much to scroll each row. Same thing for
  "cols" and "colscroll".
  If the bitmap cannot scroll in one direction, set rows or columns to 0.
  If the bitmap scrolls as a whole, set rows and/or cols to 1.
  Bidirectional scrolling is, of course, supported only if the bitmap
  scrolls as a whole in at least one direction.

***************************************************************************/
void copyscrollbitmap(struct mame_bitmap *dest,struct mame_bitmap *src,
		int rows,const int *rowscroll,int cols,const int *colscroll,
		const struct rectangle *clip,int transparency,int transparent_color)
{
	/* translate to proper transparency here */
	if (transparency == TRANSPARENCY_NONE)
		transparency = TRANSPARENCY_NONE_RAW;
	else if (transparency == TRANSPARENCY_PEN)
		transparency = TRANSPARENCY_PEN_RAW;
	else if (transparency == TRANSPARENCY_COLOR)
	{
		transparent_color = Machine->pens[transparent_color];
		transparency = TRANSPARENCY_PEN_RAW;
	}

	copyscrollbitmap_remap(dest,src,rows,rowscroll,cols,colscroll,clip,transparency,transparent_color);
}

void copyscrollbitmap_remap(struct mame_bitmap *dest,struct mame_bitmap *src,
		int rows,const int *rowscroll,int cols,const int *colscroll,
		const struct rectangle *clip,int transparency,int transparent_color)
{
	int srcwidth,srcheight,destwidth,destheight;
	struct rectangle orig_clip;


	if (clip)
	{
		orig_clip.min_x = clip->min_x;
		orig_clip.max_x = clip->max_x;
		orig_clip.min_y = clip->min_y;
		orig_clip.max_y = clip->max_y;
	}
	else
	{
		orig_clip.min_x = 0;
		orig_clip.max_x = dest->width-1;
		orig_clip.min_y = 0;
		orig_clip.max_y = dest->height-1;
	}
	clip = &orig_clip;

	if (rows == 0 && cols == 0)
	{
		copybitmap(dest,src,0,0,0,0,clip,transparency,transparent_color);
		return;
	}

	profiler_mark(PROFILER_COPYBITMAP);

	srcwidth = src->width;
	srcheight = src->height;
	destwidth = dest->width;
	destheight = dest->height;

	if (rows == 0)
	{
		/* scrolling columns */
		int col,colwidth;
		struct rectangle myclip;


		colwidth = srcwidth / cols;

		myclip.min_y = clip->min_y;
		myclip.max_y = clip->max_y;

		col = 0;
		while (col < cols)
		{
			int cons,scroll;


			/* count consecutive columns scrolled by the same amount */
			scroll = colscroll[col];
			cons = 1;
			while (col + cons < cols && colscroll[col + cons] == scroll)
				cons++;

			if (scroll < 0) scroll = srcheight - (-scroll) % srcheight;
			else scroll %= srcheight;

			myclip.min_x = col * colwidth;
			if (myclip.min_x < clip->min_x) myclip.min_x = clip->min_x;
			myclip.max_x = (col + cons) * colwidth - 1;
			if (myclip.max_x > clip->max_x) myclip.max_x = clip->max_x;

			copybitmap(dest,src,0,0,0,scroll,&myclip,transparency,transparent_color);
			copybitmap(dest,src,0,0,0,scroll - srcheight,&myclip,transparency,transparent_color);

			col += cons;
		}
	}
	else if (cols == 0)
	{
		/* scrolling rows */
		int row,rowheight;
		struct rectangle myclip;


		rowheight = srcheight / rows;

		myclip.min_x = clip->min_x;
		myclip.max_x = clip->max_x;

		row = 0;
		while (row < rows)
		{
			int cons,scroll;


			/* count consecutive rows scrolled by the same amount */
			scroll = rowscroll[row];
			cons = 1;
			while (row + cons < rows && rowscroll[row + cons] == scroll)
				cons++;

			if (scroll < 0) scroll = srcwidth - (-scroll) % srcwidth;
			else scroll %= srcwidth;

			myclip.min_y = row * rowheight;
			if (myclip.min_y < clip->min_y) myclip.min_y = clip->min_y;
			myclip.max_y = (row + cons) * rowheight - 1;
			if (myclip.max_y > clip->max_y) myclip.max_y = clip->max_y;

			copybitmap(dest,src,0,0,scroll,0,&myclip,transparency,transparent_color);
			copybitmap(dest,src,0,0,scroll - srcwidth,0,&myclip,transparency,transparent_color);

			row += cons;
		}
	}
	else if (rows == 1 && cols == 1)
	{
		/* XY scrolling playfield */
		int scrollx,scrolly,sx,sy;


		if (rowscroll[0] < 0) scrollx = srcwidth - (-rowscroll[0]) % srcwidth;
		else scrollx = rowscroll[0] % srcwidth;

		if (colscroll[0] < 0) scrolly = srcheight - (-colscroll[0]) % srcheight;
		else scrolly = colscroll[0] % srcheight;

		for (sx = scrollx - srcwidth;sx < destwidth;sx += srcwidth)
			for (sy = scrolly - srcheight;sy < destheight;sy += srcheight)
				copybitmap(dest,src,0,0,sx,sy,clip,transparency,transparent_color);
	}
	else if (rows == 1)
	{
		/* scrolling columns + horizontal scroll */
		int col,colwidth;
		int scrollx;
		struct rectangle myclip;


		if (rowscroll[0] < 0) scrollx = srcwidth - (-rowscroll[0]) % srcwidth;
		else scrollx = rowscroll[0] % srcwidth;

		colwidth = srcwidth / cols;

		myclip.min_y = clip->min_y;
		myclip.max_y = clip->max_y;

		col = 0;
		while (col < cols)
		{
			int cons,scroll;


			/* count consecutive columns scrolled by the same amount */
			scroll = colscroll[col];
			cons = 1;
			while (col + cons < cols && colscroll[col + cons] == scroll)
				cons++;

			if (scroll < 0) scroll = srcheight - (-scroll) % srcheight;
			else scroll %= srcheight;

			myclip.min_x = col * colwidth + scrollx;
			if (myclip.min_x < clip->min_x) myclip.min_x = clip->min_x;
			myclip.max_x = (col + cons) * colwidth - 1 + scrollx;
			if (myclip.max_x > clip->max_x) myclip.max_x = clip->max_x;

			copybitmap(dest,src,0,0,scrollx,scroll,&myclip,transparency,transparent_color);
			copybitmap(dest,src,0,0,scrollx,scroll - srcheight,&myclip,transparency,transparent_color);

			myclip.min_x = col * colwidth + scrollx - srcwidth;
			if (myclip.min_x < clip->min_x) myclip.min_x = clip->min_x;
			myclip.max_x = (col + cons) * colwidth - 1 + scrollx - srcwidth;
			if (myclip.max_x > clip->max_x) myclip.max_x = clip->max_x;

			copybitmap(dest,src,0,0,scrollx - srcwidth,scroll,&myclip,transparency,transparent_color);
			copybitmap(dest,src,0,0,scrollx - srcwidth,scroll - srcheight,&myclip,transparency,transparent_color);

			col += cons;
		}
	}
	else if (cols == 1)
	{
		/* scrolling rows + vertical scroll */
		int row,rowheight;
		int scrolly;
		struct rectangle myclip;


		if (colscroll[0] < 0) scrolly = srcheight - (-colscroll[0]) % srcheight;
		else scrolly = colscroll[0] % srcheight;

		rowheight = srcheight / rows;

		myclip.min_x = clip->min_x;
		myclip.max_x = clip->max_x;

		row = 0;
		while (row < rows)
		{
			int cons,scroll;


			/* count consecutive rows scrolled by the same amount */
			scroll = rowscroll[row];
			cons = 1;
			while (row + cons < rows && rowscroll[row + cons] == scroll)
				cons++;

			if (scroll < 0) scroll = srcwidth - (-scroll) % srcwidth;
			else scroll %= srcwidth;

			myclip.min_y = row * rowheight + scrolly;
			if (myclip.min_y < clip->min_y) myclip.min_y = clip->min_y;
			myclip.max_y = (row + cons) * rowheight - 1 + scrolly;
			if (myclip.max_y > clip->max_y) myclip.max_y = clip->max_y;

			copybitmap(dest,src,0,0,scroll,scrolly,&myclip,transparency,transparent_color);
			copybitmap(dest,src,0,0,scroll - srcwidth,scrolly,&myclip,transparency,transparent_color);

			myclip.min_y = row * rowheight + scrolly - srcheight;
			if (myclip.min_y < clip->min_y) myclip.min_y = clip->min_y;
			myclip.max_y = (row + cons) * rowheight - 1 + scrolly - srcheight;
			if (myclip.max_y > clip->max_y) myclip.max_y = clip->max_y;

			copybitmap(dest,src,0,0,scroll,scrolly - srcheight,&myclip,transparency,transparent_color);
			copybitmap(dest,src,0,0,scroll - srcwidth,scrolly - srcheight,&myclip,transparency,transparent_color);

			row += cons;
		}
	}

	profiler_mark(PROFILER_END);
}

#if (0==PSP_FORCE_NO_ROZ)
/* notes:
   - startx and starty MUST be UINT32 for calculations to work correctly
   - srcbitmap->width and height are assumed to be a power of 2 to speed up wraparound
   */
void copyrozbitmap(struct mame_bitmap *dest,struct mame_bitmap *src,
		UINT32 startx,UINT32 starty,int incxx,int incxy,int incyx,int incyy,int wraparound,
		const struct rectangle *clip,int transparency,int transparent_color,UINT32 priority)
{
	profiler_mark(PROFILER_COPYBITMAP);

	/* cheat, the core doesn't support TRANSPARENCY_NONE yet */
	if (transparency == TRANSPARENCY_NONE)
	{
		transparency = TRANSPARENCY_PEN;
		transparent_color = -1;
	}

	/* if necessary, remap the transparent color */
	if (transparency == TRANSPARENCY_COLOR)
	{
		transparency = TRANSPARENCY_PEN;
		transparent_color = Machine->pens[transparent_color];
	}

	if (transparency != TRANSPARENCY_PEN)
	{
		usrintf_showmessage("copyrozbitmap unsupported trans %02x",transparency);
		return;
	}

#if (0==PSP_FORCE16)
	if (dest->depth == 8)
		copyrozbitmap_core8(dest,src,startx,starty,incxx,incxy,incyx,incyy,wraparound,clip,transparency,transparent_color,priority);
	else if(dest->depth == 15 || dest->depth == 16)
		copyrozbitmap_core16(dest,src,startx,starty,incxx,incxy,incyx,incyy,wraparound,clip,transparency,transparent_color,priority);
	else
		copyrozbitmap_core32(dest,src,startx,starty,incxx,incxy,incyx,incyy,wraparound,clip,transparency,transparent_color,priority);
#else
	else
		copyrozbitmap_core16(dest,src,startx,starty,incxx,incxy,incyx,incyy,wraparound,clip,transparency,transparent_color,priority);
#endif //(PSP_FORCE16)

	profiler_mark(PROFILER_END);
}
#endif //(0==PSP_FORCE_NO_ROZ)



/* fill a bitmap using the specified pen */
void fillbitmap(struct mame_bitmap *dest,pen_t pen,const struct rectangle *clip)
{
	int sx,sy,ex,ey,y;

	sx = 0;
	ex = dest->width - 1;
	sy = 0;
	ey = dest->height - 1;

	if (clip && sx < clip->min_x) sx = clip->min_x;
	if (clip && ex > clip->max_x) ex = clip->max_x;
	if (sx > ex) return;
	if (clip && sy < clip->min_y) sy = clip->min_y;
	if (clip && ey > clip->max_y) ey = clip->max_y;
	if (sy > ey) return;

#if (0==PSP_FORCE16)
	if (dest->depth == 32)
	{
		if (((pen >> 8) == (pen & 0xff)) && ((pen>>16) == (pen & 0xff)))
		{
			for (y = sy;y <= ey;y++)
				memset(((UINT32 *)dest->line[y]) + sx,pen&0xff,(ex-sx+1)*4);
		}
		else
		{
			UINT32 *sp = (UINT32 *)dest->line[sy];
			int x;

			for (x = sx;x <= ex;x++)
				sp[x] = pen;
			sp+=sx;
			for (y = sy+1;y <= ey;y++)
				memcpy(((UINT32 *)dest->line[y]) + sx,sp,(ex-sx+1)*4);
		}
	}
	else
#endif //(PSP_FORCE16)
		if (dest->depth == 15 || dest->depth == 16)
	{
		if ((pen >> 8) == (pen & 0xff))
		{
			for (y = sy;y <= ey;y++)
				memset(((UINT16 *)dest->line[y]) + sx,pen&0xff,(ex-sx+1)*2);
		}
		else
		{
			UINT16 *sp = (UINT16 *)dest->line[sy];
			int x;

			for (x = sx;x <= ex;x++)
				sp[x] = pen;
			sp+=sx;
			for (y = sy+1;y <= ey;y++)
				memcpy(((UINT16 *)dest->line[y]) + sx,sp,(ex-sx+1)*2);
		}
	}
	else
	{
		for (y = sy;y <= ey;y++)
			memset(((UINT8 *)dest->line[y]) + sx,pen,ex-sx+1);
	}
}



INLINE void common_drawgfxzoom( struct mame_bitmap *dest_bmp,const struct GfxElement *gfx,
		unsigned int code,unsigned int color,int flipx,int flipy,int sx,int sy,
		const struct rectangle *clip,int transparency,int transparent_color,
		int scalex, int scaley,struct mame_bitmap *pri_buffer,UINT32 pri_mask)
{
	struct rectangle myclip;
	int alphapen = 0;

	UINT8 ah, al;

	al = (pdrawgfx_shadow_lowpri) ? 0 : 0x80;

	if (!scalex || !scaley) return;

	if (scalex == 0x10000 && scaley == 0x10000)
	{
		common_drawgfx(dest_bmp,gfx,code,color,flipx,flipy,sx,sy,clip,transparency,transparent_color,pri_buffer,pri_mask);
		return;
	}

	if (transparency != TRANSPARENCY_PEN && transparency != TRANSPARENCY_PEN_RAW
			&& transparency != TRANSPARENCY_PENS && transparency != TRANSPARENCY_COLOR
			&& transparency != TRANSPARENCY_PEN_TABLE && transparency != TRANSPARENCY_PEN_TABLE_RAW
			&& transparency != TRANSPARENCY_BLEND_RAW && transparency != TRANSPARENCY_ALPHAONE
			&& transparency != TRANSPARENCY_ALPHA && transparency != TRANSPARENCY_ALPHARANGE
			&& transparency != TRANSPARENCY_NONE)
	{
		usrintf_showmessage("drawgfxzoom unsupported trans %02x",transparency);
		return;
	}

	if (!alpha_active && (transparency == TRANSPARENCY_ALPHAONE || transparency == TRANSPARENCY_ALPHA || transparency == TRANSPARENCY_ALPHARANGE))
	{
		transparency = TRANSPARENCY_PEN;
		transparent_color &= 0xff;
	}

	if (transparency == TRANSPARENCY_ALPHAONE)
	{
		alphapen = transparent_color >> 8;
		transparent_color &= 0xff;
	}

	if (transparency == TRANSPARENCY_COLOR)
		transparent_color = Machine->pens[transparent_color];


	/*
	scalex and scaley are 16.16 fixed point numbers
	1<<15 : shrink to 50%
	1<<16 : uniform scale
	1<<17 : double to 200%
	*/


	/* KW 991012 -- Added code to force clip to bitmap boundary */
	if(clip)
	{
		myclip.min_x = clip->min_x;
		myclip.max_x = clip->max_x;
		myclip.min_y = clip->min_y;
		myclip.max_y = clip->max_y;

		if (myclip.min_x < 0) myclip.min_x = 0;
		if (myclip.max_x >= dest_bmp->width) myclip.max_x = dest_bmp->width-1;
		if (myclip.min_y < 0) myclip.min_y = 0;
		if (myclip.max_y >= dest_bmp->height) myclip.max_y = dest_bmp->height-1;

		clip=&myclip;
	}


	/* ASG 980209 -- added 16-bit version */
	if (dest_bmp->depth == 8)
	{
		if( gfx && gfx->colortable )
		{
			const pen_t *pal = &gfx->colortable[gfx->color_granularity * (color % gfx->total_colors)]; /* ASG 980209 */
			UINT8 *source_base = gfx->gfxdata + (code % gfx->total_elements) * gfx->char_modulo;

			int sprite_screen_height = (scaley*gfx->height+0x8000)>>16;
			int sprite_screen_width = (scalex*gfx->width+0x8000)>>16;

			if (sprite_screen_width && sprite_screen_height)
			{
				/* compute sprite increment per screen pixel */
				int dx = (gfx->width<<16)/sprite_screen_width;
				int dy = (gfx->height<<16)/sprite_screen_height;

				int ex = sx+sprite_screen_width;
				int ey = sy+sprite_screen_height;

				int x_index_base;
				int y_index;

				if( flipx )
				{
					x_index_base = (sprite_screen_width-1)*dx;
					dx = -dx;
				}
				else
				{
					x_index_base = 0;
				}

				if( flipy )
				{
					y_index = (sprite_screen_height-1)*dy;
					dy = -dy;
				}
				else
				{
					y_index = 0;
				}

				if( clip )
				{
					if( sx < clip->min_x)
					{ /* clip left */
						int pixels = clip->min_x-sx;
						sx += pixels;
						x_index_base += pixels*dx;
					}
					if( sy < clip->min_y )
					{ /* clip top */
						int pixels = clip->min_y-sy;
						sy += pixels;
						y_index += pixels*dy;
					}
					/* NS 980211 - fixed incorrect clipping */
					if( ex > clip->max_x+1 )
					{ /* clip right */
						int pixels = ex-clip->max_x-1;
						ex -= pixels;
					}
					if( ey > clip->max_y+1 )
					{ /* clip bottom */
						int pixels = ey-clip->max_y-1;
						ey -= pixels;
					}
				}

				if( ex>sx )
				{ /* skip if inner loop doesn't draw anything */
					int y;

					/* case 0: TRANSPARENCY_NONE */
					if (transparency == TRANSPARENCY_NONE)
					{
						if (pri_buffer)
						{
							if (gfx->flags & GFX_PACKED)
							{
								for( y=sy; y<ey; y++ )
								{
									UINT8 *source = source_base + (y_index>>16) * gfx->line_modulo;
									UINT8 *dest = dest_bmp->line[y];
									UINT8 *pri = pri_buffer->line[y];

									int x, x_index = x_index_base;
									for( x=sx; x<ex; x++ )
									{
										if (((1 << pri[x]) & pri_mask) == 0)
											dest[x] = pal[(source[x_index>>17] >> ((x_index & 0x10000) >> 14)) & 0x0f];
										pri[x] = 31;
										x_index += dx;
									}

									y_index += dy;
								}
							}
							else
							{
								for( y=sy; y<ey; y++ )
								{
									UINT8 *source = source_base + (y_index>>16) * gfx->line_modulo;
									UINT8 *dest = dest_bmp->line[y];
									UINT8 *pri = pri_buffer->line[y];

									int x, x_index = x_index_base;
									for( x=sx; x<ex; x++ )
									{
										if (((1 << pri[x]) & pri_mask) == 0)
											dest[x] = pal[source[x_index>>16]];
										x_index += dx;
									}

									y_index += dy;
								}
							}
						}
						else
						{
							if (gfx->flags & GFX_PACKED)
							{
								for( y=sy; y<ey; y++ )
								{
									UINT8 *source = source_base + (y_index>>16) * gfx->line_modulo;
									UINT8 *dest = dest_bmp->line[y];

									int x, x_index = x_index_base;
									for( x=sx; x<ex; x++ )
									{
										dest[x] = pal[(source[x_index>>17] >> ((x_index & 0x10000) >> 14)) & 0x0f];
										x_index += dx;
									}

									y_index += dy;
								}
							}
							else
							{
								for( y=sy; y<ey; y++ )
								{
									UINT8 *source = source_base + (y_index>>16) * gfx->line_modulo;
									UINT8 *dest = dest_bmp->line[y];

									int x, x_index = x_index_base;
									for( x=sx; x<ex; x++ )
									{
										dest[x] = pal[source[x_index>>16]];
										x_index += dx;
									}

									y_index += dy;
								}
							}
						}
					}

					/* case 1: TRANSPARENCY_PEN */
					if (transparency == TRANSPARENCY_PEN)
					{
						if (pri_buffer)
						{
							if (gfx->flags & GFX_PACKED)
							{
								for( y=sy; y<ey; y++ )
								{
									UINT8 *source = source_base + (y_index>>16) * gfx->line_modulo;
									UINT8 *dest = dest_bmp->line[y];
									UINT8 *pri = pri_buffer->line[y];

									int x, x_index = x_index_base;
									for( x=sx; x<ex; x++ )
									{
										int c = (source[x_index>>17] >> ((x_index & 0x10000) >> 14)) & 0x0f;
										if( c != transparent_color )
										{
											if (((1 << pri[x]) & pri_mask) == 0)
												dest[x] = pal[c];
											pri[x] = 31;
										}
										x_index += dx;
									}

									y_index += dy;
								}
							}
							else
							{
								for( y=sy; y<ey; y++ )
								{
									UINT8 *source = source_base + (y_index>>16) * gfx->line_modulo;
									UINT8 *dest = dest_bmp->line[y];
									UINT8 *pri = pri_buffer->line[y];

									int x, x_index = x_index_base;
									for( x=sx; x<ex; x++ )
									{
										int c = source[x_index>>16];
										if( c != transparent_color )
										{
											if (((1 << pri[x]) & pri_mask) == 0)
												dest[x] = pal[c];
											pri[x] = 31;
										}
										x_index += dx;
									}

									y_index += dy;
								}
							}
						}
						else
						{
							if (gfx->flags & GFX_PACKED)
							{
								for( y=sy; y<ey; y++ )
								{
									UINT8 *source = source_base + (y_index>>16) * gfx->line_modulo;
									UINT8 *dest = dest_bmp->line[y];

									int x, x_index = x_index_base;
									for( x=sx; x<ex; x++ )
									{
										int c = (source[x_index>>17] >> ((x_index & 0x10000) >> 14)) & 0x0f;
										if( c != transparent_color ) dest[x] = pal[c];
										x_index += dx;
									}

									y_index += dy;
								}
							}
							else
							{
								for( y=sy; y<ey; y++ )
								{
									UINT8 *source = source_base + (y_index>>16) * gfx->line_modulo;
									UINT8 *dest = dest_bmp->line[y];

									int x, x_index = x_index_base;
									for( x=sx; x<ex; x++ )
									{
										int c = source[x_index>>16];
										if( c != transparent_color ) dest[x] = pal[c];
										x_index += dx;
									}

									y_index += dy;
								}
							}
						}
					}

					/* case 1b: TRANSPARENCY_PEN_RAW */
					if (transparency == TRANSPARENCY_PEN_RAW)
					{
						if (pri_buffer)
						{
							for( y=sy; y<ey; y++ )
							{
								UINT8 *source = source_base + (y_index>>16) * gfx->line_modulo;
								UINT8 *dest = dest_bmp->line[y];
								UINT8 *pri = pri_buffer->line[y];

								int x, x_index = x_index_base;
								for( x=sx; x<ex; x++ )
								{
									int c = source[x_index>>16];
									if( c != transparent_color )
									{
										if (((1 << pri[x]) & pri_mask) == 0)
											dest[x] = color + c;
										pri[x] = 31;
									}
									x_index += dx;
								}

								y_index += dy;
							}
						}
						else
						{
							for( y=sy; y<ey; y++ )
							{
								UINT8 *source = source_base + (y_index>>16) * gfx->line_modulo;
								UINT8 *dest = dest_bmp->line[y];

								int x, x_index = x_index_base;
								for( x=sx; x<ex; x++ )
								{
									int c = source[x_index>>16];
									if( c != transparent_color ) dest[x] = color + c;
									x_index += dx;
								}

								y_index += dy;
							}
						}
					}

					/* case 1c: TRANSPARENCY_BLEND_RAW */
					if (transparency == TRANSPARENCY_BLEND_RAW)
					{
						if (pri_buffer)
						{
							for( y=sy; y<ey; y++ )
							{
								UINT8 *source = source_base + (y_index>>16) * gfx->line_modulo;
								UINT8 *dest = dest_bmp->line[y];
								UINT8 *pri = pri_buffer->line[y];

								int x, x_index = x_index_base;
								for( x=sx; x<ex; x++ )
								{
									int c = source[x_index>>16];
									if( c != transparent_color )
									{
										if (((1 << pri[x]) & pri_mask) == 0)
											dest[x] |= (color + c);
										pri[x] = 31;
									}
									x_index += dx;
								}

								y_index += dy;
							}
						}
						else
						{
							for( y=sy; y<ey; y++ )
							{
								UINT8 *source = source_base + (y_index>>16) * gfx->line_modulo;
								UINT8 *dest = dest_bmp->line[y];

								int x, x_index = x_index_base;
								for( x=sx; x<ex; x++ )
								{
									int c = source[x_index>>16];
									if( c != transparent_color ) dest[x] |= (color + c);
									x_index += dx;
								}

								y_index += dy;
							}
						}
					}

					/* case 2: TRANSPARENCY_PENS */
					if (transparency == TRANSPARENCY_PENS)
					{
						if (pri_buffer)
						{
							for( y=sy; y<ey; y++ )
							{
								UINT8 *source = source_base + (y_index>>16) * gfx->line_modulo;
								UINT8 *dest = dest_bmp->line[y];
								UINT8 *pri = pri_buffer->line[y];

								int x, x_index = x_index_base;
								for( x=sx; x<ex; x++ )
								{
									int c = source[x_index>>16];
									if (((1 << c) & transparent_color) == 0)
									{
										if (((1 << pri[x]) & pri_mask) == 0)
											dest[x] = pal[c];
										pri[x] = 31;
									}
									x_index += dx;
								}

								y_index += dy;
							}
						}
						else
						{
							for( y=sy; y<ey; y++ )
							{
								UINT8 *source = source_base + (y_index>>16) * gfx->line_modulo;
								UINT8 *dest = dest_bmp->line[y];

								int x, x_index = x_index_base;
								for( x=sx; x<ex; x++ )
								{
									int c = source[x_index>>16];
									if (((1 << c) & transparent_color) == 0)
										dest[x] = pal[c];
									x_index += dx;
								}

								y_index += dy;
							}
						}
					}

					/* case 3: TRANSPARENCY_COLOR */
					else if (transparency == TRANSPARENCY_COLOR)
					{
						if (pri_buffer)
						{
							for( y=sy; y<ey; y++ )
							{
								UINT8 *source = source_base + (y_index>>16) * gfx->line_modulo;
								UINT8 *dest = dest_bmp->line[y];
								UINT8 *pri = pri_buffer->line[y];

								int x, x_index = x_index_base;
								for( x=sx; x<ex; x++ )
								{
									int c = pal[source[x_index>>16]];
									if( c != transparent_color )
									{
										if (((1 << pri[x]) & pri_mask) == 0)
											dest[x] = c;
										pri[x] = 31;
									}
									x_index += dx;
								}

								y_index += dy;
							}
						}
						else
						{
							for( y=sy; y<ey; y++ )
							{
								UINT8 *source = source_base + (y_index>>16) * gfx->line_modulo;
								UINT8 *dest = dest_bmp->line[y];

								int x, x_index = x_index_base;
								for( x=sx; x<ex; x++ )
								{
									int c = pal[source[x_index>>16]];
									if( c != transparent_color ) dest[x] = c;
									x_index += dx;
								}

								y_index += dy;
							}
						}
					}

					/* case 4: TRANSPARENCY_PEN_TABLE */
					if (transparency == TRANSPARENCY_PEN_TABLE)
					{
						if (pri_buffer)
						{
							for( y=sy; y<ey; y++ )
							{
								UINT8 *source = source_base + (y_index>>16) * gfx->line_modulo;
								UINT8 *dest = dest_bmp->line[y];
								UINT8 *pri = pri_buffer->line[y];

								int x, x_index = x_index_base;
								for( x=sx; x<ex; x++ )
								{
									int c = source[x_index>>16];
									if( c != transparent_color )
									{
										switch(gfx_drawmode_table[c])
										{
										case DRAWMODE_SOURCE:
											if (((1 << (pri[x] & 0x1f)) & pri_mask) == 0)
											{
												if (pri[x] & 0x80)
													dest[x] = palette_shadow_table[pal[c]];
												else
													dest[x] = pal[c];
											}
											pri[x] = (pri[x] & 0x7f) | 31;
											break;
										case DRAWMODE_SHADOW:
											if (((1 << pri[x]) & pri_mask) == 0)
												dest[x] = palette_shadow_table[dest[x]];
											pri[x] |= al;
											break;
										}
									}
									x_index += dx;
								}

								y_index += dy;
							}
						}
						else
						{
							for( y=sy; y<ey; y++ )
							{
								UINT8 *source = source_base + (y_index>>16) * gfx->line_modulo;
								UINT8 *dest = dest_bmp->line[y];

								int x, x_index = x_index_base;
								for( x=sx; x<ex; x++ )
								{
									int c = source[x_index>>16];
									if( c != transparent_color )
									{
										switch(gfx_drawmode_table[c])
										{
										case DRAWMODE_SOURCE:
											dest[x] = pal[c];
											break;
										case DRAWMODE_SHADOW:
											dest[x] = palette_shadow_table[dest[x]];
											break;
										}
									}
									x_index += dx;
								}

								y_index += dy;
							}
						}
					}

					/* case 4b: TRANSPARENCY_PEN_TABLE_RAW */
					if (transparency == TRANSPARENCY_PEN_TABLE_RAW)
					{
						if (pri_buffer)
						{
							for( y=sy; y<ey; y++ )
							{
								UINT8 *source = source_base + (y_index>>16) * gfx->line_modulo;
								UINT8 *dest = dest_bmp->line[y];
								UINT8 *pri = pri_buffer->line[y];

								int x, x_index = x_index_base;
								for( x=sx; x<ex; x++ )
								{
									int c = source[x_index>>16];
									if( c != transparent_color )
									{
										switch(gfx_drawmode_table[c])
										{
										case DRAWMODE_SOURCE:
											if (((1 << (pri[x] & 0x1f)) & pri_mask) == 0)
											{
												if (pri[x] & 0x80)
													dest[x] = palette_shadow_table[color + c];
												else
													dest[x] = color + c;
											}
											pri[x] = (pri[x] & 0x7f) | 31;
											break;
										case DRAWMODE_SHADOW:
											if (((1 << pri[x]) & pri_mask) == 0)
												dest[x] = palette_shadow_table[dest[x]];
											pri[x] |= al;
											break;
										}
									}
									x_index += dx;
								}

								y_index += dy;
							}
						}
						else
						{
							for( y=sy; y<ey; y++ )
							{
								UINT8 *source = source_base + (y_index>>16) * gfx->line_modulo;
								UINT8 *dest = dest_bmp->line[y];

								int x, x_index = x_index_base;
								for( x=sx; x<ex; x++ )
								{
									int c = source[x_index>>16];
									if( c != transparent_color )
									{
										switch(gfx_drawmode_table[c])
										{
										case DRAWMODE_SOURCE:
											dest[x] = color + c;
											break;
										case DRAWMODE_SHADOW:
											dest[x] = palette_shadow_table[dest[x]];
											break;
										}
									}
									x_index += dx;
								}

								y_index += dy;
							}
						}
					}
				}
			}
		}
	}

	/* ASG 980209 -- new 16-bit part */
	else if (dest_bmp->depth == 15 || dest_bmp->depth == 16)
	{
		if( gfx && gfx->colortable )
		{
			const pen_t *pal = &gfx->colortable[gfx->color_granularity * (color % gfx->total_colors)]; /* ASG 980209 */
			UINT8 *source_base = gfx->gfxdata + (code % gfx->total_elements) * gfx->char_modulo;

			int sprite_screen_height = (scaley*gfx->height+0x8000)>>16;
			int sprite_screen_width = (scalex*gfx->width+0x8000)>>16;

			if (sprite_screen_width && sprite_screen_height)
			{
				/* compute sprite increment per screen pixel */
				int dx = (gfx->width<<16)/sprite_screen_width;
				int dy = (gfx->height<<16)/sprite_screen_height;

				int ex = sx+sprite_screen_width;
				int ey = sy+sprite_screen_height;

				int x_index_base;
				int y_index;

				if( flipx )
				{
					x_index_base = (sprite_screen_width-1)*dx;
					dx = -dx;
				}
				else
				{
					x_index_base = 0;
				}

				if( flipy )
				{
					y_index = (sprite_screen_height-1)*dy;
					dy = -dy;
				}
				else
				{
					y_index = 0;
				}

				if( clip )
				{
					if( sx < clip->min_x)
					{ /* clip left */
						int pixels = clip->min_x-sx;
						sx += pixels;
						x_index_base += pixels*dx;
					}
					if( sy < clip->min_y )
					{ /* clip top */
						int pixels = clip->min_y-sy;
						sy += pixels;
						y_index += pixels*dy;
					}
					/* NS 980211 - fixed incorrect clipping */
					if( ex > clip->max_x+1 )
					{ /* clip right */
						int pixels = ex-clip->max_x-1;
						ex -= pixels;
					}
					if( ey > clip->max_y+1 )
					{ /* clip bottom */
						int pixels = ey-clip->max_y-1;
						ey -= pixels;
					}
				}

				if( ex>sx )
				{ /* skip if inner loop doesn't draw anything */
					int y;

					/* case 0: TRANSPARENCY_NONE */
					if (transparency == TRANSPARENCY_NONE)
					{
						if (pri_buffer)
						{
							if (gfx->flags & GFX_PACKED)
							{
								for( y=sy; y<ey; y++ )
								{
									UINT8 *source = source_base + (y_index>>16) * gfx->line_modulo;
									UINT16 *dest = (UINT16 *)dest_bmp->line[y];
									UINT8 *pri = pri_buffer->line[y];

									int x, x_index = x_index_base;
									for( x=sx; x<ex; x++ )
									{
										if (((1 << pri[x]) & pri_mask) == 0)
											dest[x] = pal[(source[x_index>>17] >> ((x_index & 0x10000) >> 14)) & 0x0f];
										pri[x] = 31;
										x_index += dx;
									}

									y_index += dy;
								}
							}
							else
							{
								for( y=sy; y<ey; y++ )
								{
									UINT8 *source = source_base + (y_index>>16) * gfx->line_modulo;
									UINT16 *dest = (UINT16 *)dest_bmp->line[y];
									UINT8 *pri = pri_buffer->line[y];

									int x, x_index = x_index_base;
									for( x=sx; x<ex; x++ )
									{
										if (((1 << pri[x]) & pri_mask) == 0)
											dest[x] = pal[source[x_index>>16]];
										pri[x] = 31;
										x_index += dx;
									}

									y_index += dy;
								}
							}
						}
						else
						{
							if (gfx->flags & GFX_PACKED)
							{
								for( y=sy; y<ey; y++ )
								{
									UINT8 *source = source_base + (y_index>>16) * gfx->line_modulo;
									UINT16 *dest = (UINT16 *)dest_bmp->line[y];

									int x, x_index = x_index_base;
									for( x=sx; x<ex; x++ )
									{
										dest[x] = pal[(source[x_index>>17] >> ((x_index & 0x10000) >> 14)) & 0x0f];
										x_index += dx;
									}

									y_index += dy;
								}
							}
							else
							{
								for( y=sy; y<ey; y++ )
								{
									UINT8 *source = source_base + (y_index>>16) * gfx->line_modulo;
									UINT16 *dest = (UINT16 *)dest_bmp->line[y];

									int x, x_index = x_index_base;
									for( x=sx; x<ex; x++ )
									{
										dest[x] = pal[source[x_index>>16]];
										x_index += dx;
									}

									y_index += dy;
								}
							}
						}
					}

					/* case 1: TRANSPARENCY_PEN */
					if (transparency == TRANSPARENCY_PEN)
					{
						if (pri_buffer)
						{
							if (gfx->flags & GFX_PACKED)
							{
								for( y=sy; y<ey; y++ )
								{
									UINT8 *source = source_base + (y_index>>16) * gfx->line_modulo;
									UINT16 *dest = (UINT16 *)dest_bmp->line[y];
									UINT8 *pri = pri_buffer->line[y];

									int x, x_index = x_index_base;
									for( x=sx; x<ex; x++ )
									{
										int c = (source[x_index>>17] >> ((x_index & 0x10000) >> 14)) & 0x0f;
										if( c != transparent_color )
										{
											if (((1 << pri[x]) & pri_mask) == 0)
												dest[x] = pal[c];
											pri[x] = 31;
										}
										x_index += dx;
									}

									y_index += dy;
								}
							}
							else
							{
								for( y=sy; y<ey; y++ )
								{
									UINT8 *source = source_base + (y_index>>16) * gfx->line_modulo;
									UINT16 *dest = (UINT16 *)dest_bmp->line[y];
									UINT8 *pri = pri_buffer->line[y];

									int x, x_index = x_index_base;
									for( x=sx; x<ex; x++ )
									{
										int c = source[x_index>>16];
										if( c != transparent_color )
										{
											if (((1 << pri[x]) & pri_mask) == 0)
												dest[x] = pal[c];
											pri[x] = 31;
										}
										x_index += dx;
									}

									y_index += dy;
								}
							}
						}
						else
						{
							if (gfx->flags & GFX_PACKED)
							{
								for( y=sy; y<ey; y++ )
								{
									UINT8 *source = source_base + (y_index>>16) * gfx->line_modulo;
									UINT16 *dest = (UINT16 *)dest_bmp->line[y];

									int x, x_index = x_index_base;
									for( x=sx; x<ex; x++ )
									{
										int c = (source[x_index>>17] >> ((x_index & 0x10000) >> 14)) & 0x0f;
										if( c != transparent_color ) dest[x] = pal[c];
										x_index += dx;
									}

									y_index += dy;
								}
							}
							else
							{
								for( y=sy; y<ey; y++ )
								{
									UINT8 *source = source_base + (y_index>>16) * gfx->line_modulo;
									UINT16 *dest = (UINT16 *)dest_bmp->line[y];

									int x, x_index = x_index_base;
									for( x=sx; x<ex; x++ )
									{
										int c = source[x_index>>16];
										if( c != transparent_color ) dest[x] = pal[c];
										x_index += dx;
									}

									y_index += dy;
								}
							}
						}
					}

					/* case 1b: TRANSPARENCY_PEN_RAW */
					if (transparency == TRANSPARENCY_PEN_RAW)
					{
						if (pri_buffer)
						{
							for( y=sy; y<ey; y++ )
							{
								UINT8 *source = source_base + (y_index>>16) * gfx->line_modulo;
								UINT16 *dest = (UINT16 *)dest_bmp->line[y];
								UINT8 *pri = pri_buffer->line[y];

								int x, x_index = x_index_base;
								for( x=sx; x<ex; x++ )
								{
									int c = source[x_index>>16];
									if( c != transparent_color )
									{
										if (((1 << pri[x]) & pri_mask) == 0)
											dest[x] = color + c;
										pri[x] = 31;
									}
									x_index += dx;
								}

								y_index += dy;
							}
						}
						else
						{
							for( y=sy; y<ey; y++ )
							{
								UINT8 *source = source_base + (y_index>>16) * gfx->line_modulo;
								UINT16 *dest = (UINT16 *)dest_bmp->line[y];

								int x, x_index = x_index_base;
								for( x=sx; x<ex; x++ )
								{
									int c = source[x_index>>16];
									if( c != transparent_color ) dest[x] = color + c;
									x_index += dx;
								}

								y_index += dy;
							}
						}
					}

					/* case 1c: TRANSPARENCY_BLEND_RAW */
					if (transparency == TRANSPARENCY_BLEND_RAW)
					{
						if (pri_buffer)
						{
							for( y=sy; y<ey; y++ )
							{
								UINT8 *source = source_base + (y_index>>16) * gfx->line_modulo;
								UINT16 *dest = (UINT16 *)dest_bmp->line[y];
								UINT8 *pri = pri_buffer->line[y];

								int x, x_index = x_index_base;
								for( x=sx; x<ex; x++ )
								{
									int c = source[x_index>>16];
									if( c != transparent_color )
									{
										if (((1 << pri[x]) & pri_mask) == 0)
											dest[x] |= color + c;
										pri[x] = 31;
									}
									x_index += dx;
								}

								y_index += dy;
							}
						}
						else
						{
							for( y=sy; y<ey; y++ )
							{
								UINT8 *source = source_base + (y_index>>16) * gfx->line_modulo;
								UINT16 *dest = (UINT16 *)dest_bmp->line[y];

								int x, x_index = x_index_base;
								for( x=sx; x<ex; x++ )
								{
									int c = source[x_index>>16];
									if( c != transparent_color ) dest[x] |= color + c;
									x_index += dx;
								}

								y_index += dy;
							}
						}
					}

					/* case 2: TRANSPARENCY_PENS */
					if (transparency == TRANSPARENCY_PENS)
					{
						if (pri_buffer)
						{
							for( y=sy; y<ey; y++ )
							{
								UINT8 *source = source_base + (y_index>>16) * gfx->line_modulo;
								UINT16 *dest = (UINT16 *)dest_bmp->line[y];
								UINT8 *pri = pri_buffer->line[y];

								int x, x_index = x_index_base;
								for( x=sx; x<ex; x++ )
								{
									int c = source[x_index>>16];
									if (((1 << c) & transparent_color) == 0)
									{
										if (((1 << pri[x]) & pri_mask) == 0)
											dest[x] = pal[c];
										pri[x] = 31;
									}
									x_index += dx;
								}

								y_index += dy;
							}
						}
						else
						{
							for( y=sy; y<ey; y++ )
							{
								UINT8 *source = source_base + (y_index>>16) * gfx->line_modulo;
								UINT16 *dest = (UINT16 *)dest_bmp->line[y];

								int x, x_index = x_index_base;
								for( x=sx; x<ex; x++ )
								{
									int c = source[x_index>>16];
									if (((1 << c) & transparent_color) == 0)
										dest[x] = pal[c];
									x_index += dx;
								}

								y_index += dy;
							}
						}
					}

					/* case 3: TRANSPARENCY_COLOR */
					else if (transparency == TRANSPARENCY_COLOR)
					{
						if (pri_buffer)
						{
							for( y=sy; y<ey; y++ )
							{
								UINT8 *source = source_base + (y_index>>16) * gfx->line_modulo;
								UINT16 *dest = (UINT16 *)dest_bmp->line[y];
								UINT8 *pri = pri_buffer->line[y];

								int x, x_index = x_index_base;
								for( x=sx; x<ex; x++ )
								{
									int c = pal[source[x_index>>16]];
									if( c != transparent_color )
									{
										if (((1 << pri[x]) & pri_mask) == 0)
											dest[x] = c;
										pri[x] = 31;
									}
									x_index += dx;
								}

								y_index += dy;
							}
						}
						else
						{
							for( y=sy; y<ey; y++ )
							{
								UINT8 *source = source_base + (y_index>>16) * gfx->line_modulo;
								UINT16 *dest = (UINT16 *)dest_bmp->line[y];

								int x, x_index = x_index_base;
								for( x=sx; x<ex; x++ )
								{
									int c = pal[source[x_index>>16]];
									if( c != transparent_color ) dest[x] = c;
									x_index += dx;
								}

								y_index += dy;
							}
						}
					}

					/* case 4: TRANSPARENCY_PEN_TABLE */
					if (transparency == TRANSPARENCY_PEN_TABLE)
					{
						if (pri_buffer)
						{
							for( y=sy; y<ey; y++ )
							{
								UINT8 *source = source_base + (y_index>>16) * gfx->line_modulo;
								UINT16 *dest = (UINT16 *)dest_bmp->line[y];
								UINT8 *pri = pri_buffer->line[y];

								int x, x_index = x_index_base;
								for( x=sx; x<ex; x++ )
								{
									int c = source[x_index>>16];
									if( c != transparent_color )
									{
										switch(gfx_drawmode_table[c])
										{
										case DRAWMODE_SOURCE:
											ah = pri[x];
											if (((1 << (ah & 0x1f)) & pri_mask) == 0)
											{
												if (ah & 0x80)
													dest[x] = palette_shadow_table[pal[c]];
												else
													dest[x] = pal[c];
											}
											pri[x] = (ah & 0x7f) | 31;
											break;
										case DRAWMODE_SHADOW:
											if (((1 << pri[x]) & pri_mask) == 0)
												dest[x] = palette_shadow_table[dest[x]];
											pri[x] |= al;
											break;
										}
									}
									x_index += dx;
								}

								y_index += dy;
							}
						}
						else
						{
							for( y=sy; y<ey; y++ )
							{
								UINT8 *source = source_base + (y_index>>16) * gfx->line_modulo;
								UINT16 *dest = (UINT16 *)dest_bmp->line[y];

								int x, x_index = x_index_base;
								for( x=sx; x<ex; x++ )
								{
									int c = source[x_index>>16];
									if( c != transparent_color )
									{
										switch(gfx_drawmode_table[c])
										{
										case DRAWMODE_SOURCE:
											dest[x] = pal[c];
											break;
										case DRAWMODE_SHADOW:
											dest[x] = palette_shadow_table[dest[x]];
											break;
										}
									}
									x_index += dx;
								}

								y_index += dy;
							}
						}
					}

					/* case 4b: TRANSPARENCY_PEN_TABLE_RAW */
					if (transparency == TRANSPARENCY_PEN_TABLE_RAW)
					{
						if (pri_buffer)
						{
							for( y=sy; y<ey; y++ )
							{
								UINT8 *source = source_base + (y_index>>16) * gfx->line_modulo;
								UINT16 *dest = (UINT16 *)dest_bmp->line[y];
								UINT8 *pri = pri_buffer->line[y];

								int x, x_index = x_index_base;
								for( x=sx; x<ex; x++ )
								{
									int c = source[x_index>>16];
									if( c != transparent_color )
									{
										switch(gfx_drawmode_table[c])
										{
										case DRAWMODE_SOURCE:
											ah = pri[x];
											if (((1 << (ah & 0x1f)) & pri_mask) == 0)
											{
												if (ah & 0x80)
													dest[x] = palette_shadow_table[color + c];
												else
													dest[x] = color + c;
											}
											pri[x] = (ah & 0x7f) | 31;
											break;
										case DRAWMODE_SHADOW:
											if (((1 << pri[x]) & pri_mask) == 0)
												dest[x] = palette_shadow_table[dest[x]];
											pri[x] |= al;
											break;
										}
									}
									x_index += dx;
								}

								y_index += dy;
							}
						}
						else
						{
							for( y=sy; y<ey; y++ )
							{
								UINT8 *source = source_base + (y_index>>16) * gfx->line_modulo;
								UINT16 *dest = (UINT16 *)dest_bmp->line[y];

								int x, x_index = x_index_base;
								for( x=sx; x<ex; x++ )
								{
									int c = source[x_index>>16];
									if( c != transparent_color )
									{
										switch(gfx_drawmode_table[c])
										{
										case DRAWMODE_SOURCE:
											dest[x] = color + c;
											break;
										case DRAWMODE_SHADOW:
											dest[x] = palette_shadow_table[dest[x]];
											break;
										}
									}
									x_index += dx;
								}

								y_index += dy;
							}
						}
					}

					/* case 5: TRANSPARENCY_ALPHAONE */
					if (transparency == TRANSPARENCY_ALPHAONE)
					{
						if (pri_buffer)
						{
							for( y=sy; y<ey; y++ )
							{
								UINT8 *source = source_base + (y_index>>16) * gfx->line_modulo;
								UINT16 *dest = (UINT16 *)dest_bmp->line[y];
								UINT8 *pri = pri_buffer->line[y];

								int x, x_index = x_index_base;
								for( x=sx; x<ex; x++ )
								{
									int c = source[x_index>>16];
									if( c != transparent_color )
									{
										if (((1 << pri[x]) & pri_mask) == 0)
										{
											if( c == alphapen)
												dest[x] = alpha_blend16(dest[x], pal[c]);
											else
												dest[x] = pal[c];
										}
										pri[x] = 31;
									}
									x_index += dx;
								}

								y_index += dy;
							}
						}
						else
						{
							for( y=sy; y<ey; y++ )
							{
								UINT8 *source = source_base + (y_index>>16) * gfx->line_modulo;
								UINT16 *dest = (UINT16 *)dest_bmp->line[y];

								int x, x_index = x_index_base;
								for( x=sx; x<ex; x++ )
								{
									int c = source[x_index>>16];
									if( c != transparent_color )
									{
										if( c == alphapen)
											dest[x] = alpha_blend16(dest[x], pal[c]);
										else
											dest[x] = pal[c];
									}
									x_index += dx;
								}

								y_index += dy;
							}
						}
					}

					/* case 6: TRANSPARENCY_ALPHA */
					if (transparency == TRANSPARENCY_ALPHA)
					{
						if (pri_buffer)
						{
							for( y=sy; y<ey; y++ )
							{
								UINT8 *source = source_base + (y_index>>16) * gfx->line_modulo;
								UINT16 *dest = (UINT16 *)dest_bmp->line[y];
								UINT8 *pri = pri_buffer->line[y];

								int x, x_index = x_index_base;
								for( x=sx; x<ex; x++ )
								{
									int c = source[x_index>>16];
									if( c != transparent_color )
									{
										if (((1 << pri[x]) & pri_mask) == 0)
											dest[x] = alpha_blend16(dest[x], pal[c]);
										pri[x] = 31;
									}
									x_index += dx;
								}

								y_index += dy;
							}
						}
						else
						{
							for( y=sy; y<ey; y++ )
							{
								UINT8 *source = source_base + (y_index>>16) * gfx->line_modulo;
								UINT16 *dest = (UINT16 *)dest_bmp->line[y];

								int x, x_index = x_index_base;
								for( x=sx; x<ex; x++ )
								{
									int c = source[x_index>>16];
									if( c != transparent_color ) dest[x] = alpha_blend16(dest[x], pal[c]);
									x_index += dx;
								}

								y_index += dy;
							}
						}
					}

					/* pjp 31/5/02 */
					/* case 7: TRANSPARENCY_ALPHARANGE */
					if (transparency == TRANSPARENCY_ALPHARANGE)
					{
						if (pri_buffer)
						{
							for( y=sy; y<ey; y++ )
							{
								UINT8 *source = source_base + (y_index>>16) * gfx->line_modulo;
								UINT16 *dest = (UINT16 *)dest_bmp->line[y];
								UINT8 *pri = pri_buffer->line[y];

								int x, x_index = x_index_base;
								for( x=sx; x<ex; x++ )
								{
									int c = source[x_index>>16];
									if( c != transparent_color )
									{
										if (((1 << pri[x]) & pri_mask) == 0)
										{
											if( gfx_alpharange_table[c] == 0xff )
												dest[x] = pal[c];
											else
												dest[x] = alpha_blend_r16(dest[x], pal[c], gfx_alpharange_table[c]);
										}
										pri[x] = 31;
									}
									x_index += dx;
								}

								y_index += dy;
							}
						}
						else
						{
							for( y=sy; y<ey; y++ )
							{
								UINT8 *source = source_base + (y_index>>16) * gfx->line_modulo;
								UINT16 *dest = (UINT16 *)dest_bmp->line[y];

								int x, x_index = x_index_base;
								for( x=sx; x<ex; x++ )
								{
									int c = source[x_index>>16];
									if( c != transparent_color )
									{
										if( gfx_alpharange_table[c] == 0xff )
											dest[x] = pal[c];
										else
											dest[x] = alpha_blend_r16(dest[x], pal[c], gfx_alpharange_table[c]);
									}
									x_index += dx;
								}

								y_index += dy;
							}
						}
					}
				}
			}
		}
	}
	/* 32bit part */
#if (0==PSP_FORCE16)
	else
	{
		if( gfx && gfx->colortable )
		{
			const pen_t *pal = &gfx->colortable[gfx->color_granularity * (color % gfx->total_colors)]; /* ASG 980209 */
			UINT8 *source_base = gfx->gfxdata + (code % gfx->total_elements) * gfx->char_modulo;

			int sprite_screen_height = (scaley*gfx->height+0x8000)>>16;
			int sprite_screen_width = (scalex*gfx->width+0x8000)>>16;

			if (sprite_screen_width && sprite_screen_height)
			{
				/* compute sprite increment per screen pixel */
				int dx = (gfx->width<<16)/sprite_screen_width;
				int dy = (gfx->height<<16)/sprite_screen_height;

				int ex = sx+sprite_screen_width;
				int ey = sy+sprite_screen_height;

				int x_index_base;
				int y_index;

				if( flipx )
				{
					x_index_base = (sprite_screen_width-1)*dx;
					dx = -dx;
				}
				else
				{
					x_index_base = 0;
				}

				if( flipy )
				{
					y_index = (sprite_screen_height-1)*dy;
					dy = -dy;
				}
				else
				{
					y_index = 0;
				}

				if( clip )
				{
					if( sx < clip->min_x)
					{ /* clip left */
						int pixels = clip->min_x-sx;
						sx += pixels;
						x_index_base += pixels*dx;
					}
					if( sy < clip->min_y )
					{ /* clip top */
						int pixels = clip->min_y-sy;
						sy += pixels;
						y_index += pixels*dy;
					}
					/* NS 980211 - fixed incorrect clipping */
					if( ex > clip->max_x+1 )
					{ /* clip right */
						int pixels = ex-clip->max_x-1;
						ex -= pixels;
					}
					if( ey > clip->max_y+1 )
					{ /* clip bottom */
						int pixels = ey-clip->max_y-1;
						ey -= pixels;
					}
				}

				if( ex>sx )
				{ /* skip if inner loop doesn't draw anything */
					int y;

					/* case 0: TRANSPARENCY_NONE */
					if (transparency == TRANSPARENCY_NONE)
					{
						if (pri_buffer)
						{
							for( y=sy; y<ey; y++ )
							{
								UINT8 *source = source_base + (y_index>>16) * gfx->line_modulo;
								UINT32 *dest = (UINT32 *)dest_bmp->line[y];
								UINT8 *pri = pri_buffer->line[y];

								int x, x_index = x_index_base;
								for( x=sx; x<ex; x++ )
								{
									if (((1 << pri[x]) & pri_mask) == 0)
										dest[x] = pal[source[x_index>>16]];
									pri[x] = 31;
									x_index += dx;
								}

								y_index += dy;
							}
						}
						else
						{
							for( y=sy; y<ey; y++ )
							{
								UINT8 *source = source_base + (y_index>>16) * gfx->line_modulo;
								UINT32 *dest = (UINT32 *)dest_bmp->line[y];

								int x, x_index = x_index_base;
								for( x=sx; x<ex; x++ )
								{
									dest[x] = pal[source[x_index>>16]];
									x_index += dx;
								}

								y_index += dy;
							}
						}
					}

					/* case 1: TRANSPARENCY_PEN */
					if (transparency == TRANSPARENCY_PEN)
					{
						if (pri_buffer)
						{
							for( y=sy; y<ey; y++ )
							{
								UINT8 *source = source_base + (y_index>>16) * gfx->line_modulo;
								UINT32 *dest = (UINT32 *)dest_bmp->line[y];
								UINT8 *pri = pri_buffer->line[y];

								int x, x_index = x_index_base;
								for( x=sx; x<ex; x++ )
								{
									int c = source[x_index>>16];
									if( c != transparent_color )
									{
										if (((1 << pri[x]) & pri_mask) == 0)
											dest[x] = pal[c];
										pri[x] = 31;
									}
									x_index += dx;
								}

								y_index += dy;
							}
						}
						else
						{
							for( y=sy; y<ey; y++ )
							{
								UINT8 *source = source_base + (y_index>>16) * gfx->line_modulo;
								UINT32 *dest = (UINT32 *)dest_bmp->line[y];

								int x, x_index = x_index_base;
								for( x=sx; x<ex; x++ )
								{
									int c = source[x_index>>16];
									if( c != transparent_color ) dest[x] = pal[c];
									x_index += dx;
								}

								y_index += dy;
							}
						}
					}

					/* case 1b: TRANSPARENCY_PEN_RAW */
					if (transparency == TRANSPARENCY_PEN_RAW)
					{
						if (pri_buffer)
						{
							for( y=sy; y<ey; y++ )
							{
								UINT8 *source = source_base + (y_index>>16) * gfx->line_modulo;
								UINT32 *dest = (UINT32 *)dest_bmp->line[y];
								UINT8 *pri = pri_buffer->line[y];

								int x, x_index = x_index_base;
								for( x=sx; x<ex; x++ )
								{
									int c = source[x_index>>16];
									if( c != transparent_color )
									{
										if (((1 << pri[x]) & pri_mask) == 0)
											dest[x] = color + c;
										pri[x] = 31;
									}
									x_index += dx;
								}

								y_index += dy;
							}
						}
						else
						{
							for( y=sy; y<ey; y++ )
							{
								UINT8 *source = source_base + (y_index>>16) * gfx->line_modulo;
								UINT32 *dest = (UINT32 *)dest_bmp->line[y];

								int x, x_index = x_index_base;
								for( x=sx; x<ex; x++ )
								{
									int c = source[x_index>>16];
									if( c != transparent_color ) dest[x] = color + c;
									x_index += dx;
								}

								y_index += dy;
							}
						}
					}

					/* case 1c: TRANSPARENCY_BLEND_RAW */
					if (transparency == TRANSPARENCY_BLEND_RAW)
					{
						if (pri_buffer)
						{
							for( y=sy; y<ey; y++ )
							{
								UINT8 *source = source_base + (y_index>>16) * gfx->line_modulo;
								UINT32 *dest = (UINT32 *)dest_bmp->line[y];
								UINT8 *pri = pri_buffer->line[y];

								int x, x_index = x_index_base;
								for( x=sx; x<ex; x++ )
								{
									int c = source[x_index>>16];
									if( c != transparent_color )
									{
										if (((1 << pri[x]) & pri_mask) == 0)
											dest[x] |= color + c;
										pri[x] = 31;
									}
									x_index += dx;
								}

								y_index += dy;
							}
						}
						else
						{
							for( y=sy; y<ey; y++ )
							{
								UINT8 *source = source_base + (y_index>>16) * gfx->line_modulo;
								UINT32 *dest = (UINT32 *)dest_bmp->line[y];

								int x, x_index = x_index_base;
								for( x=sx; x<ex; x++ )
								{
									int c = source[x_index>>16];
									if( c != transparent_color ) dest[x] |= color + c;
									x_index += dx;
								}

								y_index += dy;
							}
						}
					}

					/* case 2: TRANSPARENCY_PENS */
					if (transparency == TRANSPARENCY_PENS)
					{
						if (pri_buffer)
						{
							for( y=sy; y<ey; y++ )
							{
								UINT8 *source = source_base + (y_index>>16) * gfx->line_modulo;
								UINT32 *dest = (UINT32 *)dest_bmp->line[y];
								UINT8 *pri = pri_buffer->line[y];

								int x, x_index = x_index_base;
								for( x=sx; x<ex; x++ )
								{
									int c = source[x_index>>16];
									if (((1 << c) & transparent_color) == 0)
									{
										if (((1 << pri[x]) & pri_mask) == 0)
											dest[x] = pal[c];
										pri[x] = 31;
									}
									x_index += dx;
								}

								y_index += dy;
							}
						}
						else
						{
							for( y=sy; y<ey; y++ )
							{
								UINT8 *source = source_base + (y_index>>16) * gfx->line_modulo;
								UINT32 *dest = (UINT32 *)dest_bmp->line[y];

								int x, x_index = x_index_base;
								for( x=sx; x<ex; x++ )
								{
									int c = source[x_index>>16];
									if (((1 << c) & transparent_color) == 0)
										dest[x] = pal[c];
									x_index += dx;
								}

								y_index += dy;
							}
						}
					}

					/* case 3: TRANSPARENCY_COLOR */
					else if (transparency == TRANSPARENCY_COLOR)
					{
						if (pri_buffer)
						{
							for( y=sy; y<ey; y++ )
							{
								UINT8 *source = source_base + (y_index>>16) * gfx->line_modulo;
								UINT32 *dest = (UINT32 *)dest_bmp->line[y];
								UINT8 *pri = pri_buffer->line[y];

								int x, x_index = x_index_base;
								for( x=sx; x<ex; x++ )
								{
									int c = pal[source[x_index>>16]];
									if( c != transparent_color )
									{
										if (((1 << pri[x]) & pri_mask) == 0)
											dest[x] = c;
										pri[x] = 31;
									}
									x_index += dx;
								}

								y_index += dy;
							}
						}
						else
						{
							for( y=sy; y<ey; y++ )
							{
								UINT8 *source = source_base + (y_index>>16) * gfx->line_modulo;
								UINT32 *dest = (UINT32 *)dest_bmp->line[y];

								int x, x_index = x_index_base;
								for( x=sx; x<ex; x++ )
								{
									int c = pal[source[x_index>>16]];
									if( c != transparent_color ) dest[x] = c;
									x_index += dx;
								}

								y_index += dy;
							}
						}
					}

					/* case 4: TRANSPARENCY_PEN_TABLE */
					if (transparency == TRANSPARENCY_PEN_TABLE)
					{
						UINT8 *source, *pri;
						UINT32 *dest;
						int c, x, x_index;

						if (pri_buffer)
						{
							for( y=sy; y<ey; y++ )
							{
								source = source_base + (y_index>>16) * gfx->line_modulo;
								y_index += dy;
								dest = (UINT32 *)dest_bmp->line[y];
								pri = pri_buffer->line[y];
								x_index = x_index_base;

								for( x=sx; x<ex; x++ )
								{
									int ebx = x_index;
									x_index += dx;
									ebx >>= 16;
									al = pri[x];
									c = source[ebx];
									ah = al;
									al &= 0x1f;

									if (gfx_drawmode_table[c] == DRAWMODE_NONE) continue;

									if (!(1<<al & pri_mask))
									{
										if (gfx_drawmode_table[c] == DRAWMODE_SOURCE)
										{
											ah &= 0x7f;
											ebx = pal[c];
											ah |= 0x1f;
											dest[x] = ebx;
											pri[x] = ah;
										}
										else if (!(ah & 0x80))
										{
											ebx = SHADOW32(dest[x]);
											pri[x] |= 0x80;
											dest[x] = ebx;
										}
									}
								}
							}
						}
						else
						{
							for( y=sy; y<ey; y++ )
							{
								source = source_base + (y_index>>16) * gfx->line_modulo;
								y_index += dy;
								dest = (UINT32 *)dest_bmp->line[y];
								x_index = x_index_base;

								for( x=sx; x<ex; x++ )
								{
									c = source[x_index>>16];
									x_index += dx;

									if (gfx_drawmode_table[c] == DRAWMODE_NONE) continue;
									if (gfx_drawmode_table[c] == DRAWMODE_SOURCE)
										dest[x] = pal[c];
									else
										dest[x] = SHADOW32(dest[x]);
								}
							}
						}
					}

					/* case 4b: TRANSPARENCY_PEN_TABLE_RAW */
					if (transparency == TRANSPARENCY_PEN_TABLE_RAW)
					{
						UINT8 *source, *pri;
						UINT32 *dest;
						int c, x, x_index;

						if (pri_buffer)
						{
							for( y=sy; y<ey; y++ )
							{
								source = source_base + (y_index>>16) * gfx->line_modulo;
								y_index += dy;
								dest = (UINT32 *)dest_bmp->line[y];
								pri = pri_buffer->line[y];
								x_index = x_index_base;

								for( x=sx; x<ex; x++ )
								{
									int ebx = x_index;
									x_index += dx;
									ebx >>= 16;
									al = pri[x];
									c = source[ebx];
									ah = al;
									al &= 0x1f;

									if (gfx_drawmode_table[c] == DRAWMODE_NONE) continue;

									if (!(1<<al & pri_mask))
									{
										if (gfx_drawmode_table[c] == DRAWMODE_SOURCE)
										{
											ah &= 0x7f;
											ebx = color + c;
											ah |= 0x1f;
											dest[x] = ebx;
											pri[x] = ah;
										}
										else if (!(ah & 0x80))
										{
											ebx = SHADOW32(dest[x]);
											pri[x] |= 0x80;
											dest[x] = ebx;
										}
									}
								}
							}
						}
						else
						{
							for( y=sy; y<ey; y++ )
							{
								source = source_base + (y_index>>16) * gfx->line_modulo;
								y_index += dy;
								dest = (UINT32 *)dest_bmp->line[y];
								x_index = x_index_base;

								for( x=sx; x<ex; x++ )
								{
									c = source[x_index>>16];
									x_index += dx;

									if (gfx_drawmode_table[c] == DRAWMODE_NONE) continue;
									if (gfx_drawmode_table[c] == DRAWMODE_SOURCE)
										dest[x] = color + c;
									else
										dest[x] = SHADOW32(dest[x]);
								}
							}
						}
					}


					/* case 5: TRANSPARENCY_ALPHAONE */
					if (transparency == TRANSPARENCY_ALPHAONE)
					{
						if (pri_buffer)
						{
							for( y=sy; y<ey; y++ )
							{
								UINT8 *source = source_base + (y_index>>16) * gfx->line_modulo;
								UINT32 *dest = (UINT32 *)dest_bmp->line[y];
								UINT8 *pri = pri_buffer->line[y];

								int x, x_index = x_index_base;
								for( x=sx; x<ex; x++ )
								{
									int c = source[x_index>>16];
									if( c != transparent_color )
									{
										if (((1 << pri[x]) & pri_mask) == 0)
										{
											if( c == alphapen)
												dest[x] = alpha_blend32(dest[x], pal[c]);
											else
												dest[x] = pal[c];
										}
										pri[x] = 31;
									}
									x_index += dx;
								}

								y_index += dy;
							}
						}
						else
						{
							for( y=sy; y<ey; y++ )
							{
								UINT8 *source = source_base + (y_index>>16) * gfx->line_modulo;
								UINT32 *dest = (UINT32 *)dest_bmp->line[y];

								int x, x_index = x_index_base;
								for( x=sx; x<ex; x++ )
								{
									int c = source[x_index>>16];
									if( c != transparent_color )
									{
										if( c == alphapen)
											dest[x] = alpha_blend32(dest[x], pal[c]);
										else
											dest[x] = pal[c];
									}
									x_index += dx;
								}

								y_index += dy;
							}
						}
					}

					/* case 6: TRANSPARENCY_ALPHA */
					if (transparency == TRANSPARENCY_ALPHA)
					{
						if (pri_buffer)
						{
							for( y=sy; y<ey; y++ )
							{
								UINT8 *source = source_base + (y_index>>16) * gfx->line_modulo;
								UINT32 *dest = (UINT32 *)dest_bmp->line[y];
								UINT8 *pri = pri_buffer->line[y];

								int x, x_index = x_index_base;
								for( x=sx; x<ex; x++ )
								{
									int c = source[x_index>>16];
									if( c != transparent_color )
									{
										if (((1 << pri[x]) & pri_mask) == 0)
											dest[x] = alpha_blend32(dest[x], pal[c]);
										pri[x] = 31;
									}
									x_index += dx;
								}

								y_index += dy;
							}
						}
						else
						{
							for( y=sy; y<ey; y++ )
							{
								UINT8 *source = source_base + (y_index>>16) * gfx->line_modulo;
								UINT32 *dest = (UINT32 *)dest_bmp->line[y];

								int x, x_index = x_index_base;
								for( x=sx; x<ex; x++ )
								{
									int c = source[x_index>>16];
									if( c != transparent_color ) dest[x] = alpha_blend32(dest[x], pal[c]);
									x_index += dx;
								}

								y_index += dy;
							}
						}
					}

					/* pjp 31/5/02 */
					/* case 7: TRANSPARENCY_ALPHARANGE */
					if (transparency == TRANSPARENCY_ALPHARANGE)
					{
						if (pri_buffer)
						{
							for( y=sy; y<ey; y++ )
							{
								UINT8 *source = source_base + (y_index>>16) * gfx->line_modulo;
								UINT32 *dest = (UINT32 *)dest_bmp->line[y];
								UINT8 *pri = pri_buffer->line[y];

								int x, x_index = x_index_base;
								for( x=sx; x<ex; x++ )
								{
									int c = source[x_index>>16];
									if( c != transparent_color )
									{
										if (((1 << pri[x]) & pri_mask) == 0)
										{
											if( gfx_alpharange_table[c] == 0xff )
												dest[x] = pal[c];
											else
												dest[x] = alpha_blend_r32(dest[x], pal[c], gfx_alpharange_table[c]);
										}
										pri[x] = 31;
									}
									x_index += dx;
								}

								y_index += dy;
							}
						}
						else
						{
							for( y=sy; y<ey; y++ )
							{
								UINT8 *source = source_base + (y_index>>16) * gfx->line_modulo;
								UINT32 *dest = (UINT32 *)dest_bmp->line[y];

								int x, x_index = x_index_base;
								for( x=sx; x<ex; x++ )
								{
									int c = source[x_index>>16];
									if( c != transparent_color )
									{
										if( gfx_alpharange_table[c] == 0xff )
											dest[x] = pal[c];
										else
											dest[x] = alpha_blend_r32(dest[x], pal[c], gfx_alpharange_table[c]);
									}
									x_index += dx;
								}

								y_index += dy;
							}
						}
					}
				}
			}
		}
	}
#endif //(PSP_FORCE16)
}

void drawgfxzoom( struct mame_bitmap *dest_bmp,const struct GfxElement *gfx,
		unsigned int code,unsigned int color,int flipx,int flipy,int sx,int sy,
		const struct rectangle *clip,int transparency,int transparent_color,int scalex, int scaley)
{
	profiler_mark(PROFILER_DRAWGFX);
	common_drawgfxzoom(dest_bmp,gfx,code,color,flipx,flipy,sx,sy,
			clip,transparency,transparent_color,scalex,scaley,NULL,0);
	profiler_mark(PROFILER_END);
}

void pdrawgfxzoom( struct mame_bitmap *dest_bmp,const struct GfxElement *gfx,
		unsigned int code,unsigned int color,int flipx,int flipy,int sx,int sy,
		const struct rectangle *clip,int transparency,int transparent_color,int scalex, int scaley,
		UINT32 priority_mask)
{
	profiler_mark(PROFILER_DRAWGFX);
	common_drawgfxzoom(dest_bmp,gfx,code,color,flipx,flipy,sx,sy,
			clip,transparency,transparent_color,scalex,scaley,priority_bitmap,priority_mask | (1<<31));
	profiler_mark(PROFILER_END);
}

void mdrawgfxzoom( struct mame_bitmap *dest_bmp,const struct GfxElement *gfx,
		unsigned int code,unsigned int color,int flipx,int flipy,int sx,int sy,
		const struct rectangle *clip,int transparency,int transparent_color,int scalex, int scaley,
		UINT32 priority_mask)
{
	profiler_mark(PROFILER_DRAWGFX);
	common_drawgfxzoom(dest_bmp,gfx,code,color,flipx,flipy,sx,sy,
			clip,transparency,transparent_color,scalex,scaley,priority_bitmap,priority_mask);
	profiler_mark(PROFILER_END);
}

void plot_pixel2(struct mame_bitmap *bitmap1,struct mame_bitmap *bitmap2,int x,int y,pen_t pen)
{
	plot_pixel(bitmap1, x, y, pen);
	plot_pixel(bitmap2, x, y, pen);
}

static void  pp_8(struct mame_bitmap *b,int x,int y,pen_t p)  { ((UINT8 *)b->line[y])[x] = p; }
static pen_t rp_8(struct mame_bitmap *b,int x,int y)  { return ((UINT8 *)b->line[y])[x]; }
static void  pb_8(struct mame_bitmap *b,int x,int y,int w,int h,pen_t p)  { int t=x; while(h-->0){ int c=w; x=t; while(c-->0){ ((UINT8 *)b->line[y])[x] = p; x++; } y++; } }

static void  pp_16(struct mame_bitmap *b,int x,int y,pen_t p)  { ((UINT16 *)b->line[y])[x] = p; }
static pen_t rp_16(struct mame_bitmap *b,int x,int y)  { return ((UINT16 *)b->line[y])[x]; }
static void  pb_16(struct mame_bitmap *b,int x,int y,int w,int h,pen_t p)  { int t=x; while(h-->0){ int c=w; x=t; while(c-->0){ ((UINT16 *)b->line[y])[x] = p; x++; } y++; } }

#if (0==PSP_FORCE16)
static void  pp_32(struct mame_bitmap *b,int x,int y,pen_t p)  { ((UINT32 *)b->line[y])[x] = p; }
static pen_t rp_32(struct mame_bitmap *b,int x,int y)  { return ((UINT32 *)b->line[y])[x]; }
static void  pb_32(struct mame_bitmap *b,int x,int y,int w,int h,pen_t p)  { int t=x; while(h-->0){ int c=w; x=t; while(c-->0){ ((UINT32 *)b->line[y])[x] = p; x++; } y++; } }
#endif //(PSP_FORCE16)

void set_pixel_functions(struct mame_bitmap *bitmap)
{
	if (bitmap->depth == 8)
	{
		bitmap->read = rp_8;
		bitmap->plot = pp_8;
		bitmap->plot_box = pb_8;
	}
	else
#if (0==PSP_FORCE16)
		 if(bitmap->depth == 15 || bitmap->depth == 16)
#endif //(PSP_FORCE16)
	{
		bitmap->read = rp_16;
		bitmap->plot = pp_16;
		bitmap->plot_box = pb_16;
	}
#if (0==PSP_FORCE16)
	else
	{
		bitmap->read = rp_32;
		bitmap->plot = pp_32;
		bitmap->plot_box = pb_32;
	}
#endif //(PSP_FORCE16)

	/* while we're here, fill in the raw drawing mode table as well */
	is_raw[TRANSPARENCY_NONE_RAW]	   = 1;
	is_raw[TRANSPARENCY_PEN_RAW]	   = 1;
	is_raw[TRANSPARENCY_PENS_RAW]	   = 1;
	is_raw[TRANSPARENCY_PEN_TABLE_RAW] = 1;
	is_raw[TRANSPARENCY_BLEND_RAW]	   = 1;
}


INLINE void plotclip(struct mame_bitmap *bitmap,int x,int y,int pen,const struct rectangle *clip)
{
	if (x >= clip->min_x && x <= clip->max_x && y >= clip->min_y && y <= clip->max_y)
		plot_pixel(bitmap,x,y,pen);
}

static int crosshair_enable=1;

void drawgfx_toggle_crosshair(void)
{
	crosshair_enable^=1;
}

void draw_crosshair(struct mame_bitmap *bitmap,int x,int y,const struct rectangle *clip,int player)
{
	unsigned short black,white;
	int i;

	if (!crosshair_enable)
		return;

	black = Machine->uifont->colortable[0];
	white = Machine->uifont->colortable[1];

	for (i = 1;i < 6;i++)
	{
		switch (player)
		{
			case 0:
				plotclip(bitmap,x+i,y,white,clip);
				plotclip(bitmap,x-i,y,white,clip);
				plotclip(bitmap,x,y+i,white,clip);
				plotclip(bitmap,x,y-i,white,clip);
				break;

			case 1:
				plotclip(bitmap,x+i,y-i,white,clip);
				plotclip(bitmap,x-i,y-i,white,clip);
				plotclip(bitmap,x+i,y+i,white,clip);
				plotclip(bitmap,x-i,y+i,white,clip);
				break;

			case 2:
				plotclip(bitmap,x-5,y-i,white,clip);
				plotclip(bitmap,x+5,y-i,white,clip);
				plotclip(bitmap,x-5,y+i,white,clip);
				plotclip(bitmap,x+5,y+i,white,clip);
				plotclip(bitmap,x-i,y-5,white,clip);
				plotclip(bitmap,x+i,y-5,white,clip);
				plotclip(bitmap,x-i,y+5,white,clip);
				plotclip(bitmap,x+i,y+5,white,clip);
				break;

			case 3:
				plotclip(bitmap,x-i,y-5+i,white,clip);
				plotclip(bitmap,x+i,y-5+i,white,clip);
				plotclip(bitmap,x-i,y+5-i,white,clip);
				plotclip(bitmap,x+i,y+5-i,white,clip);
				break;
		}
	}
}


#else /* DECLARE */


#endif /* DECLARE */
