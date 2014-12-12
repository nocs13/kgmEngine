#include "kgmMaterial.h"
#include "kgmTexture.h"
#include "../kgmBase/kgmIGC.h"

KGMOBJECT_IMPLEMENT(kgmMaterial, kgmResource);

kgmMaterial::kgmMaterial()
{
  m_flags = MF_Textures | MF_Depth | MF_Color;

  m_color        = Color(1.0, 1.0, 1.0, 1.0);
  m_specular     = Color(1.0, 1.0, 1.0, 1.0);
  m_shininess    = 1.0;
  m_transparency = 0.0;

  m_alpha        = false;
  m_2side        = false;
  m_blend        = false;
  m_depth        = true;

  m_tex_color    = m_tex_specular = m_tex_normal = 0;

  m_shader       = null;
}

kgmMaterial::~kgmMaterial()
{
  if(m_tex_color)
    m_tex_color->release();

  if(m_tex_normal)
    m_tex_normal->release();

  if(m_tex_specular)
    m_tex_specular->release();

  if(m_shader)
    m_shader->release();
}

kgmMaterial* kgmMaterial::clone()
{
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

void kgmMaterial::setTexColor(kgmTexture* t)
{
  if(!t || !t->m_texture)
    return;

  if(m_tex_color)
    m_tex_color->release();

  m_tex_color = t;

  t->increment();
}

void kgmMaterial::setTexNormal(kgmTexture* t)
{
  if(!t || !t->m_texture)
    return;

  if(m_tex_normal)
    m_tex_normal->release();

  m_tex_normal = t;

  t->increment();
}

void kgmMaterial::setTexSpecular(kgmTexture* t)
{
  if(!t || !t->m_texture)
    return;

  if(m_tex_specular)
    m_tex_specular->release();

  m_tex_specular = t;

  t->increment();
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

void kgmMaterial::setShader(Shader shader)
{
  if(shader == null)
    return;

  if(m_shader)
    m_shader->release();

  m_shader = shader;

  m_shader->increment();
}
