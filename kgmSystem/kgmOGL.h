// kgmOGLDevice.h: interface for the kgmOGLDevice class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "../kgmBase/kgmIGC.h"
#include "kgmWindow.h"

#ifdef WIN32
#include <windows.h>
#elif defined(ANDROID)
#else
#endif

#ifdef ANDROID

#include <unistd.h>

#ifdef GLES_2
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <GLES2/gl2platform.h>
#else
#include <GLES/gl.h>
#include <GLES/glext.h>
#include <GLES/glplatform.h>
#endif

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <EGL/eglplatform.h>

typedef u32 GLu32;

#else

#include <GL/gl.h>
#include "inc/glext.h"

#ifndef GL_OBJECT_COMPILE_STATUS
#define GL_OBJECT_COMPILE_STATUS GL_OBJECT_COMPILE_STATUS_ARB
#endif

#ifndef GL_OBJECT_LINK_STATUS
#define GL_OBJECT_LINK_STATUS GL_OBJECT_LINK_STATUS_ARB

#endif
#endif

#ifdef ANDROID

#ifdef GLES_1
#define glClearDepth           glClearDepthf
#define glOrtho                glOrthof

#endif

#ifdef GLES_2

#define glClearDepth           glClearDepthf
#define glCreateProgramObject  glCreateProgram
#define glCreateShaderObject   glCreateShader
#define glUseProgramObject     glUseProgram
#define glDeleteObject         glDeleteShader
#define glAttachObject         glAttachShader
#define glGetInfoLog           glGetProgramInfoLog
#define glGetObjectParameteriv glGetProgramiv

#define GL_DEPTH_TEXTURE_MODE    GL_DEPTH_COMPONENT
#define GL_OBJECT_COMPILE_STATUS GL_ATTACHED_SHADERS
#define GL_OBJECT_LINK_STATUS    GL_LINK_STATUS
#define GL_GENERATE_MIPMAP       GL_GENERATE_MIPMAP_HINT

#endif

#define GL_RED                   0x1903
#define GL_BGR_EXT               GL_RGB
#define GL_INTENSITY             GL_UNSIGNED_BYTE
#define GL_CLAMP                 GL_CLAMP_TO_EDGE

#define GLdouble  GLfloat
#define GLhandle  GLuint
#define GLcharARB GLchar

#else

#define GLhandle GLhandleARB

#endif

class kgmOGLWindow;

//////// OPENGL GRAPHYCAL CONTEXT
class kgmOGL: public kgmIGC
{
public:
  //VERTEX & INDEX BUFFER
  struct VBO
  {
    GLu32 vb, ib;
    u32   vsize, isize;
  };

  //FRAME BUFFER
  struct RenderBuffer
  {
    GLu32 frame, color, depth, stencil;
    u16 width, height;
  };

  struct Texture
  {
    u32   type;
    GLu32 buffer, texture;
    u16 width,  height, format;
  };

private:

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

  kgmWindow* m_wnd = null;

  //Current Render Buffer
  RenderBuffer* m_renderbuffer = null;

  // Mode Matrices
  float mtx_mode_view[16],
        mtx_mode_proj[16];

  // Active lights
  int m_lights;

  int m_is_shader;
  int m_is_framebuffer;

  int m_min_filter;
  int m_mag_filter;

public:
  kgmOGL(kgmWindow* wnd);
  virtual ~kgmOGL();


  //RENDER
  void  gcSet(u32 param, void* value);
  void  gcGet(u32 param, void* value);
  void  gcClear(u32 flag, u32 col, f32 depth, u32 sten);
  void  gcBegin();
  void  gcEnd();
  void  gcRender();
  void  gcSetTarget(void* t);

  //DRAW  
  void  gcDraw(u32 pmt, u32 v_fmt, u32 v_size, u32 v_cnt, void *v_pnt, u32 i_size, u32 i_cnt, void *i_pnt);

  //TEXTURE
  void* gcGenTexture(void *mf, u32 w, u32 h, u32 bpp, u32 type);
  void  gcFreeTexture(void* tex);
  void  gcSetTexture(u32 stage, void* tex);

  //MATRIX
  void  gcSetMatrix(u32 mm, float* mtx);
  void  gcGetMatrix(u32 mm, float* mtx);

  //VIEWPORT
  void  gcSetViewport(int x, int y, int w, int h, float n, float f);

  //BLEND
  void  gcBlend(bool enabla, u32 fnsrc, u32 fndst);

  //ALPHA
  void  gcAlpha(bool en, u32 fn, float ref);

  //CULL
  void  gcCull(u32 mode);

  //DEPTH
  void  gcDepth(bool en, bool mask, u32 mode);

  //CLIP PLANE
  void  gcClipPlane(bool, u32 id, float*);

  //STENCIL
  void  gcStencil(bool en, u32 func, u32 mask, u32 ref,
                  u32 fail, u32 zfail, u32 zpass);

