#pragma once
#include "../kgmBase/kgmIGC.h"
#include "../kgmBase/kgmTab.h"
#include "../kgmBase/kgmIResources.h"

#include "../kgmSystem/kgmWindow.h"

#include "kgmNode.h"
#include "kgmGui.h"
#include "kgmGuiTab.h"
#include "kgmGuiList.h"
#include "kgmGuiText.h"
#include "kgmGuiButton.h"
#include "kgmGuiScroll.h"
#include "kgmGuiProgress.h"
#include "kgmGuiStyle.h"
#include "kgmIGuiDraw.h"
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

  struct Shadow
  {
    kgmLight*   light;
    kgmTexture* map;
  };

  enum
  {
    kgmShader_TypeGui = 100,
    kgmShader_TypeIcon,
  };

private:
  kgmIGC* gc;
  kgmIResources* rc;

  kgmFont* font;

  iRect       m_viewport;
  kgmCamera*  m_camera;

  kgmList<kgmGui*>     m_guis;
  kgmList<kgmIcon*>    m_icons;
  kgmList<kgmLight*>   m_lights;
  kgmList<kgmVisual*>  m_visuals;

  kgmList<kgmResource*> m_resources;

#ifdef DEBUG
  kgmList<kgmBody*>      m_bodies;
  kgmList<kgmObstacle*>  m_obstacles;
#endif

  kgmGuiStyle* gui_style = null;

  void* tcolor;
  void* tnormal;
  void* tspecular;
  void* tdepth;
  void* shader;
  mtx4  location;

  kgmTab<u16, kgmShader*>  shaders;

  kgmArray<kgmVisual*> m_visible_sprites;
  kgmArray<kgmVisual*> m_visible_visuals;
  kgmArray<kgmVisual*> m_visible_visuals_alpha;

  kgmTexture *m_shadowmap;

  kgmArray<Shadow> m_shadows;

  bool  m_depth;
  bool  m_alpha;
  bool  m_culling;

  bool  m_editor;

  uint32 m_max_faces;

  u32    m_bg_color;

  FpsRender*    m_r_fps;
  GuiRender*    m_r_gui = null;
  SpriteRender* m_r_sprite = null;
public:
  kgmGraphics(kgmIGC*, kgmIResources*);
  ~kgmGraphics();

  void build();
  void render();
  void update();
  void resize(float, float);
  void setEditor(bool);

  void setGuiStyle(kgmGuiStyle*);
  void setDefaultFont(kgmFont*);

  s32  getShaderId(kgmString);

  void clean();

  bool resource(kgmResource*);
  kgmResource* resource(kgmString);

private:
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

  void add(kgmMaterial* mtl)
  {
  }

  void add(kgmLight* lgt)
  {
    if(!lgt)
      return;

    m_lights.add(lgt);
  }

  void add(kgmVisual* a)
  {
    if(!a)
      return;

    m_visuals.add(a);
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

#ifdef DEBUG
  void add(kgmBody* a)
  {
    if(a)
    {
      m_bodies.add(a);
    }
  }

  void add(kgmObstacle* a)
  {
    if(a)
    {
      m_obstacles.add(a);
    }
  }

  void remove(kgmBody* a)
  {
    for(kgmList<kgmBody*>::iterator i = m_bodies.begin(); i != m_bodies.end(); ++i)
    {
      if(a == (*i))
      {
        (*i) = null;

        break;
      }
    }
  }

  void remove(kgmObstacle* o)
  {
    for(kgmList<kgmObstacle*>::iterator i = m_obstacles.begin(); i != m_obstacles.end(); ++i)
    {
      if(o == (*i))
      {
        (*i) = null;

        break;
      }
    }
  }
#endif

  void remove(kgmVisual* v)
  {
    for(kgmList<kgmVisual*>::iterator i = m_visuals.begin(); i != m_visuals.end(); ++i)
    {
      if((*i) == v)
      {
        (*i) = null;

        break;
      }
    }
  }

  void remove(kgmLight* l)
  {
    for(kgmList<kgmLight*>::iterator i = m_lights.begin(); i != m_lights.end(); ++i)
    {
      if((*i) == l)
      {
        (*i) = null;

        break;
      }
    }
  }

  void remove(kgmGui* g)
  {
    for(kgmList<kgmGui*>::iterator i = m_guis.begin(); i != m_guis.end(); ++i)
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
    for(kgmList<kgmIcon*>::iterator i = m_icons.begin(); i != m_icons.end(); ++i)
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

  void setBgColor(u32 col)
  {
    m_bg_color = col;
  }

  kgmShader* toShader(kgmShader::Shader shader);
};
