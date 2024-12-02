///////////////
// kgmWindow
#include "kgmWindow.h"
#include "kgmApp.h"
#include "kgmSystem.h"
#include "kgmOGL.h"
#include "../kgmBase/kgmLog.h"

kgmIGC* kgmCreateGLESContext(kgmWindow* w);
kgmIGC* kgmCreateOGLContext(kgmWindow* w);
kgmIGC* kgmCreateVKContext(kgmWindow* w);

#ifdef WIN32
typedef unsigned short GLushort;
typedef unsigned int   GLuint;

#include <GL/gl.h>
#include <GL/wgl.h>
#include <GL/wglext.h>
#endif

#ifdef WIN32

inline void msCoord(bool abs, kgmEvent::Event& e, LPARAM lPar, int& x, int &y)
{
  int mx = LOWORD(lPar);
  int my = HIWORD(lPar);

  if(abs)
  {
    x = e.msx = mx;
    y = e.msy = my;
  }
  else
  {
    e.msx = mx - x;
    e.msy = my - y;
    x = mx;
    y = my;
  }
}

inline u32 WPARAM_KEY(WPARAM wPar)
{
  u32 key = 0;

  if(LOWORD(wPar) & MK_CONTROL) key |= KB_KEYCTRL;
  if(LOWORD(wPar) & MK_SHIFT)   key |= KB_KEYSHFT;
  if(LOWORD(wPar) & MK_ALT)     key |= KB_KEYALT;
  if(LOWORD(wPar) & MK_LBUTTON) key |= KB_MSBLEFT;
  if(LOWORD(wPar) & MK_RBUTTON) key |= KB_MSBRIGHT;

  return key;
}

