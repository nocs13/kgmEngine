#include "kgmGCNone.h"
#include "kgmWindow.h"

kgmGCNone::kgmGCNone()
{
}

kgmGCNone::kgmGCNone(kgmWindow *wnd)
{
}

kgmGCNone::~kgmGCNone()
{
}

void kgmGCNone::gcSet(u32 param, void* value)
{
}

void kgmGCNone::gcGet(u32 param, void* value)
{
}

u32 kgmGCNone::gcError()
{
  return 0;
}

void kgmGCNone::gcClear(u32 flag, u32 col, float depth, u32 sten)
{
}

void kgmGCNone::gcBegin()
{
}

void kgmGCNone::gcEnd()
{
}

void kgmGCNone::gcRender()
{
}

void kgmGCNone::gcResize(u32 width, u32 height)
{

}

void kgmGCNone::gcSetViewport(int x, int y, int w, int h, float n, float f)
{
}


void  kgmGCNone::gcBlend(bool en, u32 eq, u32 fnsrc, u32 fndst)
{
}

void kgmGCNone::gcCull(u32 mode)
{
}

void kgmGCNone::gcDepth(bool depth, bool mask, u32 mode)
{
}

void  kgmGCNone::gcScissor(bool on, int x, int y, int w, int h)
{
}

gchandle kgmGCNone::gcGenTexture(void *pd, u32 w, u32 h, u32 fmt, u32 type)
{
  return null;
}

void kgmGCNone::gcFreeTexture(gchandle t)
{
}

void kgmGCNone::gcSetTexture(u32 stage, gchandle t)
{
}

gchandle kgmGCNone::gcGenTarget(u32 w, u32 h, bool d, bool s)
{
  return (gchandle) null;
}

bool kgmGCNone::gcTexTarget(gchandle tar, gchandle tex, u32 type)
{
  return true;
}

void kgmGCNone::gcFreeTarget(gchandle t)
{
}

void kgmGCNone::gcSetTarget(gchandle t)
{
}

void kgmGCNone::gcClipPlane(bool en, u32 id, f64 plane[4])
{
}

void kgmGCNone::gcStencil(bool en, u32 func, u32 mask, u32 ref,
               u32 fail, u32 zfail, u32 zpass)
{
}

void kgmGCNone::gcDraw(u32 pmt, u32 v_fmt, u32 v_size, u32 v_cnt, void *v_pnt,
                    u32 i_size, u32 i_cnt, void *i_pnt)
{
}

void* kgmGCNone::gcGenVertexBuffer(void* vdata, u32 vsize, void* idata, u32 isize)
{
  return null;
}

void  kgmGCNone::gcFreeVertexBuffer(void* b)
{
}

void  kgmGCNone::gcDrawVertexBuffer(void* b, u32 pmt, u32 vfmt, u32 vsize, u32 vcnt,
                                 u32 isize, u32 icnt, u32 ioff)
{
}

void* kgmGCNone::gcGenShader(kgmArray<u8>& vsrc, kgmArray<u8>& fsrc)
{
  return null;
}

void kgmGCNone::gcFreeShader(void* s)
{
}

void kgmGCNone::gcSetShader(void* s)
{
}

void  kgmGCNone::gcBindAttribute(void* s, int i, const char* attr)
{
}

void kgmGCNone::gcUniform(void* s, u32 type, u32 cnt, const char* par, void* val)
{
}

void kgmGCNone::gcUniformMatrix(void* s, u32 type, u32 cnt, u32 trn, const char* par, void* val)
{
}

void kgmGCNone::gcUniformSampler(void* s, const char* par, void* val)
{
}

#ifdef DEBUG
void kgmGCNone::gcGetUniform(void* s, const char* par, void* val)
{
}
#endif


