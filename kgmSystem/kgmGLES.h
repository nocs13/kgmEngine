#ifndef KGMGLES_H
#define KGMGLES_H

#include "../kgmBase/kgmIGC.h"
#include "kgmLib.h"

class kgmWindow;

#ifdef GLES_2

#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#ifndef GLhandle
typedef size_t GLhandle;
#endif

class kgmGLES : public kgmObject, public kgmIGC
{
  KGM_OBJECT(kgmGLES)
  kgmWindow* m_wnd = null;

  kgmLib m_egl;
  kgmLib m_gles;

  EGLDisplay  m_display = EGL_NO_DISPLAY;
  EGLSurface  m_surface = EGL_NO_SURFACE;
  EGLContext  m_context = EGL_NO_CONTEXT;

  s32 m_error = 0;

  s32 m_renderbuffer;

  GLuint m_shader = 0;

  s32 m_min_filter;
  s32 m_mag_filter;

  s32 m_viewport[4];

  u32 m_cubemapside = 0;

public:
  //VERTEX & INDEX BUFFER
  struct VBO
  {
    u32 vb, ib;
    u32   vsize, isize;
  };

  //FRAME BUFFER
  struct RenderBuffer
  {
    u32 frame, color, depth, stencil;
    u16   width, height;
    u32   type, cmside;
  };

  struct Texture
  {
    u32   type;
    u32 buffer, texture;
    u16   width,  height, format;
  };
public:
  kgmGLES(kgmWindow* wnd);
  ~kgmGLES();

  //BASE
  gc_enum gcGetBase() { return gc_opengl; }

  //RENDER
  void  gcSet(u32 param, void* value);
  void  gcGet(u32 param, void* value);

  u32   gcError();

  void  gcClear(u32 flag, u32 col, f32 depth, u32 sten);
  void  gcBegin();
  void  gcEnd();
  void  gcRender();

  //RESIZE MAIN BUFFER
  void gcResize(u32 width, u32 height);

  //DRAW
  void  gcDraw(u32 pmt, u32 v_fmt, u32 v_size, u32 v_cnt, void *v_pnt, u32 i_size, u32 i_cnt, void *i_pnt);

  //TEXTURE
  gchandle gcGenTexture(void *mf, u32 w, u32 h, u32 bpp, u32 type);
  void     gcFreeTexture(gchandle tex);
  void     gcSetTexture(u32 stage, gchandle tex);

  // TARGET
  gchandle gcGenTarget(u32 w, u32 h, bool depth, bool stencil);
  bool     gcTexTarget(gchandle tar, gchandle tex, u32 type);
  void     gcFreeTarget(gchandle t);
  void     gcSetTarget(gchandle  t);

  //VIEWPORT
  void  gcSetViewport(int x, int y, int w, int h, float n, float f);

  //BLEND
  void  gcBlend(bool enabla, u32 eq, u32 fnsrc, u32 fndst);

  //CULL
  void  gcCull(u32 mode);

  //DEPTH
  void  gcDepth(bool en, bool mask, u32 mode);

  //STENCIL
  void  gcStencil(bool en, u32 func, u32 mask, u32 ref,
                  u32 fail, u32 zfail, u32 zpass);

  //SCISSOR
  void  gcScissor(bool on, int x, int y, int w, int h);

  //VERTEX & INDEX BUFFER
  gchandle gcGenVertexBuffer(void* vdata, u32 vsize, void* idata, u32 isize);
  void     gcFreeVertexBuffer(gchandle);
  void     gcDrawVertexBuffer(gchandle buf, u32 pmt, u32 vfmt, u32 vsize, u32 vcnt, u32 isize, u32 icnt, u32 ioff);

  // SHADER
  gchandle gcGenShader(kgmArray<u8>&, kgmArray<u8>&);
  void     gcFreeShader(gchandle s);
  void     gcSetShader(gchandle s);
  void     gcBindAttribute(gchandle s, int i, const char* attr);
  void     gcUniform(gchandle s, u32 type, u32 cnt, const char* par, void* val);
  void     gcUniformMatrix(gchandle s, u32 type, u32 cnt, u32 tsp, const char*, void*);
  void     gcUniformSampler(gchandle s, const char*, void*);
#ifdef DEBUG
  void  gcGetUniform(gchandle s, const char*, void*);
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

      case gcpar_texfltmag: return GL_TEXTURE_MAG_FILTER;
      case gcpar_texfltmin: return GL_TEXTURE_MIN_FILTER;

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

#endif

#endif // KGMGLES_H
