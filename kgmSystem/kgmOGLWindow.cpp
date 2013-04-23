///////////////
// kgmWindow
#include "kgmOGLWindow.h"
#include "kgmOGL.h"
#include "kgmApp.h"
#include "../kgmBase/kgmLog.h"


//kgmOGLWindow::kgmOGLWindow()
//:kgmWindow(0, "", 10, 10, 500, 400, 16, false){
//}

kgmOGLWindow::kgmOGLWindow(kgmWindow* wp, char* wname, int x, int y, int w, int h, int bpp, bool fs)
:kgmWindow(0, "", x, y, w, h, bpp, fs){
 m_gc = 0;

 kgmLog::log("init ogl screen");

#ifdef WIN32
 PIXELFORMATDESCRIPTOR pfd = {0};
 pfd.nSize = sizeof(pfd);
 pfd.nVersion = 1;
 pfd.dwFlags = PFD_DRAW_TO_WINDOW | 
               PFD_SUPPORT_OPENGL |
               PFD_DOUBLEBUFFER;// |
//				  PFD_GENERIC_ACCELERATED | 
//				   PFD_SWAP_LAYER_BUFFERS;
 pfd.cColorBits = 24;
 pfd.cDepthBits = 16;
 pfd.cStencilBits = 8;
 pfd.iPixelType = PFD_TYPE_RGBA;
 pfd.iLayerType = PFD_MAIN_PLANE;

 m_hdc = GetDC(m_wnd);
 if(!m_hdc)
	 return;
 int pf = ChoosePixelFormat(m_hdc,&pfd);
 if(!pf)
	 return;
 if(!SetPixelFormat(m_hdc,pf,&pfd))
	 return;
 m_hrc = wglCreateContext(m_hdc);
 if(!m_hrc)
	 return;
 if(!wglMakeCurrent(m_hdc,m_hrc))
	 return;
#endif

#ifdef LINUX
/* attributes for a single buffered visual in RGBA format with at least
   4 bits per color and a 16 bit depth buffer
*/
static int attrSgl[] = {
    GLX_RGBA,
    GLX_RED_SIZE, 4,
    GLX_GREEN_SIZE, 4,
    GLX_BLUE_SIZE, 4,
    GLX_DEPTH_SIZE, 16,
    None
};

/* attributes for a double buffered visual in RGBA format with at least
   4 bits per color and a 16 bit depth buffer
*/
static int attrDbl[] = { GLX_RGBA, GLX_DOUBLEBUFFER,
    GLX_RED_SIZE, 4,
    GLX_GREEN_SIZE, 4,
    GLX_BLUE_SIZE, 4,
    GLX_DEPTH_SIZE, 8,
    None
};

  Colormap cmap;
  XVisualInfo* vi = 0;
  XF86VidModeModeInfo **modes;
  int rx, ry, rw, rh;
  int numModes = 0;
  int bestMode = -1;
  int i = 0;
  XF86VidModeGetAllModeLines(m_dpy, m_screen, &numModes, &modes);
  m_mode = *modes[0];
  getRect(rx, ry, rw, rh);
  for(i = 0; i < numModes; i++){
   if( (modes[i]->hdisplay == rw) && (modes[i]->vdisplay == rh) ){
	bestMode = i;
	break;
   }
  }
  vi = glXChooseVisual(m_dpy, m_screen, attrDbl);
  if(vi == 0){
   kgmLog::log("No Doublebuffered Visual");
   vi = glXChooseVisual(m_dpy, m_screen, attrSgl);
   if(vi == 0){
    kgmLog::log("No Singlebuffered Visual");
    return;
   }
   else
    kgmLog::log("\nGot Singlebuffered Visual");
  }
  else
   kgmLog::log("\nGot Doublebuffered Visual");

  m_glctx = glXCreateContext(m_dpy, vi, 0, GL_TRUE);
  cmap = XCreateColormap(m_dpy, RootWindow(m_dpy, m_screen), vi->visual, AllocNone);
  m_xswa.colormap = cmap;
  m_xswa.border_pixel = 0;
  
  XTextProperty tprop;
  XGetWMName(m_dpy, m_wnd, &tprop);
  XDestroyWindow(m_dpy, m_wnd);

  /* create a window in window mode*/
  m_xswa.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask | StructureNotifyMask;
  m_wnd = XCreateWindow(m_dpy, RootWindow(m_dpy, vi->screen),  x, y, w, h,
	 	0, vi->depth, InputOutput, vi->visual, CWBorderPixel | CWColormap | CWEventMask, &m_xswa);
  Atom delWindow = XInternAtom( m_dpy, "WM_DELETE_WINDOW", 0 );
  XSetWMProtocols(m_dpy, m_wnd, &delWindow, 1);
  glXMakeCurrent(m_dpy, m_wnd, m_glctx);
  if(glXIsDirect(m_dpy, m_glctx))
   kgmLog::log("\nDirect Rendering!");
  else
   kgmLog::log("\nNo Direct Rendering!\n");
  XMapWindow(m_dpy, m_wnd);
  XFlush(m_dpy);
  XFree(modes);

  if(tprop.value)
   XStoreName(m_dpy, m_wnd, (const char*)tprop.value);

  if(m_fs)
   fullscreen(true);
#endif

#ifdef ANDROIDXXX
  const EGLint RGBX_888_ATTRIBS[] =
  {
  #ifdef GLES_2
    EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
  #endif
    EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
    EGL_BLUE_SIZE, 8, EGL_GREEN_SIZE, 8,
    EGL_RED_SIZE, 8, EGL_DEPTH_SIZE, 8, EGL_NONE
  };

  const EGLint RGB_565_ATTRIBS[] =
  {
  #ifdef GLES_2
    EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
  #endif
    EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
    EGL_BLUE_SIZE, 5, EGL_GREEN_SIZE, 6,
    EGL_RED_SIZE, 5, EGL_DEPTH_SIZE, 8, EGL_NONE
  };

  EGLint    dummy, format;
  EGLint    numConfigs;
  EGLConfig config;

  display = EGL_NO_DISPLAY;
  context = EGL_NO_CONTEXT;
  surface = EGL_NO_SURFACE;

  display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
  kgm_log() << "kgmEngine: eglGetDisplay\n";

  eglInitialize(display, 0, 0);
  kgm_log() << "kgmEngine: eglInitialize\n";

  if(eglChooseConfig(display, RGBX_888_ATTRIBS, &config, 1, &numConfigs) == 0)
  {
    if(eglChooseConfig(display, RGB_565_ATTRIBS, &config, 1, &numConfigs) == 0)
    {
      return;
    }
  }
  kgm_log() << "kgmEngine: eglChooseConfig\n";

  eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);
  kgm_log() << "kgmEngine: eglGetConfigAttrib\n";

  ANativeWindow_setBuffersGeometry((ANativeWindow*)kgmApp::application()->m_nativeWindow, 0, 0, format);
  kgm_log() << "kgmEngine: ANativeWindow_setBuffersGeometry\n";

  surface = eglCreateWindowSurface(display, config, (ANativeWindow*)kgmApp::application()->m_nativeWindow, NULL);
  kgm_log() << "kgmEngine: eglCreateWindowSurface\n";
  context = eglCreateContext(display, config, NULL, NULL);
  kgm_log() << "kgmEngine: eglCreateContext\n";

  if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
    kgm_log() << "kgmEngine: Unable to eglMakeCurrent \n";

    return;
  }

  eglQuerySurface(display, surface, EGL_WIDTH, &w);
  eglQuerySurface(display, surface, EGL_HEIGHT, &h);

  //glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
  //glEnable(GL_CULL_FACE);
  //glShadeModel(GL_SMOOTH);
  //glDisable(GL_DEPTH_TEST);
  //glClearColor(1, 1, 1, 1);
  //glClear(GL_COLOR_BUFFER_BIT);

  //eglSwapBuffers(display, surface);
