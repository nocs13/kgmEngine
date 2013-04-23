///////////////
// kgmWindow
#include "kgmWindow.h"
#include "kgmApp.h"
#include "../kgmBase/kgmLog.h"



#ifdef WIN32
inline void msCoord(bool abs, kgmEvent::Event& e, LPARAM lPar, int& x, int &y)
{
 int mx = LOWORD(lPar);
 int my = HIWORD(lPar);
 if(abs){
  x = e.msx = mx;
  y = e.msy = my;
 }
 else{
  e.msx = mx - x;
  e.msy = my - y;
  x = mx;
  y = my;
 }
}

inline u32 WPARAM_KEY(WPARAM wPar){
 u32 key = 0;
 if(LOWORD(wPar) & MK_CONTROL) key |= KB_KEYCTRL;
 if(LOWORD(wPar) & MK_SHIFT)   key |= KB_KEYSHFT;
 if(LOWORD(wPar) & MK_ALT)     key |= KB_KEYALT;
 if(LOWORD(wPar) & MK_LBUTTON) key |= KB_MSBLEFT;
 if(LOWORD(wPar) & MK_RBUTTON) key |= KB_MSBRIGHT;
 return key;
}

inline u16 keyTranslate(int key){
 switch(key){
 case VK_ESCAPE:
	 return KEY_ESCAPE;
 case VK_SPACE:
	 return KEY_SPACE;
 case VK_RETURN:
	 return KEY_ENTER;
 case VK_UP:
	 return KEY_UP;
 case VK_LEFT:
	 return KEY_LEFT;
 case VK_RIGHT:
	 return KEY_RIGHT;
 case VK_DOWN:
	 return KEY_DOWN;
 case VK_SHIFT:
	 return KEY_LSHIFT;
 case 0x13:
	 return KEY_PAUSE;
 case 0x30:
	 return KEY_0;
 case 0x31:
	 return KEY_1;
 case 0x32:
	 return KEY_2;
 case 0x33:
	 return KEY_3;
 case 0x34:
	 return KEY_4;
 case 0x35:
	 return KEY_5;
 case 0x36:
	 return KEY_6;
 case 0x37:
	 return KEY_7;
 case 0x38:
	 return KEY_8;
 case 0x39:
	 return KEY_9;
 case 0x41:
	 return KEY_A;
 case 0x42:
	 return KEY_B;
 case 0x43:
	 return KEY_C;
 case 0x44:
	 return KEY_D;
 case 0x45:
	 return KEY_E;
 case 0x46:
	 return KEY_F;
 case 0x47:
	 return KEY_G;
 case 0x48:
	 return KEY_H;
 case 0x49:
	 return KEY_I;
 case 0x4a:
	 return KEY_J;
 case 0x4b:
	 return KEY_K;
 case 0x4c:
	 return KEY_L;
 case 0x4d:
	 return KEY_M;
 case 0x4e:
	 return KEY_N;
 case 0x4f:
	 return KEY_O;
 case 0x50:
	 return KEY_P;
 case 0x51:
	 return KEY_Q;
 case 0x52:
	 return KEY_R;
 case 0x53:
	 return KEY_S;
 case 0x54:
	 return KEY_T;
 case 0x55:
	 return KEY_U;
 case 0x56:
	 return KEY_V;
 case 0x57:
	 return KEY_W;
 case 0x58:
	 return KEY_X;
 case 0x59:
	 return KEY_Y;
 case 0x5a:
	 return KEY_Z;
 }
 return KEY_NONE;
}


