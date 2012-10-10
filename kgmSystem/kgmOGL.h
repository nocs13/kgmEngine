// kgmOGLDevice.h: interface for the kgmOGLDevice class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "../kgmBase/kgmIGraphics.h"
#include "kgmWindow.h"

#ifdef WIN32
 #include <windows.h>
#elif defined LINUX
#endif

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
//#include "inc/glext.h"

#ifdef WIN32
 #pragma comment(lib, "opengl32.lib")
 #pragma comment(lib, "glu32.lib")
#endif


class kgmOGLWindow;

//////// OPENGL GRAPHYCAL CONTEXT
class kgmOGL: public kgmIGraphics{
public:
//VERTEX & INDEX BUFFER 
struct VBO{
 GLuint vb, ib;
 uint   vsize, isize;
};
//FRAME BUFFER 
struct RenderBuffer{
  GLuint frame, color, depth, stencil;
  u16 width, height;
};

struct Texture{
 u32   type;
 GLuint buffer, texture;
 u16 width,  height, format;
};
private:
 kgmOGLWindow* m_wnd;
 
//Current Render Buffer
 RenderBuffer* m_renderbuffer;
// Mode Matrices
 float mtx_mode_view[16], mtx_mode_proj[16];
public:
 kgmOGL(kgmOGLWindow* wnd);
 virtual ~kgmOGL();


//RENDER
 void  gcSetParameter(u32 param, void* value);
 void  gcGetParameter(u32 param, void* value);
 void  gcClear(uint flag, u32 col, f32 depth, u32 sten);
 void  gcBegin();
 void  gcEnd();
 void  gcRender();
 void  gcSetTarget(void* t);
//MODE
 void  gc2DMode();
 void  gc3DMode();
//DRAW
 void  gcDraw(u32 pmt, u32 v_fmt, uint v_size, uint v_cnt, void *v_pnt, uint i_size, uint i_cnt, void *i_pnt);
 void  gcDrawRect(int x, int y, int w, int h, uint col);
 //void  gcDrawText(lRect clip, float w, float h, uint col, char *text, uint tlen);
 void  gcDrawBillboard(vec3 v, float w, float h, uint col);
//TEXTURE
 void* gcGenTexture(void *mf, uint w, uint h, uint bpp, uint type);
 void  gcFreeTexture(void* tex);
 void  gcSetTexture(uint stage, void* tex);
//MATRIX
 void  gcSetMatrix(uint mm, float* mtx);
 void  gcGetMatrix(uint mm, float* mtx);
//VIEWPORT
 void  gcSetViewport(int x, int y, int w, int h, float n, float f);
 //BLEND
 void  gcBlend(bool enabla, uint fnsrc, uint fndst);
 //ALPHA
 void  gcAlpha(bool en, uint fn, float ref);
 //CULL
 void  gcCull(uint mode);
 //DEPTH
 void  gcDepth(bool en, bool mask, uint mode);
 //CLIP PLANE
 void  gcClipPlane(bool, uint id, float*);
 //STENCIL
 void  gcStencil(bool en, uint func, uint mask, uint ref, 
		 uint fail, uint zfail, uint zpass); 
 //VERTEX & INDEX BUFFER 
 void* gcGenVertexBuffer(void* vdata, uint vsize, void* idata, uint isize);
 void  gcFreeVertexBuffer(void*);
 void  gcDrawVertexBuffer(void* buf, uint pmt, uint vfmt, uint vsize, uint vcnt, uint isize, uint icnt, uint ioff);
 // SHADER
 void* gcGenShader(const char*, const char*);
 void  gcFreeShader(void* s);
 void  gcSetShader(void* s);
 void  gcUniform(void* s, uint type, uint cnt, const char* par, void* val);
 void  gcUniformMatrix(void* s, uint, uint, const char*, void*);
 void  gcUniformSampler(void* s, const char*, void*);

 //////////////////////////// inlines
 GLenum gl_enum(uint e){
  switch(e){
  case gccmp_always:    return GL_ALWAYS;
  case gccmp_never:	return GL_NEVER;
  case gccmp_equal:	return GL_EQUAL;
  case gccmp_nequal:    return GL_NOTEQUAL;
  case gccmp_less:      return GL_LESS;
  case gccmp_lequal:    return GL_LEQUAL;
  case gccmp_great:	return GL_GREATER;
  case gccmp_gequal:    return GL_GEQUAL;

  case gcblend_zero:		return GL_ZERO;
  case gcblend_one:		return GL_ONE;
  case gcblend_dstcol:		return GL_DST_COLOR;
  case gcblend_srccol:		return GL_SRC_COLOR;
  case gcblend_dsticol:		return GL_ONE_MINUS_DST_COLOR;
  case gcblend_srcicol:		return GL_ONE_MINUS_SRC_COLOR;
  case gcblend_dstalpha:	return GL_DST_ALPHA;
  case gcblend_srcalpha:	return GL_SRC_ALPHA;
  case gcblend_dstialpha:	return GL_ONE_MINUS_DST_ALPHA;
  case gcblend_srcialpha:	return GL_ONE_MINUS_SRC_ALPHA;

  case gcpmt_points:		 return  GL_POINTS;
  case gcpmt_lines:			 return  GL_LINES;
  case gcpmt_triangles:		 return  GL_TRIANGLES;
  case gcpmt_linestrip:      return  GL_LINE_STRIP;
  case gcpmt_trianglestrip:  return  GL_TRIANGLE_STRIP;
  case gcpmt_trianglefan:    return  GL_TRIANGLE_FAN;
  }
  return GL_ZERO;
 }
 GLenum gl_blend(gc_enum e){
  switch(e){
  case gcblend_zero:		return GL_ZERO;
  case gcblend_one:		return GL_ONE;
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
 GLenum gl_primitive(gc_enum e){
  switch(e){
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

