
/* for psp TAITO_X (seta dummy file for taito_x system) */



/***************************************************************************

                            -= Seta Hardware =-

                    driver by   Luca Elia (l.elia@tin.it)


Note:   if MAME_DEBUG is defined, pressing Z with:

        Q           shows layer 0
        W           shows layer 1
        A           shows the sprites

        Keys can be used together!


                        [ 0, 1 Or 2 Scrolling Layers ]

    Each layer consists of 2 tilemaps: only one can be displayed at a
    given time (the games usually flip continuously between the two).
    The two tilemaps share the same scrolling registers.

        Layer Size:             1024 x 512
        Tiles:                  16x16x4 (16x16x6 in some games)
        Tile Format:

            Offset + 0x0000:
                            f--- ---- ---- ----     Flip X
                            -e-- ---- ---- ----     Flip Y
                            --dc ba98 7654 3210     Code

            Offset + 0x1000:

                            fedc ba98 765- ----     -
                            ---- ---- ---4 3210     Color

            The other tilemap for this layer (always?) starts at
            Offset + 0x2000.


                            [ 1024 Sprites ]

    Sprites are 16x16x4. They are just like those in "The Newzealand Story",
    "Revenge of DOH" etc (tnzs.c). Obviously they're hooked to a 16 bit
    CPU here, so they're mapped a bit differently in memory. Additionally,
    there are two banks of sprites. The game can flip between the two to
    do double buffering, writing to a bit of a control register(see below)


        Spriteram16_2 + 0x000.w

                        f--- ---- ---- ----     Flip X
                        -e-- ---- ---- ----     Flip Y
                        --dc b--- ---- ----     -
                        ---- --98 7654 3210     Code (Lower bits)

        Spriteram16_2 + 0x400.w

                        fedc b--- ---- ----     Color
                        ---- -a9- ---- ----     Code (Upper Bits)
                        ---- ---8 7654 3210     X

        Spriteram16   + 0x000.w

                        fedc ba98 ---- ----     -
                        ---- ---- 7654 3210     Y



                            [ Floating Tilemap ]

    There's a floating tilemap made of vertical colums composed of 2x16
    "sprites". Each 32 consecutive "sprites" define a column.

    For column I:

        Spriteram16_2 + 0x800 + 0x40 * I:

                        f--- ---- ---- ----     Flip X
                        -e-- ---- ---- ----     Flip Y
                        --dc b--- ---- ----     -
                        ---- --98 7654 3210     Code (Lower bits)

        Spriteram16_2 + 0xc00 + 0x40 * I:

                        fedc b--- ---- ----     Color
                        ---- -a9- ---- ----     Code (Upper Bits)
                        ---- ---8 7654 3210     -

    Each column has a variable horizontal position and a vertical scrolling
    value (see also the Sprite Control Registers). For column I:


        Spriteram16   + 0x400 + 0x20 * I:

                        fedc ba98 ---- ----     -
                        ---- ---- 7654 3210     Y

        Spriteram16   + 0x408 + 0x20 * I:

                        fedc ba98 ---- ----     -
                        ---- ---- 7654 3210     Low Bits Of X



                        [ Sprites Control Registers ]


        Spriteram16   + 0x601.b

                        7--- ----       0
                        -6-- ----       Flip Screen
                        --5- ----       0
                        ---4 ----       1 (Sprite Enable?)
                        ---- 3210       ???

        Spriteram16   + 0x603.b

                        7--- ----       0
                        -6-- ----       Sprite Bank
                        --5- ----       0 = Sprite Buffering (blandia,msgundam,qzkklogy)
                        ---4 ----       0
                        ---- 3210       Columns To Draw (1 is the special value for 16)

        Spriteram16   + 0x605.b

                        7654 3210       High Bit Of X For Columns 7-0

        Spriteram16   + 0x607.b

                        7654 3210       High Bit Of X For Columns f-8




***************************************************************************/

#include "vidhrdw/generic.h"
//#include "sound/x1_010.h"
#include "seta.h"






/* Variables only used here */

//static struct tilemap *tilemap_0, *tilemap_1;	// Layer 0
//static struct tilemap *tilemap_2, *tilemap_3;	// Layer 1
//static int tilemaps_flip;

