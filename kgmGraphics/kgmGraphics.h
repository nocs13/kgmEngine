#pragma once
#include "../kgmBase/kgmIGC.h"
#include "../kgmBase/kgmMap.h"
#include "../kgmBase/kgmTab.h"
#include "../kgmBase/kgmIResources.h"

#include "../kgmSystem/kgmWindow.h"

#include "kgmGui.h"
#include "kgmGuiTab.h"
#include "kgmGuiList.h"
#include "kgmGuiText.h"
#include "kgmGuiButton.h"
#include "kgmGuiScroll.h"
#include "kgmGuiProgress.h"
#include "kgmGuiStyle.h"
#include "kgmIGraphics.h"

#include "kgmMesh.h"
#include "kgmText.h"
#include "kgmIcon.h"
#include "kgmLight.h"
#include "kgmSprite.h"
#include "kgmCamera.h"
#include "kgmMaterial.h"

#include "kgmNode.h"

#include "render/Render.h"
#include "render/FpsRender.h"
#include "render/GuiRender.h"
#include "render/IconRender.h"
#include "render/LineRender.h"
#include "render/ColorRender.h"
#include "render/PhongRender.h"
#include "render/SpriteRender.h"
#include "render/ShadowRender.h"
#include "render/LightmapRender.h"
#include "render/EnvironmentRender.h"
#include "render/Terrain.h"

#ifdef DEBUG
#include "../kgmPhysics/kgmBody.h"
#include "../kgmPhysics/kgmObstacle.h"
#endif

class kgmGraphics: public kgmObject, public kgmIGraphics
{
  KGM_OBJECT(kgmGraphics)

  friend class BaseRender;
  friend class Render;
  friend class FpsRender;
  friend class GuiRender;
  friend class IconRender;
  friend class LineRender;
  friend class ColorRender;
  friend class PhongRender;
  friend class SpriteRender;
  friend class ShadowRender;
  friend class LightmapRender;
  friend class ParticlesRender;
  friend class EnvironmentRender;
  friend class Terrain;

#define MAX_LIGHTS   8
#define MAX_SHADOWS  2
#define MAX_RENDERS  24

public:
  enum GraphicsQuality
  {
    GraphicsQualityLow,
    GraphicsQualityMedium,
    GraphicsQualityHight,
    GraphicsQualitySuper
  };

  static GraphicsQuality textureQuality;
  static GraphicsQuality shadowQuality;

  struct Viewport
  {
    u32 x, y, w, h;
    f32 n, f;
  };

  struct Shadow
  {
    vec3  lpos, ldir;
    mtx4  mvp, mv, mp;
    gchandle fbo, tex;
    bool  valid;
    u16   w, h;
  };

  enum
  {
    //kgmShader_TypeGui = 100,
    kgmShader_TypeIcon
  };

  struct Options
  {
    bool color    = true;
    bool light    = false;
    bool shadows  = false;
    bool clipping = false;

    u32  width, height;
    f32  plane[4];

    kgmIGraphics::INode *discard = null;
  };

  struct LightData
  {
    vec4 pos_pow[MAX_LIGHTS];
    vec4 dir_ang[MAX_LIGHTS];
    vec4 col_per[MAX_LIGHTS];

    u32 count;
  };

  struct MapLight
  {
    gchandle m_fbo;
    gchandle m_col;
    gchandle m_dep;

    u32      m_res[2];
  };

    struct OLight
  {
    kgmIGraphics::INode* node;
    f32    force;
  };

  struct OMesh
  {
    kgmIGraphics::INode* node;
    f32    distance;
  };

protected:
  kgmIGC* gc;
  kgmIResources* rc;

  kgmFont* font;

  iRect       m_viewport;
  kgmCamera*  m_camera;

  kgmList<kgmGui*>     m_guis;
  kgmList<kgmIcon*>    m_icons;
  kgmList<kgmSprite*>  m_sprites;

  kgmList<INode*>      m_lights;
  kgmList<INode*>      m_meshes;
  kgmList<INode*>      m_particles;

  kgmMap<INode*, gchandle> m_environments;

  //active nodes.
  kgmArray<INode*> m_a_lights;
  kgmArray<INode*> m_a_meshes;
  kgmArray<INode*> m_a_bmeshes;
  kgmArray<INode*> m_a_particles;

  INode* m_terrain = null;

  u32  m_a_light_count     = 0;
  u32  m_a_meshes_count    = 0;
  u32  m_a_bmeshes_count   = 0;
  u32  m_a_particles_count = 0;

  kgmList<OLight> m_o_lights;
  kgmList<OMesh>  m_o_meshes;
  kgmList<OMesh>  m_o_bmeshes;

  LightData m_light_data;

  INode*  m_def_light = null;

  kgmList<kgmResource*> m_resources;

  kgmGuiStyle* gui_style = null;

  void* textures[GC_MAX_TERTURE_UNITS];
  void* tcolor;
  void* tnormal;
  void* tspecular;
  void* tdepth;
  void* shader;

