

#include "osd_cpu.h"
#include "memory.h"
#include "mame.h"//#include "mamedbg.h"
#include "osdepend.h"
#include "tile_m72.h"


#include "driver.h"
#include "sndhrdw/m72.h"
#include "vidhrdw/generic.h"
#include "state.h"

#include "./../tilemap88m72.c"

unsigned char *m72_videoram1,*m72_videoram2;
//unsigned char *majtitle_rowscrollram;
static unsigned char *m72_spriteram;
static int rastersplit;
static int splitline;

static int xadjust;
static int scrollx1[256],scrolly1[256],scrollx2[256],scrolly2[256];
static int video_off;

static struct mame_bitmap *g_bitmap;

extern unsigned char *spriteram,*spriteram_2;
extern size_t spriteram_size;

static int irqbase;

MACHINE_INIT( m72 )
{
	irqbase = 0x20;
	machine_init_m72_sound();
}

MACHINE_INIT( xmultipl )
{
	irqbase = 0x08;
	machine_init_m72_sound();
}

MACHINE_INIT( kengo )
{
	irqbase = 0x18;
	machine_init_m72_sound();
}

INTERRUPT_GEN( m72_interrupt )
{
	int line = 255 - cpu_getiloops();

	if (line == 255)	/* vblank */
	{
		rastersplit = 0;
		cpunum_set_input_line_and_vector(0, 0, HOLD_LINE, irqbase+0);
	}
	else
	{
		if (line != splitline - 128)
			return;

		rastersplit = line + 1;

		/* this is used to do a raster effect and show the score display at
           the bottom of the screen or other things. The line where the
           interrupt happens is programmable (and the interrupt can be triggered
           multiple times, by changing the interrupt line register in the
           interrupt handler).
         */
		cpunum_set_input_line_and_vector(0, 0, HOLD_LINE, irqbase+2);
	}
}



/***************************************************************************

  Callbacks for the TileMap code

***************************************************************************/

INLINE void m72_get_tile_info(int tile_index,unsigned char *vram,int gfxnum)
{
	int code,attr,color,pri;

	tile_index *= 4;

	code  = vram[tile_index];
	attr  = vram[tile_index+1];
	color = vram[tile_index+2];

	if (color & 0x80) pri = 2;
	else if (color & 0x40) pri = 1;
	else pri = 0;
/* color & 0x10 is used in bchopper and hharry, more priority? */

	SET_TILE_INFO(
			gfxnum,
			code + ((attr & 0x3f) << 8),
			color & 0x0f,
			TILE_FLIPYX((attr & 0xc0) >> 6) | TILE_SPLIT(pri))
}

INLINE void rtype2_get_tile_info(int tile_index,unsigned char *vram,int gfxnum)
{
	int code,attr,color,pri;

	tile_index *= 4;

	code  = vram[tile_index] + (vram[tile_index+1] << 8);
	color = vram[tile_index+2];
	attr  = vram[tile_index+3];

	if (attr & 0x01) pri = 2;
	else if (color & 0x80) pri = 1;
	else pri = 0;

/* (vram[tile_index+2] & 0x10) is used by majtitle on the green, but it's not clear for what */
/* (vram[tile_index+3] & 0xfe) are used as well */

	SET_TILE_INFO(
			gfxnum,
			code,
			color & 0x0f,
			TILE_FLIPYX((color & 0x60) >> 5) | TILE_SPLIT(pri))
}


static void m72_get_bg_tile_info(int tile_index){		m72_get_tile_info(tile_index,m72_videoram2,2);}
static void m72_get_fg_tile_info(int tile_index){		m72_get_tile_info(tile_index,m72_videoram1,1);}
static void hharry_get_bg_tile_info(int tile_index){	m72_get_tile_info(tile_index,m72_videoram2,1);}
static void hharry_get_fg_tile_info(int tile_index){	m72_get_tile_info(tile_index,m72_videoram1,1);}
static void rtype2_get_bg_tile_info(int tile_index){	rtype2_get_tile_info(tile_index,m72_videoram2,1);}
static void rtype2_get_fg_tile_info(int tile_index){	rtype2_get_tile_info(tile_index,m72_videoram1,1);}