long CALLBACK kgmWindow::WndProc(HWND hWnd, u32 msg, WPARAM wPar, LPARAM lPar){
 static bool set_cursor = false;
  kgmWindow* wnd = (kgmWindow*)GetWindowLong(hWnd, GWL_USERDATA);
  kgmEvent::Event evt;
  RECT     wRect;
  RECT     wcRect;
  POINT    msPoint;
  MSG m;
  m.message = msg;
  m.wParam = wPar;
  m.lParam = lPar;

  
  if(!wnd)
	  return 1;

  GetClientRect(hWnd, &wcRect);
  GetWindowRect(hWnd, &wRect);
  GetCursorPos(&msPoint);

/*
  if(msg == WM_MOUSEMOVE){
   evt.m_evt = evtMsMove;
   if(!wnd->m_msAbs){
    int cent_x = (wcRect.right - wcRect.left) / 2;
    int cent_y = (wcRect.bottom - wcRect.top) / 2;
    int ms_x = LOWORD(lPar);
	int ms_y = HIWORD(lPar);
	evt.m_KeyMask = WPARAM_KEY(wPar);
	evt.m_X = ms_x - cent_x;
	evt.m_Y = ms_y - cent_y;
	msPoint.x = cent_x;
	msPoint.y = cent_y;
    ClientToScreen(hWnd, &msPoint);
	if(!set_cursor){
     wnd->onEvent(&evt);
     SetCursorPos(msPoint.x, msPoint.y);
	 set_cursor = true;
	}else
		set_cursor = false;
   }else{
	evt.m_KeyMask = WPARAM_KEY(wPar);
	evt.m_X = msPoint.x;
	evt.m_Y = msPoint.y;
    wnd->onEvent(&evt);
   }
  }
*/
  switch(msg){
  case WM_DESTROY:
    //wnd->close();
    evt.event = evtClose;
    break;
  case WM_SIZE:
    evt.event = evtResize;
    evt.width = LOWORD(lPar);
    evt.height = HIWORD(wPar);
  //wnd->onSize(LOWORD(lPar), HIWORD(wPar));
  break;
  case WM_PAINT:
  	  evt.event = evtPaint;
	  evt.gc = 0;
	  ValidateRect(hWnd, NULL);
	  //wnd->onPaint();
      break;
  case WM_MOUSEWHEEL:
  	  evt.event = evtMsWheel;
	  msPoint.x = LOWORD(lPar);
	  msPoint.y = HIWORD(lPar);
	  ClientToScreen(hWnd, &msPoint);
	  evt.msx = msPoint.x;
	  evt.msy = msPoint.y;
	  evt.msz = HIWORD(wPar);
	  //wnd->onMsWheel(WPARAM_KEY(wPar), LOWORD(lPar), HIWORD(lPar), HIWORD(wPar)); 
	  break;
  case WM_MOUSEMOVE:
  	  evt.event = evtMsMove;
	  msPoint.x = LOWORD(lPar);
	  msPoint.y = HIWORD(lPar);
	  ClientToScreen(hWnd, &msPoint);
	  evt.msx = msPoint.x;
	  evt.msy = msPoint.y;
	  //wnd->onMsMove(keyTranslate(wPar), msPoint.x, msPoint.y);
	  break;
  case WM_LBUTTONDOWN:
  	  evt.event = evtMsLeftDown;
	  evt.key = keyTranslate(WPARAM_KEY(wPar));
	  evt.msx = LOWORD(lPar);
	  evt.msy = HIWORD(lPar);
	  //wnd->onMsLeftDown(WPARAM_KEY(wPar), LOWORD(lPar), HIWORD(lPar));
	  break;
  case WM_LBUTTONUP:
  	  evt.event = evtMsLeftUp;
	  evt.key = keyTranslate(WPARAM_KEY(wPar));
	  evt.msx = LOWORD(lPar);
	  evt.msy = HIWORD(lPar);
	  //wnd->onMsLeftUp(WPARAM_KEY(wPar), LOWORD(lPar), HIWORD(lPar));
	  break;
  case WM_RBUTTONDOWN:
  	  evt.event = evtMsRightDown;
	  evt.key = keyTranslate(WPARAM_KEY(wPar));
	  evt.msx = LOWORD(lPar);
	  evt.msy = HIWORD(lPar);
	  //wnd->onMsRightDown(WPARAM_KEY(wPar), LOWORD(lPar), HIWORD(lPar));
	  break;
  case WM_RBUTTONUP:
  	  evt.event = evtMsRightUp;
	  evt.key = keyTranslate(WPARAM_KEY(wPar));
	  evt.msx = LOWORD(lPar);
	  evt.msy = HIWORD(lPar);
	  //wnd->onMsRightUp(WPARAM_KEY(wPar), LOWORD(lPar), HIWORD(lPar));
	  break;
  case WM_KEYDOWN:
  	  evt.event = evtKeyDown;
	  evt.key = keyTranslate(wPar);
	  //wnd->onKeyDown(keyTranslate(wPar));
	  break;
  case WM_KEYUP:
  	  evt.event = evtKeyUp;
	  evt.key = keyTranslate(wPar);
	  //wnd->onKeyUp(keyTranslate(wPar));
	  break;
  default:
	  return DefWindowProc(hWnd, msg, wPar, lPar);
	  break;
  }
  if(evt.event)
	  wnd->onEvent(&evt);
  return 1;
}


