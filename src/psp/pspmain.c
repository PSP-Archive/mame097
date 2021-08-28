#include <time.h>
#include "driver.h"

//#include "pg.h"
#include "syscall.h"
#include "menu.h"



extern SETTING setting;

/*static*/ int psp_exit/* =0*/;

int _main (int argc, char **argv);

extern void Get_DriverList(void);
extern void Draw_All(void);
extern int Confirm_Control(void);
extern int Control(void);
extern int	dlist_curpos;
extern void Draw_Confirm(void);
//============================================================
//	HOMEキー関連
//============================================================

/*--------------------------------------------------------
	Exit Callback
--------------------------------------------------------*/

static /*SceKernelCallbackFunction*/int ExitCallback(int arg1, int arg2, void *arg)
{
	psp_exit =1;
//	malloc_psp_term();
//	malloc_term();
	pgaTerm();

	scePowerSetClockFrequency(222,222,111);
	save_config();

	sceKernelExitGame();	/* ここで強制終了 orz (本当はダメ) */
	return 0;
}

/*--------------------------------------------------------
	Power Callback
--------------------------------------------------------*/

static /*SceKernelCallbackFunction*/int PowerCallback(int unknown, int pwrflags, void *arg)
{
	int cbid;
	if(pwrflags & POWER_CB_POWER){
		scePowerSetClockFrequency(222,222,111);
		save_config();
	}
	cbid = sceKernelCreateCallback("Power Callback", (void *)PowerCallback, NULL);
	scePowerRegisterCallback(0, cbid);
	return 0;
}

/*--------------------------------------------------------
	コールバックスレッド作成
--------------------------------------------------------*/

static int CallbackThread(/*SceSize*/int args, void *argp)
{
	int cbid;

	cbid = sceKernelCreateCallback("Exit Callback", (void *)ExitCallback, NULL);
	sceKernelRegisterExitCallback(cbid);

	cbid = sceKernelCreateCallback("Power Callback", (void *)PowerCallback, NULL);
	scePowerRegisterCallback(0, cbid);

//	sceKernelPollCallbacks();
	sceKernelSleepThreadCB();
	return 0;
}

/*--------------------------------------------------------
	コールバックスレッド設定
--------------------------------------------------------*/

/* Sets up the callback thread and returns its thread id */
int SetupCallbacks(void)
{
	/*SceUID*/int thread_id = 0;
	thread_id = sceKernelCreateThread("Update Thread", CallbackThread, 0x12, 0xFA0, 0, NULL);
//	thread_id = sceKernelCreateThread("update_thread", (pg_threadfunc_t)CallbackThread, 0x11, 0xFA0, 0, 0);
	if(thread_id >= 0)
	{
		sceKernelStartThread(thread_id, 0, 0);
	}

	return thread_id;
}

//============================================================
//	GUI
//============================================================






//============================================================
//	メイン
//============================================================
int xmain(int argc, char *argv)
{
//	int game_index;
	int res = 0;
	char *_argv[5];
	int _argc =0;

	psp_exit =0;

	// Initialize
//	pgaInit();
	SetupCallbacks();

	pgcInit();
	pgScreenFrame(2,0);

	sceCtrlSetSamplingMode(1);//sceCtrlSetAnalogMode(1);
	sceCtrlSetSamplingCycle(0);//sceCtrlInit(0);

	//
	Get_DriverList();


	char sPath[40], *pPtr;
	strcpy(sPath, argv);
	pPtr =strrchr(sPath, '/');
	*pPtr = 0;
	chdir(sPath);

	_argv[_argc++] =argv;
	//_argv[_argc++] ="-log";

	load_config();
	load_menu_bg();
	bgbright_change();

	while(/*1*/0==psp_exit) {
		Draw_All();
		switch(Control()) {
		case 2:
			Draw_Confirm();
			if (Confirm_Control()!=0) {
				pgFillvram(0);
				pgScreenFlipV();
				pgFillvram(0);
				pgScreenFlipV();

				_argv[_argc] =drivers[dlist_curpos]->name;

				switch(setting.cpu_clock){
				case 0:		scePowerSetClockFrequency(222,222,111);	break;
				case 1:		scePowerSetClockFrequency(266,266,133);	break;
				default:
				case 2:		scePowerSetClockFrequency(333,333,166);	break;
				}
				psp_exit =0;

				res = _main(_argc+1, _argv);
				//res = run_game (game_index);

				scePowerSetClockFrequency(222,222,111);
				malloc_psp_term();
//				malloc_term();
//				_sbrk_term();
			}
			break;
		case 3:
			psp_menu();
			break;
		}
	}
	// 一応終了
	pgaTerm();
	scePowerSetClockFrequency(222,222,111);
	save_config();
	sceKernelExitGame();
	return 0;
}
#if 00
void exit(void)
{
	pgaTerm();

	pgScreenFlip();
	Confirm_Control();

	sceKernelExitGame();
//	_exit(i);

}

void raise(int i)
{
//	sceKernelExitGame();
	exit();
}
#endif

int printf2_( const char *format, /*const*/ char *format2)
{
int rtn;

	rtn =printf2nw_( format, format2);

	Confirm_Control();
	return(rtn);
}

int printf2nw_( const char *format, /*const*/ char *format2)
{
char buff[1024];

	strcpy(buff, format);
	strcat(buff, ":");
	strcat(buff, format2);

	return(printfnw_(buff));
}

int printf_( /*const*/ char *format)
{
int rtn;

	rtn =printfnw_(format);

	Confirm_Control();
	return(rtn);
}

int printfnw_(/*const*/ char *format)
{
int rtn;

	rtn =printflow(format, strlen(format), 0);
	fflush_();
	return rtn;
}

static int printbuflen =0;
int printflow(/*const*/ char *format, int len, int flush)
{
static int sx =0, sy =0;
int i,j,k;
char buf[128];

	if (!printbuflen) {
		pgFillvram(0);
		sx =sy =0;
	}

	for(i=sy, k=0; i <27 && k <strlen(format) && k <len; i++)
	{
		for(j=0;(sx+j) <96 && format[k] !='\n' && k <strlen(format) && k <len; j++, k++)
			buf[j] =format[k];
		buf[j] =0x00;
		mh_print(sx *5, i*10, buf, RGB(255,255,255));
		sx +=j;
		if (95 <=sx) {
			sx =0;
			sy ++;
		}
		if ('\n' ==format[k]) {
			sx =0;
			sy ++;
			k ++;
		}

		printbuflen +=j;
	}

	if (flush) {
//		fflush_();
		printbuflen =0;
		pgScreenFlipV();
		Confirm_Control();
	}
	return(k);
}

static int fl_mode =1;
int fflushmode_(int fast)
{
	fl_mode =fast;
}

int printflen_(/*const*/ char *format, int len)
{
	return(printflow(format, len, fl_mode));
}

int fflush_(void)
{
	if (printbuflen) {
		pgScreenFlipV();
		if (0 ==fl_mode)
			Confirm_Control();
	}
	printbuflen =0;

	return(0);
}

//int puts( const char *string )
//{
//	mh_print(0, 0, (unsigned char *)string, RGB(255,255,255),0,0);
//
//	return(1);
//}
//
//int *__errno() {
//	return 0;
//}
