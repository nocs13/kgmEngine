#pragma once
#include "../kgmBase/kgmList.h"
#include "kgm4Color.h"
#include "kgmShader.h"

class kgmIGC;
class kgmTexture;
class kgmShader;

#define KGM_TEXTURES_PER_MATERIAL 8

class kgmMaterial: public kgmResource
{
  KGM_OBJECT(kgmMaterial);
protected:
  kgmIGC*  m_gc;

public:
  enum Flags
  {
    MF_Color     = 1 << 0L,
    MF_Depth     = 1 << 1L,
    MF_Stencil   = 1 << 2L,
    MF_Blend     = 1 << 3L,
    MF_Alpha     = 1 << 4L,
    MF_Cull      = 1 << 5L,
    MF_Textures  = 1 << 6L,
  };

  class Color
  {
  public:
    union
    {
      struct { float r, g, b, a; };
      float c[4];
    };

    Color()
    {
      r = g = b = a = 1.0;
    }

    Color(float rx, float gx, float bx, float ax)
    {
      r = rx, g = gx, b = bx, a = ax;
    }

    void get(uchar &r, uchar &g, uchar &b, uchar &a)
    {
      r = 255.0 * this->r;
      g = 255.0 * this->g;
      b = 255.0 * this->b;
      a = 255.0 * this->a;
    }
  };

protected:
  kgmTexture *m_tex_color, *m_tex_normal, *m_tex_specular;

public:
  Color  m_color, m_specular, m_emision;
  float  m_shininess, m_transparency;


  kgmShader::Shader m_shader;

  u32         m_flags;            //render specisific flags enable/disable

  bool        m_alpha;
  bool        m_2side;
  bool        m_blend;
  bool        m_depth;
  bool        m_light;

  u32         m_srcblend, m_dstblend;

  kgmString   m_type;

public:
  kgmMaterial();
  virtual ~kgmMaterial();

  kgmMaterial* clone();

  void setTexColor(kgmTexture* t);
  void setTexNormal(kgmTexture* t);
  void setTexSpecular(kgmTexture* t);

  bool hasTexColor();
  bool hasTexNormal();
  bool hasTexSpecular();

  kgmTexture* getTexColor();
  kgmTexture* getTexNormal();
  kgmTexture* getTexSpecular();

  void setShader(kgmShader::Shader shader);
  kgmShader::Shader getShader() const
  {
    return m_shader;
  }

  void light(bool l = true)
  {
    m_light = l;
  }
  
  bool light() const
  {
    return m_light;
  }
};

typedef kgmList<kgmMaterial*> kgmMaterialList;
