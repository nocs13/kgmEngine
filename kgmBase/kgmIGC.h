#pragma once
#include "../kgmMath/kgmMath.h"
#include "kgmObject.h"

class kgmIGC;
class kgmObject;

typedef void* gchandle;

// RENDER DEVICE
enum gc_flags
{
  gcflag_color   = 1L << 0,
  gcflag_depth   = 1L << 1,
  gcflag_stencil = 1L << 2,
  gcflag_blend   = 1L << 3,
  gcflag_light   = 1L << 4,

  gcv_xyz = 1L << 0, //vertex position
  gcv_wgh = 1L << 1, //vertex weights
  gcv_nor = 1L << 2, //vertex normal
  gcv_col = 1L << 3, //vertex diffuse
  gcv_uv0 = 1L << 4, //vertex texture coord 1
  gcv_uv1 = 1L << 5, // ...2 float
  gcv_uv2 = 1L << 6,
  gcv_uv3 = 1L << 7,
  gcv_uv4 = 1L << 8,
  gcv_uv5 = 1L << 9,
  gcv_uv6 = 1L << 10,
  gcv_uv7 = 1L << 11, //vertex texture coord 8
  gcv_bn0 = 1L << 12, //vertex bone weight&index float[4], int[4]
};

enum gc_enum
{
  gc_none = 0,
  gc_null = 0,
  gc_opengl = 0,
  gc_soft,
  gc_d3d8,
  gc_d3d9,
  gc_d3d10,
  gc_d3d11,

  gc_rdev,

  gcpar_lighting,
  gcpar_blending,
  gcpar_culling,
  gcpar_textures,
  gcpar_depth,
  gcpar_color,
  gcpar_alpha,
  gcpar_stencil,
  gcpar_depthmask,
  gcpar_colormask,

  gccmp_never,
  gccmp_always,
  gccmp_equal,
  gccmp_nequal,
  gccmp_less,
  gccmp_great,
  gccmp_lequal,
  gccmp_gequal,

  gcblend_zero,
  gcblend_one,
  gcblend_dstcol,
  gcblend_srccol,
  gcblend_dsticol,
  gcblend_srcicol,
  gcblend_dstalpha,
  gcblend_srcalpha,
  gcblend_dstialpha,
  gcblend_srcialpha,
  gcblend_dstsatalpha,
  gcblend_srcsatalpha,

  gcblend_eqadd,
  gcblend_eqsub,
  gcblend_eqmin,
  gcblend_eqmax,
  gcblend_eqrsub,

  gcpmt_points,
  gcpmt_lines,
  gcpmt_triangles,
  gcpmt_linestrip,
  //gcpmt_lineloop; /* disabled as no in d3d. */
  gcpmt_trianglestrip,
  gcpmt_trianglefan,

  gcmtx_view,
  gcmtx_proj,
  gcmtx_text,

  gctex_s,
  gctex_t,
  gctex_r,
  gctex_q,
  gctex_gen,
  gctex_obj,
  gctex_eye,
  gctex_sphere,

  gctex_fmt8,
  gctex_fmt16,
  gctex_fmt24,
  gctex_fmt32,
  gctex_fmtdepth,
  gctex_fmtfloat,

  gctex_fltmin,
  gctex_fltmag,

  gctype_tex2d,
  gctype_texcube,
  gctype_texdepth,

  gccull_back,
  gccull_front,

  gcflt_near,
  gcflt_linear,
  gcflt_near_mip_near,
  gcflt_near_mip_linear,
  gcflt_linear_mip_near,
  gcflt_linear_mip_linear,


  gcunitype_float1,
  gcunitype_float2,
  gcunitype_float3,
  gcunitype_float4,
  gcunitype_int1,
  gcunitype_int2,
  gcunitype_int3,
  gcunitype_int4,
  gcunitype_mtx2,
  gcunitype_mtx3,
  gcunitype_mtx4,

  //hardware support
  gcsup_shaders = 1024, // vertex,format shaders
  gcsup_fbuffers,       // frame buffers
};

#define GC_MAX_TERTURE_UNITS 8

class kgmIGC: public kgmObject
{
public:
  virtual void  gcSet(u32 param, void* value) = 0;
  virtual void  gcGet(u32 param, void* value) = 0;

  virtual u32   gcError() = 0;

  virtual void  gcClear(u32 flag, u32 col, float depth, u32 sten) = 0;
  virtual void  gcBegin() = 0;
  virtual void  gcEnd() = 0;
  virtual void  gcRender() = 0;

  // DRAWING
  virtual void  gcDraw(u32 pmt, u32 v_fmt, u32 v_size, u32 v_cnt, void *v_pnt, u32 i_size, u32 i_cnt, void *i_pnt) = 0;

  // TEXTURE
  virtual gchandle gcGenTexture(void *m, u32 w, u32 h, u32 bpp, u32 type) = 0;
  virtual void     gcFreeTexture(gchandle t) = 0;
  virtual void     gcSetTexture(u32 stage, gchandle t) = 0;

  // TARGET
  virtual gchandle gcGenTarget(u32 w, u32 h, bool depth) = 0;
  virtual bool     gcTexTarget(gchandle tar, gchandle tex, u32 type) = 0;
  virtual void     gcFreeTarget(gchandle t) = 0;
  virtual void     gcSetTarget(gchandle t) = 0;

  // MATRIX
  virtual void  gcSetMatrix(u32 mode, float* mtx) = 0;
  virtual void  gcGetMatrix(u32 mode, float* mtx) = 0;
  virtual void  gcSetViewport(int x, int y, int w, int h, float n, float f) = 0;

  // BLEND
  virtual void  gcBlend(bool, u32, u32, u32) = 0;

  // ALPHA
  virtual void  gcAlpha(bool, u32, float) = 0;

  // CULL
  virtual void  gcCull(u32 mode) = 0;

  // DEPTH
  virtual void  gcDepth(bool en, bool mask, u32 mode) = 0;

  // VERTEX & INDEX BUFFERS
  virtual gchandle gcGenVertexBuffer(void* vdata, u32 vsize, void* idata, u32 isize) = 0;
  virtual void     gcFreeVertexBuffer(gchandle) = 0;
  virtual void     gcDrawVertexBuffer(gchandle, u32 pmt, u32 vfmt, u32 vsize, u32 vcnt, u32 isize, u32 icnt, u32 ioff) = 0;

  // SHADER
  virtual gchandle gcGenShader(const char*, const char*) = 0;
  virtual void     gcFreeShader(gchandle s) = 0;
  virtual void     gcSetShader(gchandle s) = 0;
  virtual void     gcBindAttribute(gchandle s, int, const char*) = 0;
  virtual void     gcUniform(gchandle s, u32, u32, const char*, void*) = 0;
  virtual void     gcUniformMatrix(gchandle s, u32, u32, u32, const char*, void*) = 0;
  virtual void     gcUniformSampler(gchandle s, const char*, void*) = 0;
#ifdef DEBUG
  virtual void  gcGetUniform(gchandle s, const char*, void*) = 0;
#endif
};
