#pragma once
#include "../kgmBase/kgmObject.h"
#include "../kgmBase/kgmList.h"

#include "kgmTexture.h"
#include "kgmIcon.h"

class kgmLight: public kgmObject
{
  KGM_OBJECT(kgmLight);

public:
    static float LIGHT_RANGE;

    enum Type
    {
      TypePoint,
      TypeSpot,
      TypeSun
    };

private:
  Type  m_type;

  vec3  m_direction;
  vec3  m_color;

  float m_intensity;
  float m_angle;

  bool  m_active;
  bool  m_shadows;

  kgmString m_id;
  u32       m_group;

public:
  kgmLight()
  {
    m_type = TypePoint;

    m_direction = vec3(0, 0, 0);

    m_color = vec3(1, 1, 1);

    m_intensity = 1.0f;
    m_angle     = 0.0f;

    m_shadows = false;
    m_active  = true;

    m_group = 0;
  }

  kgmLight(const kgmLight& light)
  {
    m_type = light.m_type;

    m_direction = light.m_direction;

    m_color     = light.m_color;

    m_intensity = light.m_intensity;
    m_angle     = light.m_angle;

    m_shadows   = light.m_shadows;
    m_active    = light.m_active;

    m_group   = light.m_group;
  }

  ~kgmLight()
  {
  }

  vec3 direction() const
  {
    return m_direction;
  }

  void direction(vec3 v)
  {
    m_direction = v;
  }

  void angle(f32 a)
  {
    m_angle = a;
  }

  f32 angle() const
  {
    return m_angle;
  }

  f32 intensity() const
  {
    return m_intensity;
  }

  void intensity(f32 i)
  {
    m_intensity = i;
  }

  vec3 color() const
  {
    return m_color;
  }

  void color(vec3 c)
  {
    m_color = c;
  }

  bool shadows() const
  {
    return m_shadows;
  }

  void shadows(bool s)
  {
    m_shadows = s;
  }
};

typedef kgmList<kgmLight*> kgmLights;



