///////////////
// kgmWindow
#include "kgmOGLWindow.h"
#include "kgmOGL.h"
#include "kgmApp.h"
#include "../kgmBase/kgmLog.h"


kgmOGLWindow::kgmOGLWindow(kgmWindow* wp, char* wname, int x, int y, int w, int h, int bpp, bool fs)
  :kgmWindow(null, "", x, y, w, h, bpp, fs)
{
#ifdef DEBUG
  kgm_log() << "init ogl screen\n";
#endif

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

  SendMessage(m_wnd, WM_ACTIVATE, NULL, NULL);
  SendMessage(m_wnd, WM_PAINT, NULL, NULL);

#elif defined(ANDROID)

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
  eglInitialize(display, 0, 0);

  if(eglChooseConfig(display, RGBX_888_ATTRIBS, &config, 1, &numConfigs) == 0)
  {
    if(eglChooseConfig(display, RGB_565_ATTRIBS, &config, 1, &numConfigs) == 0)
    {
      return;
    }
  }

  eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);
  ANativeWindow_setBuffersGeometry((ANativeWindow*)kgmApp::application()->m_nativeWindow, 0, 0, format);
  surface = eglCreateWindowSurface(display, config, (ANativeWindow*)kgmApp::application()->m_nativeWindow, NULL);
  context = eglCreateContext(display, config, NULL, NULL);

  if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE)
  {
#ifdef DEBUG
    kgm_log() << "kgmEngine: Unable to eglMakeCurrent \n";
#endif

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

#else
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
                           GLX_DEPTH_SIZE, 16,
                           None
                         };

  Colormap cmap;
  //XF86VidModeModeInfo **modes;
  int rx, ry, rw, rh;
  //int numModes = 0;
  //int bestMode = -1;

  //XF86VidModeGetAllModeLines(m_dpy, m_screen, &numModes, &modes);
  //m_mode = *modes[0];

  getRect(rx, ry, rw, rh);

  /*for(int i = 0; i < numModes; i++)
  {
    if( (modes[i]->hdisplay == rw) && (modes[i]->vdisplay == rh) )
    {
      bestMode = i;

      break;
    }
  }*/

  m_visual = glXChooseVisual(m_dpy, m_screen, attrDbl);

  if(m_visual == null)
  {
    kgmLog::log("No Doublebuffered Visual");

    m_visual = glXChooseVisual(m_dpy, m_screen, attrSgl);

    if(m_visual == null)
    {
      kgmLog::log("No Singlebuffered Visual");

      return;
    }
    else
    {
      kgmLog::log("\nGot Singlebuffered Visual");
    }
  }
  else
  {
    kgmLog::log("\nGot Doublebuffered Visual");
  }

  m_glctx = glXCreateContext(m_dpy, m_visual, 0, GL_TRUE);

  if(!glXMakeCurrent(m_dpy, m_wnd, m_glctx))
  {
    kgmLog::log("\nError: Cannot activate ogl context.");
  }

  if(glXIsDirect(m_dpy, m_glctx))
    kgmLog::log("\nDirect Rendering!");
  else
    kgmLog::log("\nNo Direct Rendering!\n");

#endif

  m_gc = kgm_ptr<kgmOGL>(new kgmOGL(this));
}

kgmOGLWindow::~kgmOGLWindow()
{

  m_gc = (kgmOGL*)null;

#ifdef WIN32

  wglDeleteContext(m_hrc);
  wglMakeCurrent(m_hdc, 0);
  ReleaseDC(m_wnd,m_hdc);

#elif defined(ANDROIDXXX)

  if (display != EGL_NO_DISPLAY)
  {
    eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

    if (context != EGL_NO_CONTEXT)
    {
      eglDestroyContext(display, context);
    }

    if (surface != EGL_NO_SURFACE)
    {
      eglDestroySurface(display, surface);
    }

    eglTerminate(display);
  }

  display = EGL_NO_DISPLAY;
  context = EGL_NO_CONTEXT;
  surface = EGL_NO_SURFACE;

#else

  if (m_glctx)
  {
    if(!glXMakeCurrent(m_dpy, None, NULL))
      kgmLog::log("Could not release drawing context.\n");

    glXDestroyContext(m_dpy, m_glctx);

    m_glctx = NULL;
  }

  if (m_visual)
  {
    XFree(m_visual);

    m_visual = null;
  }

#endif

#ifdef DEBUG
  kgm_log() << "kgmOGLWindow::~kgmOGLWindow.\n";
#endif
}

kgmIGC* kgmOGLWindow::getGC()
{
  return (kgmOGL*) m_gc;
}