inline u16 keyTranslate(int key)
{
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
  case VK_DELETE:
    return KEY_DELETE;
  case VK_CONTROL:
    return KEY_LCTRL;
  case 0x13:
    return KEY_PAUSE;
  case VK_F1:
    return KEY_F1;
    break;
  case VK_F2:
    return KEY_F2;
    break;
  case VK_F3:
    return KEY_F3;
    break;
  case VK_F4:
    return KEY_F4;
    break;
  case VK_F5:
    return KEY_F5;
    break;
  case VK_F6:
    return KEY_F6;
    break;
  case VK_F7:
    return KEY_F7;
    break;
  case VK_F8:
    return KEY_F8;
    break;
  case VK_F9:
    return KEY_F9;
    break;
  case VK_F10:
    return KEY_F10;
    break;
  case VK_F11:
    return KEY_F11;
    break;
  case VK_F12:
    return KEY_F12;
    break;
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


long CALLBACK kgmWindow::WndProc(HWND hWnd, u32 msg, WPARAM wPar, LPARAM lPar)
{
  static bool set_cursor = false;
  kgmWindow* wnd = (kgmWindow*) GetWindowLongPtr(hWnd, GWLP_USERDATA);
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

  switch(msg){
  case WM_CLOSE:
    evt.event = evtClose;
    break;
  case WM_SIZE:
    evt.event = evtResize;
    evt.width = LOWORD(lPar);
    evt.height = HIWORD(lPar);
    break;
  case WM_PAINT:
    evt.event = evtPaint;
    evt.gc = 0;
    ValidateRect(hWnd, NULL);
    break;
  case WM_MOUSEWHEEL:
    evt.event = evtMsWheel;
    msPoint.x = LOWORD(lPar);
    msPoint.y = HIWORD(lPar);
    ClientToScreen(hWnd, &msPoint);
    evt.msx = msPoint.x;
    evt.msy = msPoint.y;
    evt.msz = HIWORD(wPar);
    break;
  case WM_MOUSEMOVE:
    evt.event = evtMsMove;
    msPoint.x = LOWORD(lPar);
    msPoint.y = HIWORD(lPar);

    switch(wPar)
    {
    case MK_LBUTTON:
      evt.keyMask = KEY_MSBLEFT;
      break;
    case MK_RBUTTON:
      evt.keyMask = KEY_MSBRIGHT;
      break;
    case MK_MBUTTON:
      evt.keyMask = KEY_MSBMIDDLE;
      break;
    default:
      evt.key = KEY_NONE;
    }
    //ClientToScreen(hWnd, &msPoint);

    if(!wnd->m_msAbs)
    {
      RECT rc;
      GetClientRect(wnd->m_wnd, (LPRECT)&rc);

      RECT wrc;
      GetWindowRect(wnd->m_wnd, (LPRECT)&wrc);

      evt.msx = rc.right / 2 - msPoint.x;
      evt.msy = rc.bottom / 2 - msPoint.y;

      static bool seted = false;

      if(seted)
      {
        seted = false;
      }
      else
      {
        POINT pt = {rc.right / 2, rc.bottom / 2};
        ClientToScreen(wnd->m_wnd, &pt);
        SetCursorPos(pt.x, pt.y);
        seted = true;
      }
    }
    else
    {
      evt.msx = msPoint.x;
      evt.msy = msPoint.y;
    }
    break;
  case WM_LBUTTONDOWN:
    evt.event = evtMsLeftDown;
    evt.key = keyTranslate(WPARAM_KEY(wPar));
    evt.msx = LOWORD(lPar);
    evt.msy = HIWORD(lPar);
    break;
  case WM_LBUTTONUP:
    evt.event = evtMsLeftUp;
    evt.key = keyTranslate(WPARAM_KEY(wPar));
    evt.msx = LOWORD(lPar);
    evt.msy = HIWORD(lPar);
    break;
  case WM_RBUTTONDOWN:
    evt.event = evtMsRightDown;
    evt.key = keyTranslate(WPARAM_KEY(wPar));
    evt.msx = LOWORD(lPar);
    evt.msy = HIWORD(lPar);
    break;
  case WM_RBUTTONUP:
    evt.event = evtMsRightUp;
    evt.key = keyTranslate(WPARAM_KEY(wPar));
    evt.msx = LOWORD(lPar);
    evt.msy = HIWORD(lPar);
    break;
  case WM_KEYDOWN:
    evt.event = evtKeyDown;
    evt.key = keyTranslate(wPar);
    break;
  case WM_KEYUP:
    evt.event = evtKeyUp;
    evt.key = keyTranslate(wPar);
    break;
  default:
    return DefWindowProc(hWnd, msg, wPar, lPar);
    break;
  }

  if(evt.event)
    wnd->onEvent(&evt);

  if ((evt.event == evtClose) && (hWnd == wnd->m_wnd))
  {
    DestroyWindow(hWnd);

    wnd->m_wnd = null;
  }

  return 1;
}

void kgmRegisterWindowClass(){
  WNDCLASS *wcl = (WNDCLASS*)kgm_alloc(sizeof(WNDCLASS));
  memset(wcl, 0, sizeof(WNDCLASS));
  wcl->style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;//CS_CLASSDC;
  wcl->lpfnWndProc = (WNDPROC)kgmWindow::WndProc;
  wcl->lpszClassName = cWndClass;
  wcl->hIcon = ::LoadIcon(0, IDI_APPLICATION);
  wcl->hCursor = ::LoadCursor(0, IDC_ARROW);
  wcl->hbrBackground = (HBRUSH)::GetStockObject(WHITE_BRUSH);

  if(!RegisterClass(wcl))
    abort();
  kgm_free(wcl);
}
void kgmUnregisterWindowClass(){
  WNDCLASS wcl;
  if(GetClassInfo(0, cWndClass, &wcl))
    UnregisterClass(cWndClass, 0);
}
#elif defined(WAYLAND)

  void kgmWindow::onRemoveRegistry(void* a, struct wl_registry* b, uint32_t c)
  {

  }

  void kgmWindow::onRegistry(void* data, struct wl_registry* registry, uint32_t name,
                             const char* interface, uint32_t version)
  {
    kgmWindow* me = static_cast<kgmWindow*>(data);

    if (!strcmp(interface, wl_compositor_interface.name)) {
      me->m_compositor = static_cast<wl_compositor*>(
          wl_registry_bind(registry, name, &wl_compositor_interface, version));
    } else if (!strcmp(interface, wl_shm_interface.name)) {
      me->m_shm = static_cast<wl_shm*>(
          wl_registry_bind(registry, name, &wl_shm_interface, version));
    } else if (!strcmp(interface, wl_shell_interface.name)) {
      me->m_shell = static_cast<wl_shell*>(
          wl_registry_bind(registry, name, &wl_shell_interface, version));
    } else if (!strcmp(interface, wl_seat_interface.name)) {
      me->m_seat = static_cast<wl_seat*>(
          wl_registry_bind(registry, name, &wl_seat_interface, version));
      me->m_pointer = wl_seat_get_pointer(me->m_seat);
      const struct wl_pointer_listener pointer_listener = {
          kgmWindow::onPointerEnter, kgmWindow::onPointerLeave, kgmWindow::onPointerMotion, kgmWindow::onPointerButton,
          kgmWindow::onPointerAxis};
      wl_pointer_add_listener(me->m_pointer, &pointer_listener, data);
    }
  }

  void kgmWindow::onPointerEnter(void* data,
                             struct wl_pointer* wl_pointer,
                             uint32_t serial,
                             struct wl_surface* surface,
                             wl_fixed_t surface_x,
                             wl_fixed_t surface_y)
  {}

  void kgmWindow::onPointerLeave(void* data,
                             struct wl_pointer* wl_pointer,
                             uint32_t serial,
                             struct wl_surface* wl_surface)
  {}

  void kgmWindow::onPointerMotion(void* data,
                              struct wl_pointer* wl_pointer,
                              uint32_t time,
                              wl_fixed_t surface_x,
                              wl_fixed_t surface_y)
  {}

  // Program exits if clicking any mouse buttons.
  void kgmWindow::onPointerButton(void* data,
                              struct wl_pointer* wl_pointer,
                              uint32_t serial,
                              uint32_t time,
                              uint32_t button,
                              uint32_t state)
  {
    kgmWindow* me = static_cast<kgmWindow*>(data);
    //me->m_loop = false;
  }

  void kgmWindow::onPointerAxis(void* data,
                            struct wl_pointer* wl_pointer,
                            uint32_t time,
                            uint32_t axis,
                            wl_fixed_t value)
                            {}


#elif defined(LINUX)

#include <dlfcn.h>

u16 keyTranslate(KeySym key)
{
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
  case XK_BackSpace:
    return KEY_BACK;
  case XK_Delete:
    return KEY_DELETE;
  case XK_minus:
    return KEY_SUB;
  case XK_comma:
    return KEY_COMMA;
  case XK_period:
    return KEY_DOT;
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
  case XK_F1:
    return KEY_F1;
  case XK_F2:
    return KEY_F2;
  case XK_F3:
    return KEY_F3;
  case XK_F4:
    return KEY_F4;
  case XK_F5:
    return KEY_F5;
  case XK_F6:
    return KEY_F6;
  case XK_F7:
    return KEY_F7;
  case XK_F8:
    return KEY_F8;
  case XK_F9:
    return KEY_F9;
  case XK_F10:
    return KEY_F10;
  case XK_F11:
    return KEY_F11;
  case XK_F12:
    return KEY_F12;
  }
  return KEY_NONE;
}

