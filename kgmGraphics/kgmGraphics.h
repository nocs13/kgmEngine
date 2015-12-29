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

#ifdef DEBUG
#include "../kgmPhysics/kgmBody.h"
#endif

class kgmGraphics: public kgmObject, public kgmIGraphics
{
  KGM_OBJECT(kgmGraphics);
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

  template <class T>
  class Node
  {
    T* m_object = null;

    bool m_remove;

  public:
    Node()
    {
      m_object = null;
      m_remove = false;
    }

    Node(T* obj)
    {
      m_object = obj;
      m_remove = false;
    }

    Node(const Node<T>& node)
    {
      m_object = node.m_object;
      m_remove = node.m_remove;
    }

    ~Node()
    {
    }

    T* operator()()
    {
      return m_object;
    }

    bool removed()
    {
      return m_remove;
    }

    void remove()
    {
      m_remove = true;
    }
  };

private:
  kgmIGC* gc;
  kgmIResources* rc;

  kgmFont* font;

  iRect       m_viewport;
  kgmCamera*  m_camera;

  kgmList< Node<kgmLight>  > m_lights;
  kgmList< Node<kgmVisual> > m_visuals;
  kgmList< Node<kgmGui>    > m_guis;
  kgmList< Node<kgmIcon>   > m_icons;

#ifdef DEBUG
  kgmList<kgmBody*>     m_bodies;
#endif

  //kgm_ptr<kgmGuiStyle> gui_style;
  kgmGuiStyle* gui_style = null;

  void* tcolor;
  void* tnormal;
  void* tspecular;
  void* tdepth;
  void* shader;
  mtx4  location;

  kgmTab<u16, kgmShader*>  shaders;
  //kgmTab<u16, kgmTexture*> textures;

  kgmArray<kgmVisual*> m_vis_mesh_scene;
  s32                  m_cnt_vis_mesh_scene;

  kgmTexture *m_shadowmap;

  bool  m_depth;
  bool  m_alpha;
  bool  m_culling;

  bool  m_editor;

  uint32 m_max_faces;

  u32    m_bg_color;
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

private:
  void render(kgmGui*);
  void render(kgmMesh*);
  void render(kgmVisual*);
  void render(kgmSprite*);
  void render(kgmShader*);
  void render(kgmMaterial*);
  void render(kgmParticles*);
  void render(kgmIcon*);

  void renderGuiMenuItem(kgmGui*, void*);

  void gcDrawText(kgmFont*, u32, u32, u32, kgmGui::Rect, kgmString&);
  void gcDrawRect(kgmGui::Rect, u32, kgmTexture*);
  void gcDrawBillboard(box b, u32 col);

  void trash();

  void setProjMatrix(mtx4&);
  void setViewMatrix(mtx4&);
  void setWorldMatrix(mtx4&);

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

    Node<kgmLight> light(lgt);

    m_lights.add(light);
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

    m_icons.add(Node<kgmIcon>(ico));
  }

  void add(kgmGui* gui)
  {
    if(gui)
    {
      m_guis.add(gui);
    }
  }

#ifdef DEBUG
  void add(kgmBody* a)
  {
    if(a)
    {
      m_bodies.add(a);
    }
  }
#endif

  void remove(kgmLight* light)
  {
    for(int i = 0; i < m_lights.length(); i++)
    {
      if(m_lights[i]() == light)
      {
        m_lights[i].remove();

        break;
      }
    }
  }

  void remove(kgmIcon* ico)
  {
    for(int i = 0; i < m_icons.length(); i++)
    {
      if(m_icons[i]() == ico)
      {
        m_icons[i].remove();

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
