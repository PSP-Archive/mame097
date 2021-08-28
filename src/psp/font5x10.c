// font for Hello World PSP

const unsigned char hankaku_font5x10[]={
#if 0
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0x38,0x44,0x82,0x82,0x82,0x44,0x38,0x00,	//Åõ
	0x82,0x44,0x28,0x10,0x28,0x44,0x82,0x00,	//Å~
	0xFE,0x82,0x82,0x82,0x82,0x82,0xFE,0x00,	//Å†
	0x10,0x28,0x28,0x44,0x44,0x82,0xFE,0x00,	//Å¢
	0x10,0x38,0x7C,0x10,0x10,0x10,0x10,0x00,	//Å™
	0x00,0x08,0x0C,0xFE,0x0C,0x08,0x00,0x00,	//Å®
	0x10,0x10,0x10,0x10,0x7C,0x38,0x10,0x00,	//Å´

	0x00,0x20,0x60,0xFE,0x60,0x20,0x00,0x00,	//Å©
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,

	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,

	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
#else
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	/* [ 0- 1] */
	0x7c,0x80,0x98,0x90,0x80,0xbc,0x80,0x7c,0xf8,0x04,0x64,0x44,0x04,0xf4,0x04,0xf8,	/* [ 2- 3] tape pos 1 */
	0x7c,0x80,0x98,0x88,0x80,0xbc,0x80,0x7c,0xf8,0x04,0x64,0x24,0x04,0xf4,0x04,0xf8,	/* [ 4- 5] tape pos 2 */
	0x7c,0x80,0x88,0x98,0x80,0xbc,0x80,0x7c,0xf8,0x04,0x24,0x64,0x04,0xf4,0x04,0xf8,	/* [ 6- 7] tape pos 3 */

	0x7c,0x80,0x90,0x98,0x80,0xbc,0x80,0x7c,0xf8,0x04,0x44,0x64,0x04,0xf4,0x04,0xf8,	/* [ 8- 9] tape pos 3 */
	0x30,0x48,0x84,0xb4,0xb4,0x84,0x48,0x30,0x30,0x48,0x84,0x84,0x84,0x84,0x48,0x30,	/* [10-11] */
	0x00,0xfc,0x84,0x8c,0xd4,0xa4,0xfc,0x00,0x00,0xfc,0x84,0x84,0x84,0x84,0xfc,0x00,	/* [12-13] */
	0x00,0x38,0x7c,0x7c,0x7c,0x38,0x00,0x00,0x00,0x30,0x68,0x78,0x78,0x30,0x00,0x00,	/* [14-15] circle & bullet */

	0x80,0xc0,0xe0,0xf0,0xe0,0xc0,0x80,0x00,0x04,0x0c,0x1c,0x3c,0x1c,0x0c,0x04,0x00,	/* [16-17] R/L triangle */
	0x20,0x70,0xf8,0x20,0x20,0xf8,0x70,0x20,0x48,0x48,0x48,0x48,0x48,0x00,0x48,0x00,
	0x00,0x00,0x30,0x68,0x78,0x30,0x00,0x00,0x00,0x30,0x68,0x78,0x78,0x30,0x00,0x00,
	0x70,0xd8,0xe8,0xe8,0xf8,0xf8,0x70,0x00,0x1c,0x7c,0x74,0x44,0x44,0x4c,0xcc,0xc0,

	0x20,0x70,0xf8,0x70,0x70,0x70,0x70,0x00,0x70,0x70,0x70,0x70,0xf8,0x70,0x20,0x00,
	0x00,0x10,0xf8,0xfc,0xf8,0x10,0x00,0x00,0x00,0x20,0x7c,0xfc,0x7c,0x20,0x00,0x00,
	0xb0,0x54,0xb8,0xb8,0x54,0xb0,0x00,0x00,0x00,0x28,0x6c,0xfc,0x6c,0x28,0x00,0x00,
	0x00,0x30,0x30,0x78,0x78,0xfc,0x00,0x00,0xfc,0x78,0x78,0x30,0x30,0x00,0x00,0x00,
#endif
	//0x20

/*------------------------------------------------------
	gryph data
------------------------------------------------------*/


#define HANKAKU10_OFFS 0x00
//0x20

#define M 1
#define v 0
#define bc(aa,bb,cc,dd,ee,ff,gg,hh) (\
(aa?0x80:0x00)+\
(bb?0x40:0x00)+\
(cc?0x20:0x00)+\
(dd?0x10:0x00)+\
(ee?0x08:0x00)+\
(ff?0x04:0x00)+\
(gg?0x02:0x00)+\
(hh?0x01:0x00)\
)

//const unsigned char font_s[]={ /* x:y == 5x10 */

//bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
//bc(v,v,v,v,v,v,v,v),	/*0x20 ' ' */

