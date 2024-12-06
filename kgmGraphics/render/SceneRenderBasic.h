#include "BaseRender.h"

class SceneRenderBasic: public BaseRender
{

  struct LightData
  {
    vec4 pos_pow[MAX_LIGHTS];
    vec4 dir_ang[MAX_LIGHTS];
    vec4 col_per[MAX_LIGHTS];

    u32 count;
  };

  mtx4 m_g_mtx_world;
  mtx4 m_g_mtx_proj;
  mtx4 m_g_mtx_view;
  mtx3 m_g_mtx_normal;
  mtx4 m_g_mtx_orto;
  mtx4 m_g_mtx_iden;

  s32 m_a_meshes_count    = 0;
  s32 m_a_bmeshes_count   = 0;
  s32 m_a_particles_count = 0;

  iRect      m_viewport;
  kgmCamera* m_camera = null;

  LightData m_light_data;

  public:
  SceneRenderBasic(kgmGraphics* gr);
  ~SceneRenderBasic();

  virtual void preRender();
  virtual void Render();
  virtual void postRender();

  void setCamera(kgmCamera* c)
  {
    m_camera = c;
  }

  void setViewport(s32 x, s32 y, s32 w, s32 h)
  {
    m_viewport = iRect(x, y, w, h);
  }
};