  mtx4  location;

  kgmMaterial*  m_def_material = null;

  kgmMaterial*  m_mtl_active = null;
  kgmShader*    m_shd_active = null;

  bool  m_depth;
  bool  m_alpha;
  bool  m_culling;
  bool  m_wired;
  bool  m_2dmode;

  u32   m_max_faces;
  u32   m_bg_color;

  FpsRender*    m_r_fps;
  GuiRender*    m_r_gui = null;
  IconRender*   m_r_icon;
  SpriteRender* m_r_sprite = null;

  kgmTexture*  m_tex_black = null;
  kgmTexture*  m_tex_white = null;
  kgmTexture*  m_tex_gray  = null;
  kgmTexture*  m_map_shadow = null;

  MapLight m_map_light;

  mtx4 m_g_mtx_world;
  mtx4 m_g_mtx_proj;
  mtx4 m_g_mtx_view;
  mtx3 m_g_mtx_normal;
  mtx4 m_g_mtx_orto;
  mtx4 m_g_mtx_iden;

  gchandle m_rnd_target = null;

  Render*            m_render = null;
  BaseRender*        m_rnd_base = null;
  LineRender*        m_rnd_lines = null;
  ColorRender*       m_rnd_color = null;
  PhongRender*       m_rnd_phong = null;
  ShadowRender*      m_rnd_shadows = null;
  LightmapRender*    m_rnd_lightmap = null;
  EnvironmentRender* m_rnd_environment = null;
  Terrain*           m_rnd_terrain = null;

  //BaseRender*        m_rnd_list[];

  //For virtual visualization.

  bool m_show_lights = false;
  kgmIcon* m_icon_light = null;

protected:
  kgmShader*  m_shaders[32] = {0};

public:
  kgmGraphics(kgmIGC*, kgmIResources*);
  virtual ~kgmGraphics();

  void build();
  void render();
  void render_a();
  void update();
  void resize(float, float);

  void setDefaultFont(kgmFont*);

  s32  getShaderId(kgmString);

  void clear();

  bool resource(kgmResource*);
  kgmResource* resource(kgmString);

protected:
  void render(kgmCamera &cam, Options &op);

  void draw(kgmMesh*);
  void draw(kgmParticles*);
  void draw(kgmIcon*);

  void set(kgmMaterial*);
  void set(kgmShader*);
  void set(kgmLight*, u32);

  void renderGuiMenuItem(kgmGui*, void*);

  void gcDrawText(kgmFont*, u32, u32, u32, kgmGui::Rect, kgmString&);
  void gcDrawText(kgmFont*, u32, u32, u32, kgmGui::Rect, s8*);
  void gcDrawRect(kgmGui::Rect, u32, kgmTexture*);
  void gcDrawRect(kgmGui::Rect, u32, void*);
  void gcDrawBillboard(box b, u32 col);

  void trash();

  void setProjMatrix(mtx4&);
  void setViewMatrix(mtx4&);
  void setWorldMatrix(mtx4&);

  mtx4 getProjMatrix();
  mtx4 getViewMatrix();
  mtx4 getWorldMatrix();

  u32 collectLights(kgmCamera*, kgmArray<INode*>&, u32);
  u32 collectMeshes(kgmCamera*, kgmArray<INode*>&, u32);
  u32 collectParticles(kgmCamera*, kgmArray<INode*>&, u32);

  void collectOLights(kgmCamera& cam, kgmList<INode*>& nodes);
  void collectOMeshes(kgmCamera& cam, kgmList<INode*> &meshes, kgmList<INode*> &bmeshes);

  void shaderSetGeneral();
  void shaderSetPrivate();

public:
  void gc2DMode();
  void gc3DMode();

  void render_3d()
  {
  }

  void render_2d()
  {
  }

  void add(INode* nod);

  void add(kgmIcon* ico)
  {
    if(!ico)
      return;

    m_icons.add(ico);
  }

  void add(kgmGui* gui)
  {
    if(!gui)
      return;

    m_guis.add(gui);
  }

  void terrain(INode* t)
  {
    m_terrain = t;
  }

  void remove(kgmGui* g)
  {
    for(kgmList<kgmGui*>::iterator i = m_guis.begin(); i != m_guis.end(); i++)
    {
      if((*i) == g)
      {
        (*i) = null;

        break;
      }
    }
  }

  void remove(kgmIcon* c)
  {
    for(kgmList<kgmIcon*>::iterator i = m_icons.begin(); i != m_icons.end(); i++)
    {
      if((*i) == c)
      {
        (*i) = null;

        break;
      }
    }
  }

  iRect viewport()
  {
    return m_viewport;
  }

  kgmCamera& camera()
  {
    return *m_camera;
  }

  bool wired() const
  {
    return m_wired;
  }

  void wired(bool w)
  {
    m_wired = w;
  }

  void setBgColor(u32 col)
  {
    m_bg_color = col;
  }

  void showLights(bool show)
  {
    m_show_lights = show;
  }
};
