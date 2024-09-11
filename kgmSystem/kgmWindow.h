#pragma once
#include "../kgmBase/kgmEvent.h"
#include "../kgmBase/kgmIGC.h"
#include "../kgmBase/kgmString.h"
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

#elif defined(ANDROID)

#include <android/keycodes.h>
u16 keyTranslate(int key);

#elif defined DARWIN

#elif defined WAYLAND

#include "inc/wayland/wayland-client.h"

#else

#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
//#include "inc/X11/Xlib.h"
#include <X11/extensions/Xrender.h>
#include "inc/X11/XKBlib.h"
#include "inc/X11/keysym.h"
#include "inc/X11/extensions/xf86vmode.h"

//#include <X11/XKBlib.h>
//#include <X11/keysym.h>
//#include <X11/extensions/xf86vmode.h>
//#include <GL/glx.h>

u16 keyTranslate(KeySym);

#endif

enum
{
  KGMWINDOWOPENGL     = 0X00001111,
  KGMWINDOWDIRECT3D8  = 0X00002222,
  KGMWINDOWDIRECT3D9  = 0X00003333,
  KGMWINDOWDIRECT3D10 = 0X00004444,
  KGMWINDOWDIRECT3D11 = 0X00005555,
};


class kgmWindow: public kgmEvent
{
  KGM_OBJECT(kgmWindow);

public:
#ifdef WIN32

  //microsoft windows 32 window
  HWND m_wnd;		//window descriptor

#elif defined(ANDROID)

  void* m_hanle;
  int   m_wRect[4];

#elif defined(DARWIN)
  void* m_handle;
  int   m_wRect[4];
#elif defined(WAYLAND)
  struct wl_compositor* m_compositor = nullptr;
  struct wl_display*    m_display = nullptr;
  struct wl_surface*    m_surface = nullptr;
  struct wl_pointer*    m_pointer = nullptr;
  struct wl_seat*       m_seat = nullptr;
  struct wl_shell*      m_shell = nullptr;
  struct wl_shm*        m_shm = nullptr;

  struct xdg_surface*   m_xdg_surface;
  struct xdg_toplevel*  m_xdg_toplevel;
#else

  Display             *m_dpy;
  Window               m_wnd;
  XVisualInfo*         m_visual;
  XF86VidModeModeInfo  m_mode;
  XSetWindowAttributes m_xswa;
  Atom                 m_wmDelete;
  s32                  m_screen;

#endif

  kgmWindow* m_parent;
  kgmIGC*    m_gc;

  bool m_msAbs;
  bool m_msf;

private:
  bool m_loop;

public:
  kgmWindow();
  kgmWindow(kgmWindow*, kgmString, int, int, int, int, int);
  ~kgmWindow();

  static kgmWindow* createGLWindow(kgmString, int, int);
  static kgmWindow* createVKWindow(kgmString, int, int);

  void   loop();
  void   close();
  void   show(bool);
  void   fullscreen(bool);
  void   setRect(int x, int y, int w, int h);
  void   getRect(int& x, int& y, int& w, int& h);
  void   setMouse(int x, int y);
  void   getMouse(int& x, int& y);
  void   setTitle(char*);

  bool   getMsAbsolute(){ return m_msAbs; }
  void   setMsAbsolute(bool s){ m_msAbs = s; }

  virtual kgmIGC* getGC();

#ifdef WIN32
  static long CALLBACK WndProc(HWND, u32, WPARAM, LPARAM);
#elif defined(ANDROID)
#elif defined(DARWIN)
#elif defined(WAYLAND)
  static void onRemoveRegistry(void* a, struct wl_registry* b, uint32_t c);
  static void onRegistry(void* data,
                        struct wl_registry* registry,
                        uint32_t name,
                        const char* interface,
                        uint32_t version);
  static void onPointerEnter(void* data,
                             struct wl_pointer* wl_pointer,
                             uint32_t serial,
                             struct wl_surface* surface,
                             wl_fixed_t surface_x,
                             wl_fixed_t surface_y);
  static void onPointerLeave(void* data,
                             struct wl_pointer* wl_pointer,
                             uint32_t serial,
                             struct wl_surface* wl_surface);

  static void onPointerMotion(void* data,
                              struct wl_pointer* wl_pointer,
                              uint32_t time,
                              wl_fixed_t surface_x,
                              wl_fixed_t surface_y);

  static void onPointerButton(void* data,
                              struct wl_pointer* wl_pointer,
                              uint32_t serial,
                              uint32_t time,
                              uint32_t button,
                              uint32_t state);

  static void onPointerAxis(void* data,
                            struct wl_pointer* wl_pointer,
                            uint32_t time,
                            uint32_t axis,
                            wl_fixed_t value);

#else
  static int WndProc(kgmWindow*, XEvent*);
#endif

  //void onEvent(kgmEvent::Event* e);

  void setHandle(void*);

  private:

};