/* Variables used elsewhere */

int seta_tiles_offset;

UINT16 *seta_vram_0, *seta_vctrl_0;
UINT16 *seta_vram_2, *seta_vram_3, *seta_vctrl_2;
UINT16 *seta_vregs;

//UINT16 *seta_workram; // Used for zombraid crosshair hack

//static int twineagl_tilebank[4];
//static int	seta_samples_bank;

//struct x_offset
//{
//	/* 2 values, for normal and flipped */
//	const char *gamename;
//	int sprite_offs[2];
//	int tilemap_offs[2];
//};

/* note that drgnunit, stg and qzkklogy run on the same board, yet they need different alignment */
//static struct x_offset game_offsets[] =
//{
//	/* only sprites */
//	{ "tndrcade", { -1,  0 } },				// correct (wall at beginning of game)
//	{ "tndrcadj", { -1,  0 } },				// correct (wall at beginning of game)
//	{ "wits",     {  0,  0 } },				// unknown
//	{ "thunderl", {  0,  0 } },				// unknown
//	{ "wiggie",   {  0,  0 } },				// some problems but they seem y co-ordinate related?
//	{ "blockcar", {  0,  0 } },				// unknown
//	{ "umanclub", {  0,  0 } },				// unknown
//	{ "atehate",  {  0,  0 } },				// correct (test grid)
//	{ "kiwame",   {  0,-16 } },				// correct (test grid)
//	{ "krzybowl", {  0,  0 } },				// correct (test grid)
//
//	/* 1 layer */
//	{ "twineagl", {  0,  0 }, {  0, -3 } },	// unknown
//	{ "downtown", {  1,  0 }, { -1,  0 } },	// sprites correct (test grid), tilemap unknown but at least -1 non-flipped to fix glitches later in the game
//	{ "usclssic", {  1,  2 }, {  0, -1 } },	// correct (test grid and bg)
//	{ "calibr50", { -1,  2 }, { -3, -2 } },	// correct (test grid and roof in animation at beginning of game)
//	{ "arbalest", {  0,  1 }, { -2, -1 } },	// correct (test grid and landing pad at beginning of game)
//	{ "metafox",  {  0,  0 }, { 16,-19 } },	// sprites unknown, tilemap correct (test grid)
//	{ "drgnunit", {  2,  2 }, { -2, -2 } },	// correct (test grid and I/O test)
//	{ "stg",      {  0,  0 }, { -2, -2 } },	// sprites correct? (panel), tilemap correct (test grid)
//	{ "qzkklogy", {  1,  1 }, { -1, -1 } },	// correct (timer, test grid)
//	{ "qzkklgy2", {  0,  0 }, { -1, -3 } },	// sprites unknown, tilemaps correct (test grid)
//
//	/* 2 layers */
//	{ "rezon",    {  0,  0 }, { -2, -2 } },	// correct (test grid)
//	{ "blandia",  {  0,  8 }, { -2,  6 } },	// correct (test grid, startup bg)
//	{ "blandiap", {  0,  8 }, { -2,  6 } },	// correct (test grid, startup bg)
//	{ "zingzip",  {  0,  0 }, { -1, -2 } },	// sprites unknown, tilemaps correct (test grid)
//	{ "eightfrc", {  3,  4 }, {  0,  0 } },	// unknown
//	{ "daioh",    {  1,  1 }, { -1, -1 } },	// correct? (launch window and test grid are right, but planet is wrong)
//	{ "msgundam", {  0,  0 }, { -2, -2 } },	// correct (test grid, banpresto logo)
//	{ "msgunda1", {  0,  0 }, { -2, -2 } },	// correct (test grid, banpresto logo)
//	{ "oisipuzl", {  0,  0 }, { -1, -1 } },	// correct (test mode) flip screen not supported?
//	{ "triplfun", {  0,  0 }, { -1, -1 } },	// correct (test mode) flip screen not supported?
//	{ "wrofaero", {  0,  0 }, {  0,  0 } },	// unknown
//	{ "jjsquawk", {  1,  1 }, { -1, -1 } },	// correct (test mode)
//	{ "kamenrid", {  0,  0 }, { -2, -2 } },	// correct (map, banpresto logo)
//	{ "extdwnhl", {  0,  0 }, { -2, -2 } },	// correct (test grid, background images)
//	{ "sokonuke", {  0,  0 }, { -2, -2 } },	// correct (game selection, test grid)
//	{ "gundhara", {  0,  0 }, {  0,  0 } },	// unknown, flip screen not supported?
//	{ "zombraid", {  0,  0 }, { -2, -2 } },	// correct for normal, flip screen not working yet
//	{ "madshark", {  0,  0 }, {  0,  0 } },	// unknown (wrong when flipped, but along y)
//	{ "utoukond", {  0,  0 }, { -2,  0 } }, // unknown (wrong when flipped, but along y)
//	{ NULL }
//};