//bc(v,v,v,v,v,v,v,v),
bc(v,v,M,v,v,v,v,v),
bc(v,v,M,v,v,v,v,v),
bc(v,v,M,v,v,v,v,v),
bc(v,v,M,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,M,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
//bc(v,v,v,v,v,v,v,v),	/*0x21 '!' */

//bc(v,v,v,v,v,v,v,v),
bc(v,M,v,M,v,v,v,v),
bc(v,M,v,M,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
//bc(v,v,v,v,v,v,v,v),	/*0x22 '"' */

//bc(v,v,v,v,v,v,v,v),
bc(v,M,v,M,v,v,v,v),
bc(v,M,v,M,v,v,v,v),
bc(M,M,M,M,M,v,v,v),
bc(v,M,v,M,v,v,v,v),
bc(M,M,M,M,M,v,v,v),
bc(v,M,v,M,v,v,v,v),
bc(v,M,v,M,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
//bc(v,v,v,v,v,v,v,v),	/*0x23 '#' */

//bc(v,v,v,v,v,v,v,v),
bc(v,v,M,M,M,M,v,v),
bc(v,M,v,M,v,v,v,v),
bc(v,v,M,M,v,v,v,v),
bc(v,v,v,M,v,v,v,v),
bc(v,v,v,M,M,v,v,v),
bc(v,v,v,M,v,M,v,v),
bc(v,v,v,M,v,M,v,v),
bc(v,M,M,M,M,v,v,v),
//bc(v,v,v,v,v,v,v,v),	/*0x24 '$' */

//bc(v,v,v,v,v,v,v,v),
bc(v,v,M,v,v,M,v,v),
bc(v,M,v,M,v,M,v,v),
bc(v,v,M,v,M,v,v,v),
bc(v,v,v,M,v,v,v,v),
bc(v,v,v,M,v,v,v,v),
bc(v,v,M,v,M,v,v,v),
bc(v,M,v,M,v,M,v,v),
bc(v,M,v,v,M,v,v,v),
//bc(v,v,v,v,v,v,v,v),	/*0x25 '%' */

//bc(v,v,v,v,v,v,v,v),
bc(v,v,M,v,v,v,v,v),
bc(v,M,v,M,v,v,v,v),
bc(v,M,v,v,v,v,v,v),
bc(v,v,M,v,v,v,v,v),
bc(v,M,v,v,v,v,v,v),
bc(v,M,v,M,v,M,v,v),
bc(v,M,v,v,M,v,v,v),
bc(v,v,M,M,v,M,v,v),
//bc(v,v,v,v,v,v,v,v),	/*0x26 '&' */

//bc(v,v,v,v,v,v,v,v),
bc(v,v,M,v,v,v,v,v),
bc(v,v,M,v,v,v,v,v),
bc(v,M,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
//bc(v,v,v,v,v,v,v,v),	/*0x27 ''' */

//bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,M,v,v),
bc(v,v,v,v,M,v,v,v),
bc(v,v,v,M,v,v,v,v),
bc(v,v,v,M,v,v,v,v),
bc(v,v,v,M,v,v,v,v),
bc(v,v,v,M,v,v,v,v),
bc(v,v,v,v,M,v,v,v),
bc(v,v,v,v,v,M,v,v),
//bc(v,v,v,v,v,v,v,v),	/*0x28 '(' */

//bc(v,v,v,v,v,v,v,v),
bc(v,M,v,v,v,v,v,v),
bc(v,v,M,v,v,v,v,v),
bc(v,v,v,M,v,v,v,v),
bc(v,v,v,M,v,v,v,v),
bc(v,v,v,M,v,v,v,v),
bc(v,v,v,M,v,v,v,v),
bc(v,v,M,v,v,v,v,v),
bc(v,M,v,v,v,v,v,v),
//bc(v,v,v,v,v,v,v,v),	/*0x29 ')' */

//bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,M,v,v,v,M,v,v),
bc(v,v,M,v,M,v,v,v),
bc(v,M,M,M,M,M,v,v),
bc(v,v,M,v,M,v,v,v),
bc(v,M,v,v,v,M,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
//bc(v,v,v,v,v,v,v,v),	/*0x2a '*' */

//bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,v,M,v,v,v,v),
bc(v,v,v,M,v,v,v,v),
bc(v,M,M,M,M,M,v,v),
bc(v,v,v,M,v,v,v,v),
bc(v,v,v,M,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
//bc(v,v,v,v,v,v,v,v),	/*0x2b '+' */

//bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,M,v,v,v,v,v),
bc(v,v,M,v,v,v,v,v),
bc(v,M,v,v,v,v,v,v),
//bc(v,v,v,v,v,v,v,v),	/*0x2c ',' */

//bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,M,M,M,M,M,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
//bc(v,v,v,v,v,v,v,v),	/*0x2d '-' */

//bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,M,v,v,v,v,v),
//bc(v,v,v,v,v,v,v,v),	/*0x2e '.' */

//bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,M,v,v),
bc(v,v,v,v,v,M,v,v),
bc(v,v,v,v,M,v,v,v),
bc(v,v,v,M,v,v,v,v),
bc(v,v,v,M,v,v,v,v),
bc(v,v,M,v,v,v,v,v),
bc(v,M,v,v,v,v,v,v),
bc(v,M,v,v,v,v,v,v),
//bc(v,v,v,v,v,v,v,v),	/*0x2f '/' */

//bc(v,v,v,v,v,v,v,v),
bc(v,v,M,M,M,v,v,v),
bc(v,M,v,v,v,M,v,v),
bc(v,M,v,v,v,M,v,v),
bc(v,M,v,v,v,M,v,v),
bc(v,M,v,v,v,M,v,v),
bc(v,M,v,v,v,M,v,v),
bc(v,M,v,v,v,M,v,v),
bc(v,v,M,M,M,v,v,v),
//bc(v,v,v,v,v,v,v,v),	/*0x30 '0' */

//bc(v,v,v,v,v,v,v,v),
bc(v,v,v,M,v,v,v,v),
bc(v,v,M,M,v,v,v,v),
bc(v,v,v,M,v,v,v,v),
bc(v,v,v,M,v,v,v,v),
bc(v,v,v,M,v,v,v,v),
bc(v,v,v,M,v,v,v,v),
bc(v,v,v,M,v,v,v,v),
bc(v,v,v,M,v,v,v,v),
//bc(v,v,v,v,v,v,v,v),	/*0x31 '1' */

//bc(v,v,v,v,v,v,v,v),
bc(v,M,M,M,M,v,v,v),
bc(v,v,v,v,v,M,v,v),
bc(v,v,v,v,v,M,v,v),
bc(v,v,v,v,M,v,v,v),
bc(v,v,v,M,v,v,v,v),
bc(v,v,M,v,v,v,v,v),
bc(v,M,v,v,v,v,v,v),
bc(v,M,M,M,M,M,v,v),
//bc(v,v,v,v,v,v,v,v),	/*0x32 '2' */

//bc(v,v,v,v,v,v,v,v),
bc(v,M,M,M,M,v,v,v),
bc(v,v,v,v,v,M,v,v),
bc(v,v,v,v,v,M,v,v),
bc(v,v,v,M,M,v,v,v),
bc(v,v,v,v,v,M,v,v),
bc(v,v,v,v,v,M,v,v),
bc(v,v,v,v,v,M,v,v),
bc(v,M,M,M,M,v,v,v),
//bc(v,v,v,v,v,v,v,v),	/*0x33 '3' */

//bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,M,v,v,v),
bc(v,v,v,M,M,v,v,v),
bc(v,v,M,v,M,v,v,v),
bc(v,v,M,v,M,v,v,v),
bc(v,M,v,v,M,v,v,v),
bc(v,M,M,M,M,M,v,v),
bc(v,v,v,v,M,v,v,v),
bc(v,v,v,v,M,v,v,v),
//bc(v,v,v,v,v,v,v,v),	/*0x34 '4' */

//bc(v,v,v,v,v,v,v,v),
bc(v,M,M,M,M,M,v,v),
bc(v,M,v,v,v,v,v,v),
bc(v,M,v,v,v,v,v,v),
bc(v,M,M,M,M,v,v,v),
bc(v,v,v,v,v,M,v,v),
bc(v,v,v,v,v,M,v,v),
bc(v,v,v,v,v,M,v,v),
bc(v,M,M,M,M,v,v,v),
//bc(v,v,v,v,v,v,v,v),	/*0x35 '5' */

//bc(v,v,v,v,v,v,v,v),
bc(v,v,M,M,M,M,v,v),
bc(v,M,v,v,v,v,v,v),
bc(v,M,v,v,v,v,v,v),
bc(v,M,M,M,M,v,v,v),
bc(v,M,v,v,v,M,v,v),
bc(v,M,v,v,v,M,v,v),
bc(v,M,v,v,v,M,v,v),
bc(v,v,M,M,M,v,v,v),
//bc(v,v,v,v,v,v,v,v),	/*0x36 '6' */

//bc(v,v,v,v,v,v,v,v),
bc(v,M,M,M,M,M,v,v),
bc(v,v,v,v,v,M,v,v),
bc(v,v,v,v,M,v,v,v),
bc(v,v,v,v,M,v,v,v),
bc(v,v,v,M,v,v,v,v),
bc(v,v,v,M,v,v,v,v),
bc(v,v,v,M,v,v,v,v),
bc(v,v,v,M,v,v,v,v),
//bc(v,v,v,v,v,v,v,v),	/*0x37 '7' */

//bc(v,v,v,v,v,v,v,v),
bc(v,v,M,M,M,v,v,v),
bc(v,M,v,v,v,M,v,v),
bc(v,M,v,v,v,M,v,v),
bc(v,v,M,M,M,v,v,v),
bc(v,M,v,v,v,M,v,v),
bc(v,M,v,v,v,M,v,v),
bc(v,M,v,v,v,M,v,v),
bc(v,v,M,M,M,v,v,v),
//bc(v,v,v,v,v,v,v,v),	/*0x38 '8' */

//bc(v,v,v,v,v,v,v,v),
bc(v,v,M,M,M,v,v,v),
bc(v,M,v,v,v,M,v,v),
bc(v,M,v,v,v,M,v,v),
bc(v,v,M,M,M,M,v,v),
bc(v,v,v,v,v,M,v,v),
bc(v,v,v,v,v,M,v,v),
bc(v,v,v,v,v,M,v,v),
bc(v,M,M,M,M,v,v,v),
//bc(v,v,v,v,v,v,v,v),	/*0x39 '9' */

//bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,M,M,v,v,v,v),
bc(v,v,M,M,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,M,M,v,v,v,v),
bc(v,v,M,M,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
//bc(v,v,v,v,v,v,v,v),	/*0x3a ':' */

//bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,M,M,v,v,v,v),
bc(v,v,M,M,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,M,M,v,v,v,v),
bc(v,v,v,M,v,v,v,v),
bc(v,v,M,v,v,v,v,v),
//bc(v,v,v,v,v,v,v,v),	/*0x3b ';' */

//bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,M,v,v,v),
bc(v,v,v,M,v,v,v,v),
bc(v,v,M,v,v,v,v,v),
bc(v,M,v,v,v,v,v,v),
bc(v,v,M,v,v,v,v,v),
bc(v,v,v,M,v,v,v,v),
bc(v,v,v,v,M,v,v,v),
bc(v,v,v,v,v,v,v,v),
//bc(v,v,v,v,v,v,v,v),	/*0x3c '<' */

//bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,M,M,M,M,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,M,M,M,M,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
//bc(v,v,v,v,v,v,v,v),	/*0x3d '=' */

//bc(v,v,v,v,v,v,v,v),
bc(v,M,v,v,v,v,v,v),
bc(v,v,M,v,v,v,v,v),
bc(v,v,v,M,v,v,v,v),
bc(v,v,v,v,M,v,v,v),
bc(v,v,v,M,v,v,v,v),
bc(v,v,M,v,v,v,v,v),
bc(v,M,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
//bc(v,v,v,v,v,v,v,v),	/*0x3e '>' */

//bc(v,v,v,v,v,v,v,v),
bc(v,v,M,M,v,v,v,v),
bc(v,M,v,v,M,v,v,v),
bc(v,v,v,v,M,v,v,v),
bc(v,v,v,M,v,v,v,v),
bc(v,v,v,M,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,v,M,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
//bc(v,v,v,v,v,v,v,v),	/*0x3f '?' */

//bc(v,v,v,v,v,v,v,v),
bc(v,v,M,M,v,v,v,v),
bc(v,M,v,v,M,v,v,v),
bc(v,M,v,M,M,v,v,v),
bc(v,M,M,v,M,v,v,v),
bc(v,M,v,M,v,v,v,v),
bc(v,M,v,v,v,v,v,v),
bc(v,v,M,M,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
//bc(v,v,v,v,v,v,v,v),	/*0x40 '@' */

//bc(v,v,v,v,v,v,v,v),
bc(v,v,v,M,v,v,v,v),
bc(v,v,v,M,v,v,v,v),
bc(v,v,M,v,M,v,v,v),
bc(v,v,M,v,M,v,v,v),
bc(v,v,M,M,M,v,v,v),
bc(v,M,v,v,v,M,v,v),
bc(v,M,v,v,v,M,v,v),
bc(v,M,v,v,v,M,v,v),
//bc(v,v,v,v,v,v,v,v),	/*0x41 'A' */

//bc(v,v,v,v,v,v,v,v),
bc(v,M,M,M,v,v,v,v),
bc(v,M,v,v,M,v,v,v),
bc(v,M,v,v,M,v,v,v),
bc(v,M,M,M,M,v,v,v),
bc(v,M,v,v,v,M,v,v),
bc(v,M,v,v,v,M,v,v),
bc(v,M,v,v,v,M,v,v),
bc(v,M,M,M,M,v,v,v),
//bc(v,v,v,v,v,v,v,v),	/*0x42 'B' */

//bc(v,v,v,v,v,v,v,v),
bc(v,v,M,M,M,M,v,v),
bc(v,M,v,v,v,v,v,v),
bc(v,M,v,v,v,v,v,v),
bc(v,M,v,v,v,v,v,v),
bc(v,M,v,v,v,v,v,v),
bc(v,M,v,v,v,v,v,v),
bc(v,M,v,v,v,v,v,v),
bc(v,v,M,M,M,M,v,v),
//bc(v,v,v,v,v,v,v,v),	/*0x43 'C' */

//bc(v,v,v,v,v,v,v,v),
bc(v,M,M,M,M,v,v,v),
bc(v,M,v,v,v,M,v,v),
bc(v,M,v,v,v,M,v,v),
bc(v,M,v,v,v,M,v,v),
bc(v,M,v,v,v,M,v,v),
bc(v,M,v,v,v,M,v,v),
bc(v,M,v,v,v,M,v,v),
bc(v,M,M,M,M,v,v,v),
//bc(v,v,v,v,v,v,v,v),	/*0x44 'D' */

//bc(v,v,v,v,v,v,v,v),
bc(v,M,M,M,M,M,v,v),
bc(v,M,v,v,v,v,v,v),
bc(v,M,v,v,v,v,v,v),
bc(v,M,M,M,M,v,v,v),
bc(v,M,v,v,v,v,v,v),
bc(v,M,v,v,v,v,v,v),
bc(v,M,v,v,v,v,v,v),
bc(v,M,M,M,M,M,v,v),
//bc(v,v,v,v,v,v,v,v),	/*0x45 'E' */

//bc(v,v,v,v,v,v,v,v),
bc(v,M,M,M,M,M,v,v),
bc(v,M,v,v,v,v,v,v),
bc(v,M,v,v,v,v,v,v),
bc(v,M,M,M,M,v,v,v),
bc(v,M,v,v,v,v,v,v),
bc(v,M,v,v,v,v,v,v),
bc(v,M,v,v,v,v,v,v),
bc(v,M,v,v,v,v,v,v),
//bc(v,v,v,v,v,v,v,v),	/*0x46 'F' */

//bc(v,v,v,v,v,v,v,v),
bc(v,v,M,M,M,M,v,v),
bc(v,M,v,v,v,v,v,v),
bc(v,M,v,v,v,v,v,v),
bc(v,M,v,M,M,M,v,v),
bc(v,M,v,v,v,M,v,v),
bc(v,M,v,v,v,M,v,v),
bc(v,M,v,v,v,M,v,v),
bc(v,v,M,M,M,M,v,v),
//bc(v,v,v,v,v,v,v,v),	/*0x47 'G' */

//bc(v,v,v,v,v,v,v,v),
bc(v,M,v,v,v,M,v,v),
bc(v,M,v,v,v,M,v,v),
bc(v,M,v,v,v,M,v,v),
bc(v,M,M,M,M,M,v,v),
bc(v,M,v,v,v,M,v,v),
bc(v,M,v,v,v,M,v,v),
bc(v,M,v,v,v,M,v,v),
bc(v,M,v,v,v,M,v,v),
//bc(v,v,v,v,v,v,v,v),	/*0x48 'H' */

//bc(v,v,v,v,v,v,v,v),
bc(v,v,M,M,M,v,v,v),
bc(v,v,v,M,v,v,v,v),
bc(v,v,v,M,v,v,v,v),
bc(v,v,v,M,v,v,v,v),
bc(v,v,v,M,v,v,v,v),
bc(v,v,v,M,v,v,v,v),
bc(v,v,v,M,v,v,v,v),
bc(v,v,M,M,M,v,v,v),
//bc(v,v,v,v,v,v,v,v),	/*0x49 'I' */

//bc(v,v,v,v,v,v,v,v),
bc(v,v,v,M,M,M,v,v),
bc(v,v,v,v,M,v,v,v),
bc(v,v,v,v,M,v,v,v),
bc(v,v,v,v,M,v,v,v),
bc(v,v,v,v,M,v,v,v),
bc(v,v,v,v,M,v,v,v),
bc(v,v,v,M,v,v,v,v),
bc(v,M,M,v,v,v,v,v),
//bc(v,v,v,v,v,v,v,v),	/*0x4a 'J' */

//bc(v,v,v,v,v,v,v,v),
bc(v,M,v,v,M,v,v,v),
bc(v,M,v,M,v,v,v,v),
bc(v,M,M,v,v,v,v,v),
bc(v,M,v,v,v,v,v,v),
bc(v,M,M,v,v,v,v,v),
bc(v,M,v,M,v,v,v,v),
bc(v,M,v,v,M,v,v,v),
bc(v,M,v,v,v,M,v,v),
//bc(v,v,v,v,v,v,v,v),	/*0x4b 'K' */

//bc(v,v,v,v,v,v,v,v),
bc(v,M,v,v,v,v,v,v),
bc(v,M,v,v,v,v,v,v),
bc(v,M,v,v,v,v,v,v),
bc(v,M,v,v,v,v,v,v),
bc(v,M,v,v,v,v,v,v),
bc(v,M,v,v,v,v,v,v),
bc(v,M,v,v,v,v,v,v),
bc(v,M,M,M,M,M,v,v),
//bc(v,v,v,v,v,v,v,v),	/*0x4c 'L' */

//bc(v,v,v,v,v,v,v,v),
bc(v,M,v,v,v,M,v,v),
bc(v,M,M,v,M,M,v,v),
bc(v,M,M,v,M,M,v,v),
bc(v,M,M,v,M,M,v,v),
bc(v,M,v,M,v,M,v,v),
bc(v,M,v,M,v,M,v,v),
bc(v,M,v,M,v,M,v,v),
bc(v,M,v,M,v,M,v,v),
//bc(v,v,v,v,v,v,v,v),	/*0x4d 'M' */

//bc(v,v,v,v,v,v,v,v),
bc(v,M,v,v,v,M,v,v),
bc(v,M,M,v,v,M,v,v),
bc(v,M,M,v,v,M,v,v),
bc(v,M,v,M,v,M,v,v),
bc(v,M,v,M,v,M,v,v),
bc(v,M,v,v,M,M,v,v),
bc(v,M,v,v,M,M,v,v),
bc(v,M,v,v,v,M,v,v),
//bc(v,v,v,v,v,v,v,v),	/*0x4e 'N' */

//bc(v,v,v,v,v,v,v,v),
bc(v,v,M,M,M,v,v,v),
bc(v,M,v,v,v,M,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,M,v,v,v,M,v,v),
bc(v,M,v,v,v,M,v,v),
bc(v,M,v,v,v,M,v,v),
bc(v,M,v,v,v,M,v,v),
bc(v,v,M,M,M,v,v,v),
//bc(v,v,v,v,v,v,v,v),	/*0x4f 'O' */

//bc(v,v,v,v,v,v,v,v),
bc(v,M,M,M,M,v,v,v),
bc(v,M,v,v,v,M,v,v),
bc(v,M,v,v,v,M,v,v),
bc(v,M,v,v,v,M,v,v),
bc(v,M,M,M,M,v,v,v),
bc(v,M,v,v,v,v,v,v),
bc(v,M,v,v,v,v,v,v),
bc(v,M,v,v,v,v,v,v),
//bc(v,v,v,v,v,v,v,v),	/*0x50 'P' */

//bc(v,v,v,v,v,v,v,v),
bc(v,v,M,M,M,v,v,v),
bc(v,M,v,v,v,M,v,v),
bc(v,M,v,v,v,M,v,v),
bc(v,M,v,v,v,M,v,v),
bc(v,M,v,v,v,M,v,v),
bc(v,M,v,M,v,M,v,v),
bc(v,M,v,v,M,v,v,v),
bc(v,v,M,M,v,M,v,v),
//bc(v,v,v,v,v,v,v,v),	/*0x51 'Q' */

//bc(v,v,v,v,v,v,v,v),
bc(v,M,M,M,M,v,v,v),
bc(v,M,v,v,v,M,v,v),
bc(v,M,v,v,v,M,v,v),
bc(v,M,v,v,v,M,v,v),
bc(v,M,M,M,M,v,v,v),
bc(v,M,v,M,v,v,v,v),
bc(v,M,v,v,M,v,v,v),
bc(v,M,v,v,v,M,v,v),
//bc(v,v,v,v,v,v,v,v),	/*0x52 'R' */

//bc(v,v,v,v,v,v,v,v),
bc(v,v,M,M,M,M,v,v),
bc(v,M,v,v,v,v,v,v),
bc(v,v,M,v,v,v,v,v),
bc(v,v,v,M,v,v,v,v),
bc(v,v,v,v,M,v,v,v),
bc(v,v,v,v,v,M,v,v),
bc(v,v,v,v,v,M,v,v),
bc(v,M,M,M,M,v,v,v),
//bc(v,v,v,v,v,v,v,v),	/*0x53 'S' */

//bc(v,v,v,v,v,v,v,v),
bc(v,M,M,M,M,M,v,v),
bc(v,v,v,M,v,v,v,v),
bc(v,v,v,M,v,v,v,v),
bc(v,v,v,M,v,v,v,v),
bc(v,v,v,M,v,v,v,v),
bc(v,v,v,M,v,v,v,v),
bc(v,v,v,M,v,v,v,v),
bc(v,v,v,M,v,v,v,v),
//bc(v,v,v,v,v,v,v,v),	/*0x54 'T' */

//bc(v,v,v,v,v,v,v,v),
bc(v,M,v,v,v,M,v,v),
bc(v,M,v,v,v,M,v,v),
bc(v,M,v,v,v,M,v,v),
bc(v,M,v,v,v,M,v,v),
bc(v,M,v,v,v,M,v,v),
bc(v,M,v,v,v,M,v,v),
bc(v,M,v,v,v,M,v,v),
bc(v,v,M,M,M,v,v,v),
//bc(v,v,v,v,v,v,v,v),	/*0x55 'U' */

//bc(v,v,v,v,v,v,v,v),
bc(v,M,v,v,v,M,v,v),
bc(v,M,v,v,v,M,v,v),
bc(v,M,v,v,v,M,v,v),
bc(v,v,M,v,M,v,v,v),
bc(v,v,M,v,M,v,v,v),
bc(v,v,M,v,M,v,v,v),
bc(v,v,v,M,v,v,v,v),
bc(v,v,v,M,v,v,v,v),
//bc(v,v,v,v,v,v,v,v),	/*0x56 'V' */

//bc(v,v,v,v,v,v,v,v),
bc(v,M,v,M,v,M,v,v),
bc(v,M,v,M,v,M,v,v),
bc(v,M,v,M,v,M,v,v),
bc(v,M,M,v,M,M,v,v),
bc(v,M,M,v,M,M,v,v),
bc(v,M,M,v,M,M,v,v),
bc(v,M,v,v,v,M,v,v),
bc(v,M,v,v,v,M,v,v),
//bc(v,v,v,v,v,v,v,v),	/*0x57 'W' */

//bc(v,v,v,v,v,v,v,v),
bc(v,M,v,v,v,M,v,v),
bc(v,M,v,v,v,M,v,v),
bc(v,v,M,v,M,v,v,v),
bc(v,v,v,M,v,v,v,v),
bc(v,v,v,M,v,v,v,v),
bc(v,v,M,v,M,v,v,v),
bc(v,M,v,v,v,M,v,v),
bc(v,M,v,v,v,M,v,v),
//bc(v,v,v,v,v,v,v,v),	/*0x58 'X' */

//bc(v,v,v,v,v,v,v,v),
bc(v,M,v,v,v,M,v,v),
bc(v,M,v,v,v,M,v,v),
bc(v,v,M,v,M,v,v,v),
bc(v,v,v,M,v,v,v,v),
bc(v,v,v,M,v,v,v,v),
bc(v,v,v,M,v,v,v,v),
bc(v,v,v,M,v,v,v,v),
bc(v,v,v,M,v,v,v,v),
//bc(v,v,v,v,v,v,v,v),	/*0x59 'Y' */

//bc(v,v,v,v,v,v,v,v),
bc(v,M,M,M,M,M,v,v),
bc(v,v,v,v,v,M,v,v),
bc(v,v,v,v,M,v,v,v),
bc(v,v,v,M,v,v,v,v),
bc(v,v,M,v,v,v,v,v),
bc(v,v,M,v,v,v,v,v),
bc(v,M,v,v,v,v,v,v),
bc(v,M,M,M,M,M,v,v),
//bc(v,v,v,v,v,v,v,v),	/*0x5a 'Z' */

bc(v,v,M,M,M,v,v,v),
//bc(v,v,M,v,v,v,v,v),
bc(v,v,M,v,v,v,v,v),
bc(v,v,M,v,v,v,v,v),
bc(v,v,M,v,v,v,v,v),
bc(v,v,M,v,v,v,v,v),
bc(v,v,M,v,v,v,v,v),
bc(v,v,M,v,v,v,v,v),
bc(v,v,M,M,M,v,v,v),
//bc(v,v,v,v,v,v,v,v),	/*0x5b '[' */

//bc(v,v,v,v,v,v,v,v),
bc(M,v,v,v,M,v,v,v),
bc(v,M,v,M,v,v,v,v),
bc(v,v,M,v,v,v,v,v),
bc(M,M,M,M,M,v,v,v),
bc(v,v,M,v,v,v,v,v),
bc(M,M,M,M,M,v,v,v),
bc(v,v,M,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
//bc(v,v,v,v,v,v,v,v),	/*0x5c '/'(yen) */

bc(v,M,M,M,v,v,v,v),
//bc(v,v,v,M,v,v,v,v),
bc(v,v,v,M,v,v,v,v),
bc(v,v,v,M,v,v,v,v),
bc(v,v,v,M,v,v,v,v),
bc(v,v,v,M,v,v,v,v),
bc(v,v,v,M,v,v,v,v),
bc(v,v,v,M,v,v,v,v),
bc(v,M,M,M,v,v,v,v),
//bc(v,v,v,v,v,v,v,v),	/*0x5d ']' */

bc(v,v,M,v,v,v,v,v),
bc(v,M,v,M,v,v,v,v),
bc(M,v,v,v,M,v,v,v),
//bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
//bc(v,v,v,v,v,v,v,v),	/*0x5e '^' */

//bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,M,M,M,M,v,v,v),
//bc(v,v,v,v,v,v,v,v),	/*0x5f '_' */

//bc(v,v,v,v,v,v,v,v),
bc(v,v,M,v,v,v,v,v),
bc(v,v,M,v,v,v,v,v),
bc(v,v,v,M,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
//bc(v,v,v,v,v,v,v,v),	/*0x60 '`' */

//bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,M,M,v,M,v,v,v),
bc(M,v,v,M,M,v,v,v),
bc(M,v,v,v,M,v,v,v),
bc(M,v,v,v,M,v,v,v),
bc(M,v,v,M,M,v,v,v),
bc(v,M,M,v,M,v,v,v),
//bc(v,v,v,v,v,v,v,v),	/*0x61 'a' */

//bc(v,v,v,v,v,v,v,v),
bc(M,v,v,v,v,v,v,v),
bc(M,v,v,v,v,v,v,v),
bc(M,M,M,M,v,v,v,v),
bc(M,v,v,v,M,v,v,v),
bc(M,v,v,v,M,v,v,v),
bc(M,v,v,v,M,v,v,v),
bc(M,v,v,v,M,v,v,v),
bc(M,M,M,M,v,v,v,v),
//bc(v,v,v,v,v,v,v,v),	/*0x62 'b' */

//bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,M,M,M,M,v,v,v),
bc(M,v,v,v,v,v,v,v),
bc(M,v,v,v,v,v,v,v),
bc(M,v,v,v,v,v,v,v),
bc(M,v,v,v,v,v,v,v),
bc(v,M,M,M,M,v,v,v),
//bc(v,v,v,v,v,v,v,v),	/*0x63 'c' */

//bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,M,v,v,v),
bc(v,v,v,v,M,v,v,v),
bc(v,M,M,M,M,v,v,v),
bc(M,v,v,v,M,v,v,v),
bc(M,v,v,v,M,v,v,v),
bc(M,v,v,v,M,v,v,v),
bc(M,v,v,v,M,v,v,v),
bc(v,M,M,M,M,v,v,v),
//bc(v,v,v,v,v,v,v,v),	/*0x64 'd' */

//bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,M,M,M,v,v,v,v),
bc(M,v,v,v,M,v,v,v),
bc(M,M,M,M,M,v,v,v),
bc(M,v,v,v,v,v,v,v),
bc(M,v,v,v,v,v,v,v),
bc(v,M,M,M,M,v,v,v),
//bc(v,v,v,v,v,v,v,v),	/*0x65 'e' */

//bc(v,v,v,v,v,v,v,v),
bc(v,v,v,M,M,v,v,v),
bc(v,v,M,v,v,v,v,v),
bc(v,M,M,M,M,v,v,v),
bc(v,v,M,v,v,v,v,v),
bc(v,v,M,v,v,v,v,v),
bc(v,v,M,v,v,v,v,v),
bc(v,v,M,v,v,v,v,v),
bc(v,v,M,v,v,v,v,v),
//bc(v,v,v,v,v,v,v,v),	/*0x66 'f' */

//bc(v,v,v,v,v,v,v,v),
//bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,M,M,M,M,v,v,v),
bc(M,v,v,v,M,v,v,v),
bc(M,v,v,v,M,v,v,v),
bc(v,M,M,M,M,v,v,v),
bc(v,v,v,v,M,v,v,v),
bc(v,v,v,M,v,v,v,v),
bc(v,M,M,v,v,v,v,v),	/*0x67 'g' */

//bc(v,v,v,v,v,v,v,v),
bc(M,v,v,v,v,v,v,v),
bc(M,v,v,v,v,v,v,v),
bc(M,M,M,M,v,v,v,v),
bc(M,v,v,v,M,v,v,v),
bc(M,v,v,v,M,v,v,v),
bc(M,v,v,v,M,v,v,v),
bc(M,v,v,v,M,v,v,v),
bc(M,v,v,v,M,v,v,v),
//bc(v,v,v,v,v,v,v,v),	/*0x68 'h' */

//bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,M,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,M,v,v,v,v,v),
bc(v,v,M,v,v,v,v,v),
bc(v,v,M,v,v,v,v,v),
bc(v,v,M,v,v,v,v,v),
bc(v,v,M,v,v,v,v,v),
//bc(v,v,v,v,v,v,v,v),	/*0x69 'i' */

//bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,M,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,M,v,v,v),
bc(v,v,v,v,M,v,v,v),
//bc(v,v,v,v,M,v,v,v),
bc(v,v,v,v,M,v,v,v),
bc(v,v,v,M,v,v,v,v),
bc(v,v,M,v,v,v,v,v),	/*0x6a 'j' */

//bc(v,v,v,v,v,v,v,v),
bc(M,v,v,v,v,v,v,v),
bc(M,v,v,v,v,v,v,v),
bc(M,v,v,v,M,v,v,v),
bc(M,v,v,M,v,v,v,v),
bc(M,v,M,v,v,v,v,v),
bc(M,M,M,v,v,v,v,v),
bc(M,v,v,M,v,v,v,v),
bc(M,v,v,v,M,v,v,v),
//bc(v,v,v,v,v,v,v,v),	/*0x6b 'k' */

//bc(v,v,v,v,v,v,v,v),
bc(v,M,M,v,v,v,v,v),
bc(v,v,M,v,v,v,v,v),
bc(v,v,M,v,v,v,v,v),
bc(v,v,M,v,v,v,v,v),
bc(v,v,M,v,v,v,v,v),
bc(v,v,M,v,v,v,v,v),
bc(v,v,M,v,v,v,v,v),
bc(v,M,M,M,v,v,v,v),
//bc(v,v,v,v,v,v,v,v),	/*0x6c 'l' */

//bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(M,M,v,M,v,v,v,v),
bc(M,v,M,v,M,v,v,v),
bc(M,v,M,v,M,v,v,v),
bc(M,v,M,v,M,v,v,v),
bc(M,v,M,v,M,v,v,v),
bc(M,v,M,v,M,v,v,v),
//bc(v,v,v,v,v,v,v,v),	/*0x6d 'm' */

//bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(M,M,M,M,v,v,v,v),
bc(M,v,v,v,M,v,v,v),
bc(M,v,v,v,M,v,v,v),
bc(M,v,v,v,M,v,v,v),
bc(M,v,v,v,M,v,v,v),
bc(M,v,v,v,M,v,v,v),
//bc(v,v,v,v,v,v,v,v),	/*0x6e 'n' */

//bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,M,M,M,v,v,v,v),
bc(M,v,v,v,M,v,v,v),
bc(M,v,v,v,M,v,v,v),
bc(M,v,v,v,M,v,v,v),
bc(M,v,v,v,M,v,v,v),
bc(v,M,M,M,v,v,v,v),
//bc(v,v,v,v,v,v,v,v),	/*0x6f 'o' */

//bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(M,M,M,M,v,v,v,v),
bc(M,v,v,v,M,v,v,v),
bc(M,v,v,v,M,v,v,v),
bc(M,M,M,M,v,v,v,v),
bc(M,v,v,v,v,v,v,v),
bc(M,v,v,v,v,v,v,v),
//bc(M,v,v,v,v,v,v,v),	/*0x70 'p' */

//bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,M,M,M,M,v,v,v),
bc(M,v,v,v,M,v,v,v),
bc(M,v,v,v,M,v,v,v),
bc(v,M,M,M,M,v,v,v),
bc(v,v,v,v,M,v,v,v),
bc(v,v,v,v,M,v,v,v),
//bc(v,v,v,v,M,v,v,v),	/*0x71 'q' */

//bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,M,v,M,M,v,v,v),
bc(v,M,M,v,v,v,v,v),
bc(v,M,v,v,v,v,v,v),
bc(v,M,v,v,v,v,v,v),
bc(v,M,v,v,v,v,v,v),
bc(v,M,v,v,v,v,v,v),
//bc(v,v,v,v,v,v,v,v),	/*0x72 'r' */

//bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,M,M,M,v,v,v),
bc(v,M,v,v,v,v,v,v),
bc(v,v,M,M,v,v,v,v),
bc(v,v,v,v,M,v,v,v),
bc(v,v,v,v,M,v,v,v),
bc(v,M,M,M,v,v,v,v),
//bc(v,v,v,v,v,v,v,v),	/*0x73 's' */

//bc(v,v,v,v,v,v,v,v),
bc(v,v,M,v,v,v,v,v),
bc(v,v,M,v,v,v,v,v),
bc(v,M,M,M,M,v,v,v),
bc(v,v,M,v,v,v,v,v),
bc(v,v,M,v,v,v,v,v),
bc(v,v,M,v,v,v,v,v),
bc(v,v,M,v,v,v,v,v),
bc(v,v,v,M,M,v,v,v),
//bc(v,v,v,v,v,v,v,v),	/*0x74 't' */

//bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(M,v,v,v,M,v,v,v),
bc(M,v,v,v,M,v,v,v),
bc(M,v,v,v,M,v,v,v),
bc(M,v,v,v,M,v,v,v),
bc(M,v,v,v,M,v,v,v),
bc(v,M,M,M,M,v,v,v),
//bc(v,v,v,v,v,v,v,v),	/*0x75 'u' */

//bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(M,v,v,v,M,v,v,v),
bc(M,v,v,v,M,v,v,v),
bc(M,v,v,v,M,v,v,v),
bc(v,M,v,M,v,v,v,v),
bc(v,M,v,M,v,v,v,v),
bc(v,v,M,v,v,v,v,v),
//bc(v,v,v,v,v,v,v,v),	/*0x76 'v' */

//bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(M,v,v,v,M,v,v,v),
bc(M,v,M,v,M,v,v,v),
bc(M,v,M,v,M,v,v,v),
bc(M,v,M,v,M,v,v,v),
bc(M,v,M,v,M,v,v,v),
bc(v,M,v,M,v,v,v,v),
//bc(v,v,v,v,v,v,v,v),	/*0x77 'w' */

//bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(M,v,v,v,M,v,v,v),
bc(v,M,v,M,v,v,v,v),
bc(v,v,M,v,v,v,v,v),
bc(v,M,v,M,v,v,v,v),
bc(v,M,v,M,v,v,v,v),
bc(M,v,v,v,M,v,v,v),
//bc(v,v,v,v,v,v,v,v),	/*0x78 'x' */

//bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,M,v,v,v,M,v,v),
bc(v,v,M,v,M,v,v,v),
bc(v,v,v,M,M,v,v,v),
bc(v,v,v,M,v,v,v,v),
bc(v,v,v,M,v,v,v,v),
bc(v,v,M,v,v,v,v,v),
//bc(v,v,v,v,v,v,v,v),	/*0x79 'y' */

//bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,M,M,M,M,v,v,v),
bc(v,v,v,v,M,v,v,v),
bc(v,v,v,M,v,v,v,v),
bc(v,v,M,v,v,v,v,v),
bc(v,M,v,v,v,v,v,v),
bc(v,M,M,M,M,v,v,v),
//bc(v,v,v,v,v,v,v,v),	/*0x7a 'z' */

bc(v,v,v,M,M,v,v,v),
bc(v,v,M,v,v,v,v,v),
//bc(v,v,M,v,v,v,v,v),
bc(v,v,M,v,v,v,v,v),
bc(v,M,v,v,v,v,v,v),
bc(v,v,M,v,v,v,v,v),
//bc(v,v,M,v,v,v,v,v),
bc(v,v,M,v,v,v,v,v),
bc(v,v,M,v,v,v,v,v),
bc(v,v,v,M,M,v,v,v),	/*0x7b '{' */

//bc(v,v,M,v,v,v,v,v),
bc(v,v,M,v,v,v,v,v),
bc(v,v,M,v,v,v,v,v),
bc(v,v,M,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,M,v,v,v,v,v),
bc(v,v,M,v,v,v,v,v),
bc(v,v,M,v,v,v,v,v),
bc(v,v,M,v,v,v,v,v),
//bc(v,v,M,v,v,v,v,v),	/*0x7c '|' */

bc(v,M,M,v,v,v,v,v),
bc(v,v,v,M,v,v,v,v),
//bc(v,v,v,M,v,v,v,v),
bc(v,v,v,M,v,v,v,v),
bc(v,v,v,v,M,v,v,v),
bc(v,v,v,M,v,v,v,v),
//bc(v,v,v,M,v,v,v,v),
bc(v,v,v,M,v,v,v,v),
bc(v,v,v,M,v,v,v,v),
bc(v,M,M,v,v,v,v,v),	/*0x7d '}' */

bc(v,M,M,M,M,v,v,v),
//bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
//bc(v,v,v,v,v,v,v,v),	/*0x7e '~' */

//bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
bc(v,v,v,v,v,v,v,v),
//bc(v,v,v,v,v,v,v,v),	/*0x7f ' ' */

};
#undef M
#undef v
#undef bc

