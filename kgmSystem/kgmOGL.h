// kgmOGLDevice.h: interface for the kgmOGLDevice class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "../kgmBase/kgmIGC.h"
#include "kgmWindow.h"
#include "kgmLib.h"

#ifdef OGL

#ifdef WIN32

#include <windows.h>
#include <GL/gl.h>
#include <GL/glext.h>

#define GL_OBJECT_COMPILE_STATUS GL_OBJECT_COMPILE_STATUS_ARB

#elif defined DARWIN

#include <GL/gl.h>
#include <GL/glext.h>

#ifndef GL_OBJECT_COMPILE_STATUS
#define GL_OBJECT_COMPILE_STATUS GL_OBJECT_COMPILE_STATUS_ARB
#endif

#ifndef GL_OBJECT_LINK_STATUS
#define GL_OBJECT_LINK_STATUS GL_OBJECT_LINK_STATUS_ARB
#endif

#else

#include <GL/gl.h>
#include "inc/GL/glx.h"
#include "inc/glext.h"

#ifndef GL_OBJECT_COMPILE_STATUS
#define GL_OBJECT_COMPILE_STATUS GL_OBJECT_COMPILE_STATUS_ARB
#endif

#ifndef GL_OBJECT_LINK_STATUS
#define GL_OBJECT_LINK_STATUS GL_OBJECT_LINK_STATUS_ARB
#endif

#endif

#define GLhandle GLhandleARB
typedef u32 GLu32;

class kgmOGLWindow;

struct kgmGLX
{
#ifdef WIN32
#else
  void (*glXSwapBuffers)(Display *dpy, GLXDrawable drawable);
  GLXContext (*glXCreateContext)(Display *dpy, XVisualInfo *vis, GLXContext share_list, Bool direct);
  GLXContext (*glXCreateContextAttribsARB)(Display *, GLXFBConfig, GLXContext, Bool, const int *);
  Bool (*glXMakeCurrent)(Display *, GLXDrawable, GLXContext);
  Bool (*glXIsDirect)(Display *dpy, GLXContext ctx);
  Bool (*glXQueryVersion)(Display *dpy, int *major, int *minor);
  void (*glXDestroyContext)(Display *dpy, GLXContext ctx);
  void *(*glXGetProcAddress)(const s8 *);
  GLXContext (*glXGetCurrentContext)();
  GLXFBConfig* (*glXChooseFBConfig)(Display * dpy, int   screen, int * AttributeList, int * nelements);
  XVisualInfo* (*glXGetVisualFromFBConfig)(Display * dpy, GLXFBConfig  config);
  int (*glXGetFBConfigAttrib)(Display * dpy, GLXFBConfig  config, int  Attribute, int * Value);

#endif

  kgmLib lib;

  kgmGLX()
  {
    #ifdef WIN32
    #else
    glXGetProcAddress = null;

    if (!lib.open("libGLX.so"))
    {
      if (!lib.open("libGLX.so.0"))
      {
        if (!lib.open("libGLX.so.1"))
        {
          return;
        }
      }
    }

    glXGetProcAddress = (void *(*)(const s8 *))lib.get("glXGetProcAddress");

    if (glXGetProcAddress == null)
    {
      return;
    }

    #define GLX_FN(func) func = (typeof(func))glXGetProcAddress(#func)

    GLX_FN(glXSwapBuffers);
    GLX_FN(glXCreateContext);
    GLX_FN(glXMakeCurrent);
    GLX_FN(glXIsDirect);
    GLX_FN(glXDestroyContext);
    GLX_FN(glXCreateContextAttribsARB);
    GLX_FN(glXQueryVersion);
    GLX_FN(glXGetCurrentContext);
    GLX_FN(glXChooseFBConfig);
    GLX_FN(glXGetVisualFromFBConfig);
    GLX_FN(glXGetFBConfigAttrib);
    #endif
  }
};

