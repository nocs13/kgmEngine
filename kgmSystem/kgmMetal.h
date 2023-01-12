#include "../kgmBase/kgmIGC.h"

#include "kgmWindow.h"
#include "kgmLib.h"

#ifdef DARWIN
#include "kgmIGC.h"

class kgmMetal: public kgmIGC
{
 public:
  kgmMetal();
  kgmMetal(kgmWindow *wnd);
  ~kgmMetal();

  u32   gcError();

  //BASE
  gc_enum gcGetBase() { return gc_vulkan; }

  void  gcSet(u32 param, void* value);
  void  gcGet(u32 param, void* value);
  void  gcClear(u32 flag, u32 col, float depth, u32 sten);
  void  gcBegin();
  void  gcEnd();
  void  gcRender();
  void  gcSetTarget(void*  rt);

  void gcResize(u32 width, u32 height);

  // DRAWING
  void  gcDraw(u32 pmt, u32 v_fmt, u32 v_size, u32 v_cnt, void *v_pnt, u32 i_size, u32 i_cnt, void *i_pnt);

  // TEXTURE
  gchandle gcGenTexture(void *m, u32 w, u32 h, u32 bpp, u32 type);
  void  gcFreeTexture(gchandle t);
  void  gcSetTexture(u32 stage, gchandle t);

  // VIEWPORT
  void  gcSetViewport(int x, int y, int w, int h, float n, float f);

  //BLEND
  void  gcBlend(bool, u32, u32, u32);

  //CULL
  void  gcCull(u32 mode);

  //DEPTH
  void  gcDepth(bool en, bool mask, u32 mode);

  //CLIP PLANE
  void  gcClipPlane(bool, u32 id, f64 plane[4]);
  
  //STENCIL
  void  gcStencil(bool en, u32 func, u32 mask, u32 ref,
                  u32 fail, u32 zfail, u32 zpass);

  // SCISSOR
  void  gcScissor(bool on, int x, int y, int w, int h);

  //VERTEX & INDEX BUFFERS
  gchandle gcGenVertexBuffer(void* vdata, u32 vsize, void* idata, u32 isize);
  void  gcFreeVertexBuffer(gchandle buf);
  void  gcDrawVertexBuffer(gchandle buf, u32 pmt, u32 vfmt, u32 vsize, u32 vcnt, u32 isize, u32 icnt, u32 ioff);

  // SHADER
  gchandle gcGenShader(kgmArray<u8>&, kgmArray<u8>&);
  void  gcFreeShader(gchandle s);
  void  gcSetShader(gchandle s);
  void  gcBindAttribute(gchandle s, int, const char*);
  void  gcUniform(gchandle s, u32, u32, const char*, void*);
  void  gcUniformMatrix(gchandle s, u32, u32, u32, const char*, void*);
  void  gcUniformSampler(gchandle s, const char*, void*);

  // TARGET
  gchandle gcGenTarget(u32 w, u32 h, bool depth, bool stencil);
  bool     gcTexTarget(gchandle tar, gchandle tex, u32 type);
  void     gcFreeTarget(gchandle t);

#ifdef DEBUG
  void  gcGetUniform(void* s, const char*, void*);
#endif  
};

#endif
