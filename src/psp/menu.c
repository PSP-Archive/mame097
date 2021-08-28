
#include <stdio.h>
#include <string.h>

#include "driver.h"

#include "menu.h"
#include "syscall.h"
//#include "colbl.c"



#define NOW_DATE   " "__DATE__" "
#define DEVELOPPER "∽∝∵∫∬開発幼稚園"

#if (1==namcoNA)
	#define APP_TITLE "￢⇒⇔∀∃∠⊥⌒NA1/NA2(A10)" NOW_DATE "まだたまに死ぬ。" DEVELOPPER
#elif (1==taitoAX)
	#define APP_TITLE " taito AX(A05)" NOW_DATE "まだ重いかなぁ？" DEVELOPPER
#elif (1==m72)
	#define APP_TITLE " irem m72(A02)" NOW_DATE "まだ重いです。" DEVELOPPER
#elif (1==namcos86)
	#define APP_TITLE "￢⇒⇔∀∃∠⊥⌒86(A03)" NOW_DATE "落ちないでくれええええぇ。" DEVELOPPER
#else
	#define APP_TITLE "pspMAME r0.4(0.97s)" NOW_DATE "もっと速くないと遊べん！" DEVELOPPER
#endif

#define MENU_Y_OFFS (5-1)

/* 文字列に１０進数を嵌め込む。*p:嵌め込み位置。val:１０進数、数値。n:嵌め込み桁数。 */
 void decputn( char *p, int val, int n )
{
int i;
	switch(n){
		case 5: i=0; while( 9999 < val){val-=10000; i++;}; *p=('0'+i);p++;
		case 4: i=0; while(  999 < val){val-= 1000; i++;}; *p=('0'+i);p++;
		case 3: i=0; while(   99 < val){val-=  100; i++;}; *p=('0'+i);p++;
		case 2: i=0; while(    9 < val){val-=	10; i++;}; *p=('0'+i);p++;
		case 1: i=0; while(    0 < val){val--;		i++;}; *p=('0'+i);
	}
}
// ----------------------------------------
static SceCtrlData/*ctrl_data_t*/ ctl;
// ----------------------------------------
//static	unsigned long is_PAD;
/* triggered based the pad input */
//[is_#] now input key.
//[rs_#] old key.(before key)
//[ps_#] is pressed.(or pushed)[trigged the key]
//[cs_#] count holding key at time.(fps base counted)

static unsigned int is_PAD; // read pad raw data.

