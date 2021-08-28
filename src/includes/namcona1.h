/* namcona1.h */

enum
{
	NAMCO_CGANGPZL,
	NAMCO_EMERALDA,
	NAMCO_KNCKHEAD,
	NAMCO_BKRTMAQ,
	NAMCO_EXBANIA,
	NAMCO_QUIZTOU,
	NAMCO_SWCOURT,
	NAMCO_TINKLPIT,
	NAMCO_NUMANATH,
	NAMCO_FA,
	NAMCO_XDAY,
	NAMCO_XDAY2
};
//#define NAMCO_DEVNUM_BKRTMAQ	(0x015c)
//#define NAMCO_DEVNUM_FA		(0x015d)
//#define NAMCO_DEVNUM_EXBANIA	(0x015e)
//#define NAMCO_DEVNUM_CGANGPZL	(0x0164)
//#define NAMCO_DEVNUM_SWCOURT	(0x0165)
//#define NAMCO_DEVNUM_EMERALDA	(0x0166)
//#define NAMCO_DEVNUM_NUMANATH	(0x0167)
//#define NAMCO_DEVNUM_KNCKHEAD	(0x0168)
//#define NAMCO_DEVNUM_QUIZTOU	(0x016d)
//#define NAMCO_DEVNUM_TINKLPIT	(0x016f)
//#define NAMCO_DEVNUM_XDAY		(0x01??)
//#define NAMCO_DEVNUM_XDAY2	(0x018a)


extern int namcona1_gametype;

#define NA1_NVRAM_SIZE (0x800)

extern UINT16 *namcona1_workram;
extern UINT16 *namcona1_vreg;
extern UINT16 *namcona1_scroll;
extern UINT16 *namcona1_sparevram;

extern WRITE16_HANDLER( namcona1_videoram_w );
extern READ16_HANDLER( namcona1_videoram_r );

extern READ16_HANDLER( namcona1_gfxram_r );
extern WRITE16_HANDLER( namcona1_gfxram_w );

extern READ16_HANDLER( namcona1_paletteram_r );
extern WRITE16_HANDLER( namcona1_paletteram_w );

extern VIDEO_UPDATE( namcona1 );
extern VIDEO_START( namcona1 );
