#pragma once

#include "kgmWindow.h"
#include "../kgmBase/kgmPointer.h"

#ifdef WIN32

#include <windows.h>

#elif defined(ANDROID)

#include <jni.h>
#include <errno.h>
#include <EGL/egl.h>
#include <android/native_window.h>

extern ANativeWindow* kgm_getNativeWindow();

#else

#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <X11/extensions/xf86vmode.h>
#include <GL/gl.h>
#include <GL/glx.h>

#endif


class kgmOGL;

class kgmOGLWindow: public kgmWindow
{
public:
#ifdef WIN32

  HDC        m_hdc;
  HGLRC      m_hrc;

#elif defined(ANDROID)

  EGLSurface surface;
  EGLContext context;
  EGLDisplay display;

#else

  XVisualInfo* m_visual = null;
  GLXContext   m_glctx  = null;

#endif

private:
  kgmOGL* m_gc = null;

public:
  kgmOGLWindow(kgmWindow*, char*, int, int, int, int, int, bool);
  virtual ~kgmOGLWindow();

  kgmIGC* getGC();
};
