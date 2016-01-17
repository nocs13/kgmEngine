#include "kgmMaterial.h"
#include "kgmTexture.h"
#include "../kgmBase/kgmIGC.h"

KGMOBJECT_IMPLEMENT(kgmMaterial, kgmObject);

kgmMaterial::kgmMaterial()
{
  m_flags = MF_Textures | MF_Depth | MF_Color;

  m_color        = Color(1.0f, 1.0f, 1.0f, 1.0f);
  m_specular     = Color(1.0f, 1.0f, 1.0f, 1.0f);
  m_shininess    = 0.0;

  m_alpha  = false;
  m_blend  = false;
  m_depth  = true;
  m_cull   = true;
}

kgmMaterial::~kgmMaterial()
{
}

kgmMaterial* kgmMaterial::clone()
{
  kgmMaterial* m = new kgmMaterial;

  m->m_color     = m_color;
  m->m_emision   = m_emision;
  m->m_specular  = m_specular;

  m->m_shininess    = m_shininess;

  m->m_shader = m_shader;

  m->m_gc = m_gc;

  return m;
}

void kgmMaterial::setTexColor(kgmTexture* t)
{
  if(!t || !t->m_texture)
    return;

  m_tex_color = t;
}

void kgmMaterial::setTexNormal(kgmTexture* t)
{
  if(!t || !t->m_texture)
    return;

  m_tex_normal = t;
}

void kgmMaterial::setTexSpecular(kgmTexture* t)
{
  if(!t || !t->m_texture)
    return;

  m_tex_specular = t;
}

bool kgmMaterial::hasTexColor()
{
  return (m_tex_color && m_tex_color->m_texture);
}

bool kgmMaterial::hasTexNormal()
{
  return (m_tex_normal && m_tex_normal->m_texture);
}

bool kgmMaterial::hasTexSpecular()
{
  return (m_tex_specular && m_tex_specular->m_texture);
}

kgmTexture* kgmMaterial::getTexColor()
{
  return m_tex_color;
}

kgmTexture* kgmMaterial::getTexNormal()
{
  return m_tex_normal;
}

kgmTexture* kgmMaterial::getTexSpecular()
{
  return m_tex_specular;
}

void kgmMaterial::setShader(kgmShader* shader)
{
  m_shader = shader;
}
