#pragma once
#include "../kgmMath/kgmMath.h"
#include "../kgmBase/kgmResource.h"

class kgmIGC;

class kgmTexture: public kgmResource
{
  KGM_OBJECT(kgmTexture)
  public:
    enum Type
  {
    Type_Color,
    Type_Depth,
    Type_CubeMap,

    Type_RT_Color,      // RenderTexture
    Type_RT_Color8,     // RenderTexture
    Type_RT_Color16,    // RenderTexture
    Type_RT_Color24,    // RenderTexture
    Type_RT_Color32,    // RenderTexture
    Type_RT_Depth,      // RenderTexture
    Type_RT_Depth8,     // RenderTexture
    Type_RT_Depth16,    // RenderTexture
    Type_RT_Depth24,    // RenderTexture
    Type_RT_Depth32,    // RenderTexture
    Type_RT_Stencil,    // RenderTexture
  };

  enum Flags
  {
    min_filter	= 1 << 0L,
    mag_filter	= 1 << 1L,
    wrap_s		= 1 << 2L,
    wrap_t		= 1 << 3L,
    border		= 1 << 4L,
    priority		= 1 << 5L,

    env_mode		= 1 << 6L,
    env_color		= 1 << 7L,

    coord_s		= 1 << 8L,
    coord_t		= 1 << 9L,
    coord_r		= 1 << 10L,
    coord_q		= 1 << 11L,

    transform		= 1 << 13L,
  };

  enum CoordMode
  {
    gen_mode	= 0,
    eye_plane,
    obj_plane,
  };

public:
  void* m_texture = null;

  u32 m_stage = 0;
  u32 m_type = 0;
  u32 m_flags = 0;

  u32 m_bpp = 0;
  u32 m_width = 0;
  u32 m_height = 0;

  u32 m_mag_filter = 0;
  u32 m_min_filter = 0;
  u32 m_wrap_s = 0;
  u32 m_wrap_t = 0;
  u32 m_border = 0;
  u32 m_priority = 0;

  u32 m_environment = 0;

  u32 m_cmode_s = 0;
  u32 m_const_s = 0;
  vec4 m_param_s = {0,0,0,0};
  u32 m_cmode_t = 0;
  u32 m_const_t = 0;
  vec4 m_param_t = {0,0,0,0};
  u32 m_cmode_r = 0;
  u32 m_const_r = 0;
  vec4 m_param_r = {0,0,0,0};
  u32 m_cmode_q = 0;
  u32 m_const_q = 0;
  vec4 m_param_q = {0,0,0,0};

  mtx4 m_transform;

public:
  kgmTexture();
  kgmTexture(kgmIGC*, u32 type);
  ~kgmTexture();

  void setFlags(u32 f);
  void addFlags(u32 f);
};