//atic unsigned char is_E,rs_E,ps_E;//,cs_E;	// E: Select//disabled
//atic unsigned char is_S,rs_S,ps_S;//,cs_S;	// S: Start
static unsigned char is_U,rs_U,ps_U,cs_U;	// U: Up
static unsigned char is_J,rs_J,ps_J,cs_J;	// J: Right (see your keyboard)
static unsigned char is_D,rs_D,ps_D,cs_D;	// D: Down
static unsigned char is_F,rs_F,ps_F,cs_F;	// F: Left  (see your keyboard)
static unsigned char is_L,rs_L,ps_L;//,cs_L;	// L: L-trigger
static unsigned char is_R,rs_R,ps_R;//,cs_R;	// R: R-trigger
static unsigned char is_A,rs_A,ps_A;//,cs_A;	// A: triangle
static unsigned char is_O,rs_O,ps_O;//,cs_O;	// O: Circle
static unsigned char is_X,rs_X,ps_X;//,cs_X;	// X: Cross
static unsigned char is_Q,rs_Q,ps_Q;//,cs_Q;	// Q: Square
// ----------------------------------------
static void pad_read(void)
{
static char ccc; //counter
unsigned char ttt; // work
	ccc++; //count up
	ccc&=1; // masked on/off
	//key = Read_Key();
//	sceCtrlReadBufferPositive(&ctl,1);
	sceCtrlPeekBufferPositive(&ctl,1);
// 0123456789abcdef
// Uuuu--------dddD

	/* pached the analog stick to digitily sense. */
	ttt=ctl.Ly;//analog[CTRL_ANALOG_Y];
	     if(ttt < 0x10) {         ctl.Buttons |= PSP_CTRL_UP;    /* UP HIGH */ }
	else if(ttt < 0x40) { if(ccc) ctl.Buttons |= PSP_CTRL_UP;    /* UP LOW  */ }
	else if(ttt < 0xc0) { ; /* NONE */ }
	else if(ttt < 0xf0) { if(ccc) ctl.Buttons |= PSP_CTRL_DOWN;  /* DOWN LOW  */ }
	else                {         ctl.Buttons |= PSP_CTRL_DOWN;  /* DOWN HIGH */ }

	ttt=ctl.Lx;//analog[CTRL_ANALOG_X];
	     if(ttt < 0x10) {         ctl.Buttons |= PSP_CTRL_LEFT;   /* LEFT HIGH */ }
	else if(ttt < 0x40) { if(ccc) ctl.Buttons |= PSP_CTRL_LEFT;   /* LEFT LOW  */ }
	else if(ttt < 0xc0) { ; /* NONE */ }
	else if(ttt < 0xf0) { if(ccc) ctl.Buttons |= PSP_CTRL_RIGHT;  /* RIGHT LOW  */ }
	else                {         ctl.Buttons |= PSP_CTRL_RIGHT;  /* RIGHT HIGH */ }

	/* before buttons sense */
//	rs_E=is_E;
//	rs_S=is_S;
	rs_L=is_L;
	rs_R=is_R;

	rs_U=is_U;
	rs_J=is_J;
	rs_D=is_D;
	rs_F=is_F;

	rs_A=is_A;
	rs_O=is_O;
	rs_X=is_X;
	rs_Q=is_Q;

	/* now buttons sense */
	is_PAD=ctl.Buttons;
//	is_E=(is_PAD & PSP_CTRL_SELECT);
//	is_S=(is_PAD & PSP_CTRL_START);
	is_L=(is_PAD & PSP_CTRL_LTRIGGER)?1:0;
	is_R=(is_PAD & PSP_CTRL_RTRIGGER)?1:0;

	is_U=(is_PAD & PSP_CTRL_UP);
	is_J=(is_PAD & PSP_CTRL_RIGHT);
	is_D=(is_PAD & PSP_CTRL_DOWN);
	is_F=(is_PAD & PSP_CTRL_LEFT);

	is_A=(is_PAD & PSP_CTRL_TRIANGLE)?1:0;
	is_O=(is_PAD & PSP_CTRL_CIRCLE)?1:0;
	is_X=(is_PAD & PSP_CTRL_CROSS)?1:0;
	is_Q=(is_PAD & PSP_CTRL_SQUARE)?1:0;

	/* now pushed or pulled buttons(triggered check) */
//	ps_E=(is_E != rs_E);
//	ps_S=(is_S != rs_S);
	ps_L=(is_L != rs_L);
	ps_R=(is_R != rs_R);

	ps_U=(is_U != rs_U);
	ps_J=(is_J != rs_J);
	ps_D=(is_D != rs_D);
	ps_F=(is_F != rs_F);

	ps_A=(is_A != rs_A);
	ps_O=(is_O != rs_O);
	ps_X=(is_X != rs_X);
	ps_Q=(is_Q != rs_Q);

	/* key pressed time counter at a frame */ /* 要するにオートリピート */
	if(is_U) cs_U++;	if(ps_U) cs_U=0;
	if(is_D) cs_D++;	if(ps_D) cs_D=0;
	if(is_F) cs_F++;	if(ps_F) cs_F=0;
	if(is_J) cs_J++;	if(ps_J) cs_J=0;
//	if(is_E) cs_E++;	if(ps_E) cs_E=0;//disabled
}
//unsigned long Read_Key(void);
//u32 new_pad;


/////////////////Filer
#define PATHLIST_H 20
//#define MAXPATH   0x108
static int	dlist_num;
static int	dlist_start;
int	dlist_curpos;


extern SETTING setting;

//#define REPEAT_TIME 0x40000
//static unsigned long control_bef_ctl  = 0;
//static unsigned long control_bef_tick = 0;

//unsigned long Read_Key(void) {
//	/*ctrl_data_t*/SceCtrlData ctl;
//
//	//sceCtrlReadBufferPositive(&ctl,1);
//	sceCtrlPeekBufferPositive(&ctl,1);
//	if (ctl.Buttons == control_bef_ctl) {
//		if ((ctl.TimeStamp - control_bef_tick) > REPEAT_TIME) {
//			return control_bef_ctl;
//		}
//		return 0;
//	}
//	control_bef_ctl  = ctl.Buttons;
//	control_bef_tick = ctl.TimeStamp;
//	return control_bef_ctl;
//}

void Get_DriverList(void) {
	int i;

	dlist_num = 0;

	for (i = 0;drivers[i];i++)
		dlist_num +=1;

	dlist_start  = 0;
	dlist_curpos = 0;
}