//////// OPENGL GRAPHYCAL CONTEXT
class kgmOGL : public kgmObject, public kgmIGC
{
  KGM_OBJECT(kgmOGL)
public:
  // VERTEX & INDEX BUFFER
  struct VBO
  {
    GLu32 vb, ib;
    u32 vsize, isize;
  };

  // FRAME BUFFER
  struct RenderBuffer
  {
    GLu32 frame, color, depth, stencil;
    u16 width, height;
    u32 type, cmside;
  };

  struct Texture
  {
    u32 type;
    GLu32 buffer, texture;
    u16 width, height, format;
  };

private:
#ifdef WIN32

  HDC m_hdc;
  HGLRC m_hrc;

#elif defined(DARWIN)
#else

  kgmLib m_glx;

  GLXContext m_glctx = null;

#endif

  kgmWindow *m_wnd = null;

  // Current Render Buffer
  RenderBuffer *m_renderbuffer = null;

  // Mode Matrices
  f32 mtx_mode_view[16],
      mtx_mode_proj[16];

  // Active lights
  s32 m_min_filter;
  s32 m_mag_filter;

  s32 m_viewport[4];

  u32 m_cubemapside = 0;

  GLhandle m_shader = null;

  u32 m_error = 0;

public:
  kgmOGL(kgmWindow *wnd, void *ctx = null);
  ~kgmOGL();

  // BASE
  gc_enum gcGetBase() { return gc_opengl; }

  // RENDER
  void gcSet(u32 param, void *value);
  void gcGet(u32 param, void *value);

  u32 gcError();

  void gcClear(u32 flag, u32 col, f32 depth, u32 sten);
  void gcBegin();
  void gcEnd();
  void gcRender();

  // RESIZE MAIN BUFFER
  void gcResize(u32 width, u32 height);

  // DRAW
  void gcDraw(u32 pmt, u32 v_fmt, u32 v_size, u32 v_cnt, void *v_pnt, u32 i_size, u32 i_cnt, void *i_pnt);

  // TEXTURE
  gchandle gcGenTexture(void *mf, u32 w, u32 h, u32 bpp, u32 type);
  void gcFreeTexture(gchandle tex);
  void gcSetTexture(u32 stage, gchandle tex);

  // TARGET
  gchandle gcGenTarget(u32 w, u32 h, bool depth, bool stencil);
  bool gcTexTarget(gchandle tar, gchandle tex, u32 type);
  void gcFreeTarget(gchandle t);
  void gcSetTarget(gchandle t);

  // VIEWPORT
  void gcSetViewport(int x, int y, int w, int h, float n, float f);

  // BLEND
  void gcBlend(bool enabla, u32 eq, u32 fnsrc, u32 fndst);

  // CULL
  void gcCull(u32 mode);

  // DEPTH
  void gcDepth(bool en, bool mask, u32 mode);

  // STENCIL
  void gcStencil(bool en, u32 func, u32 mask, u32 ref,
                 u32 fail, u32 zfail, u32 zpass);

  // SCISSOR
  void gcScissor(bool on, int x, int y, int w, int h);

  // VERTEX & INDEX BUFFER
  gchandle gcGenVertexBuffer(void *vdata, u32 vsize, void *idata, u32 isize);
  void gcFreeVertexBuffer(gchandle);
  void gcDrawVertexBuffer(gchandle buf, u32 pmt, u32 vfmt, u32 vsize, u32 vcnt, u32 isize, u32 icnt, u32 ioff);

  // SHADER
  gchandle gcGenShader(kgmArray<u8> &, kgmArray<u8> &);
  void gcFreeShader(gchandle s);
  void gcSetShader(gchandle s);
  void gcBindAttribute(gchandle s, int i, const char *attr);
  void gcUniform(gchandle s, u32 type, u32 cnt, const char *par, void *val);
  void gcUniformMatrix(gchandle s, u32 type, u32 cnt, u32 tsp, const char *, void *);
  void gcUniformSampler(gchandle s, const char *, void *);
#ifdef DEBUG
  void gcGetUniform(gchandle s, const char *, void *);
#endif

