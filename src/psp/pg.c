// primitive graphics for Hello World PSP

#include "pg.h"
#include "syscall.h"

#include "font3333.c"
#include "font5x10.c"

void Trans( unsigned long *s1, unsigned long *s2, unsigned long *d1, int a);


//constants

//480*272 = 60*38
#define CMAX_X 60
#define CMAX_Y 38
#define CMAX2_X 30
#define CMAX2_Y 19
#define CMAX4_X 15
#define CMAX4_Y 9


//variables
//char *pg_vramtop=(char *)0x04000000;
#define pg_vramtop ((char *)0x04000000)
long pg_screenmode;
long pg_showframe;
long pg_drawframe;
unsigned long pgc_csr_x[2], pgc_csr_y[2];
unsigned long pgc_fgcolor[2], pgc_bgcolor[2];

void pgWaitVn(unsigned long count)
{
	for (; count>0; --count) {
		sceDisplayWaitVblankStart();
	}
}


void pgWaitV(void)
{
	sceDisplayWaitVblankStart();
}


unsigned char *pgGetVramAddr(unsigned long x,unsigned long y)
{
	return pg_vramtop+(pg_drawframe?FRAMESIZE:0)+x*PIXELSIZE*2+y*LINESIZE*2+0x40000000;
}


void pgInit(void)
{
	sceDisplaySetMode(0,SCREEN_WIDTH,SCREEN_HEIGHT);
	pgScreenFrame(0,0);
}



void pgDrawFrame(unsigned long x1, unsigned long y1, unsigned long x2, unsigned long y2, unsigned long color)
{
	unsigned char *vptr0;		//pointer to vram
	unsigned long i;

	vptr0=pgGetVramAddr(0,0);
	for(i=x1; i<=x2; i++){
		((unsigned short *)vptr0)[i*PIXELSIZE + y1*LINESIZE] = color;
		((unsigned short *)vptr0)[i*PIXELSIZE + y2*LINESIZE] = color;
	}
	for(i=y1; i<=y2; i++){
		((unsigned short *)vptr0)[x1*PIXELSIZE + i*LINESIZE] = color;
		((unsigned short *)vptr0)[x2*PIXELSIZE + i*LINESIZE] = color;
	}
}



void pgFillvram(unsigned long color)
{
	unsigned char *vptr0;		//pointer to vram
	unsigned long i;

	vptr0=pgGetVramAddr(0,0);
	for (i=0; i<FRAMESIZE/2; i++) {
		*(unsigned short *)vptr0=color;
		vptr0+=PIXELSIZE*2;
	}
}


void pgBitBlt(unsigned long x,unsigned long y,unsigned long w,unsigned long h,unsigned long mag,const unsigned short *d)
{
	unsigned char *vptr0;		//pointer to vram
	unsigned char *vptr;		//pointer to vram
	unsigned long xx,yy,mx,my;
	const unsigned short *dd;

	vptr0=pgGetVramAddr(x,y);
	for (yy=0; yy<h; yy++) {
		for (my=0; my<mag; my++) {
			vptr=vptr0;
			dd=d;
			for (xx=0; xx<w; xx++) {
				for (mx=0; mx<mag; mx++) {
					*(unsigned short *)vptr=*dd;
					vptr+=PIXELSIZE*2;
				}
				dd++;
			}
			vptr0+=LINESIZE*2;
		}
		d+=w;
	}

}

void pgPutChar(unsigned long x,
unsigned long y,
unsigned long color,unsigned long bgcolor,
unsigned char c,
char drawbg)
{
	unsigned short *vptr0;		//pointer to vram
	unsigned short *vptr;		//pointer to vram
	unsigned char *fnt;		//pointer to font
	unsigned long cx,cy;
	unsigned long b;

	//if (c>255) return;
	//cfont=hankaku_font5x10+(c*(8));
	/* ascii(0x00-0x7f) */
	fnt = (unsigned char *)&hankaku_font5x10[((c-HANKAKU10_OFFS)&0x7f)*(10-2)];
	vptr0=pgGetVramAddr(x,y);
	for (cy=0; cy<(8); cy++) {
		{
			vptr=vptr0;
			b=(*fnt);
			for (cx=0; cx<8; cx++) {
				{
					if ((b&0x80)!=0) {
						/*if (drawfg)*/ *vptr=(unsigned short)color;
					} else {
						if (drawbg) *vptr=(unsigned short)bgcolor;
					}
					vptr+=PIXELSIZE;//*2;
				}
				b<<=1;
			}
			vptr0+=LINESIZE;//*2;
		}
		fnt++;
	}
}
void pgPrint(unsigned long x,unsigned long y,unsigned long color,const char *str)
{
	while (*str!=0 && x<CMAX_X && y<CMAX_Y) {
		pgPutChar(x*8,y*(8+2),color,0,*str,0);
		str++;
		x++;
		if (x>=CMAX_X) {
			x=0;
			y++;
		}
	}
}