void kgmRegisterWindowClass(){
  WNDCLASS *wcl = (WNDCLASS*)malloc(sizeof(WNDCLASS));
  memset(wcl, 0, sizeof(WNDCLASS));
  wcl->style = CS_CLASSDC;
  wcl->lpfnWndProc = (WNDPROC)kgmWindow::WndProc;
  wcl->lpszClassName = cWndClass;
  wcl->hIcon = ::LoadIcon(0, IDI_APPLICATION);
  wcl->hCursor = ::LoadCursor(0, IDC_ARROW);
  wcl->hbrBackground = (HBRUSH)::GetStockObject(WHITE_BRUSH);

  if(!RegisterClass(wcl))
   abort(); 
  free(wcl);
}
void kgmUnregisterWindowClass(){
  WNDCLASS wcl;
  if(GetClassInfo(0, cWndClass, &wcl))
	  UnregisterClass(cWndClass, 0);
}
#endif

#ifdef LINUX
/* attributes for a single buffered visual in RGBA format with at least
 * 4 bits per color and a 16 bit depth buffer */
static int attrSgl[] = {
    GLX_RGBA, 
    GLX_RED_SIZE, 4, 
    GLX_GREEN_SIZE, 4, 
    GLX_BLUE_SIZE, 4, 
    GLX_DEPTH_SIZE, 16,
    None
};

/* attributes for a double buffered visual in RGBA format with at least
 * 4 bits per color and a 16 bit depth buffer */
static int attrDbl[] = { GLX_RGBA, GLX_DOUBLEBUFFER, 
    GLX_RED_SIZE, 4, 
    GLX_GREEN_SIZE, 4, 
    GLX_BLUE_SIZE, 4, 
    GLX_DEPTH_SIZE, 8,
    None 
};

u16 keyTranslate(KeySym key){
 switch(key){
 case XK_Escape:
	 return KEY_ESCAPE;
 case XK_space:
	 return KEY_SPACE;
 case XK_Return:
	 return KEY_ENTER;
 case XK_Up:
	 return KEY_UP;
 case XK_Left:
	 return KEY_LEFT;
 case XK_Right:
	 return KEY_RIGHT;
 case XK_Down:
	 return KEY_DOWN;
 case XK_Shift_L:
	 return KEY_LSHIFT;
 case XK_Shift_R:
	 return KEY_RSHIFT;
 case XK_Control_L:
	 return KEY_LCTRL;
 case XK_Control_R:
	 return KEY_RCTRL;
 case XK_Alt_L:
	 return KEY_LALT;
 case XK_Alt_R:
	 return KEY_RALT;
 case XK_Pause:
	 return KEY_PAUSE;
 case XK_0:
	 return KEY_0;
 case XK_1:
	 return KEY_1;
 case XK_2:
	 return KEY_2;
 case XK_3:
	 return KEY_3;
 case XK_4:
	 return KEY_4;
 case XK_5:
	 return KEY_5;
 case XK_6:
	 return KEY_6;
 case XK_7:
	 return KEY_7;
 case XK_8:
	 return KEY_8;
 case XK_9:
	 return KEY_9;
 case XK_a:
	 return KEY_A;
 case XK_b:
	 return KEY_B;
 case XK_c:
	 return KEY_C;
 case XK_d:
	 return KEY_D;
 case XK_e:
	 return KEY_E;
 case XK_f:
	 return KEY_F;
 case XK_g:
	 return KEY_G;
 case XK_h:
	 return KEY_H;
 case XK_i:
	 return KEY_I;
 case XK_j:
	 return KEY_J;
 case XK_k:
	 return KEY_K;
 case XK_l:
	 return KEY_L;
 case XK_m:
	 return KEY_M;
 case XK_n:
	 return KEY_N;
 case XK_o:
	 return KEY_O;
 case XK_p:
	 return KEY_P;
 case XK_q:
	 return KEY_Q;
 case XK_r:
	 return KEY_R;
 case XK_s:
	 return KEY_S;
 case XK_t:
	 return KEY_T;
 case XK_u:
	 return KEY_U;
 case XK_v:
	 return KEY_V;
 case XK_w:
	 return KEY_W;
 case XK_x:
	 return KEY_X;
 case XK_y:
	 return KEY_Y;
 case XK_z:
	 return KEY_Z;
 }
 return KEY_NONE;
}

