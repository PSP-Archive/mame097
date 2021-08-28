



#if (1==namcos1)
	#undef PSP_CHR_MAP88
	#define PSP_CHR_MAP88 2
#elif (1==namcos86)
	#undef PSP_CHR_MAP88
	#define PSP_CHR_MAP88 2
#elif (1==taitoAX)
/*taitoは地味に反転使うなー．*/
	#undef PSP_CHR_MAP88
	#define PSP_CHR_MAP88 1
#endif //

#if (1==m72)
	/* tilemap88n normal m72 only (m72専用版) */
	//#include "tilemap88m72.c"
#else
	#if (2==PSP_CHR_MAP88)
		/* tilemap88n normal only(8x8専用で反転なし版) */
		#include "tilemap88n.c"
	#elif (1==PSP_CHR_MAP88)
		/* tilemap88 (8x8専用版) */
		#include "tilemap88.c"
	#else
		/* tilemap normal mame (汎用版) */
		#include "tilemap.c"
	#endif //
#endif //
