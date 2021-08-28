#include "driver.h"

#ifdef __cplusplus
extern "C" {
#endif

extern UINT8 *videoram;
extern UINT16 *videoram16;
#if (0==PSP_NO_CPU32)
extern UINT32 *videoram32;
#endif //(0==PSP_NO_CPU32)
extern size_t videoram_size;
extern UINT8 *colorram;
extern UINT16 *colorram16;
#if (0==PSP_NO_CPU32)
extern UINT32 *colorram32;
#endif //(0==PSP_NO_CPU32)
extern UINT8 *spriteram;
extern UINT16 *spriteram16;
#if (0==PSP_NO_CPU32)
extern UINT32 *spriteram32;
#endif //(0==PSP_NO_CPU32)
extern UINT8 *spriteram_2;
extern UINT16 *spriteram16_2;
#if (0==PSP_NO_CPU32)
extern UINT32 *spriteram32_2;
#endif //(0==PSP_NO_CPU32)
extern UINT8 *spriteram_3;
extern UINT16 *spriteram16_3;
#if (0==PSP_NO_CPU32)
extern UINT32 *spriteram32_3;
#endif //(0==PSP_NO_CPU32)
extern UINT8 *buffered_spriteram;
extern UINT16 *buffered_spriteram16;
#if (0==PSP_NO_CPU32)
extern UINT32 *buffered_spriteram32;
#endif //(0==PSP_NO_CPU32)
extern UINT8 *buffered_spriteram_2;
extern UINT16 *buffered_spriteram16_2;
#if (0==PSP_NO_CPU32)
extern UINT32 *buffered_spriteram32_2;
#endif //(0==PSP_NO_CPU32)
extern size_t spriteram_size;
extern size_t spriteram_2_size;
extern size_t spriteram_3_size;
extern UINT8 *dirtybuffer;
extern UINT16 *dirtybuffer16;
#if (0==PSP_NO_CPU32)
extern UINT32 *dirtybuffer32;
#endif //(0==PSP_NO_CPU32)
extern struct mame_bitmap *tmpbitmap;


VIDEO_START( generic );
VIDEO_START( generic_bitmapped );
void video_stop_generic(void);
void video_stop_generic_bitmapped(void);
VIDEO_UPDATE( generic_bitmapped );

READ8_HANDLER( videoram_r );
READ8_HANDLER( colorram_r );
WRITE8_HANDLER( videoram_w );
WRITE8_HANDLER( colorram_w );
READ8_HANDLER( spriteram_r );
WRITE8_HANDLER( spriteram_w );
READ16_HANDLER( spriteram16_r );
WRITE16_HANDLER( spriteram16_w );
READ8_HANDLER( spriteram_2_r );
WRITE8_HANDLER( spriteram_2_w );
WRITE8_HANDLER( buffer_spriteram_w );
WRITE16_HANDLER( buffer_spriteram16_w );
#if (0==PSP_NO_CPU32)
WRITE32_HANDLER( buffer_spriteram32_w );
#endif //(0==PSP_NO_CPU32)
WRITE8_HANDLER( buffer_spriteram_2_w );
WRITE16_HANDLER( buffer_spriteram16_2_w );
#if (0==PSP_NO_CPU32)
WRITE32_HANDLER( buffer_spriteram32_2_w );
#endif //(0==PSP_NO_CPU32)
void buffer_spriteram(unsigned char *ptr,int length);
void buffer_spriteram_2(unsigned char *ptr,int length);

/* screen flipping */
extern int flip_screen_x, flip_screen_y;
void flip_screen_set(int on);
void flip_screen_x_set(int on);
void flip_screen_y_set(int on);
#define flip_screen flip_screen_x

/* sets a variable and schedules a full screen refresh if it changed */
void set_vh_global_attribute(int *addr, int data);
int get_vh_global_attribute_changed(void);


#ifdef __cplusplus
}
#endif