  //////////////////////////// inlines
  GLenum gl_enum(u32 e)
  {
    switch (e)
    {
    case gccmp_always:
      return GL_ALWAYS;
    case gccmp_never:
      return GL_NEVER;
    case gccmp_equal:
      return GL_EQUAL;
    case gccmp_nequal:
      return GL_NOTEQUAL;
    case gccmp_less:
      return GL_LESS;
    case gccmp_lequal:
      return GL_LEQUAL;
    case gccmp_great:
      return GL_GREATER;
    case gccmp_gequal:
      return GL_GEQUAL;

    case gcblend_zero:
      return GL_ZERO;
    case gcblend_one:
      return GL_ONE;
    case gcblend_dstcol:
      return GL_DST_COLOR;
    case gcblend_srccol:
      return GL_SRC_COLOR;
    case gcblend_dsticol:
      return GL_ONE_MINUS_DST_COLOR;
    case gcblend_srcicol:
      return GL_ONE_MINUS_SRC_COLOR;
    case gcblend_dstalpha:
      return GL_DST_ALPHA;
    case gcblend_srcalpha:
      return GL_SRC_ALPHA;
    case gcblend_dstialpha:
      return GL_ONE_MINUS_DST_ALPHA;
    case gcblend_srcialpha:
      return GL_ONE_MINUS_SRC_ALPHA;

    case gcpmt_points:
      return GL_POINTS;
    case gcpmt_lines:
      return GL_LINES;
    case gcpmt_triangles:
      return GL_TRIANGLES;
    case gcpmt_linestrip:
      return GL_LINE_STRIP;
    case gcpmt_trianglestrip:
      return GL_TRIANGLE_STRIP;
    case gcpmt_trianglefan:
      return GL_TRIANGLE_FAN;

    case gcpar_texfltmag:
      return GL_TEXTURE_MAG_FILTER;
    case gcpar_texfltmin:
      return GL_TEXTURE_MIN_FILTER;

    case gcflt_linear:
      return GL_LINEAR;
    case gcflt_near:
      return GL_NEAREST;
    case gcflt_near_mip_near:
      return GL_NEAREST_MIPMAP_NEAREST;
    case gcflt_near_mip_linear:
      return GL_NEAREST_MIPMAP_LINEAR;
    case gcflt_linear_mip_near:
      return GL_LINEAR_MIPMAP_NEAREST;
    case gcflt_linear_mip_linear:
      return GL_LINEAR_MIPMAP_LINEAR;
    }

    return GL_ZERO;
  }

  GLenum gl_blend(u32 e)
  {
    switch (e)
    {
    case gcblend_zero:
      return GL_ZERO;
    case gcblend_one:
      return GL_ONE;
    case gcblend_dstcol:
      return GL_DST_COLOR;
    case gcblend_srccol:
      return GL_SRC_COLOR;
    case gcblend_dsticol:
      return GL_ONE_MINUS_DST_COLOR;
    case gcblend_srcicol:
      return GL_ONE_MINUS_SRC_COLOR;
    case gcblend_dstalpha:
      return GL_DST_ALPHA;
    case gcblend_srcalpha:
      return GL_SRC_ALPHA;
    case gcblend_dstialpha:
      return GL_ONE_MINUS_DST_ALPHA;
    case gcblend_srcialpha:
      return GL_ONE_MINUS_SRC_ALPHA;
    }

    return GL_ZERO;
  }

  GLenum gl_primitive(u32 e)
  {
    switch (e)
    {
    case gcpmt_points:
      return GL_POINTS;
    case gcpmt_lines:
      return GL_LINES;
    case gcpmt_triangles:
      return GL_TRIANGLES;
    case gcpmt_linestrip:
      return GL_LINE_STRIP;
    case gcpmt_trianglestrip:
      return GL_TRIANGLE_STRIP;
    case gcpmt_trianglefan:
      return GL_TRIANGLE_FAN;
    }

    return GL_ZERO;
  }
};

#endif