int kgmWindow::WndProc(kgmWindow* wnd, XEvent* evt)
{
  Window wroot, wchild;
  kgmEvent::Event m_evt = {0};
  int rx, ry, cx, cy;
  unsigned int mask;
  KeySym   ksym;

  switch(evt->type)
  {
  case Expose:
    m_evt.event = evtPaint;
    m_evt.x = ((XExposeEvent*)evt)->x;
    m_evt.y = ((XExposeEvent*)evt)->y;
    m_evt.width = ((XExposeEvent*)evt)->width;
    m_evt.height = ((XExposeEvent*)evt)->height;
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

    switch(((XMotionEvent*)evt)->state)
    {
    case Button1Mask:
      m_evt.keyMask = KEY_MSBLEFT;
      break;
    case Button2Mask:
      m_evt.keyMask = KEY_MSBMIDDLE;
      break;
    case Button3Mask:
      m_evt.keyMask = KEY_MSBRIGHT;
      break;
    default:
      m_evt.keyMask = KEY_NONE;
    }

    if(!wnd->m_msAbs)
    {
      unsigned int  x, y, w, h, border, depth;

      Window dummy;
      XGetGeometry(wnd->m_dpy, wnd->m_wnd, &dummy, (int*)&x, (int*)&y, (u32*)&w, (u32*)&h, &border, &depth);

      m_evt.msx = w / 2 - evt->xbutton.x;
      m_evt.msy = h / 2 - evt->xbutton.y;

      static bool seted = false;

      if(seted)
      {
        seted = false;
      }
      else
      {
        XWarpPointer(wnd->m_dpy, wnd->m_wnd, wnd->m_wnd, 0, 0, w, h, w / 2, h / 2);
        seted = true;
      }
    }
    else
    {
      m_evt.msx = evt->xbutton.x;
      m_evt.msy = evt->xbutton.y;
    }
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
        m_evt.event = evtMsMiddleDown;
      else
        m_evt.event = evtMsMiddleUp;
      break;
    case Button3:
      if(evt->type == ButtonPress)
        m_evt.event = evtMsRightDown;
      else
        m_evt.event = evtMsRightUp;
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
    if (evt->xclient.data.l[0] == wnd->m_wmDelete)
      m_evt.event = evtClose;
    else
      m_evt.event = evtNone;
    break;
  default:
    m_evt.event = evtNone;
    break;
  }

  if(wnd && m_evt.event)
    wnd->onEvent(&m_evt);

  return 1;
}