void pgScreenFrame(long mode,long frame)
{
	pg_screenmode=mode;
	frame=(frame?1:0);
	pg_showframe=frame;
	switch(mode)
	{
	case 0:	//screen off
		pg_drawframe=frame;
		sceDisplaySetFrameBuf(0,0,0,1);
		break;
	case 1:	//show/draw same
		pg_drawframe=frame;
		sceDisplaySetFrameBuf(pg_vramtop+(pg_showframe?FRAMESIZE:0),LINESIZE,PIXELSIZE,1);
		break;
	case 2:
		//show/draw different
		pg_drawframe=(frame?0:1);
		sceDisplaySetFrameBuf(pg_vramtop+(pg_showframe?FRAMESIZE:0),LINESIZE,PIXELSIZE,1);
		break;
	}
}


void pgScreenFlip()
{
	pg_showframe^=1;//(pg_showframe?0:1);
	pg_drawframe^=1;//(pg_drawframe?0:1);
	sceDisplaySetFrameBuf(pg_vramtop+(pg_showframe?FRAMESIZE:0),LINESIZE,PIXELSIZE,0);
}


void pgScreenFlipV()
{
	pgWaitV();
	pgScreenFlip();
}


// by kwn
void Draw_Char_Hankaku(int x,int y,/*const*/ unsigned char c,int col) {
	unsigned short *vr;
	unsigned char  *fnt;
	unsigned char  pt;
//	unsigned char ch;
	int x1,y1;

//	ch = c;
	// mapping
//	if (ch<0x20)		ch = 0;
//	else if (ch<0x80)	ch -= 0x20;
//	else if (ch<0xa0)	ch = 0;
//	else				ch -= 0x40;
//	fnt = (unsigned char *)&hankaku_font10[ch*10];
	/* ascii(0x00-0x7f) */
	fnt = (unsigned char *)&hankaku_font5x10[((c-HANKAKU10_OFFS)&0x7f)*(10-2)];

	// draw
	vr = (unsigned short *)pgGetVramAddr(x,(1+y));
	for(y1=0;y1<(10-2);y1++) {
		pt = *fnt++;
		for(x1=(0);x1<(5+2);x1++) {
			if (pt & 0x80)
			{	*vr = col;}
			vr++;
			pt <<= 1;
		}
		vr += LINESIZE-(5+2);
	}
}

// by kwn
void Draw_Char_Zenkaku(int x,int y,/*const*/ unsigned char u,unsigned char d,int col) {
	// ELISA100.FNTに存在しない文字
	const unsigned short font404[] = {
		0xA2AF, 11,
		0xA2C2, 8,
		0xA2D1, 11,
		0xA2EB, 7,
		0xA2FA, 4,
		0xA3A1, 15,
		0xA3BA, 7,
		0xA3DB, 6,
		0xA3FB, 4,
		0xA4F4, 11,
		0xA5F7, 8,
		0xA6B9, 8,
		0xA6D9, 38,
		0xA7C2, 15,
		0xA7F2, 13,
		0xA8C1, 720,
		0xCFD4, 43,
		0xF4A5, 1030,
		0,0
	};
	unsigned short *vr;
	unsigned short *fnt;
	unsigned short pt;
	int x1,y1;

	unsigned long n;
	unsigned short code;
//	int i;
	int j;

	// SJISコードの生成
	code = u;
	code = (code<<8) + d;

	// SJISからEUCに変換
	if(code >= 0xE000) code-=0x4000;
	code = ((((code>>8)&0xFF)-0x81)<<9) + (code&0x00FF);
	if((code & 0x00FF) >= 0x80) code--;
	if((code & 0x00FF) >= 0x9E) code+=0x62;
	else code-=0x40;
	code += 0x2121 + 0x8080;

	// EUCから恵梨沙フォントの番号を生成
	n = (((code>>8)&0xFF)-0xA1)*(0xFF-0xA1)
		+ (code&0xFF)-0xA1;
	j=0;
	while(font404[j]) {
		if(code >= font404[j]) {
			if(code <= font404[j]+font404[j+1]-1) {
				n = -1;
				break;
			} else {
				n-=font404[j+1];
			}
		}
		j+=2;
	}
	fnt = (unsigned short *)&zenkaku_font10[n*10];

	// draw
	vr = (unsigned short *)pgGetVramAddr(x,y);
	for(y1=0;y1<10;y1++) {
		pt = *fnt++;
		for(x1=0;x1<10;x1++) {
			if (pt & 1)
				*vr = col;
			vr++;
			pt = pt >> 1;
		}
		vr += LINESIZE-10;
	}
}