void Draw_All(void)
{
	int	i, col;
//	char wk[20];
//	int clk =CLOCKS_PER_SEC;

//	pgFillvram(0);

//	mh_print(0, 0, APP_TITLE, setting.color[1]);
//	sprintf(wk, "%d", clk);
//	mh_print(0,10, wk, 0xffff);

	psp_frame(0, 0);

	// ゲームリスト
	i = dlist_start;
	while (i<(dlist_start+PATHLIST_H)) {
		if (i<dlist_num) {
			col = setting.color[2];
			if (i==dlist_curpos) {
				col = setting.color[3];
			}
#if (1==namcoNA)
			/*namcoNA*/
			mh_print(((4-1)*8)-2, (((i-dlist_start)+4)*10)+((i>10)?5:0), (unsigned char *)drivers[i]->description, col);
#elif (1==namcos86)
			mh_print(((4-1)*8)-2, ((i-dlist_start)+3)*14, (unsigned char *)drivers[i]->description, col);
#else
//			mh_print(((4-1)*8)-2, ((i-dlist_start)+2)*10, (unsigned char *)drivers[i]->name, col);
			mh_print(((4-1)*8)-2, ((i-dlist_start)+4)*10, (unsigned char *)drivers[i]->description, col);
#endif
		}
		i++;
	}
	pgScreenFlipV();
}

int Confirm_Control(void) {
//	unsigned long key;

	while(1) {
		while(1) {
			pad_read();//key = Read_Key();
			if (is_PAD != 0) break;
			pgWaitV();
		}

		if (ps_O && is_O) {
			return 1;
		}
		if (ps_X && is_X) {
			return 0;
		}
	}
}

int Control(void) {
//	unsigned long key;
//	int i;

	// wait key
	while(1) {
		pad_read();//key = Read_Key();
		if (is_PAD != 0) break;
		pgWaitV();
	}
//#define REPEAT_TIME_W 6
//	if ( ((ps_U ||(REPEAT_TIME_W<cs_U)) && is_U) ){
	if ((is_U)&&((6<cs_U)&&(0==(cs_U&7))||(ps_U))) {
		if (dlist_curpos > 0) {
			dlist_curpos--;
			if (dlist_curpos < dlist_start) { dlist_start = dlist_curpos; }
		}
	}
	if ((is_D)&&((6<cs_D)&&(0==(cs_D&7))||(ps_D))) {
		if (dlist_curpos < (dlist_num-1)) {
			dlist_curpos++;
			if (dlist_curpos >= (dlist_start+PATHLIST_H)) { dlist_start++; }
		}
	}
	if (ps_F && is_F) {
		dlist_curpos -= PATHLIST_H;
		if (dlist_curpos <  0)          { dlist_curpos = 0;           }
		if (dlist_curpos < dlist_start) { dlist_start = dlist_curpos; }
	}
	if (ps_J && is_J) {
		dlist_curpos += PATHLIST_H;
		if (dlist_curpos >= dlist_num) { dlist_curpos = dlist_num-1; }
		while (dlist_curpos >= (dlist_start+PATHLIST_H)) { dlist_start++; }
	}

	if (ps_O && is_O) {
		return 2;
	}
	if (ps_L && is_L) {
		return 3;
	}

	return 0;
}






void Draw_Confirm(void) {
//	pgFillvram(0);
	psp_frame(0, 0);

	mh_print(40, 80, (unsigned char *)drivers[dlist_curpos]->name, setting.color[3]);
	mh_print(50, 90, (unsigned char *)drivers[dlist_curpos]->description, setting.color[3]);
	mh_print(40,120, (unsigned char *)"を実行します。", setting.color[3]);

	mh_print(40,160, (unsigned char *)"実行：○  キャンセル：×", setting.color[3]);
	pgScreenFlipV();
}




//////////////////


const char *scr_names[] = {
	"x1",
	"stretch image",
	"stretch & aspect keep",
	"rotate",
	"rotate & stretch",
	"rotate & stretch & aspect keep",
};

int bBitmap;
unsigned short bgBitmap[480*272];

char *getCrDir(void);
SETTING setting;

void save_config(void)
{
	char CfgPath[MAX_PATH];
	char *p;

	strcpy(CfgPath, getCrDir());
	p = strrchr(CfgPath, '/')+1;
	strcpy(p, "PSPMAME.CFG");

	int fd;
	fd = sceIoOpen(CfgPath,O_CREAT|O_WRONLY|O_TRUNC, 0777);
	if(fd>=0){
		sceIoWrite(fd, &setting, sizeof(setting));
		sceIoClose(fd);
	}
}

/* now *NO* freq! divider sound...   */

#define SET_05512 (3)
#define SET_11025 (2)
#define SET_22050 (1)
#define SET_44100 (0)