#elif defined(ANDROID)

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

kgmWindow::kgmWindow()
{
  m_parent = 0;
  m_msAbs = true;
  m_msf = false;

  setHandle(null);

  #ifdef WIN32

  m_wnd = NULL;

  #elif defined(ANDROID)

  #elif defined(DARWIN)

  #elif defined(WAYLAND)

  m_compositor = nullptr;
  m_display    = nullptr;
  m_surface    = nullptr;
  m_pointer    = nullptr;
  m_seat       = nullptr;
  m_shell      = nullptr;
  m_shm        = nullptr;

  #else

  m_dpy = null;
  m_wnd = {0};
  m_screen = 0;
  m_visual = null;

  #endif

}

kgmWindow::kgmWindow(kgmWindow* wp, kgmString wname, int x, int y, int w, int h, int bpp)
{
  m_parent = wp;
  m_msAbs = true;
  m_msf = false;

  kgmLog::log("Init screen");

  setHandle(null);

#ifdef WIN32

  WNDCLASS wcl;

  if(!GetClassInfo(0, cWndClass, &wcl))
    kgmRegisterWindowClass();

  m_wnd = CreateWindow(cWndClass, wname,
                       WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                       x, y, w, h,
                       (wp) ? (wp->m_wnd) : (0), 0, 0, 0);

  SetWindowLongPtr(m_wnd, GWLP_USERDATA, (LONG_PTR) this);
  ShowWindow(m_wnd, SW_SHOW);
  UpdateWindow(m_wnd);

#elif defined(ANDROID)

  kgm_log() << "Init window rect: " << w << " " << h << ".";
  m_wRect[0] = x;
  m_wRect[1] = y;
  m_wRect[2] = w;
  m_wRect[3] = h;

#elif defined(DARWIN)

  kgm_log() << "Init window rect: " << w << " " << h << ".";
  m_wRect[0] = x;
  m_wRect[1] = y;
  m_wRect[2] = w;
  m_wRect[3] = h;

#elif defined(WAYLAND)

    m_display = wl_display_connect(nullptr);

    if (!m_display)
    {
      kgm_log() << "WL: Cannot open display.\n";

      return;
    }

    struct wl_registry* registry = wl_display_get_registry(m_display);

    const struct wl_registry_listener registry_listener = {
        kgmWindow::onRegistry, kgmWindow::onRemoveRegistry
    };

    wl_registry_add_listener(registry, &registry_listener, this);

    wl_display_dispatch(m_display);
    wl_display_roundtrip(m_display);
    wl_registry_destroy(registry);

    if (!m_compositor)
    {
      kgm_log() << "WL: No compositor.\n";

      return;
    }

    m_surface = wl_compositor_create_surface(m_compositor);

    if (!m_surface)
    {
      kgm_log() << "WL: No surface.\n";

      return;
    }

    wl_surface_commit(m_surface);
    wl_display_roundtrip(m_display);

    m_loop = true;

#else
  typedef XVisualInfo* (*XGETVISUALINFO)(Display *display, long vinfo_mask, XVisualInfo *vinfo_template, int *nitems_return);

  void* x11lib = null;

  m_dpy    = null;
  m_wnd    = {0};
  m_screen = -1;
  m_visual = nullptr;

  XVisualInfo* vi = nullptr;

  XGETVISUALINFO PfnXGetVisualInfo = NULL;

  x11lib = dlopen(NULL, RTLD_LAZY);

  if (x11lib)
    PfnXGetVisualInfo = (XGETVISUALINFO) dlsym(x11lib, "XGetVisualInfo");

  if (!m_dpy)
    m_dpy    = (wp) ? (wp->m_dpy) : XOpenDisplay(NULL);

  if (m_screen < 0)
    m_screen = (wp) ? (wp->m_screen) : DefaultScreen(m_dpy);

  kgmGLX glx;

  if (glx.glXGetProcAddress != null)
  {
    int visualData[] = {
        GLX_RENDER_TYPE, GLX_RGBA_BIT,
        GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
        GLX_DOUBLEBUFFER, True,
        GLX_RED_SIZE, 8,
        GLX_GREEN_SIZE, 8,
        GLX_BLUE_SIZE, 8,
        GLX_ALPHA_SIZE, 8,
        GLX_DEPTH_SIZE, 24,
        GLX_STENCIL_SIZE, 8,
        None
    };

    int numfbconfigs;
    int best_fbc = -1, worst_fbc = -1, best_num_samp = -1, worst_num_samp = 999;
    GLXFBConfig fbconfig = 0, *fbconfigs = glx.glXChooseFBConfig(m_dpy, DefaultScreen(m_dpy), visualData, &numfbconfigs);

    for (int i = 0; i < numfbconfigs; i++)
    {
      vi = (XVisualInfo*) glx.glXGetVisualFromFBConfig(m_dpy, fbconfigs[i]);

      if (!vi)
        continue;

      int samp_buf, samples;

      glx.glXGetFBConfigAttrib( m_dpy, fbconfigs[i], GLX_SAMPLE_BUFFERS, &samp_buf );
      glx.glXGetFBConfigAttrib( m_dpy, fbconfigs[i], GLX_SAMPLES       , &samples  );

      if ( best_fbc < 0 || samp_buf && samples > best_num_samp )
        best_fbc = i, best_num_samp = samples;
      if ( worst_fbc < 0 || !samp_buf || samples < worst_num_samp )
        worst_fbc = i, worst_num_samp = samples;
    }

    fbconfig = fbconfigs[best_fbc];

    vi = glx.glXGetVisualFromFBConfig(m_dpy, fbconfig);

    if (vi)
    {
      m_screen = vi->screen;
    }
  }

  XWindowAttributes xattr;

  Window wroot = RootWindow(m_dpy, m_screen);

  if (vi != null)
  {

  }
  else
  {
    XGetWindowAttributes(m_dpy, wroot, &xattr);
  }

  XVisualInfo vinfo;
  vinfo.screen = m_screen;
  //vinfo.visualid = XVisualIDFromVisual(xattr.visual);
  s32 n;

  if (!vi && PfnXGetVisualInfo)
  {
    vi = PfnXGetVisualInfo(m_dpy, VisualScreenMask | VisualIDMask, &vinfo, &n);
  }

  if (x11lib)
  {
    dlclose(x11lib);

    PfnXGetVisualInfo = null;
    x11lib = null;
  }

  if (vi)
  {
    kgm_log() << "VisualInfo depth: " << vi->depth << ".\n";
  }
  else
  {
    kgm_log() << "VisualInfo error: No visual info " << n << ".\n";
  }

  if (vi && vi->screen == m_screen)
  {
    m_xswa.border_pixel = BlackPixel(m_dpy, m_screen);
    m_xswa.background_pixel = WhitePixel(m_dpy, m_screen);
    m_xswa.override_redirect = True;
    m_xswa.colormap = XCreateColormap(m_dpy, RootWindow(m_dpy, m_screen), vi->visual, AllocNone);
    m_xswa.event_mask = ExposureMask;

    m_wnd = XCreateWindow(m_dpy,
                          RootWindow(m_dpy, m_screen),
                          //DefaultRootWindow(m_dpy),
                          x, y, w, h, 0, vi->depth, InputOutput, vi->visual,
                          CWBackPixel | CWColormap | CWBorderPixel | CWEventMask, &m_xswa);

    m_visual = vi;
  }
  else
  {
    kgm_fatal("Unable create opengl compatile window.\n");

    XCloseDisplay(m_dpy);

    m_dpy = null;
  }

  m_wmDelete = XInternAtom(m_dpy, "WM_DELETE_WINDOW", 0);

  XSetWMProtocols(m_dpy, m_wnd, &m_wmDelete, 1);

  XSelectInput(m_dpy, m_wnd, ExposureMask | KeyPressMask | KeyReleaseMask |  ButtonPressMask |
               ButtonReleaseMask | PointerMotionMask | StructureNotifyMask | ButtonMotionMask);

  XMapWindow(m_dpy, m_wnd);
  XStoreName(m_dpy, m_wnd, wname);
  XFlush(m_dpy);

#endif
}