int kgmWindow::WndProc(kgmWindow* wnd, XEvent* evt){
 Window wroot, wchild;
 kgmEvent::Event m_evt = {0};
 int rx, ry, cx, cy;
 unsigned int mask;
 KeySym   ksym;
	
 switch(evt->type){
 case Expose:
	m_evt.event = evtPaint;
	break;
 case ConfigureNotify:	
	m_evt.event = evtResize;
    m_evt.x = evt->xconfigure.x;
    m_evt.y = evt->xconfigure.y;
    m_evt.width = evt->xconfigure.width;
    m_evt.height = evt->xconfigure.height;
	break;
 case MotionNotify:
	XQueryPointer(wnd->m_dpy, wnd->m_wnd, &wroot, &wchild, &rx, &ry, &cx, &cy, &mask);
	m_evt.event = evtMsMove;
    m_evt.msx = evt->xbutton.x;
    m_evt.msy = evt->xbutton.y;
	break;
 case MappingNotify:
   XRefreshKeyboardMapping (&evt->xmapping) ;
        break;
 case ButtonPress:
 case ButtonRelease:
    m_evt.msx = evt->xbutton.x;
    m_evt.msy = evt->xbutton.y;
	switch(evt->xbutton.button){
	case Button1:
	    if(evt->type == ButtonPress)
                m_evt.event = evtMsLeftDown;
            else
                m_evt.event = evtMsLeftUp;
		break;
	case Button2:
	    if(evt->type == ButtonPress)
                m_evt.event = evtMsRightDown;
            else
                m_evt.event = evtMsRightUp;
		break;
	case Button3:
	    if(evt->type == ButtonPress)
                m_evt.event = evtMsMiddleDown;
            else
                m_evt.event = evtMsMiddleUp;
		break;	
	case Button4:
            m_evt.event = evtMsWheel;
            m_evt.msz = 1;
		break;	
	case Button5:
            m_evt.event = evtMsWheel;
            m_evt.msz = -1;
		break;	
	}
	break;
 case KeyPress:
        ksym = keyTranslate(XLookupKeysym(&evt->xkey, 0));
        m_evt.event = evtKeyDown;
        m_evt.key = ksym;
	break;
 case KeyRelease:
        ksym = keyTranslate(XLookupKeysym(&evt->xkey, 0));
        m_evt.event = evtKeyUp;
        m_evt.key = ksym;
	break;
 case ClientMessage:
   m_evt.event = evtClose;
        //if(evt->xclient.data.l[0] == wnd->m_wmDelete)
        //  wnd->m_loop = false;
//        if (*XGetAtomName(wnd->m_dpy, (Atom)&evt->xclient.message_type) == *"WM_PROTOCOLS")
//          m_evt.event = evtClose;
        kgm_log() << "got close event" << "\n";
        break;
 default:
	m_evt.event = evtNone;
	break;
 }
 
 if(m_evt.event)
  wnd->onEvent(&m_evt);  
 
 return 1;
}
#endif

#ifdef ANDROID

u16 keyTranslate(int key)
{
  switch(key)
  {
  case AKEYCODE_UNKNOWN:
    return KEY_NONE;
  case AKEYCODE_BACK:
    return KEY_ESCAPE;
  case AKEYCODE_DPAD_UP:
    return KEY_UP;
  case AKEYCODE_DPAD_DOWN:
    return KEY_DOWN;
  case AKEYCODE_DPAD_LEFT:
    return KEY_LEFT;
  case AKEYCODE_DPAD_RIGHT:
    return KEY_RIGHT;
//  case AKEYCODE_DPAD_CENTER:
  case AKEYCODE_0:
    return KEY_0;
  case AKEYCODE_1:
    return KEY_1;
  case AKEYCODE_2:
    return KEY_2;
  case AKEYCODE_3:
    return KEY_3;
  case AKEYCODE_4:
    return KEY_4;
  case AKEYCODE_5:
    return KEY_5;
  case AKEYCODE_6:
    return KEY_6;
  case AKEYCODE_7:
    return KEY_7;
  case AKEYCODE_8:
    return KEY_8;
  case AKEYCODE_9:
    return KEY_9;
  default:
    return -1;
  }

  return -1;
}
#endif

