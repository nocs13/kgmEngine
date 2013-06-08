#include "kgmMaterial.h"
#include "kgmTexture.h"
#include "../kgmBase/kgmIGraphics.h"

KGMOBJECT_IMPLEMENT(kgmMaterial, kgmResource);

kgmMaterial::kgmMaterial(){
  m_flags = MF_Textures | MF_Depth | MF_Color;

  m_color        = Color(1.0, 1.0, 1.0, 1.0);
  m_specular     = Color(1.0, 1.0, 1.0, 1.0);
  m_alpha        = 1.0;
  m_shininess    = 1.0;
  m_transparency = 0.0;

  m_2side        = false;

  m_tex_color    = m_tex_specular = m_tex_normal = 0;

  m_shader       = ShaderNone;
}

kgmMaterial::~kgmMaterial(){
}

kgmMaterial* kgmMaterial::clone(){
  kgmMaterial* m = new kgmMaterial;
  m->m_color   = m_color;
  m->m_specular  = m_specular;
  m->m_shininess = m_shininess;
  m->m_transparency = m_transparency;
  m->m_shader = m_shader;

  m->m_gc = m_gc;
  m->m_id = m_id;
  return m;
}

void kgmMaterial::start(kgmIGraphics* g){
 m_gc = g;
}

void kgmMaterial::stop(){
}