/* Fixed 44100 */
	#define SET_DEFAULT_SOUND_FREQ SET_44100
//	#define SET_DEFAULT_SOUND_FREQ SET_11025

#if (1==namcoNA)
	#define SET_DEFAULT_FSKIPS (8)
#elif (1==taitoAX)
	#define SET_DEFAULT_FSKIPS (9)
#elif (1==m72)
	#define SET_DEFAULT_FSKIPS (9)
#else
/* others */
	#define SET_DEFAULT_FSKIPS (8)
#endif

void load_config(void)
{
//	int i;
	char CfgPath[MAX_PATH];
	char *p;

	memset(&setting, 0, sizeof(setting));

	strcpy(CfgPath, getCrDir());
	p = strrchr(CfgPath, '/')+1;
	strcpy(p, "PSPMAME.CFG");

	int fd;
	fd = sceIoOpen(CfgPath,O_RDONLY, 0777);
	if(fd>=0){
		sceIoRead(fd, &setting, sizeof(setting));
		sceIoClose(fd);

		if(setting.screensize<0 || setting.screensize>=SCR_END)
															setting.screensize = SCR_X1;
		if(setting.frameskip > /*9*/60) 					setting.frameskip=/*0*/60;
		//if(setting.sound_rate<0 || setting.sound_rate>2)	setting.sound_rate = /*0*/2;
		setting.sound_rate &=3;
		if(setting.bgbright<0 || setting.bgbright>100)		setting.bgbright=100;
		if(setting.cpu_clock<0 || setting.cpu_clock>2)		setting.cpu_clock = /*0*/2;
		if(!strcmp(setting.vercnf, CONFIG_VER))
			return;
	}

	strcpy(setting.vercnf, CONFIG_VER);

	setting.screensize = SCR_X1;
	setting.frameskip = /*0*//*6*/SET_DEFAULT_FSKIPS/*8*/;
	setting.vsync = 0;
	setting.sound = 1;
	setting.sound_rate = /*0*/SET_DEFAULT_SOUND_FREQ/*2*/;/*default sound rate 44100 */

	setting.color[0] = DEF_COLOR0;
	setting.color[1] = DEF_COLOR1;
	setting.color[2] = DEF_COLOR2;
	setting.color[3] = DEF_COLOR3;
	setting.bgbright=100;

	setting.cpu_clock = 2/*0*/;
}

/*static*/ void load_menu_bg(void)
{
	unsigned char *menu_bg;
	unsigned short *vptr;
	static unsigned char menu_bg_buf[480*272*3+0x36];
	char BgPath[MAX_PATH];
	char *p;
	unsigned short x,y,yy,r,g,b;
	unsigned short data;

	strcpy(BgPath, getCrDir());
	p = strrchr(BgPath, '/')+1;
	strcpy(p, "MENU.BMP");

	int fd;
	fd = sceIoOpen(BgPath,O_RDONLY,0777);
	if(fd>=0){
		sceIoRead(fd, menu_bg_buf, 480*272*3+0x36);
		sceIoClose(fd);

		menu_bg = menu_bg_buf + 0x36;
		vptr=bgBitmap;
		for(y=0; y<272; y++){
			for(x=0; x<480; x++){
				yy = 271 - y;
				r = *(menu_bg + (yy*480 + x)*3 + 2);
				g = *(menu_bg + (yy*480 + x)*3 + 1);
				b = *(menu_bg + (yy*480 + x)*3);
				data = (((b & 0xf8) << 7) | ((g & 0xf8) << 2) | (r >> 3));
				*vptr=data;
				vptr+=/*2*/1;
			}
		}
		bBitmap = 1;
	}else{
		bBitmap = 0;
	}
}



/*static*/ void psp_frame(const char *msg0, const char *msg1)
{
	if(bBitmap)/* 壁紙 */
	{	pgBitBlt(0,0,480,272,1,bgBitmap);}
	else/* 壁紙なし */
	{	pgFillvram(setting.color[0/*2*/]);}
	mh_print(0, 0, APP_TITLE, setting.color[3]);
	// メッセージなど
	if(msg0) mh_print(17, 14, msg0, setting.color[2]);
	pgDrawFrame(17,25,463,248,setting.color[1]);
	pgDrawFrame(18,26,462,247,setting.color[1]);
	// 操作説明
	if(msg1) mh_print(17, 252, msg1, setting.color[2]);
}