//static struct x_offset *global_offsets;








/***************************************************************************

                        Callbacks for the TileMap code

                              [ Tiles Format ]

Offset + 0x0000:
                    f--- ---- ---- ----     Flip X
                    -e-- ---- ---- ----     Flip Y
                    --dc ba98 7654 3210     Code

Offset + 0x1000:

                    fedc ba98 765- ----     -
                    ---- ---- ---4 3210     Color


                      [ TileMaps Control Registers]

Offset + 0x0:                               Scroll X
Offset + 0x2:                               Scroll Y
Offset + 0x4:
                    fedc ba98 7654 3210     -
                    ---- ---- ---- 3---     Tilemap Select (There Are 2 Tilemaps Per Layer)
                    ---- ---- ---- -21-     0 (1 only in eightfrc, when flip is on!)
                    ---- ---- ---- ---0     ?

***************************************************************************/







//static void find_offsets(void)
//{
//	global_offsets = game_offsets;
//	while (global_offsets->gamename && strcmp(Machine->gamedrv->name,global_offsets->gamename))
//		global_offsets++;
//}





/* NO layers, only sprites */
VIDEO_START( seta_no_layers )
{
	//tilemap_0 = 0;
	//tilemap_1 = 0;
	//tilemap_2 = 0;
	//tilemap_3 = 0;
	//find_offsets();
	//seta_samples_bank = -1;	// set the samples bank to an out of range value at start-up
	return 0;
}





/***************************************************************************
                                Sprites Drawing
***************************************************************************/


