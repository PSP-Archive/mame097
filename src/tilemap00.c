



#if (1==namcos1)
	#undef PSP_CHR_MAP88
	#define PSP_CHR_MAP88 2
#elif (1==namcos86)
	#undef PSP_CHR_MAP88
	#define PSP_CHR_MAP88 2
#elif (1==taitoAX)
/*taito�͒n���ɔ��]�g���ȁ[�D*/
	#undef PSP_CHR_MAP88
	#define PSP_CHR_MAP88 1
#endif //

#if (1==m72)
	/* tilemap88n normal m72 only (m72��p��) */
	//#include "tilemap88m72.c"
#else
	#if (2==PSP_CHR_MAP88)
		/* tilemap88n normal only(8x8��p�Ŕ��]�Ȃ���) */
		#include "tilemap88n.c"
	#elif (1==PSP_CHR_MAP88)
		/* tilemap88 (8x8��p��) */
		#include "tilemap88.c"
	#else
		/* tilemap normal mame (�ėp��) */
		#include "tilemap.c"
	#endif //
#endif //