// 半透明処理
static unsigned short rgbTransp(unsigned short fgRGB, unsigned short bgRGB, int alpha) {

	unsigned int fgR, fgG, fgB;
	unsigned int bgR, bgG, bgB;
	unsigned int R, G, B;
	unsigned int rgb;

	alpha *= 256;
	alpha /= 100;

	fgB = (fgRGB >> 10) & 0x1F;	bgB = (bgRGB >> 10) & 0x1F;	R = (fgR*(alpha))+(bgR*(256-alpha)); R>>=9;
	fgG = (fgRGB >>  5) & 0x1F;	bgG = (bgRGB >>  5) & 0x1F;	G = (fgG*(alpha))+(bgG*(256-alpha)); G>>=9;
	fgR = (fgRGB      ) & 0x1F;	bgR = (bgRGB      ) & 0x1F;	B = (fgB*(alpha))+(bgB*(256-alpha)); B>>=9;

//	R = coltbl[fgR][bgR][alpha/10];
//	G = coltbl[fgG][bgG][alpha/10];
//	B = coltbl[fgB][bgB][alpha/10];

	rgb = (
		((B & 0x1F)<<10)+
		((G & 0x1F)<< 5)+
		((R & 0x1F)<< 0)+0x8000);
	return rgb;
}

/*static*/ void bgbright_change(void)
{
	unsigned short *vptr,rgb;
	int i;

//	load_menu_bg();
	vptr=bgBitmap;
	for(i=0; i<272*480; i++){
			rgb = *vptr;
			*vptr = rgbTransp(rgb, 0x0000, setting.bgbright);
			vptr++;
	}
}