kgmWindow::~kgmWindow()
{
  //Prepare to close window
#ifdef DEBUG
  kgm_log() << "Prepare to destroy window.\n";
#endif

#ifdef WIN32

  if (m_wnd)
    DestroyWindow(m_wnd);

  m_wnd = 0;

#elif defined(ANDROID)

#elif defined(DARWIN)

#elif defined(WAYLAND)

  wl_pointer_destroy(m_pointer);
  wl_seat_destroy(m_seat);
  wl_shell_destroy(m_shell);
  wl_shm_destroy(m_shm);
  wl_compositor_destroy(m_compositor);
  wl_display_disconnect(m_display);

#else

  if (m_gc)
  {
    kgmObject* o = dynamic_cast<kgmObject*>(m_gc);

    //kgmOGL* ogl = static_cast<kgmOGL*>(m_gc);

    kgmObject::Release(o);
  }

  if (m_visual)
  {
    XFree(m_visual);

    m_visual = null;
  }

  if (m_dpy)
  {
    XFreeColormap(m_dpy, m_xswa.colormap);

    XDestroyWindow(m_dpy, m_wnd);

    // XCloseDisplay(m_dpy);
  }

  m_dpy = null;
  m_wnd = null;
  m_gc  = null;

#endif

#ifdef DEBUG
  kgm_log() << "kgmWindow::~kgmWindow [" << (void*) this << "]\n";
#endif
}

