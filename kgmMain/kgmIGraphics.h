#pragma once
#include "kgmInterface.h"
#include "kgmMath.h"
class   kgmIGraphics;

// RENDER DEVICE 
enum gc_flags{
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
 gcv_uv_f4 = 1L << 12, //vertex texture size float4 else float2
};

enum gc_enum{
 gc_none = 0,
 gc_null = 0,
 gc_opengl = 0,
 gc_soft,
 gc_d3d8,
 gc_d3d9,

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

 gcpmt_points,
 gcpmt_lines,
 gcpmt_triangles,
 gcpmt_linestrip,
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

 gctype_tex2d,
 gctype_texcube,
 gctype_textarget,


 gccull_back,
 gccull_front,
};


class kgmIGraphics{
public:
 typedef void* Shader;
 typedef void* Texture;


public:
 virtual void  gcSetParameter(uint param, void* value) = 0;
 virtual void  gcGetParameter(uint param, void* value) = 0;
 virtual void  gcClear(uint flag, uint col, float depth, uint sten) = 0;
 virtual void  gcBegin() = 0;
 virtual void  gcEnd() = 0;
 virtual void  gcRender() = 0;
 virtual void  gcSetTarget(void*  rt) = 0;
 // DRAWING 
 virtual void  gcDraw(uint pmt, uint v_fmt, uint v_size, uint v_cnt, void *v_pnt, uint i_size, uint i_cnt, void *i_pnt) = 0;
 // TEXTURE 
 virtual void* gcGenTexture(void *m, uint w, uint h, uint bpp, uint type) = 0;
 virtual void  gcFreeTexture(void *t) = 0;
 virtual void  gcSetTexture(uint stage, void *t) = 0;
 //MODE 
 virtual void  gc2DMode() = 0;
 virtual void  gc3DMode() = 0;
 // MATRIX 
 virtual void  gcSetMatrix(uint mode, float* mtx) = 0;
 virtual void  gcGetMatrix(uint mode, float* mtx) = 0;
 virtual void  gcSetViewport(int x, int y, int w, int h, float n, float f) = 0;
 //FOG
 //virtual void  gcFog(kgmFog*) = 0;
 //BLEND
 virtual void  gcBlend(bool, uint, uint) = 0;
 //ALPHA
 virtual void  gcAlpha(bool, uint, float) = 0;
 //CULL
 virtual void  gcCull(uint mode) = 0;
 // LIGHT & MATERIALS
 //virtual void  gcSetLight(int i, kgmLight* l) = 0;
 //VERTEX & INDEX BUFFERS
 virtual void* gcGenVertexBuffer(void* vdata, uint vsize, void* idata, uint isize) = 0;
 virtual void  gcFreeVertexBuffer(void*) = 0;
 virtual void  gcDrawVertexBuffer(void* buf, uint pmt, uint vfmt, uint vsize, uint vcnt, uint isize, uint icnt, uint ioff) = 0;
 // SHADER
 virtual void* gcGenShader(const char*, const char*) = 0;
 virtual void  gcFreeShader(void* s) = 0;
 virtual void  gcSetShader(void* s) = 0;
 virtual void  gcUniform(void* s, uint, uint, const char*, void*) = 0;
 virtual void  gcUniformMatrix(void* s, uint, uint, const char*, void*) = 0;
 virtual void  gcUniformSampler(void* s, const char*, void*) = 0;
};