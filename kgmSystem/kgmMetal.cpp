#include "kgmMetal.h"

#ifdef DARWIN

extern void* __kgmGetMainWindow();
extern void  __kgmInitMetal(void*);

kgmMetal::kgmMetal()
{
}

kgmMetal::kgmMetal(kgmWindow *wnd)
{
  //void* w = __kgmGetMainWindow();

  //__kgmInitMetal(w);
}

kgmMetal::~kgmMetal()
{
}

void kgmMetal::gcSet(u32 param, void* value)
{
}

void kgmMetal::gcGet(u32 param, void* value)
{
}

u32 kgmMetal::gcError()
{
  return 0;
}

void kgmMetal::gcClear(u32 flag, u32 col, float depth, u32 sten)
{
}

void kgmMetal::gcBegin()
{
}

void kgmMetal::gcEnd()
{
}

void kgmMetal::gcRender()
{
}

void kgmMetal::gcResize(u32 width, u32 height)
{

}

void kgmMetal::gcSetViewport(int x, int y, int w, int h, float n, float f)
{
}

void  kgmMetal::gcBlend(bool en, u32 eq, u32 fnsrc, u32 fndst)
{
}

void kgmMetal::gcCull(u32 mode)
{
}

void kgmMetal::gcDepth(bool depth, bool mask, u32 mode)
{
}

void  kgmMetal::gcScissor(bool on, int x, int y, int w, int h)
{
}

gchandle kgmMetal::gcGenTexture(void *pd, u32 w, u32 h, u32 fmt, u32 type)
{
  return null;
}

void kgmMetal::gcFreeTexture(gchandle t)
{
}

void kgmMetal::gcSetTexture(u32 stage, gchandle t)
{
}

gchandle kgmMetal::gcGenTarget(u32 w, u32 h, bool d, bool s)
{
  return (gchandle) null;
}

bool kgmMetal::gcTexTarget(gchandle tar, gchandle tex, u32 type)
{
  return true;
}

void kgmMetal::gcFreeTarget(gchandle t)
{
}

void kgmMetal::gcSetTarget(gchandle t)
{
}

void kgmMetal::gcClipPlane(bool en, u32 id, f64 plane[4])
{
}

void kgmMetal::gcStencil(bool en, u32 func, u32 mask, u32 ref,
			 u32 fail, u32 zfail, u32 zpass)
{
}

void kgmMetal::gcDraw(u32 pmt, u32 v_fmt, u32 v_size, u32 v_cnt, void *v_pnt,
		      u32 i_size, u32 i_cnt, void *i_pnt)
{
}

void* kgmMetal::gcGenVertexBuffer(void* vdata, u32 vsize, void* idata, u32 isize)
{
  return null;
}

void  kgmMetal::gcFreeVertexBuffer(void* b)
{
}

void  kgmMetal::gcDrawVertexBuffer(void* b, u32 pmt, u32 vfmt, u32 vsize, u32 vcnt,
				   u32 isize, u32 icnt, u32 ioff)
{
}

void* kgmMetal::gcGenShader(kgmArray<u8>& vsrc, kgmArray<u8>& fsrc)
{
  return null;
}

void kgmMetal::gcFreeShader(void* s)
{
}

void kgmMetal::gcSetShader(void* s)
{
}

void  kgmMetal::gcBindAttribute(void* s, int i, const char* attr)
{
}

void kgmMetal::gcUniform(void* s, u32 type, u32 cnt, const char* par, void* val)
{
}

void kgmMetal::gcUniformMatrix(void* s, u32 type, u32 cnt, u32 trn, const char* par, void* val)
{
}

void kgmMetal::gcUniformSampler(void* s, const char* par, void* val)
{
}

#ifdef DEBUG
void kgmMetal::gcGetUniform(void* s, const char* par, void* val)
{
}
#endif

#endif