void psp_colorconfig(void)
{
	enum
	{
		COLOR0_R=0,
		COLOR0_G,
		COLOR0_B,
		COLOR1_R,
		COLOR1_G,
		COLOR1_B,
		COLOR2_R,
		COLOR2_G,
		COLOR2_B,
		COLOR3_R,
		COLOR3_G,
		COLOR3_B,
		BG_BRIGHT,
		EXIT,
		INIT,
	};
	char tmp[4], msg[256];
	int color[4][3];
	int sel=0, x, y, i, sel_count=0;

	memset(color, 0, sizeof(int)*4*3);
	for(i=0; i<4; i++){
		color[i][2] = (setting.color[i]>>10) & 0x1F;
		color[i][1] = (setting.color[i]>> 5) & 0x1F;
		color[i][0] = (setting.color[i]    ) & 0x1F;
	}

	for(;;){
		pad_read();//new_pad =Read_Key();
		if( ps_O && is_O ){
			if(sel==EXIT){
				break;
			}else if(sel==INIT){
				color[0][2] = (DEF_COLOR0>>10) & 0x1F;
				color[0][1] = (DEF_COLOR0>> 5) & 0x1F;
				color[0][0] = (DEF_COLOR0    ) & 0x1F;
				color[1][2] = (DEF_COLOR1>>10) & 0x1F;
				color[1][1] = (DEF_COLOR1>> 5) & 0x1F;
				color[1][0] = (DEF_COLOR1    ) & 0x1F;
				color[2][2] = (DEF_COLOR2>>10) & 0x1F;
				color[2][1] = (DEF_COLOR2>> 5) & 0x1F;
				color[2][0] = (DEF_COLOR2    ) & 0x1F;
				color[3][2] = (DEF_COLOR3>>10) & 0x1F;
				color[3][1] = (DEF_COLOR3>> 5) & 0x1F;
				color[3][0] = (DEF_COLOR3    ) & 0x1F;
				setting.bgbright = 100;
				if(bBitmap){
					load_menu_bg();
					bgbright_change();
				}
			}else if(sel == BG_BRIGHT) {
				//輝度変更
				setting.bgbright += 10;
				if(setting.bgbright > 100) setting.bgbright=0;
				if(bBitmap){
					load_menu_bg();
					bgbright_change();
				}
			}else{
				if(color[sel/3][sel%3]<31)
					color[sel/3][sel%3]++;
			}
		}else if( ps_X && is_X ){
			if(sel == BG_BRIGHT) {
				//輝度変更
				setting.bgbright -= 10;
				if(setting.bgbright < 0) setting.bgbright=100;
				if(bBitmap){
					load_menu_bg();
					bgbright_change();
				}
			}else if(sel>=COLOR0_R && sel<=COLOR3_B){
				if(color[sel/3][sel%3]>0)
					color[sel/3][sel%3]--;
			}
		}else if( ps_U && is_U ){
			if(sel!=0)	sel--;
			else		sel=INIT;
			sel_count=0;
		}else if( ps_D && is_D ){
			if(sel!=INIT)	sel++;
			else			sel=0;
			sel_count=0;
		}else if( ps_J && is_J ){
			if(sel<COLOR1_R)		sel=COLOR1_R;
			else if(sel<COLOR2_R)	sel=COLOR2_R;
			else if(sel<COLOR3_R)	sel=COLOR3_R;
			else if(sel<BG_BRIGHT)	sel=BG_BRIGHT;
			else if(sel<EXIT)		sel=EXIT;
			sel_count=0;
		}else if( ps_F && is_F ){
			if(sel>BG_BRIGHT)		sel=BG_BRIGHT;
			else if(sel>COLOR3_B)	sel=COLOR3_R;
			else if(sel>COLOR2_B)	sel=COLOR2_R;
			else if(sel>COLOR1_B)	sel=COLOR1_R;
			else					sel=COLOR0_R;
			sel_count=0;
		}

		for(i=0; i<4; i++)
			setting.color[i]=color[i][2]<<10|color[i][1]<<5|color[i][0]|0x8000;

		if (++sel_count>=30) sel_count=0;

		x = 2;
		y = MENU_Y_OFFS;

		if(sel>=COLOR0_R && sel<=BG_BRIGHT)
			strcpy(msg, "○：Add　×：Sub");
		else
			strcpy(msg, "○：OK");

		psp_frame(0, msg);

		pgPrint(x,y++,setting.color[3],"  COLOR0 R:");
		pgPrint(x,y++,setting.color[3],"  COLOR0 G:");
		pgPrint(x,y++,setting.color[3],"  COLOR0 B:");
		y++;
		pgPrint(x,y++,setting.color[3],"  COLOR1 R:");
		pgPrint(x,y++,setting.color[3],"  COLOR1 G:");
		pgPrint(x,y++,setting.color[3],"  COLOR1 B:");
		y++;
		pgPrint(x,y++,setting.color[3],"  COLOR2 R:");
		pgPrint(x,y++,setting.color[3],"  COLOR2 G:");
		pgPrint(x,y++,setting.color[3],"  COLOR2 B:");
		y++;
		pgPrint(x,y++,setting.color[3],"  COLOR3 R:");
		pgPrint(x,y++,setting.color[3],"  COLOR3 G:");
		pgPrint(x,y++,setting.color[3],"  COLOR3 B:");
		y++;
		if(setting.bgbright / 100 == 1)
			pgPrint(x,y++,setting.color[3],"  BG BRIGHT:100%");
		else
			pgPrint(x,y++,setting.color[3],"  BG BRIGHT:  0%");
		if(setting.bgbright % 100 != 0) 		// 10%～90%
			pgPutChar((x+13)*8,(y-1)*(8+2),setting.color[3],0,'0'+setting.bgbright/10,0);
		y++;
		pgPrint(x,y++,setting.color[3],"  Return to Main Menu");
		pgPrint(x,y++,setting.color[3],"  Initialize");

		x=14;
		y=MENU_Y_OFFS;
		for(i=0; i<12; i++){
			if(i!=0 && i%3==0) y++;
			//_itoa(color[i/3][i%3], tmp);
			sprintf(tmp, "%d", color[i/3][i%3]);
			pgPrint(x,y++,setting.color[3],tmp);
		}

		if (sel_count < 15){
			x=2;
			y=MENU_Y_OFFS;
			y+=sel;
			if(sel>=COLOR1_R) y++;
			if(sel>=COLOR2_R) y++;
			if(sel>=COLOR3_R) y++;
			if(sel>=BG_BRIGHT) y++;
			if(sel>=EXIT) y++;
			pgPutChar((x+1)*8,y*(8+2),setting.color[3],0,/*127*/'>',0);
		}

		pgScreenFlipV();
	}
}

int psp_screensize(int n)
{
	int x,y,i,sel=n;

	for(;;){
		pad_read();//new_pad =Read_Key();
		if( ps_O && is_O )
			return sel;
		else if( ps_X && is_X )
			return -1;
		else if( ps_D && is_D ){
			sel++;
			if(sel>=SCR_END) sel=0;
		}else if( ps_U && is_U ){
			sel--;
			if(sel<0) sel=SCR_END-1;
		}else if( ps_J && is_J ){
			sel+=SCR_END/2;
			if(sel>=SCR_END) sel=SCR_END-1;
		}else if( ps_F && is_F ){
			sel-=SCR_END/2;
			if(sel<0) sel=0;
		}

		psp_frame("Select Screen Mode", "○：OK  ×：Cancel");

		x=4;
		y=MENU_Y_OFFS;
		pgPrint(x++,y++,setting.color[3],"SCREEN MODE:");
		for(i=0; i<SCR_END; i++){
			if(i==sel)
			{	pgPrint(x,y++,setting.color[2],scr_names[i]);}
			else
			{	pgPrint(x,y++,setting.color[3],scr_names[i]);}
		}

		pgScreenFlipV();
	}
}