//static UINT32 tilem72_scan_rows( UINT32 col, UINT32 row, UINT32 num_cols, UINT32 num_rows )/*m72*/
//{
//	/* logical (col,row) -> memory offset */
//	return row*num_cols + col;
//}

/***************************************************************************

  Start the video hardware emulation.

***************************************************************************/

static void register_savestate(void)
{
	state_save_register_int  ("video", 0, "rastersplit",      &rastersplit);
	state_save_register_int  ("video", 0, "splitline",        &splitline);
	state_save_register_int  ("video", 0, "video_off",        &video_off);
	state_save_register_UINT8("video", 0, "scrollx1",(UINT8*) scrollx1, sizeof(scrollx1));
	state_save_register_UINT8("video", 0, "scrolly1",(UINT8*) scrolly1, sizeof(scrolly1));
	state_save_register_UINT8("video", 0, "scrollx2",(UINT8*) scrollx2, sizeof(scrollx2));
	state_save_register_UINT8("video", 0, "scrolly2",(UINT8*) scrolly2, sizeof(scrolly2));
	state_save_register_UINT8("video", 0, "m72_spriteram",    m72_spriteram, spriteram_size);
}

static struct tilemap *fg_tilemap,*bg_tilemap;
VIDEO_START( m72 )
{
	bg_tilemap = tilemap_create(m72_get_bg_tile_info/*,tilem72_scan_rows,TILEMAP_SPLIT,8,8,64,64*/);
	fg_tilemap = tilemap_create(m72_get_fg_tile_info/*,tilem72_scan_rows,TILEMAP_SPLIT,8,8,64,64*/);

	m72_spriteram = auto_malloc(spriteram_size);

	if (!fg_tilemap || !bg_tilemap || !m72_spriteram)
		return 1;

	tilemap_set_transmask(fg_tilemap,0,0xffff,0x0001);
	tilemap_set_transmask(fg_tilemap,1,0x00ff,0xff01);
	tilemap_set_transmask(fg_tilemap,2,0x0001,0xffff);

	tilemap_set_transmask(bg_tilemap,0,0xffff,0x0000);
	tilemap_set_transmask(bg_tilemap,1,0x00ff,0xff00);
	tilemap_set_transmask(bg_tilemap,2,0x0001,0xfffe);

	memset(m72_spriteram,0,spriteram_size);
	xadjust = 0;
	register_savestate();
	return 0;
}

VIDEO_START( rtype2 )
{
	bg_tilemap = tilemap_create(rtype2_get_bg_tile_info/*,tilem72_scan_rows,TILEMAP_SPLIT,8,8,64,64*/);
	fg_tilemap = tilemap_create(rtype2_get_fg_tile_info/*,tilem72_scan_rows,TILEMAP_SPLIT,8,8,64,64*/);

	m72_spriteram = auto_malloc(spriteram_size);

	if (!fg_tilemap || !bg_tilemap || !m72_spriteram)
		return 1;

	tilemap_set_transmask(fg_tilemap,0,0xffff,0x0001);
	tilemap_set_transmask(fg_tilemap,1,0x00ff,0xff01);
	tilemap_set_transmask(fg_tilemap,2,0x0001,0xffff);

	tilemap_set_transmask(bg_tilemap,0,0xffff,0x0000);
	tilemap_set_transmask(bg_tilemap,1,0x00ff,0xff00);
	tilemap_set_transmask(bg_tilemap,2,0x0001,0xfffe);

	memset(m72_spriteram,0,spriteram_size);
	xadjust = -4;
	register_savestate();
	return 0;
}

VIDEO_START( poundfor )
{
	int res = video_start_rtype2();
	xadjust = -6;
	register_savestate();
	return res;
}