kgmWindow::kgmWindow(){
 m_parent = 0;
 m_msAbs = true;
 m_msf = false;
 m_fs = false;
}

kgmWindow::kgmWindow(kgmWindow* wp, char* wname, int x, int y, int w, int h, int bpp, bool fs){
 m_parent = wp;
 m_msAbs = true;
 m_msf = false;
 m_fs = false;

 kgmLog::log("init screen");

#ifdef WIN32
  WNDCLASS wcl;
  if(!GetClassInfo(0, cWndClass, &wcl))
   kgmRegisterWindowClass();
  m_wnd = CreateWindow(cWndClass, wname,
					    (fs)?(WS_POPUP):(WS_OVERLAPPEDWINDOW),
						x, y, w, h, 
						(wp)?(wp->m_wnd):(0), 0, 0, 0);
  SetWindowLong(m_wnd, GWL_USERDATA, (LONG)this);
  ShowWindow(m_wnd, SW_SHOWNORMAL);
  UpdateWindow(m_wnd);
#endif
  
#ifdef LINUX
 m_dpy = (wp)?(wp->m_dpy):XOpenDisplay(NULL);
 m_screen = (wp)?(wp->m_screen):DefaultScreen(m_dpy);
 m_wnd = XCreateSimpleWindow(m_dpy, (wp)?(wp->m_wnd):RootWindow(m_dpy, 0),
     						 x, y, w, h, 0, BlackPixel(m_dpy, 0), BlackPixel(m_dpy, 0));
 Atom delWindow = XInternAtom( m_dpy, "WM_DELETE_WINDOW", 0 );
 XSetWMProtocols(m_dpy, m_wnd, &delWindow, 1);
 XSelectInput(m_dpy, m_wnd, ExposureMask | KeyPressMask | KeyReleaseMask |  ButtonPressMask |
     		  ButtonReleaseMask | PointerMotionMask|StructureNotifyMask );
 XMapWindow(m_dpy, m_wnd);
 XStoreName(m_dpy, m_wnd, wname);
 XFlush(m_dpy);
#endif

#ifdef ANDROID
 kgm_log() << "Init window rect: " << w << " " << h << ".";
 m_wRect[0] = x;
 m_wRect[1] = y;
 m_wRect[2] = w;
 m_wRect[3] = h;
#endif
}

kgmWindow::~kgmWindow(){
  //close();
}

void kgmWindow::fullscreen(bool fs){
 if(m_fs == fs)
  return;

#ifdef WIN32
 u32 iWidth, iHeight;
 DEVMODE Mode;

 iWidth = GetSystemMetrics(SM_CXSCREEN);
 iHeight = GetSystemMetrics(SM_CYSCREEN);

 EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &Mode);
 Mode.dmPelsWidth = iWidth;
 Mode.dmPelsHeight = iHeight;
 Mode.dmSize = sizeof(Mode);
 Mode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;
 if(fs) 
  ChangeDisplaySettings(&Mode, CDS_FULLSCREEN);
 else
  ChangeDisplaySettings(NULL, 0);
#endif

#ifdef LINUX
 XEvent xev;
 Atom wm_state = XInternAtom(m_dpy, "_NET_WM_STATE", False);
 Atom fullscreen = XInternAtom(m_dpy, "_NET_WM_STATE_FULLSCREEN", False);

 memset(&xev, 0, sizeof(xev));
 xev.type = ClientMessage;
 xev.xclient.window = m_wnd;
 xev.xclient.message_type = wm_state;
 xev.xclient.format = 32;
 xev.xclient.data.l[0] = fs ? 1 : 0;
 xev.xclient.data.l[1] = fullscreen;
 xev.xclient.data.l[2] = 0;

 XSendEvent(m_dpy, DefaultRootWindow(m_dpy), False, SubstructureNotifyMask, &xev);
#endif

 m_fs = fs;
}

void kgmWindow::show(bool sh){
#ifdef WIN32
 if(sh && !IsWindowVisible(m_wnd)){
	 ShowWindow(m_wnd, SW_SHOWNORMAL);
	 return;
 }
 if(!sh && IsWindowVisible(m_wnd)){
	 ShowWindow(m_wnd, SW_HIDE);
	 return;
 }
#endif
#ifdef LINUX
 if(sh)
     XMapWindow(m_dpy, m_wnd);
 else
     XUnmapWindow(m_dpy, m_wnd);
 XFlush(m_dpy);
#endif
}

