#include <pspkernel.h>
#include <pspthreadman.h>
#include <psppower.h>
#include <pspctrl.h>
#include <time.h>
#include "driver.h"

//#include "pg.h"
//#include "syscall.h"
#include "menu.h"


#define MAXPATH   0x108
extern SETTING setting;

/*static*/ int psp_exit/* =0*/;

int run_mame (int argc, char **argv);

extern void Get_DriverList(void);
extern void Draw_All(void);
extern int Confirm_Control(void);
extern int Control(void);
extern int	dlist_curpos;
extern void Draw_Confirm(void);

/* Define the module info section */
PSP_MODULE_INFO("PSPMAME", 0, 1, 10);

/* Define the main thread's attribute value (optional) */
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);

PSP_HEAP_SIZE_KB(-512); //-1024

//============================================================
//	HOMEキー関連
//============================================================

/*--------------------------------------------------------
	Exit Callback
--------------------------------------------------------*/

int exit_callback(int arg1, int arg2, void *common)
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

int power_callback(int unknown, int pwrflags, void *common)
{
	if(pwrflags & PSP_POWER_CB_POWER_SWITCH){
		scePowerSetClockFrequency(222,222,111);
		save_config();
	}

	int cbid = sceKernelCreateCallback("Power Callback", power_callback, 0);
	scePowerRegisterCallback(0, cbid);
}

/*--------------------------------------------------------
	コールバックスレッド作成
--------------------------------------------------------*/

int CallbackThread(int args, void *argp)
{
	int cbid;

	cbid = sceKernelCreateCallback("Exit Callback", exit_callback, 0);
	sceKernelRegisterExitCallback(cbid);
	cbid = sceKernelCreateCallback("Power Callback", power_callback, 0);
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
	int thid = 0;

	thid = sceKernelCreateThread("update_thread", CallbackThread, 0x12, 0xFA0, 0, 0);
	if(thid >= 0)
	{
		sceKernelStartThread(thid, 0, 0);
	}

	return thid;
}

static char curdir[MAXPATH];

char *getCrDir(void) {
	return curdir;
}

int chdir(const char *path)
{
	if (strchr(path, ':')) {
		strcpy(curdir, path);
	} else {
		return -1; //FIXME
	}

	int curdir_length = strlen(curdir);
	curdir[curdir_length++] = '/';
	curdir[curdir_length] = 0;

	return 0;
}

//============================================================
//	GUI
//============================================================






//============================================================
//	メイン
//============================================================
int main(int argc, char *argv[])
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

	sceCtrlSetSamplingCycle(0);//sceCtrlInit(0);
sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);
	//
	Get_DriverList();


	char sPath[40], *pPtr;
	strcpy(sPath, argv[0]);
	pPtr =strrchr(sPath, '/');
	*pPtr = 0;
	chdir(sPath);

	_argv[_argc++] =argv;
    sceIoChdir(getCrDir());
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

				res = run_mame(_argc+1, _argv);
				//res = run_game (game_index);

				scePowerSetClockFrequency(222,222,111);
				//malloc_psp_term();
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