static void seta_draw_sprites_map(struct mame_bitmap *bitmap,const struct rectangle *cliprect)
{
	int offs, col;
	int xoffs, yoffs;

	int total_color_codes	=	Machine->drv->gfxdecodeinfo[0].total_color_codes;

	int ctrl	=	spriteram16[ 0x600/2 ];
	int ctrl2	=	spriteram16[ 0x602/2 ];

	int flip	=	ctrl & 0x40;
	int numcol	=	ctrl2 & 0x000f;

	/* Sprites Banking and/or Sprites Buffering */
	UINT16 *src = spriteram16_2 + ( ((ctrl2 ^ (~ctrl2<<1)) & 0x40) ? 0x2000/2 : 0 );

	int upper	=	( spriteram16[ 0x604/2 ] & 0xFF ) +
					( spriteram16[ 0x606/2 ] & 0xFF ) * 256;

	int max_y	=	0xf0;

	int col0;		/* Kludge, needed for krzybowl and kiwame */
	switch (ctrl & 0x0f)
	{
		case 0x01:	col0	=	0x4;	break;	// krzybowl
		case 0x06:	col0	=	0x8;	break;	// kiwame

		default:	col0	=	0x0;
	}

	xoffs = 0;
	yoffs = flip ? 1 : -1;

	/* Number of columns to draw - the value 1 seems special, meaning:
       draw every column */
	if (numcol == 1)
		numcol = 16;


	/* The first column is the frontmost, see twineagl test mode
        BM 071204 - first column frontmost breaks superman.
    */
//  for ( col = numcol - 1 ; col >= 0; col -- )
	for ( col = 0 ; col < numcol; col ++ )
	{
		int x	=	spriteram16[(col * 0x20 + 0x08 + 0x400)/2] & 0xff;
		int y	=	spriteram16[(col * 0x20 + 0x00 + 0x400)/2] & 0xff;

		/* draw this column */
		for ( offs = 0 ; offs < 0x40/2; offs += 2/2 )
		{
			int	code	=	src[((col+col0)&0xf) * 0x40/2 + offs + 0x800/2];
			int	color	=	src[((col+col0)&0xf) * 0x40/2 + offs + 0xc00/2];

			int	flipx	=	code & 0x8000;
			int	flipy	=	code & 0x4000;

			int bank	=	(color & 0x0600) >> 9;

/*
twineagl:   010 02d 0f 10   (ship)
tndrcade:   058 02d 07 18   (start of game - yes, flip on!)
arbalest:   018 02d 0f 10   (logo)
metafox :   018 021 0f f0   (bomb)
zingzip :   010 02c 00 0f   (bomb)
wrofaero:   010 021 00 ff   (test mode)
thunderl:   010 06c 00 ff   (always?)
krzybowl:   011 028 c0 ff   (game)
kiwame  :   016 021 7f 00   (logo)
oisipuzl:   059 020 00 00   (game - yes, flip on!)

superman:   010 021 07 38   (game)
twineagl:   000 027 00 0f   (test mode)
*/

			int sx		=	  x + xoffs  + (offs & 1) * 16;
			int sy		=	-(y + yoffs) + (offs / 2) * 16;

			if (upper & (1 << col))	sx += 256;

			if (flip)
			{
				sy = max_y - sy;
				flipx = !flipx;
				flipy = !flipy;
			}

			color	=	( color >> (16-5) ) % total_color_codes;
			code	=	(code & 0x3fff) + (bank * 0x4000);

			drawgfx(bitmap,Machine->gfx[0],
					code,
					color,
					flipx, flipy,
					((sx + 0x10) & 0x1ff) - 0x10,((sy + 8) & 0x0ff) - 8,
					cliprect,TRANSPARENCY_PEN,0);
		}
	/* next column */
	}

}



static void seta_draw_sprites(struct mame_bitmap *bitmap,const struct rectangle *cliprect)
{
	int offs;
	int xoffs, yoffs;

	int total_color_codes	=	Machine->drv->gfxdecodeinfo[0].total_color_codes;

	int ctrl	=	spriteram16[ 0x600/2 ];
	int ctrl2	=	spriteram16[ 0x602/2 ];

	int flip	=	ctrl & 0x40;

	/* Sprites Banking and/or Sprites Buffering */
	UINT16 *src = spriteram16_2 + ( ((ctrl2 ^ (~ctrl2<<1)) & 0x40) ? 0x2000/2 : 0 );

	int max_y	=	0xf0;


	seta_draw_sprites_map(bitmap,cliprect);


	xoffs = 0;//global_offsets->sprite_offs[flip ? 1 : 0];
	yoffs = -2;

	for ( offs = (0x400-2)/2 ; offs >= 0/2; offs -= 2/2 )
	{
		int	code	=	src[offs + 0x000/2];
		int	x		=	src[offs + 0x400/2];

		int	y		=	spriteram16[offs + 0x000/2] & 0xff;

		int	flipx	=	code & 0x8000;
		int	flipy	=	code & 0x4000;

		int bank	=	(x & 0x0600) >> 9;
		int color	=	( x >> (16-5) ) % total_color_codes;

		if (flip)
		{
			y = (0x100 - Machine->drv->screen_height) + max_y - y;
			flipx = !flipx;
			flipy = !flipy;
		}

		code = (code & 0x3fff) + (bank * 0x4000);

		y = max_y - y;

		drawgfx(bitmap,Machine->gfx[0],
				code,
				color,
				flipx, flipy,
				((x + xoffs + 0x10) & 0x1ff) - 0x10,((y - yoffs + 8) & 0x0ff) - 8,
				cliprect,TRANSPARENCY_PEN,0);
	}

}


/***************************************************************************
                                Screen Drawing
***************************************************************************/

/* For games without tilemaps */
VIDEO_UPDATE( seta_no_layers )
{
	fillbitmap(bitmap,Machine->pens[0x1f0],cliprect);
	seta_draw_sprites(bitmap,cliprect);
}

