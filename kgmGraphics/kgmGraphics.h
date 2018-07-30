#pragma once
#include "../kgmBase/kgmIGC.h"
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
#include "kgmVisual.h"
#include "kgmMaterial.h"

#include "render/FpsRender.h"
#include "render/GuiRender.h"
#include "render/IconRender.h"
#include "render/SpriteRender.h"
#include "render/ShadowRender.h"

#ifdef DEBUG
#include "../kgmPhysics/kgmBody.h"
#include "../kgmPhysics/kgmObstacle.h"
#endif

class kgmGraphics: public kgmObject, public kgmIGraphics
{
  KGM_OBJECT(kgmGraphics);

  friend class BaseRender;
  friend class FpsRender;
  friend class GuiRender;
  friend class IconRender;
  friend class SpriteRender;
  friend class LightRender;
  friend class ColorRender;
  friend class ParticlesRender;
  friend class ShadowRender;

public:
  enum GraphicsQuality
  {
    GraphicsQualityLow,
    GraphicsQualityMedium,
    GraphicsQualityHight,
    GraphicsQualitySuper
  };

  enum Shader
  {
    ShaderShadowKeep = kgmMaterial::TypeExtend + 1,
    ShaderShadowDraw
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

  };

protected:
  kgmIGC* gc;
  kgmIResources* rc;

  kgmFont* font;

  iRect       m_viewport;
  kgmCamera*  m_camera;

  kgmList<kgmGui*>     m_guis;
  kgmList<kgmIcon*>    m_icons;

  kgmList<INode*>      m_lights;
  kgmList<INode*>      m_meshes;
  kgmList<INode*>      m_particles;

  //active nodes.
  kgmArray<INode*> m_a_lights;
  kgmArray<INode*> m_a_meshes;
  kgmArray<INode*> m_a_bmeshes;
  kgmArray<INode*> m_a_particles;

  u32  m_a_light_count     = 0;
  u32  m_a_meshes_count    = 0;
  u32  m_a_bmeshes_count   = 0;
  u32  m_a_particles_count = 0;

  INode*  m_a_light   = null;
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

  kgmArray<Shadow> m_shadows;

  kgmMaterial*  m_def_material = null;


  bool  m_depth;
  bool  m_alpha;
  bool  m_culling;

  bool  m_editor;
  bool  m_wired;

  uint32 m_max_faces;

  u32    m_bg_color;

  FpsRender*    m_r_fps;
  GuiRender*    m_r_gui = null;
  SpriteRender* m_r_sprite = null;

  kgmTexture*  m_tex_black = null;
  kgmTexture*  m_tex_white = null;
  kgmTexture*  m_tex_gray  = null;
  kgmTexture*  m_map_shadow = null;

  mtx4 m_g_mtx_world;
  mtx4 m_g_mtx_proj;
  mtx4 m_g_mtx_view;
  mtx3 m_g_mtx_normal;

  gchandle m_rnd_target = null;

protected:
  kgmShader*  m_shaders[32] = {0};

public:
  kgmGraphics(kgmIGC*, kgmIResources*);
  virtual ~kgmGraphics();

  void build();
  void render();
  void update();
  void resize(float, float);
  void setEditor(bool);

  void setDefaultFont(kgmFont*);

  s32  getShaderId(kgmString);

  void clear();

  bool resource(kgmResource*);
  kgmResource* resource(kgmString);

protected:
  void render(gchandle buf, kgmCamera &cam, Options &op);
  void render(kgmMesh*);
  void render(kgmVisual*);
  void render(kgmShader*);
  void render(kgmMaterial*);
  void render(kgmParticles*);
  void render(kgmIcon*);

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

public:
  void gc2DMode();
  void gc3DMode();

  void render_3d()
  {
  }

  void render_2d()
  {
  }

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

  void add(INode* nod)
  {
    if (!nod)
      return;

    switch(nod->getNodeType())
    {
    case  NodeMesh:
      m_meshes.add(nod);
      break;
    case NodeLight:
      m_lights.add(nod);
      break;
    case NodeParticles:
      m_particles.add(nod);

      if (nod->getNodeObject())
      {
        ((kgmParticles*)nod->getNodeObject())->camera(m_camera);
      }
      break;
    }
  }

  void remove(kgmGui* g)
  {
    for(kgmList<kgmGui*>::iterator i = m_guis.begin(); !i.end(); ++i)
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
    for(kgmList<kgmIcon*>::iterator i = m_icons.begin(); !i.end(); ++i)
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
};
