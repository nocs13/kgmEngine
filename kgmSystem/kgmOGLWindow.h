#pragma once
#include "kgmWindow.h"


#ifdef WIN32
 #include <windows.h>
#endif

#ifdef LINUX
 #include <unistd.h>
 #include <X11/Xlib.h>
 #include <X11/Xutil.h>
 #include <X11/keysym.h>
 #include <X11/extensions/xf86vmode.h>
 #include <GL/gl.h>
 #include <GL/glx.h>
#endif

class kgmOGL;

class kgmOGLWindow: public kgmWindow{
public:
#ifdef WIN32
 HDC        m_hdc;
 HGLRC      m_hrc;
#endif

#ifdef LINUX
 GLXContext m_glctx;
#endif

 kgmOGL *m_gc;
public:
 //kgmOGLWindow();
 kgmOGLWindow(kgmWindow*, char*, int, int, int, int, int, bool);
 virtual ~kgmOGLWindow();
 
 kgmIGraphics* getGC();
};