// by kwn
void mh_print(int x,int y,/*const*/ char *str,int col) {
	unsigned char ch = 0,bef = 0;

	while(*str != 0) {
		ch = *str++;
		if (bef!=0) {
			Draw_Char_Zenkaku(x,y,bef,ch,col);
			x+=10;
			bef=0;
		} else {
			if (((ch>=0x80) && (ch<0xa0)) || (ch>=0xe0)) {
				bef = ch;
			} else {
				Draw_Char_Hankaku(x,y,ch,col);
				x+=(5+2);
			}
		}
	}
}

/******************************************************************************/
void pgcInit(void)
{
	sceDisplaySetMode(0,SCREEN_WIDTH,SCREEN_HEIGHT);

	pgScreenFrame(0,1);
	pgcLocate(0,0);
	pgcColor(0xffff,0x0000);
	pgScreenFrame(0,0);
	pgcLocate(0,0);
	pgcColor(0xffff,0x0000);
}

void pgcLocate(unsigned long x, unsigned long y)
{
	if (x>=CMAX_X) x=0;
	if (y>=CMAX_Y) y=0;
	pgc_csr_x[pg_drawframe]=x;
	pgc_csr_y[pg_drawframe]=y;
}


void pgcColor(unsigned long fg, unsigned long bg)
{
	pgc_fgcolor[pg_drawframe]=fg;
	pgc_bgcolor[pg_drawframe]=bg;
}



void pgcCls(void)
{
	pgFillvram(pgc_bgcolor[pg_drawframe]);
	pgcLocate(0,0);
}

void pgcPutchar_nocontrol(/*const*/ char ch)
{
	pgPutChar(
		pgc_csr_x[pg_drawframe]*8,
		pgc_csr_y[pg_drawframe]*8,
		pgc_fgcolor[pg_drawframe],
		pgc_bgcolor[pg_drawframe],
		ch,
		1
	);
	pgc_csr_x[pg_drawframe]+=1/*pgc_mag*/;
	if (pgc_csr_x[pg_drawframe]>CMAX_X-1/*pgc_mag*/) {
		pgc_csr_x[pg_drawframe]=0;
		pgc_csr_y[pg_drawframe]+=1/*pgc_mag*/;
		if (pgc_csr_y[pg_drawframe]>CMAX_Y-1/*pgc_mag*/) {
			pgc_csr_y[pg_drawframe]=CMAX_Y-1/*pgc_mag*/;
//			pgMoverect(0,/*pgc_mag*/1*8,SCREEN_WIDTH,SCREEN_HEIGHT-1/*pgc_mag*/*8,0,0);
		}
	}
}

void pgcPutchar(/*const*/ char ch)
{
	if (ch==0x0d) {
		pgc_csr_x[pg_drawframe]=0;
		return;
	}
	if (ch==0x0a) {
		if ((++pgc_csr_y[pg_drawframe])>=CMAX_Y) {
			pgc_csr_y[pg_drawframe]=CMAX_Y-1;
//			pgMoverect(0,8,SCREEN_WIDTH,SCREEN_HEIGHT-8,0,0);
		}
		return;
	}
	pgcPutchar_nocontrol(ch);
}

void pgcPuthex2(/*const*/ unsigned long s)
{
	char ch;
	ch=((s>>4)&0x0f);
	pgcPutchar((ch<10)?(ch+0x30):(ch+0x40-9));
	ch=(s&0x0f);
	pgcPutchar((ch<10)?(ch+0x30):(ch+0x40-9));
}