void kgmWindow::fullscreen(bool fs)
{
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
  {
    if(ChangeDisplaySettings(&Mode, CDS_FULLSCREEN) == 0)
    {
      SetWindowLong(m_wnd,GWL_STYLE, WS_POPUP);
      SetWindowPos(m_wnd, HWND_TOP, 0, 0, iWidth, iHeight, SWP_SHOWWINDOW);
    }
  }
  else
  {
    if(ChangeDisplaySettings(NULL, 0) == 0)
    {
      SetWindowLong(m_wnd,GWL_STYLE, WS_OVERLAPPEDWINDOW);
      SetWindowPos(m_wnd, HWND_TOP, 50, 50, iWidth - 100, iHeight - 100, SWP_SHOWWINDOW);
    }
  }

#elif defined(ANDROID)

#elif defined(DARWIN)

#elif defined(WAYLAND)

#else

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
}

void kgmWindow::show(bool sh)
{
#ifdef WIN32

  if(sh && !IsWindowVisible(m_wnd))
  {
    ShowWindow(m_wnd, SW_SHOWNORMAL);
    return;
  }

  if(!sh && IsWindowVisible(m_wnd))
  {
    ShowWindow(m_wnd, SW_HIDE);
    return;
  }

#elif defined(ANDROID)

#elif defined(DARWIN)

#elif defined(WAYLAND)
#else

  if(sh)
    XMapWindow(m_dpy, m_wnd);
  else
    XUnmapWindow(m_dpy, m_wnd);

  XFlush(m_dpy);

#endif
}

