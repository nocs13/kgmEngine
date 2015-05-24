#include "kgmTexture.h"
#include "../kgmBase/kgmIGC.h"

KGMOBJECT_IMPLEMENT(kgmTexture, kgmResource);

kgmTexture::kgmTexture(){
  m_texture = 0;
  m_type = Type_Color;
  m_flags = 0;
  m_gc = 0;
}

kgmTexture::kgmTexture(kgmIGC* gc, u32 type)
{
  m_texture = 0;
  m_type = type;
  m_flags = 0;
  m_gc = gc;
}

kgmTexture::~kgmTexture()
{
//  if(m_gc && m_texture)
//    m_gc->gcFreeTexture(m_texture);
}

void kgmTexture::setFlags(u32 f)
{
  m_flags = f;
}

void kgmTexture::addFlags(u32 f)
{
  m_flags |= f;
}