void pgcPuthex8(/*const*/ unsigned long s)
{
	pgcPuthex2(s>>24);
	pgcPuthex2(s>>16);
	pgcPuthex2(s>>8);
	pgcPuthex2(s);
}



/******************************************************************************/
//#define PGA_SAMPLES 512
#define PGA_SAMPLES 1024
//#define MAXVOLUME 0x8000

//static int bSoundEnable=0;
static int sound_handle;

//int pga_volume =0x8000;/* used from src\psp\sound.c */
unsigned char bSoundEnable;


static int sound_thread;


volatile int pga_terminate=0;

extern short int *finalmix;
extern int finalmixLen;
extern int finalmixCur;
extern int finalmixCurNow;
extern int finalmixCounter;
extern unsigned long cur_play;
extern int cache_len;

//unsigned long osd_cycles(void);
#define PGA_VOLUME (0xffff)
/*--------------------------------------------------------
	サウンド更新スレッド
--------------------------------------------------------*/

static int sound_update_thread(int args, void *argp)
{
static short int *lptr =0x00;

	//int channel=*(int *)argp;

	while (pga_terminate==0) {
		if (PGA_SAMPLES >cache_len) {
//			if (bSoundEnable && !sceAudioGetChannelRestLen(sound_handle))
//				sceAudioOutputPanned(sound_handle, PGA_VOLUME, PGA_VOLUME, lptr);
			sceKernelDelayThread(1000000/60/10);
			continue;
		}
		cache_len -=PGA_SAMPLES;

		unsigned int ptr =cur_play;
		unsigned int nextptr =ptr +PGA_SAMPLES *2;
		if (nextptr >=finalmixLen) nextptr=0;
		lptr =&finalmix[ptr];
//		if (bSoundEnable && (cache_len*2) <finalmixLen)
		if(bSoundEnable){
			if (cache_len <PGA_SAMPLES*2)
			{		sceAudioOutputPannedBlocking(sound_handle, PGA_VOLUME, PGA_VOLUME, lptr);}
		}else{
			if (cache_len <PGA_SAMPLES*2)
			{		sceAudioOutputPannedBlocking(sound_handle, 0, 0, lptr);}
		}
		cur_play =nextptr;
	}
	return 0;
}


/******************************************************************************/
int pgaInit(void)
{
	int /*i,*/ret;
	int failed=0;

	pga_terminate=0;
	bSoundEnable=0;

	sound_handle=-1;
	sound_thread=-1;
	// par3 - 0x10:mono
	if ((sound_handle=sceAudioChReserve(-1,PGA_SAMPLES,0))<0) failed=1;
	if (failed) {
		if (sound_handle!=-1) sceAudioChRelease(sound_handle);
		sound_handle=-1;
		return -1;
	}
	bSoundEnable=1;


	sound_thread =sceKernelCreateThread("Sound thread", sound_update_thread, /*0x12*/0x08, 0x10000, 0, 0);
	if (sound_thread<0) {
		sound_thread=-1;
		failed=1;
		goto my_failed;//break;
	}
	ret =sceKernelStartThread(sound_thread, 0, 0);
	if (ret!=0) {
		failed=1;
		//goto my_failed;//break;
	}

my_failed:
	if (failed) {
		pga_terminate=1;
		if (sound_thread!=-1) {
			sceKernelWaitThreadEnd(sound_thread, 0);
			sceKernelDeleteThread(sound_thread);
		}
		sound_thread=-1;
		bSoundEnable=0;
		return -1;
	}
	return 0;
}

void pgaTermPre(void)
{
	bSoundEnable=0;
	pga_terminate=1;
}

void pgaTerm(void)
{
	bSoundEnable=0;
	pga_terminate=1;

	if (sound_thread!=-1) {
		sceKernelWaitThreadEnd(sound_thread, 0);
		sceKernelDeleteThread(sound_thread);
	}
	sound_thread=-1;

	if (sound_handle!=-1) {
		sceAudioChRelease(sound_handle);
		sound_handle!=-1;
	}
}
//バッファは64バイト境界じゃなくても大丈夫みたい
//[0]が左、[1]が右
//サンプル速度は44100
//vol1が左