#endif

 m_gc = new kgmOGL(this);
}

kgmOGLWindow::~kgmOGLWindow(){
}

void kgmOGLWindow::onClose()
{
  if(m_gc)
    delete m_gc;

  m_gc = 0;

 #ifdef WIN32
  wglDeleteContext(m_hrc);
  wglMakeCurrent(m_hdc, 0);
  ReleaseDC(m_wnd,m_hdc);
 #endif

 #ifdef LINUX
  if (m_glctx)
  {
   if(!glXMakeCurrent(m_dpy, None, NULL))
    kgmLog::log("Could not release drawing context.\n");
   glXDestroyContext(m_dpy, m_glctx);
   m_glctx = NULL;
  }
 #endif

 #ifdef ANDROIDXXX
  if (display != EGL_NO_DISPLAY) {
    eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    if (context != EGL_NO_CONTEXT) {
      eglDestroyContext(display, context);
    }
    if (surface != EGL_NO_SURFACE) {
      eglDestroySurface(display, surface);
    }
    eglTerminate(display);
  }

  display = EGL_NO_DISPLAY;
  context = EGL_NO_CONTEXT;
  surface = EGL_NO_SURFACE;
 #endif

  kgmWindow::onClose();
}

kgmIGraphics* kgmOGLWindow::getGC(){
  return m_gc;
}





