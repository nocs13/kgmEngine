#ifndef KGMGCNONE_H
#define KGMGCNONE_H

#include "../kgmBase/kgmIGC.h"

class kgmWindow;

class kgmGCNone : public kgmIGC
{
public:
  kgmGCNone();
  kgmGCNone(kgmWindow *wnd);
  ~kgmGCNone();

  //BASE
  gc_enum gcGetBase() { return gc_none; }

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

  // SCISSOR
  void  gcScissor(bool on, int x, int y, int w, int h);

  //CLIP PLANE
  void  gcClipPlane(bool, u32 id, f64 plane[4]);

  //STENCIL
  void  gcStencil(bool en, u32 func, u32 mask, u32 ref,
                  u32 fail, u32 zfail, u32 zpass);

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
};

#endif // KGMGCNONE_H
