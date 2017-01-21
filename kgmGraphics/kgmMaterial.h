#pragma once
#include "../kgmBase/kgmList.h"
#include "../kgmBase/kgmPointer.h"
#include "kgm4Color.h"
#include "kgmShader.h"

class kgmIGC;
class kgmTexture;
class kgmShader;

#define KGM_TEXTURES_PER_MATERIAL 4

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

    Color(u32 rx, u32 gx, u32 bx, u32 ax)
    {
      if(rx > 255) rx = 255;
      if(gx > 255) gx = 255;
      if(bx > 255) bx = 255;
      if(ax > 255) ax = 255;

      r = rx / 255.0;
      g = gx / 255.0;
      b = bx / 255.0;
      a = ax / 255.0;
    }

    vec4 get()
    {
      return vec4(r, g, b, a);
    }

    void get(uchar &r, uchar &g, uchar &b, uchar &a)
    {
      r = 255.0 * this->r;
      g = 255.0 * this->g;
      b = 255.0 * this->b;
      a = 255.0 * this->a;
    }

    void set(vec4 v)
    {
      r = v.x; g = v.y; b = v.z; a = v.w;
    }

    void set(uchar r, uchar g, uchar b, uchar a)
    {
      this->r = (float)(r / 255.0);
      this->g = (float)(g / 255.0);
      this->b = (float)(b / 255.0);
      this->a = (float)(a / 255.0);
    }

    u32 getRGBA()
    {
      return (u32)(r * 255) || ((u32)(g * 255) << 8) ||
             ((u32)(b * 255) << 16) || ((u32)(a * 255) << 24);
    }

    void setRGBA(u32 c)
    {
      r = ((c & 0x000000ff))       / 255.0;
      g = ((c & 0x0000ff00) >> 8)  / 255.0;
      b = ((c & 0x00ff0000) >> 16) / 255.0;
      a = ((c & 0xff000000) >> 24) / 255.0;
    }
  };

private:
  bool  m_cull  = true;
  bool  m_alpha = false;
  bool  m_blend = false;
  bool  m_depth = true;
  bool  m_shade = true;

  u32  m_srcblend;
  u32  m_dstblend;

  kgmTexture* m_tex_color = null;
  kgmTexture* m_tex_normal = null;
  kgmTexture* m_tex_specular = null;

  float  m_shininess;

public:
  Color  m_color, m_specular;

  kgmShader*  m_shader = null;

  u32  m_type;
  u32  m_flags;

  kgmString  m_name;

public:
  kgmMaterial();

  kgmMaterial(const kgmMaterial&);

  ~kgmMaterial();

  void setTexColor(kgmTexture* t);
  void setTexNormal(kgmTexture* t);
  void setTexSpecular(kgmTexture* t);

  bool hasTexColor();
  bool hasTexNormal();
  bool hasTexSpecular();

  kgmTexture* getTexColor();
  kgmTexture* getTexNormal();
  kgmTexture* getTexSpecular();

  void setShader(kgmShader* shader);

  kgmShader* getShader() const
  {
    return m_shader;
  }

  float transparency() const
  {
    return 1.0f - m_color.a;
  }

  void transparency(float t)
  {
    if (t > 1.0f)
      t = 1.0f;

    m_color.a = 1.0f - t;
  }

  float shininess() const
  {
    return m_shininess;
  }

  void shininess(float s)
  {
    m_shininess = s;
  }

  bool alpha() const
  {
    return (m_alpha || (m_color.a < 1.0f));
  }

  void alpha(bool a)
  {
    if(m_alpha == a)
      return;

    if(!a && m_color.a < 1.0)
      return;

    m_alpha = a;
  }

  bool depth() const
  {
    return m_depth;
  }

  void depth(bool d)
  {
    m_depth = d;
  }

  bool blend() const
  {
    return m_blend;
  }

  void blend(bool b)
  {
    m_blend = b;
  }

  bool shade() const
  {
    return m_shade;
  }

  void shade(bool s)
  {
    m_shade = s;
  }

  bool cull() const
  {
    return m_cull;
  }

  void cull(bool c)
  {
    m_cull = c;
  }

  u32 srcblend() const
  {
    return m_srcblend;
  }

  void srcblend(u32 s)
  {
    m_srcblend = s;
  }

  u32 dstblend() const
  {
    return m_dstblend;
  }

  void dstblend(u32 s)
  {
    m_dstblend = s;
  }

  kgmString name() const
  {
    return m_name;
  }

  void name(kgmString n)
  {
    m_name = n;
  }

  static kgmString blendToString(u32 blend);
  static u32 stringToBlend(kgmString blend);
};

typedef kgmList<kgmMaterial*> kgmMaterialList;
