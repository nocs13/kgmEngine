#include "kgmMaterial.h"
#include "kgmTexture.h"
#include "../kgmBase/kgmIGC.h"

kgmMaterial::kgmMaterial()
{
  m_flags = MF_Textures | MF_Depth | MF_Color;

  m_color        = Color(1.0f, 1.0f, 1.0f, 1.0f);

  m_distortion = 0.0;
  m_shininess  = 0.0;
  m_specular    = 0.0;

  m_depth  = true;

  m_cull   = CullBack;

  m_move = vec3(0, 0, 0);

  m_blend  = BlendNone;
}

kgmMaterial::kgmMaterial(const kgmMaterial& mtl)
{
  m_color     = mtl.m_color;
  m_shininess = mtl.m_shininess;

  m_gc = mtl.m_gc;

  if(mtl.m_tex_color)
    mtl.m_tex_color->assign((kgmObject**) &m_tex_color);

  if(mtl.m_tex_normal)
    mtl.m_tex_normal->assign((kgmObject**) &m_tex_normal);

  if(mtl.m_tex_specular)
    mtl.m_tex_specular->assign((kgmObject**) &m_tex_specular);
}

kgmMaterial::~kgmMaterial()
{
  if(m_tex_color)
    m_tex_color->release();

  if(m_tex_normal)
    m_tex_normal->release();

  if(m_tex_specular)
    m_tex_specular->release();
}

void kgmMaterial::setTexColor(kgmTexture* t)
{
  m_tex_color = t;
}

void kgmMaterial::setTexNormal(kgmTexture* t)
{
  m_tex_normal = t;
}

void kgmMaterial::setTexSpecular(kgmTexture* t)
{
  m_tex_specular = t;
}

/*void kgmMaterial::setTexEnvironment(kgmTexture* t)
{
  m_tex_environment = t;
}*/

bool kgmMaterial::hasTexColor()
{
  return (m_tex_color && m_tex_color->texture());
}

bool kgmMaterial::hasTexNormal()
{
  return (m_tex_normal && m_tex_normal->texture());
}

bool kgmMaterial::hasTexSpecular()
{
  return (m_tex_specular && m_tex_specular->texture());
}

/*bool kgmMaterial::hasTexEnvironment()
{
  return (m_tex_environment && m_tex_environment->texture());
}*/

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

/*kgmTexture* kgmMaterial::getTexEnvironment()
{
  return m_tex_environment;
}*/

kgmString kgmMaterial::blendToString(Blend blend)
{
  switch(blend)
  {
  case BlendNone:
    return "none";
  case BlendMix:
    return "mix";
  case BlendAdd:
    return "add";
  case BlendMul:
    return "mul";
  case BlendSub:
    return "sub";
  /*case Blend_Inter:
    return "inter";
  case Blend_CBurn:
    return "cburn";
  case Blend_LBurn:
    return "lburn";
  case Blend_CDodge:
    return "cdodge";
  case Blend_LDodge:
    return "ldodge";
  case Blend_Screen:
    return "screen";
  case Blend_Darken:
    return "darken";
  case Blend_Lighten:
    return "lighten";*/
  }

  return "";
}

kgmMaterial::Blend kgmMaterial::stringToBlend(kgmString blend)
{
  if(blend == "none")
    return BlendNone;
  else if(blend == "mix")
    return BlendMix;
  else if(blend == "add")
    return BlendAdd;
  else if(blend == "mul")
    return BlendMul;
  else if(blend == "sub")
    return BlendSub;
  /*else if(blend == "inter")
    return Blend_Inter;
  else if(blend == "cburn")
    return Blend_CBurn;
  else if(blend == "lburn")
    return Blend_LBurn;
  else if(blend == "cdodge")
    return Blend_CDodge;
  else if(blend == "ldodge")
    return Blend_LDodge;
  else if(blend == "screen")
    return Blend_Screen;
  else if(blend == "darken")
    return Blend_Darken;
  else if(blend == "lighten")
    return Blend_Lighten;*/

  return BlendNone;
}

kgmString kgmMaterial::typeToString(kgmMaterial::Type type)
{
#define s_text(x) if(x == type) return #x
  s_text(TypeBase);
  s_text(TypePhong);
  s_text(TypeRefract);
#undef s_text

  return "TypeNone";
}

kgmMaterial::Type kgmMaterial::stringToType(kgmString str)
{
  if (str.length() < 1)
    return TypeBase;

#define s_type(x) if(strcmp(str, #x) == 0) return x
  s_type(TypeBase);
  s_type(TypePhong);
  s_type(TypeRefract);
#undef s_type

  return TypeBase;
}