VIDEO_START( hharry )
{
	bg_tilemap = tilemap_create(hharry_get_bg_tile_info/*,tilem72_scan_rows,TILEMAP_SPLIT,8,8,64,64*/);
	fg_tilemap = tilemap_create(hharry_get_fg_tile_info/*,tilem72_scan_rows,TILEMAP_SPLIT,8,8,64,64*/);

	m72_spriteram = auto_malloc(spriteram_size);

	if (!fg_tilemap || !bg_tilemap || !m72_spriteram)
		return 1;

	tilemap_set_transmask(fg_tilemap,0,0xffff,0x0001);
	tilemap_set_transmask(fg_tilemap,1,0x00ff,0xff01);
	tilemap_set_transmask(fg_tilemap,2,0x0001,0xffff);

	tilemap_set_transmask(bg_tilemap,0,0xffff,0x0000);
	tilemap_set_transmask(bg_tilemap,1,0x00ff,0xff00);
	tilemap_set_transmask(bg_tilemap,2,0x0001,0xfffe);

	memset(m72_spriteram,0,spriteram_size);
	xadjust = -4;
	register_savestate();
	return 0;
}


/***************************************************************************

  Memory handlers

***************************************************************************/

READ8_HANDLER( m72_palette1_r )
{
	/* only D0-D4 are connected */
	if (offset & 1) return 0xff;

	/* A9 isn't connected, so 0x200-0x3ff mirrors 0x000-0x1ff etc. */
	offset &= ~0x200;

	return paletteram[offset] | 0xe0;	/* only D0-D4 are connected */
}

READ8_HANDLER( m72_palette2_r )
{
	/* only D0-D4 are connected */
	if (offset & 1) return 0xff;

	/* A9 isn't connected, so 0x200-0x3ff mirrors 0x000-0x1ff etc. */
	offset &= ~0x200;

	return paletteram_2[offset] | 0xe0;	/* only D0-D4 are connected */
}

INLINE void changecolor(int color,int r,int g,int b)
{
	r = (r << 3) | (r >> 2);
	g = (g << 3) | (g >> 2);
	b = (b << 3) | (b >> 2);

	palette_set_color(color,r,g,b);
}

WRITE8_HANDLER( m72_palette1_w )
{
	/* only D0-D4 are connected */
	if (offset & 1) return;

	/* A9 isn't connected, so 0x200-0x3ff mirrors 0x000-0x1ff etc. */
	offset &= ~0x200;

	paletteram[offset] = data;
	offset &= 0x1ff;
	changecolor(offset / 2,
			paletteram[offset + 0x000],
			paletteram[offset + 0x400],
			paletteram[offset + 0x800]);
}

WRITE8_HANDLER( m72_palette2_w )
{
	/* only D0-D4 are connected */
	if (offset & 1) return;

	/* A9 isn't connected, so 0x200-0x3ff mirrors 0x000-0x1ff etc. */
	offset &= ~0x200;

	paletteram_2[offset] = data;
	offset &= 0x1ff;
	changecolor(offset / 2 + 256,
			paletteram_2[offset + 0x000],
			paletteram_2[offset + 0x400],
			paletteram_2[offset + 0x800]);
}

READ8_HANDLER( m72_videoram1_r )
{
	return m72_videoram1[offset];
}

READ8_HANDLER( m72_videoram2_r )
{
	return m72_videoram2[offset];
}

WRITE8_HANDLER( m72_videoram1_w )
{
	if (m72_videoram1[offset] != data)
	{
		m72_videoram1[offset] = data;
		tilemap_mark_tile_dirty(fg_tilemap,offset/4);
	}
}

WRITE8_HANDLER( m72_videoram2_w )
{
	if (m72_videoram2[offset] != data)
	{
		m72_videoram2[offset] = data;
		tilemap_mark_tile_dirty(bg_tilemap,offset/4);
	}
}

WRITE8_HANDLER( m72_irq_line_w )
{
	offset *= 8;
	splitline = (splitline & (0xff00 >> offset)) | (data << offset);
}

WRITE8_HANDLER( m72_scrollx1_w )
{
	int i;

	offset *= 8;
	scrollx1[rastersplit] = (scrollx1[rastersplit] & (0xff00 >> offset)) | (data << offset);

	for (i = rastersplit+1;i < 256;i++)
		scrollx1[i] = scrollx1[rastersplit];
}