#if 00
int psp_frameskip(int sel)
{
	char tmp[8];
	int x,y,i;

	strcpy(tmp,"0");

	for(;;){
		pad_read();//new_pad =Read_Key();
		if( ps_O && is_O )
			return sel;
		else if( ps_X && is_X )
			return -1;
		else if( ps_D && is_D ){
			sel++;
			if(sel>10) sel=0;
		}else if( ps_U && is_U ){
			sel--;
			if(sel<0) sel=10;
		}else if( ps_J && is_J ){
			sel+=5;
			if(sel>10) sel=10;
		}else if( ps_F && is_F ){
			sel-=5;
			if(sel<0) sel=0;
		}

		psp_frame("Select Frame Skip", "○：OK  ×：Cancel");

		x=4;
		y=MENU_Y_OFFS;
		pgPrint(x++,y++,setting.color[3],"FRAME SKIP:");
		if(0==sel)
		{	pgPrint(x,y++,setting.color[2],"Auto");}
		else
		{	pgPrint(x,y++,setting.color[3],"Auto");}
		for(i=1; i<=20; i++){
			tmp[0] = i + '0' -1;
			if(i==sel)
			{	pgPrint(x,y++,setting.color[2],tmp);}
			else
			{	pgPrint(x,y++,setting.color[3],tmp);}
		}

		pgScreenFlipV();
	}
}
#endif

const char *cpu_clocks[] = {
	"222MHz",
	"266MHz",
	"333MHz (default)",
};

#if 00
int psp_cpuclock(int sel)
{
	unsigned long color;
	//char *msg;
	int x,y,i;

	for(;;){
		pad_read();//new_pad =Read_Key();
		if( ps_O && is_O )			return sel;
		else if( ps_X && is_X )		return -1;
		else if( ps_D && is_D ){
			sel++;
			if(sel>2) sel=0;
		}else if( ps_U && is_U ){
			sel--;
			if(sel<0) sel=2;
		}

		psp_frame("Select CPU Clock (DANGER! DANGER! DANGER! DANGER! DANGER!)", "○：OK  ×：Cancel");

		x=4;
		y=MENU_Y_OFFS;
		pgPrint(x++,y++,setting.color[3],"CPU CLOCK:");
		for(i=0; i<3; i++){
			if(i==sel)	color = setting.color[2];
			else		color = setting.color[3];
			pgPrint(x,y++,color,cpu_clocks[i]);
		}

		pgScreenFlipV();
	}
}
#endif

