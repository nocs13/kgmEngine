#pragma once
#include "../kgmBase/kgmList.h"
#include "../kgmBase/kgmPointer.h"
#include "kgmColor.h"
#include "kgmShader.h"

class kgmIGC;
class kgmTexture;
class kgmShader;

#define KGM_TEXTURES_PER_MATERIAL 4

class kgmMaterial: public kgmResource
{
  KGM_OBJECT(kgmMaterial)

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

  enum Blend
  {
    Blend_None,
    Blend_Add,
    Blend_Mul,
    Blend_Sub,
    Blend_Inter,
    Blend_CBurn,
    Blend_LBurn,
    Blend_CDodge,
    Blend_LDodge,
    Blend_Screen,
    Blend_Darken,
    Blend_Lighten,
  };

  enum Type
  {
    TypeBase,
    TypePhong,
    TypeLambert,
    TypeRefract
  };

  enum Environment
  {
    EnvironmentTypeNone,
    EnvironmentTypeImage,
    EnvironmentTypeStatic,
    EnvironmentTypeAnimate,

    EnvironmentMappingCube = 0,
    EnvironmentMappingPlane,
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

    Color(u32 c)
    {
      r = ((c & 0x000000ff))       / 255.0;
      g = ((c & 0x0000ff00) >> 8)  / 255.0;
      b = ((c & 0x00ff0000) >> 16) / 255.0;
      a = ((c & 0xff000000) >> 24) / 255.0;
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
  bool  m_depth = true;

  bool m_shading       = true;
  bool m_shade_cast    = false;
  bool m_shade_receive = false;

  Type  m_type  = TypeBase;
  Blend m_blend = Blend_None;

  f32  m_distortion;
  f32  m_shininess;
  f32  m_fresnel;

  kgmTexture* m_tex_color = null;
  kgmTexture* m_tex_normal = null;
  kgmTexture* m_tex_specular = null;
  kgmTexture* m_tex_environment = null;

  Environment m_env_type = EnvironmentTypeNone;
  Environment m_env_mapping = EnvironmentMappingPlane;
  vec3        m_env_viewpoint = vec3(0, 0, 0);
  f32         m_env_intensity = 1.0;

  vec3        m_move;

public:
  Color  m_color, m_specular;

  u32  m_flags;

public:
  kgmMaterial();

  kgmMaterial(const kgmMaterial&);

  ~kgmMaterial();

  void setTexColor(kgmTexture* t);
  void setTexNormal(kgmTexture* t);
  void setTexSpecular(kgmTexture* t);
  void setTexEnvironment(kgmTexture* t);

  bool hasTexColor();
  bool hasTexNormal();
  bool hasTexSpecular();
  bool hasTexEnvironment();

  kgmTexture* getTexColor();
  kgmTexture* getTexNormal();
  kgmTexture* getTexSpecular();
  kgmTexture* getTexEnvironment();

  void setShader(kgmShader* shader);

  static u32 toRgba(float x, float y, float z)
  {
    return ( ((u8)(255. * x) << 0)  | ((u8)(255. * y) << 8) |
             ((u8)(255. * z) << 16) | ((u8)(255.)     << 24) );
  }

  static u32 toRgba(float x, float y, float z, float w)
  {
    return ( ((u8)(255 * x) << 0)  | ((u8)(255 * y) << 8) |
             ((u8)(255 * z) << 16) | ((u8)(255 * w) << 24) );
  }

  static vec4 toVector(u32 c)
  {
    vec4 v;

    v.x = ((c & 0x000000ff))       / 255.0;
    v.y = ((c & 0x0000ff00) >> 8)  / 255.0;
    v.z = ((c & 0x00ff0000) >> 16) / 255.0;
    v.w = ((c & 0xff000000) >> 24) / 255.0;

    return v;
  }

  Type type() const
  {
    return m_type;
  }

  void type(Type t)
  {
    m_type = t;
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

  f32 distortion() const
  {
    return m_distortion;
  }

  void distortion(f32 s)
  {
    m_distortion = s;
  }

  f32 shininess() const
  {
    return m_shininess;
  }

  void shininess(f32 s)
  {
    m_shininess = s;
  }

  f32 fresnel() const
  {
    return m_fresnel;
  }

  void fresnel(f32 f)
  {
    m_fresnel = f;
  }

  bool depth() const
  {
    return m_depth;
  }

  void depth(bool d)
  {
    m_depth = d;
  }

  Blend blend() const
  {
    return m_blend;
  }

  void blend(Blend b)
  {
    m_blend = b;
  }

  bool shading() const
  {
    return m_shading;
  }

  void shading(bool s)
  {
    m_shading = s;
  }

  bool shade_cast() const
  {
    return m_shade_cast;
  }

  void shade_cast(bool s)
  {
    m_shade_cast = s;
  }

  bool shade_receive() const
  {
    return m_shade_receive;
  }

  void shade_receive(bool s)
  {
    m_shade_receive = s;
  }

  bool cull() const
  {
    return m_cull;
  }

  void cull(bool c)
  {
    m_cull = c;
  }

  Environment envType() const
  {
    return m_env_type;
  }

  void envType(Environment e)
  {
    m_env_type = e;
  }

  Environment envMapping() const
  {
    return m_env_mapping;
  }

  void envMapping(Environment e)
  {
    m_env_mapping = e;
  }

  vec3 envViewpoint() const
  {
    return m_env_viewpoint;
  }

  void envViewpoint(vec3 v)
  {
    m_env_viewpoint = v;
  }

  f32 envIntensity() const
  {
    return m_env_intensity;
  }

  void envIntensity(f32 i)
  {
    m_env_intensity = i;
  }

  vec3 move() const
  {
    return m_move;
  }

  void move(vec3 v)
  {
    m_move = v;
  }

  vec4 color()
  {
    vec4 col = m_color.get();

    return col;
  }

  vec4 specular()
  {
    vec4 col = m_specular.get();

    return col;
  }

  static kgmString blendToString(Blend Blend);
  static Blend     stringToBlend(kgmString blend);
  static kgmString typeToString(Type type);
  static Type      stringToType(kgmString type);
};

typedef kgmList<kgmMaterial*> kgmMaterialList;