void kgmWindow::loop()
{
  m_loop = true;

#ifdef WIN32

  MSG msg;

  while(m_wnd && m_loop)
  {
    if(PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
    {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
    else
    {
      onIdle();
    }

    Sleep(0);
  }

#elif defined(ANDROID)

#elif defined(DARWIN)

#elif defined(WAYLAND)
  while(m_loop && wl_display_dispatch(m_display))
  {
  }
#else

  XEvent evt;

  while(m_loop && m_dpy)
  {
    while(XPending(m_dpy) > 0)
    {
      XNextEvent(m_dpy, &evt);

      WndProc(this, &evt);

      if (evt.type == ClientMessage &&
          evt.xclient.data.l[0] == m_wmDelete) {
        m_loop = 0;

        break;
      }

      if(!m_dpy)
        break;
    }

    if(!m_loop || !m_dpy)
      break;

    onIdle();
    usleep(0);
  }

#endif

  m_loop = false;
}

void kgmWindow::close()
{
  #ifdef WIN32
  SendMessage(m_wnd, WM_CLOSE, 0, 0);
  #else
  XEvent event;
  event.xclient.type = ClientMessage;
  event.xclient.window = m_wnd;
  event.xclient.message_type = XInternAtom(m_dpy, "WM_PROTOCOLS", 1);
  event.xclient.format = 32;
  event.xclient.data.l[0] = XInternAtom(m_dpy, "WM_DELETE_WINDOW", 0);
  event.xclient.data.l[1] = CurrentTime;
  XSendEvent(m_dpy, m_wnd, False, NoEventMask, &event);
  #endif
}

void kgmWindow::getRect(int& x, int& y, int& w, int& h)
{
#ifdef WIN32

  RECT r;

  GetClientRect(m_wnd, (LPRECT)&r);

  x = r.left, y = r.top, w = r.right - x, h = r.bottom - y;

#elif defined(ANDROID)

  x = m_wRect[0];
  y = m_wRect[1];
  w = m_wRect[2];
  h = m_wRect[3];

#elif defined(DARWIN)

#elif defined(WAYLAND)
#elif defined(X11)

  unsigned int  width, height, border, depth;
  Window dummy;
  XGetGeometry(m_dpy, m_wnd, &dummy, &x, &y, (u32*)&w, (u32*)&h, &border, &depth);

#endif
}

void kgmWindow::setRect(int x, int y, int w, int h)
{
#ifdef WIN32

  RECT r;

  GetWindowRect(m_wnd, (LPRECT)&r);

  SetWindowPos(m_wnd, NULL, x, y, w, h, 0);

#elif defined(ANDROID)

  m_wRect[0] = x;
  m_wRect[1] = y;
  m_wRect[2] = w;
  m_wRect[3] = h;
  onResize(w, h);

#elif defined(DARWIN)

#elif defined(WAYLAND)
#else

  XMoveResizeWindow(m_dpy, m_wnd, x, y, w, h);
  XMapWindow(m_dpy, m_wnd);
  XFlush(m_dpy);

#endif
}

void kgmWindow::setMouse(int x, int y)
{
  m_msf = true;

#ifdef WIN32

  POINT pt = {x, y};
  ScreenToClient(m_wnd, &pt);
  SetCursorPos(pt.x, pt.y);

#elif defined(ANDROID)
#elif defined(DARWIN)

#elif defined(WAYLAND)
#else

  int cx, cy, w, h;
  getRect(cx, cy, w, h);
  XWarpPointer(m_dpy, m_wnd, m_wnd, 0, 0, w, h, x, y);

#endif
}

void kgmWindow::getMouse(int& x, int& y)
{
  m_msf = true;

#ifdef WIN32

  POINT pt = {x, y};
  // ScreenToClient(m_hWnd, &pt);
  // SetCursorPos(pt.x, pt.y);
#elif defined(ANDROID)
#elif defined(DARWIN)

#elif defined(WAYLAND)
#else

#endif
}

void kgmWindow::setTitle(char* title)
{
#ifdef WIN32

  SetWindowText(m_wnd, title);

#elif defined(ANDROID)

#elif defined(DARWIN)

#elif defined(WAYLAND)
#else

  XStoreName(m_dpy, m_wnd, title);

#endif
}

kgmIGC* kgmWindow::getGC()
{
  return m_gc;
}

void kgmWindow::setHandle(void* h)
{
  #ifdef ANDROID
  m_handle = h;
  #elif defined(DARWIN)
  m_handle = h;
  #endif
}

/*
void kgmWindow::onEvent(kgmEvent::Event* e)
{
  kgmEvent::onEvent(e);
}
*/

kgmWindow* kgmWindow::createGLWindow(kgmString wname, int widht, int height)
{
  kgmWindow* wnd = null;

  wnd = new kgmWindow(null, wname, 0, 0, widht, height, 32);
  kgmIGC* gc = kgmCreateOGLContext(wnd);

  if (!gc)
  {
    gc = kgmCreateGLESContext(wnd);
  }

  if (gc)
    wnd->m_gc = gc;

  return wnd;
}

kgmWindow* kgmWindow::createVKWindow(kgmString wname, int widht, int height)
{
  kgmWindow* wnd = null;

  wnd = new kgmWindow(null, wname, 0, 0, widht, height, 32);
  kgmIGC* gc = kgmCreateVKContext(wnd);

  if (gc)
    wnd->m_gc = gc;

  return wnd;
}


/* X11 borderless window example.

#include <stdlib.h>
#include <math.h>
#include <X11/Xlib.h>
#include <GL/glx.h>
#include <GL/glu.h>

int main(int argc, char *argv[]){
  XSetWindowAttributes   swa;
  XEvent                 xev;

  Display                 *dpy    = XOpenDisplay(NULL);
  Window                  root    = DefaultRootWindow(dpy);
  GLint                   att[]   = {GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, True, None};
  XVisualInfo             *vi     = glXChooseVisual(dpy, 0, att);
  GLXContext              glc     = glXCreateContext(dpy, vi, NULL, False);
  Visual                  *vis    = DefaultVisual(dpy, 0);
  Colormap                cmap    = XCreateColormap(dpy, root, vis, AllocNone);
  unsigned int            w       = XDisplayWidth(dpy, 0) / 2;
  unsigned int            h       = XDisplayHeight(dpy, 0) / 2;
  int                     dep     = DefaultDepth(dpy, 0);
  int                     cmask   = CWColormap | CWBorderPixel | CWEventMask | CWOverrideRedirect;
  XWindowAttributes       gwa;
  Window                  win;


  swa.colormap           = cmap;
  swa.border_pixel       = 0;
  swa.event_mask         = ExposureMask;
  swa.override_redirect  = 1;
  win = XCreateWindow(dpy, root, 10, 10, w, h, 0, dep, InputOutput, vis, cmask, &swa);
  XMapWindow(dpy, win);

  glXMakeCurrent(dpy, win, glc);
  glClearColor(0.00, 0.00, 0.60, 1.00);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-1., 1., -1., 1., 1., 100.);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  //gluLookAt(0., 0., 10., 0., 0., 0., 0., 1., 0.);

  while(1) {
    XNextEvent(dpy, &xev);

    if(xev.type == Expose) {
      XGetWindowAttributes(dpy, win, &gwa);
      glViewport(0, 0, gwa.width, gwa.height);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      // DRAW SOMETHING
      glXSwapBuffers(dpy, win);
    }
  }
}

//
//      gcc -o gl-base gl-base.cc -lX11 -lGL -lGLU -lm
//

*/