  //LIGHT
  void gcSetLight(int i, float* pos, float forse, float* col, float* dir, float angle);

  //VERTEX & INDEX BUFFER
  void* gcGenVertexBuffer(void* vdata, u32 vsize, void* idata, u32 isize);
  void  gcFreeVertexBuffer(void*);
  void  gcDrawVertexBuffer(void* buf, u32 pmt, u32 vfmt, u32 vsize, u32 vcnt, u32 isize, u32 icnt, u32 ioff);

  // SHADER
  void* gcGenShader(const char*, const char*);
  void  gcFreeShader(void* s);
  void  gcSetShader(void* s);
  void  gcBindAttribute(void* s, int i, const char* attr);
  void  gcUniform(void* s, u32 type, u32 cnt, const char* par, void* val);
  void  gcUniformMatrix(void* s, u32 type, u32 cnt, u32 tsp, const char*, void*);
  void  gcUniformSampler(void* s, const char*, void*);
#ifdef DEBUG
  void  gcGetUniform(void* s, const char*, void*);
#endif

  //////////////////////////// inlines
  GLenum gl_enum(u32 e)
  {
    switch(e)
    {
      case gccmp_always: return GL_ALWAYS;
      case gccmp_never:	 return GL_NEVER;
      case gccmp_equal:	 return GL_EQUAL;
      case gccmp_nequal: return GL_NOTEQUAL;
      case gccmp_less:   return GL_LESS;
      case gccmp_lequal: return GL_LEQUAL;
      case gccmp_great:	 return GL_GREATER;
      case gccmp_gequal: return GL_GEQUAL;

      case gcblend_zero:		  return GL_ZERO;
      case gcblend_one:		    return GL_ONE;
      case gcblend_dstcol:		return GL_DST_COLOR;
      case gcblend_srccol:		return GL_SRC_COLOR;
      case gcblend_dsticol:		return GL_ONE_MINUS_DST_COLOR;
      case gcblend_srcicol:		return GL_ONE_MINUS_SRC_COLOR;
      case gcblend_dstalpha:	return GL_DST_ALPHA;
      case gcblend_srcalpha:	return GL_SRC_ALPHA;
      case gcblend_dstialpha:	return GL_ONE_MINUS_DST_ALPHA;
      case gcblend_srcialpha:	return GL_ONE_MINUS_SRC_ALPHA;

      case gcpmt_points:		    return GL_POINTS;
      case gcpmt_lines:			    return GL_LINES;
      case gcpmt_triangles:		  return GL_TRIANGLES;
      case gcpmt_linestrip:     return GL_LINE_STRIP;
      case gcpmt_trianglestrip: return GL_TRIANGLE_STRIP;
      case gcpmt_trianglefan:   return GL_TRIANGLE_FAN;

      case gctex_fltmag: return GL_TEXTURE_MAG_FILTER;
      case gctex_fltmin: return GL_TEXTURE_MIN_FILTER;

      case gcflt_linear:            return GL_LINEAR;
      case gcflt_near:              return GL_NEAREST;
      case gcflt_near_mip_near:     return GL_NEAREST_MIPMAP_NEAREST;
      case gcflt_near_mip_linear:   return GL_NEAREST_MIPMAP_LINEAR;
      case gcflt_linear_mip_near:   return GL_LINEAR_MIPMAP_NEAREST;
      case gcflt_linear_mip_linear: return GL_LINEAR_MIPMAP_LINEAR;
    }

    return GL_ZERO;
  }

  GLenum gl_blend(u32 e)
  {
    switch(e)
    {
    case gcblend_zero:		  return GL_ZERO;
    case gcblend_one:		    return GL_ONE;
    case gcblend_dstcol:		return GL_DST_COLOR;
    case gcblend_srccol:		return GL_SRC_COLOR;
    case gcblend_dsticol:		return GL_ONE_MINUS_DST_COLOR;
    case gcblend_srcicol:		return GL_ONE_MINUS_SRC_COLOR;
    case gcblend_dstalpha:	return GL_DST_ALPHA;
    case gcblend_srcalpha:	return GL_SRC_ALPHA;
    case gcblend_dstialpha:	return GL_ONE_MINUS_DST_ALPHA;
    case gcblend_srcialpha:	return GL_ONE_MINUS_SRC_ALPHA;
    }

    return GL_ZERO;
  }

  GLenum gl_primitive(u32 e)
  {
    switch(e)
    {
    case gcpmt_points:         return  GL_POINTS;
    case gcpmt_lines:          return  GL_LINES;
    case gcpmt_triangles:      return  GL_TRIANGLES;
    case gcpmt_linestrip:      return  GL_LINE_STRIP;
    case gcpmt_trianglestrip:  return  GL_TRIANGLE_STRIP;
    case gcpmt_trianglefan:    return  GL_TRIANGLE_FAN;
    }

    return GL_ZERO;
  }
};

