#ifndef __WIN_MAIN_H__
#define __WIN_MAIN_H__

#include "SpeedGear.h"
#include "resource.h"


#define MainWndClassName							TEXT("App.SpeedGear.MainWnd.ClassName")
#define MainWndMenuName								TEXT("App.SpeedGear.MainWnd.MenuName")
#define MainWndAppName								TEXT("App.SpeedGear.MainWnd.AppName")
#define MainWndWindowName							TEXT(" SpeedGear - Zachary")

#define AboutWndClassName							TEXT("App.SpeedGear.AboutWnd.ClassName")
#define AboutWndMenuName							TEXT("App.SpeedGear.AboutWnd.MenuName")
#define AboutWndAppName								TEXT("App.SpeedGear.AboutWnd.AppName")
#define AboutWndWindowName							TEXT(" SpeedGear - Zachary")


#define SPEEDGEAR_MUTEX								TEXT("Global\\SPEEDGEAR_MNSAFE_MUTEX")


#define MAINWND_WINDOW_WIDTH						760
#define MAINWND_WINDOW_HEIGHT						500


#define WND_TRANSPARENCY							230


/* 创建主窗口 */
int CreateMainWindow(HINSTANCE hInstance);


/* 主窗口过程处理函数 */
LRESULT CALLBACK MainWndMsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);



#endif	// end of __WIN_MAIN_H__
