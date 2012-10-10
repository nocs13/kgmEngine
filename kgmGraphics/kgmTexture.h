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
 uint m_stage;
 uint m_type;
 uint m_flags;

 uint m_bpp;
 uint m_width;
 uint m_height;

 uint m_mag_filter;
 uint m_min_filter;
 uint m_wrap_s;
 uint m_wrap_t;
 uint m_border;
 uint m_priority;

 uint m_environment;

 uint m_cmode_s;
 uint m_const_s;
 vec4 m_param_s;
 uint m_cmode_t;
 uint m_const_t;
 vec4 m_param_t;
 uint m_cmode_r;
 uint m_const_r;
 vec4 m_param_r;
 uint m_cmode_q;
 uint m_const_q;
 vec4 m_param_q;

 mtx4 m_transform;

public:
 kgmTexture();
 kgmTexture(kgmIGraphics*, uint type);
 ~kgmTexture();

 void start(uint);
 void stop();

 void setFlags(uint f);
 void addFlags(uint f);
};
