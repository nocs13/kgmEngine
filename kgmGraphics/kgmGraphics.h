#pragma once
#include "../kgmBase/kgmInterface.h"
#include "../kgmBase/kgmIGC.h"
#include "../kgmBase/kgmIResources.h"
#include "../kgmBase/kgmTab.h"
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

#include "kgmMesh.h"
#include "kgmText.h"
#include "kgmLight.h"
#include "kgmSprite.h"
#include "kgmCamera.h"
#include "kgmVisual.h"
#include "kgmMaterial.h"

#ifdef TEST
#include "../kgmPhysics/kgmBody.h";
#endif


class kgmGraphics: public kgmObject
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

  static GraphicsQuality   textureQuality;
  static GraphicsQuality   shedowQuality;

  struct Mesh
  {
    kgmMaterial* material;
    kgmMesh*     mesh;
    mtx4         mtx;
  };

private:
  kgmIGC* gc;
  kgmIResources* rc;

  kgmFont* font;

  kgmCamera   m_camera;

  kgmList<Mesh>         m_meshes;
  kgmList<kgmMaterial*> m_materials;
  kgmList<kgmLight*>    m_lights;
  kgmList<kgmVisual*>   m_visuals;
  kgmList<kgmGui*>      m_guis;

#ifdef TEST
  kgmList<kgmBody*>     m_bodies;
#endif

  kgmGuiStyle* gui_style;

  void* tcolor;
  void* tnormal;
  void* tspecular;
  void* tdepth;
  void* shader;
  mtx4  location;

  kgmTab<u16, kgmShader*>  shaders;
  kgmTab<u16, kgmTexture*> textures;

  bool  m_has_shaders;
  bool  m_has_buffers;

  bool  m_alpha;
  bool  m_culling;

  uint32 m_max_faces;
public:
  kgmGraphics(kgmIGC*, kgmIResources*);
  ~kgmGraphics();

  void clear();
  void build();
  void render();
  void update();
  void resize(float, float);

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
  void gc2DMode();
  void gc3DMode();
  void gcDrawText(kgmFont*, u32, u32, u32, kgmGui::Rect, kgmString&);
  void gcDrawRect(kgmGui::Rect, u32, kgmTexture*);
  void gcDrawBillboard(box b, u32 col);
  void trash();


  void setProjMatrix(mtx4&);
  void setViewMatrix(mtx4&);
  void setWorldMatrix(mtx4&);

public:
  void add(kgmMaterial* mtl)
  {
    if(mtl)
    {
      m_materials.add(mtl);
      mtl->increment();
    }
  }

  void add(kgmLight* lgt)
  {
    if(!lgt)
      return;

    lgt->increment();
    m_lights.add(lgt);
  }

  void add(kgmMesh* mesh, kgmMaterial* mtl, mtx4* mtx = null)
  {
    Mesh m;

    if(!mesh)
      return;

    if(mesh) mesh->increment();
    if(mtl)  mtl->increment();

    m.mesh = mesh;
    m.material = mtl;

    if(mtx)
      m.mtx = *mtx;
    else
      m.mtx.identity();

    m_meshes.add(m);
  }

  void add(kgmVisual* a){
    if(!a)
      return;

    a->increment();

    m_visuals.add(a);



    /*switch(a->m_typerender)
    {
    case kgmVisual::RenderNone:
      //a->release();
      //break;
    case kgmVisual::RenderMesh:
      if(a->m_visuals.size() > 0 && a->m_visuals[0]->getMaterial() &&
         (a->m_visuals[0]->getMaterial()->m_transparency > 0.0 ||
          a->m_visuals[0]->getMaterial()->m_alpha))
        m_vis_blend.add(a);
      else
        m_visuals.add(a);
    break;
    case kgmVisual::RenderText:
      m_vis_text.add(a);
    break;
    case kgmVisual::RenderSprite:
      m_vis_sprite.add(a);
    break;
    case kgmVisual::RenderParticles:
      m_vis_particles.add(a);
    break;
    };*/
  }

  void add(kgmGui* gui)
  {
    if(gui)
    {
      gui->increment();

      m_guis.add(gui);
    }
  }

#ifdef TEST
  void add(kgmBody* a)
  {
    if(a)
    {
      m_bodies.add(a);
      a->increment();
    }
  }
#endif

  bool get(kgmString name, kgmMaterial** mtl)
  {
    for(int i = 0; i < m_materials.size(); i++)
    {
      kgmMaterial* m = m_materials[i];

      if(m && m->m_id.length() && (m->m_id == name))
      {
        *mtl = m;

        return true;
      }
    }

    return false;
  }

  bool set(kgmMesh* msh, kgmMaterial* mtl)
  {
    if(!msh)
      return false;

    for(int i = 0; i < m_meshes.length(); i++)
    {
      Mesh* mesh = &m_meshes[i];

      if(mesh->mesh == msh)
      {
        if(mesh->material)
          mesh->material->release();

        if(mtl)
          mtl->increment();

        mesh->material = mtl;

        return true;
      }
    }

    return false;
  }

  kgmCamera& camera(){
    return m_camera;
  }
};