void kgmWindow::loop(){
 if(!m_loop)
  m_loop = true;

#ifdef WIN32
  MSG msg;
  while(m_wnd && m_loop){
   if(PeekMessage(&msg, 0, 0, 0, PM_REMOVE)){
    TranslateMessage(&msg);
    DispatchMessage(&msg);
   }
   else{
    onIdle();
   }
   Sleep(0);
  }
#endif

#ifdef LINUX
  XEvent evt;
  while(m_loop && m_dpy){
   while(XPending(m_dpy) > 0){
    XNextEvent(m_dpy, &evt);
    WndProc(this, &evt);
    if(!m_loop || !m_dpy)
      break;
   }

   if(m_dpy)
    onIdle();

   usleep(0);
  }
#endif
  
  m_loop = false;
}

void kgmWindow::close()
{ 
//Prepae to close window
//onClose();
 
#ifdef WIN32
 DestroyWindow(m_wnd);
#endif
 
#ifdef LINUX
//XDestroyWindow(m_dpy, m_wnd);
// if(!m_parent)
//  XCloseDisplay(m_dpy);
 //XEvent ev;
 XClientMessageEvent ev;

 memset(&ev, 0, sizeof (ev));

 /*ev.xclient.type = ClientMessage;
 ev.xclient.window = m_wnd;
 ev.xclient.message_type = XInternAtom(m_dpy, "WM_PROTOCOLS", false);
 ev.xclient.format = 32;
 ev.xclient.data.l[0] = XInternAtom(m_dpy, "WM_DELETE_WINDOW", false);
 ev.xclient.data.l[1] = CurrentTime;
 XSendEvent(m_dpy, m_wnd, False, NoEventMask, &ev);*/

 ev.type = ClientMessage;
 ev.window = m_wnd;
 ev.format = 32;
 XSendEvent(m_dpy, m_wnd, 0, 0, (XEvent*)&ev);

#endif
}

void kgmWindow::getRect(int& x, int& y, int& w, int& h){
#ifdef WIN32
 RECT r;		
 GetClientRect(m_wnd, (LPRECT)&r);
 x = r.left, y = r.top, w = r.right - x, h = r.bottom - y;
#endif
#ifdef LINUX
 unsigned int  width, height, border, depth;
 Window dummy;
 XGetGeometry(m_dpy, m_wnd, &dummy, &x, &y, (u32*)&w, (u32*)&h, &border, &depth);
#endif
#ifdef ANDROID
 x = m_wRect[0];
 y = m_wRect[1];
 w = m_wRect[2];
 h = m_wRect[3];
#endif
}

void kgmWindow::setRect(int x, int y, int w, int h){
#ifdef WIN32
 RECT r;		
// GetClientRect(m_hWnd, (LPRECT)&r);
#endif

#ifdef LINUX
 XMoveResizeWindow(m_dpy, m_wnd, x, y, w, h);
 XMapWindow(m_dpy, m_wnd);
 XFlush(m_dpy);
#endif

#ifdef ANDROID
 m_wRect[0] = x;
 m_wRect[1] = y;
 m_wRect[2] = w;
 m_wRect[3] = h;
 onResize(w, h);
#endif
}

void kgmWindow::setMouse(int x, int y)
{
 m_msf = true;
#ifdef WIN32
 POINT pt = {x, y};
 ScreenToClient(m_wnd, &pt);
 SetCursorPos(pt.x, pt.y);
#endif
#ifdef LINUX
#endif
}

void kgmWindow::getMouse(int& x, int& y)
{
 m_msf = true;
#ifdef WIN32
 POINT pt = {x, y};
// ScreenToClient(m_hWnd, &pt);
// SetCursorPos(pt.x, pt.y);
#endif

#ifdef LINUX
#endif
}

void kgmWindow::setTitle(char* title)
{
#ifdef WIN32
 SetWindowText(m_wnd, title);
#endif

#ifdef LINUX
 XStoreName(m_dpy, m_wnd, title);
#endif
}

void kgmWindow::onClose()
{
  kgm_log() << "onClose" << "\n";

#ifdef WIN32
#endif

#ifdef LINUX
    XDestroyWindow(m_dpy, m_wnd);
    if(!m_parent)
    {
      XCloseDisplay(m_dpy);
      m_dpy = null;
    }
#endif

    m_loop = 0;
}

kgmIGraphics* kgmWindow::getGC()
{
 return 0;
}