void psp_menu(void)
{
	enum
	{
		SOUND,
		SOUND_RATE,
		SCREEN_SIZE,
		FRAME_SKIP,
		VSYNC,
		CPU_CLOCK,
		COLOR_CONFIG,
		CONTINUE,
	};
	char msg[256], tmp[256];
	static int sel=0;
	int x, y, sel_count=0, bLoop=1;
	int ret;
	//int bSave, fd, romsize, ramsize;
	//char *p;

	msg[0]=0;

	for(;;){
		pad_read();//new_pad =Read_Key();
		if(is_F||is_J){
			switch(sel)
			{
			case FRAME_SKIP:
				if((6<cs_F)&&(0==(cs_F&7))||(ps_F)) {setting.frameskip--;}
				if((6<cs_J)&&(0==(cs_J&7))||(ps_J)) {setting.frameskip++;}
				setting.frameskip &= 63;
				msg[0]=0;
				sel_count=0;
				break;
			}
		}
		else if( ps_O && is_O ){
			switch(sel)
			{
			case SOUND:
				setting.sound = !setting.sound;
				break;

			case SOUND_RATE:
				setting.sound_rate++;
				setting.sound_rate &=3;
				//if(setting.sound_rate>2) setting.sound_rate=0;
				break;

			case SCREEN_SIZE:
				ret = psp_screensize(setting.screensize);
				if(ret>=0)
					setting.screensize = ret;
				msg[0]=0;
				sel_count=0;
				break;

			case VSYNC:
				setting.vsync = !setting.vsync;
				break;

			case CPU_CLOCK:
				/*ret =*/ setting.cpu_clock++;//psp_cpuclock(setting.cpu_clock);
				if(setting.cpu_clock>2) setting.cpu_clock=0;
			//	if(ret>=0){
			//		setting.cpu_clock = ret;
//					if(setting.cpu_clock==0)
//						scePowerSetClockFrequency(222,222,111);
//					else if(setting.cpu_clock==1)
//						scePowerSetClockFrequency(266,266,133);
//					else if(setting.cpu_clock==2)
//						scePowerSetClockFrequency(333,333,166);
			//	}
				break;

			case COLOR_CONFIG:
				psp_colorconfig();
				msg[0]=0;
				sel_count=0;
				break;

			case CONTINUE:
				bLoop = 0;
				break;
			}
		}else if( ( ps_X && is_X ) || ( ps_L && is_L ) ){
			break;

		}else if( ps_U && is_U ){
			if(sel!=0)
				sel--;
			else
				sel=CONTINUE;
			sel_count=0;

		}else if( ps_D && is_D ){
			if(sel!=CONTINUE)	sel++;
			else				sel=0;
			sel_count=0;

//		}else if(setting.key_config[6] && (is_PAD&setting.key_config[6])==setting.key_config[6]){
//			break;
		}

		if(!bLoop) break;

		if (++sel_count>=30) sel_count=0;

		psp_frame(msg, "○：OK  ×：Continue  MenuBTN：Continue");

		x=2;
		y=MENU_Y_OFFS;
		if(setting.sound)
		{	pgPrint(x,y,setting.color[3],"  SOUND: ON");}
		else
		{	pgPrint(x,y,setting.color[3],"  SOUND: OFF");}
		y++;
		switch(setting.sound_rate){
		case 0: pgPrint(x,y,setting.color[3],"  SOUND RENDER: (1/1)"
			#if(SET_DEFAULT_SOUND_FREQ==SET_44100)
				"(default)"
			#endif
			/*"(hevy)"*/);	break;
		case 1: pgPrint(x,y,setting.color[3],"  SOUND RENDER: (1/2)"
			#if(SET_DEFAULT_SOUND_FREQ==SET_22050)
				"(default)"
			#endif
			);	break;
		case 2: pgPrint(x,y,setting.color[3],"  SOUND RENDER: (1/4)"
			#if(SET_DEFAULT_SOUND_FREQ==SET_11025)
				"(default)"
			#endif
			);	break;
	//	default:
		case 3: pgPrint(x,y,setting.color[3],"  SOUND RENDER: (1/8)"
			#if(SET_DEFAULT_SOUND_FREQ==SET_05512)
				"(default)"
			#endif
			"(buggy\?\?\?)"); break;
		}y++;
		strcpy(tmp,"  SCREEN MODE: ");
		strcat(tmp,scr_names[setting.screensize]);
		pgPrint(x,y,setting.color[3],tmp);
		y++;
#if 00
		strcpy(tmp,"  FRAME SKIP:  ");
		if (0 ==setting.frameskip)
		{	strcat(&tmp[strlen(tmp)-1], "Auto");}
		else
		{	tmp[strlen(tmp)-1] = setting.frameskip+'0' -1;}
		pgPrint(x,y,setting.color[3],tmp);
#else
		strcpy(tmp,"  FRAME SKIP:  ");
		pgPrint(x,y,setting.color[3],tmp);
			{
			//	ret = psp_frameskip(setting.frameskip);
			//	if(ret>=0)	setting.frameskip = ret;
				if(0==setting.frameskip){
					tmp[0]='A';
					tmp[1]='u';
					tmp[2]='t';
					tmp[3]='o';
					tmp[4]=0;
				}else{
					tmp[2]=' ';
					tmp[3]=' ';
					tmp[4]=0;
					decputn(&tmp[0],setting.frameskip,2);
				}
				pgPrint(x+16,y,setting.color[3],tmp);
			}
#endif
y++;

		if(setting.vsync)
		{	pgPrint(x,y,setting.color[3],"  VSYNC: ON");}
		else
		{	pgPrint(x,y,setting.color[3],"  VSYNC: OFF");}
		y++;
		strcpy(tmp,"  CPU CLOCK: ");
		strcat(tmp,cpu_clocks[setting.cpu_clock]);
		pgPrint(x,y++,setting.color[3],tmp);

		pgPrint(x,y++,setting.color[3],"  COLOR CONFIG");
		y++;
		pgPrint(x,y++,setting.color[3],"  Continue");

		if(sel_count < 15){
			y=MENU_Y_OFFS;
			y+=sel;
			if(sel >= CONTINUE) y++;
			pgPutChar((x+1)*8,y*(8+2),setting.color[3],0,/*127*/ '>',0);
		}

		pgScreenFlipV();
	}

//	pgFillvram(0);
//	pgScreenFlipV();
//	pgFillvram(0);
//	pgScreenFlipV();
	pgWaitVn(10);
}
