#pragma once
#include "../kgmMath/kgmMath.h"
#include "../kgmBase/kgmResource.h"

class kgmIGraphics;

class kgmTexture: public kgmResource{
 KGM_OBJECT(kgmTexture)
public:
 enum Type{
  Type_Color,
  Type_Depth,
  Type_CubeMap,

  Type_RT_Color,		//RenderTexture
  Type_RT_Color8,		//RenderTexture
  Type_RT_Color16,		//RenderTexture
  Type_RT_Color24,		//RenderTexture
  Type_RT_Color32,		//RenderTexture
  Type_RT_Depth,		//RenderTexture
  Type_RT_Depth8,		//RenderTexture
  Type_RT_Depth16,		//RenderTexture
  Type_RT_Depth24,		//RenderTexture
  Type_RT_Depth32,		//RenderTexture
  Type_RT_Stencil,		//RenderTexture
 };
 enum Flags{
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
 enum CoordMode{
  gen_mode	= 0,
  eye_plane,
  obj_plane,
 };

protected:
 kgmIGraphics* m_gc;

public:
 void* m_texture;
 u32 m_stage;
 u32 m_type;
 u32 m_flags;

 u32 m_bpp;
 u32 m_width;
 u32 m_height;

 u32 m_mag_filter;
 u32 m_min_filter;
 u32 m_wrap_s;
 u32 m_wrap_t;
 u32 m_border;
 u32 m_priority;

 u32 m_environment;

 u32 m_cmode_s;
 u32 m_const_s;
 vec4 m_param_s;
 u32 m_cmode_t;
 u32 m_const_t;
 vec4 m_param_t;
 u32 m_cmode_r;
 u32 m_const_r;
 vec4 m_param_r;
 u32 m_cmode_q;
 u32 m_const_q;
 vec4 m_param_q;

 mtx4 m_transform;

public:
 kgmTexture();
 kgmTexture(kgmIGraphics*, u32 type);
 ~kgmTexture();

 void setFlags(u32 f);
 void addFlags(u32 f);
};