WRITE8_HANDLER( m72_scrollx2_w )
{
	int i;

	offset *= 8;
	scrollx2[rastersplit] = (scrollx2[rastersplit] & (0xff00 >> offset)) | (data << offset);

	for (i = rastersplit+1;i < 256;i++)
		scrollx2[i] = scrollx2[rastersplit];
}

WRITE8_HANDLER( m72_scrolly1_w )
{
	int i;

	offset *= 8;
	scrolly1[rastersplit] = (scrolly1[rastersplit] & (0xff00 >> offset)) | (data << offset);

	for (i = rastersplit+1;i < 256;i++)
		scrolly1[i] = scrolly1[rastersplit];
}

WRITE8_HANDLER( m72_scrolly2_w )
{
	int i;

	offset *= 8;
	scrolly2[rastersplit] = (scrolly2[rastersplit] & (0xff00 >> offset)) | (data << offset);

	for (i = rastersplit+1;i < 256;i++)
		scrolly2[i] = scrolly2[rastersplit];
}

WRITE8_HANDLER( m72_dmaon_w )
{
	if (offset == 0)
	{
		memcpy(m72_spriteram,spriteram,spriteram_size);
	}
}


WRITE8_HANDLER( m72_port02_w )
{
	if (offset != 0)
	{
		if (data) logerror("write %02x to port 03\n",data);
		return;
	}
	if (data & 0xe0) logerror("write %02x to port 02\n",data);

	/* bits 0/1 are coin counters */
	coin_counter_w(0,data & 0x01);
	coin_counter_w(1,data & 0x02);

	/* bit 2 is flip screen (handled both by software and hardware) */
///	flip_screen_set(((data & 0x04) >> 2) ^ (~readinputport(5) & 1));

	/* bit 3 is display disable */
	video_off = data & 0x08;

	/* bit 4 resets sound CPU (active low) */
	if (data & 0x10)
		cpunum_set_input_line(1, INPUT_LINE_RESET, CLEAR_LINE);
	else
		cpunum_set_input_line(1, INPUT_LINE_RESET, ASSERT_LINE);

	/* bit 5 = "bank"? */
}

WRITE8_HANDLER( rtype2_port02_w )
{
	if (offset != 0)
	{
		if (data) logerror("write %02x to port 03\n",data);
		return;
	}
	if (data & 0xe0) logerror("write %02x to port 02\n",data);

	/* bits 0/1 are coin counters */
	coin_counter_w(0,data & 0x01);
	coin_counter_w(1,data & 0x02);

	/* bit 2 is flip screen (handled both by software and hardware) */
///	flip_screen_set(((data & 0x04) >> 2) ^ (~readinputport(5) & 1));

	/* bit 3 is display disable */
	video_off = data & 0x08;

	/* other bits unknown */
}


#if 00
static int majtitle_rowscroll;
/* the following is mostly a kludge. This register seems to be used for something else */
WRITE8_HANDLER( majtitle_gfx_ctrl_w )
{
	if (offset == 1)
	{
		if (data) majtitle_rowscroll = 1;
		else majtitle_rowscroll = 0;
	}
}
#endif

/***************************************************************************

  Display refresh

***************************************************************************/

static struct rectangle *g_cliprect;
static void draw_sprites(void)
{
	int offs;

	offs = 0;
	while (offs < spriteram_size)
	{
		int code,color,sx,sy,flipx,flipy,w,h,x,y;


		code = m72_spriteram[offs+2] | (m72_spriteram[offs+3] << 8);
		color = m72_spriteram[offs+4] & 0x0f;
		sx = -256+(m72_spriteram[offs+6] | ((m72_spriteram[offs+7] & 0x03) << 8));
		sy = 512-(m72_spriteram[offs+0] | ((m72_spriteram[offs+1] & 0x01) << 8));
		flipx = m72_spriteram[offs+5] & 0x08;
		flipy = m72_spriteram[offs+5] & 0x04;

		w = 1 << ((m72_spriteram[offs+5] & 0xc0) >> 6);
		h = 1 << ((m72_spriteram[offs+5] & 0x30) >> 4);
		sy -= 16 * h;

		if (flip_screen)
		{
			sx = 512 - 16*w - sx;
			sy = 512 - 16*h - sy;
			flipx = !flipx;
			flipy = !flipy;
		}

		{
		void* g_gfx0;
		g_gfx0=Machine->gfx[0];
			for (x = 0;x < w;x++)
			{
				for (y = 0;y < h;y++)
				{
					int c = code;

					if (flipx) c += 8*(w-1-x);
					else c += 8*x;
					if (flipy) c += h-1-y;
					else c += y;

					drawgfx(g_bitmap,g_gfx0,
							c,
							color,
							flipx,flipy,
							sx + 16*x,sy + 16*y,
							g_cliprect,TRANSPARENCY_PEN,0);
				}
			}
		}

		offs += w*8;
	}
}



