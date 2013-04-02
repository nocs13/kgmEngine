#pragma once
#include "../kgmBase/kgmEvent.h"
#include "../kgmBase/kgmIWindow.h"
#include "../kgmMath/kgmMath.h"


class kgmWindow;
#ifdef WIN32
#include <windows.h>

#define cWndClass "cWndClass"
//#define WM_MOUSEWHEEL   0x020A

inline u16 keyTranslate(int);

void kgmRegisterWindowClass();
void kgmUnregisterWindowClass();

#endif

#ifdef LINUX
 #include <unistd.h>
 #include <X11/Xlib.h>
 #include <X11/Xutil.h>
 #include <X11/keysym.h>
 #include <X11/extensions/xf86vmode.h>
 #include <GL/gl.h>
// #include <GL/glu.h>
 #include <GL/glx.h>

 u16 keyTranslate(KeySym);
#endif

enum{
 KGMWINDOWOPENGL     = 0X00001111,
 KGMWINDOWDIRECT3D8  = 0X00002222,
 KGMWINDOWDIRECT3D9  = 0X00003333,
 KGMWINDOWDIRECT3D10 = 0X00004444,
 KGMWINDOWDIRECT3D11 = 0X00005555,
};


class kgmWindow: public kgmEvent{
public:
#ifdef WIN32
 //microsoft windows 32 window
 HWND m_wnd;		//window descriptor
#endif

#ifdef LINUX
 Display             *m_dpy;
 Window               m_wnd;
 XF86VidModeModeInfo  m_mode;
 XSetWindowAttributes m_xswa;
 Atom                 m_wmDelete;
 int                  m_screen;
#endif

#ifdef ANDROID
 void* m_wnd;
#endif

 kgmWindow*	m_parent;		 
// int  m_x, m_y, m_w, m_h;
// int  m_msx, m_msy;

 bool m_fs;
 bool m_msAbs;
 bool m_msf;
 bool m_loop;
public:
 kgmWindow();
 kgmWindow(kgmWindow*, char*, int, int, int, int, int, bool);
 virtual ~kgmWindow();
 // functions
 void	loop();	
 void	close();	
 void	show(bool);
 void   fullscreen(bool);	
 void   setRect(int x, int y, int w, int h);
 void   getRect(int& x, int& y, int& w, int& h);
 void   setMouse(int x, int y);
 void   getMouse(int& x, int& y);
 void   setTitle(char*);


 virtual kgmIGraphics* getGC();

#ifdef WIN32
 static long CALLBACK WndProc(HWND, u32, WPARAM, LPARAM);
#endif
#ifdef LINUX
 static int WndProc(kgmWindow*, XEvent*);
#endif

 // Virtual Functions
 virtual void onIdle(){
 }
 virtual void onMenu(int id){
 }
 virtual void onEvent(kgmEvent::Event* e){
  if(!e)
   return;
  kgmEvent::onEvent(e);
 }
};
