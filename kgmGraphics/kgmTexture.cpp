#include "kgmTexture.h"
#include "../kgmMain/kgmIGraphics.h"

KGMOBJECT_IMPLEMENT(kgmTexture,	kgmResource);

kgmTexture::kgmTexture(){
 m_texture = 0;
 m_type = Type_Color;
 m_flags = 0;
 m_gc = 0;
}

kgmTexture::kgmTexture(kgmIGraphics* gc, uint type){
 m_texture = 0;
 m_type = type;
 m_flags = 0;
 m_gc = gc;
}

kgmTexture::~kgmTexture(){
 if(m_gc)
  m_gc->gcFreeTexture(m_texture);
}

void kgmTexture::start(uint s){
 m_stage = s;
 m_gc->gcSetTexture(m_stage, m_texture);
}

void kgmTexture::stop(){
 m_gc->gcSetTexture(m_stage, 0);
}

void kgmTexture::setFlags(uint f){
 m_flags = f;
}

void kgmTexture::addFlags(uint f){
 m_flags |= f;
}
