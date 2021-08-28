// primitive graphics for Hello World PSP

//#include "syscall.h"

#define RGB(r,g,b) ((((b>>3) & 0x1F)<<10)|(((g>>3) & 0x1F)<<5)|(((r>>3) & 0x1F)<<0)|0x8000)

#define SCREEN_WIDTH  480
#define SCREEN_HEIGHT 272

#define		PIXELSIZE	1				//in short
#define		LINESIZE	512				//in short
#define		FRAMESIZE	0x44000			//in byte

#define MAX_PATH 512		//temp, not confirmed

////480*272 = 60*38
//#define CMAX_X 60
//#define CMAX_Y 34
//#define CMAX2_X 30
//#define CMAX2_Y 17
//#define CMAX4_X 15
//#define CMAX4_Y 8

unsigned char *pgGetVramAddr(unsigned long x,unsigned long y);
void pgInit(void);
void pgWaitV(void);
void pgWaitVn(unsigned long count);
void pgScreenFrame(long mode,long frame);
void pgScreenFlip(void);
void pgScreenFlipV(void);
void pgPrint(unsigned long x,unsigned long y,unsigned long color,const char *str);
//void pgPrint2(unsigned long x,unsigned long y,unsigned long color,const char *str);
//void pgPrint4(unsigned long x,unsigned long y,unsigned long color,const char *str);
void pgFillvram(unsigned long color);

void pgBitBlt(unsigned long x,unsigned long y,unsigned long w,unsigned long h,unsigned long mag,const unsigned short *d);
void pgBitBlt2(unsigned long x,unsigned long y,unsigned long w,unsigned long h,const unsigned short *d);

void pgPutChar(unsigned long x,unsigned long y,unsigned long color,unsigned long bgcolor,unsigned char ch,char drawbg);
void pgDrawFrame(unsigned long x1, unsigned long y1, unsigned long x2, unsigned long y2, unsigned long color);
void pgFillBox(unsigned long x1, unsigned long y1, unsigned long x2, unsigned long y2, unsigned long color);
void mh_print(int x,int y,/*const*/ char *str,int col);

void pgcInit(void);
void pgcLocate(unsigned long x, unsigned long y);
void pgcColor(unsigned long fg, unsigned long bg);
void pgcDraw(char drawfg, char drawbg);
void pgcSetmag(char mag);
//void pgcCls(void);
void pgcPutchar_nocontrol(/*const*/ char ch);
void pgcPutchar(/*const*/ char ch);
void pgcPuthex2(/*const*/ unsigned long s);
void pgcPuthex8(/*const*/ unsigned long s);


//static int pga_channel_thread(int args, void *argp);
//void pga_channel_thread_callback(int channel, void *buf, unsigned long reqn);

int pgaInit(void);
void pgaTermPre(void);
void pgaTerm(void);
//int pgaOutBlocking(unsigned long channel,unsigned long vol1,unsigned long vol2,void *buf);