static void draw_bg(int priority)
{
//	draw_bg_layer(cliprect,/*bg_tilemap,*/scrollx2,scrolly2,priority);
//static void draw_bg_layer(const struct rectangle *cliprect,
//		/*struct tilemap *tilemap,*/int *scrollx,int *scrolly,int priority)
{
	int start,i;
	/* use clip regions to split the screen */
	struct rectangle clip;

	clip.min_x = g_cliprect->min_x;
	clip.max_x = g_cliprect->max_x;
	start = g_cliprect->min_y - 128;
	do
	{
		i = start;
		while (scrollx2[i+1] == scrollx2[start] && scrolly2[i+1] == scrolly2[start]
				&& i < Machine->visible_area.max_y - 128)
			i++;

		clip.min_y = start + 128;
		clip.max_y = i + 128;
		sect_rect(&clip,g_cliprect);
		z_tilemap_set_scrollx(bg_tilemap,/*0,*/scrollx2[start] + xadjust);
		z_tilemap_set_scrolly(bg_tilemap,/*0,*/scrolly2[start]);
		tilemap_draw(g_bitmap,&clip,bg_tilemap,priority,0);

		start = i+1;
	} while (start < g_cliprect->max_y - 128);
}
}


static void draw_fg(int priority)
{
//	draw_fg_layer(cliprect,/*fg_tilemap,*/scrollx1,scrolly1,priority);
//static void draw_fg_layer(const struct rectangle *cliprect,
//		/*struct tilemap *tilemap,*/int *scrollx,int *scrolly,int priority)
{
	int start,i;
	/* use clip regions to split the screen */
	struct rectangle clip;

	clip.min_x = g_cliprect->min_x;
	clip.max_x = g_cliprect->max_x;
	start = g_cliprect->min_y - 128;
	do
	{
		i = start;
		while (scrollx1[i+1] == scrollx1[start] && scrolly1[i+1] == scrolly1[start]
				&& i < Machine->visible_area.max_y - 128)
			i++;

		clip.min_y = start + 128;
		clip.max_y = i + 128;
		sect_rect(&clip,g_cliprect);
		z_tilemap_set_scrollx(fg_tilemap,/*0,*/scrollx1[start] + xadjust);
		z_tilemap_set_scrolly(fg_tilemap,/*0,*/scrolly1[start]);
		tilemap_draw(g_bitmap,&clip,fg_tilemap,priority,0);

		start = i+1;
	} while (start < g_cliprect->max_y - 128);
}
}


VIDEO_UPDATE( m72 )
{
	if (video_off)
	{
	//	fillbitmap(bitmap,Machine->pens[0],cliprect);
		return;
	}

	g_bitmap=bitmap;
	g_cliprect=cliprect;

	draw_bg(TILEMAP_BACK);
	draw_fg(TILEMAP_BACK);
	draw_sprites();
	draw_bg(TILEMAP_FRONT);
	draw_fg(TILEMAP_FRONT);
}


VIDEO_EOF( m72 )
{
	int i;
	for (i = 0;i < 255;i++)
	{
		scrollx1[i] = scrollx1[255];
		scrolly1[i] = scrolly1[255];
		scrollx2[i] = scrollx2[255];
		scrolly2[i] = scrolly2[255];
	}
}
