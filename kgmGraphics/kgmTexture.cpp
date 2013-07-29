#include "kgmTexture.h"
#include "../kgmBase/kgmIGraphics.h"

KGMOBJECT_IMPLEMENT(kgmTexture, kgmResource);

kgmTexture::kgmTexture(){
 m_texture = 0;
 m_type = Type_Color;
 m_flags = 0;
 m_gc = 0;
}

kgmTexture::kgmTexture(kgmIGraphics* gc, u32 type){
 m_texture = 0;
 m_type = type;
 m_flags = 0;
 m_gc = gc;
}

kgmTexture::~kgmTexture(){
}

void kgmTexture::setFlags(u32 f){
 m_flags = f;
}

void kgmTexture::addFlags(u32 f){
 m_flags |= f;
}
