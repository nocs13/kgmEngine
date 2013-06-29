#pragma once
#include "../kgmBase/kgmObject.h"
#include "../kgmBase/kgmOctTree.h"

#include "../kgmGraphics/kgmGui.h"
#include "../kgmGraphics/kgmGuiStyle.h"

#include "../kgmGraphics/kgmMesh.h"
#include "../kgmGraphics/kgmLight.h"
#include "../kgmGraphics/kgmCamera.h"
#include "../kgmGraphics/kgmVisual.h"
#include "../kgmGraphics/kgmMaterial.h"
#include "../kgmGraphics/kgmGraphics.h"

#include "kgmEffect.h"

class kgmGameGraphics: public kgmObject
{

  struct LightPoint
  {
    vec3 pos;
    vec3 col;
  };

  struct LightSpot
  {
    vec3 pos;
    vec3 dir;
    vec3 col;
  };

  struct VBMesh
  {
    int   id;
    void* vb;
    int v_fmt, v_size, i_size, i_count;
     box3 box;
  };

  struct Light
  {
    kgmLight* light;
  };

  struct Camera
  {
    kgmCamera  camera;
    kgmVisual* object;
    float      zdiff, dist;
  };

  class Mesh
  {
  public:
    kgmMesh*     m_mesh;
    kgmMaterial* m_mtrl;
    box3         m_box;

    Mesh()
    {
      m_mesh = null;
      m_mtrl = null;
    }

    Mesh(kgmMesh* m, kgmMaterial *l)
    {
      if(m){
        m_mesh = m;
        m_box  = m->bound();
        m->increment();
      }

      if(l){
        m_mtrl = l;
        l->increment();
      }
    }

    virtual ~Mesh()
    {
      if(m_mesh) m_mesh->release();
      if(m_mtrl) m_mtrl->release();
    }

    virtual kgmMesh::Vertex* getVertices()
    {
      return m_mesh->vertices();
    }

    virtual kgmMesh::Face* getFaces()
    {
      return m_mesh->faces();
    }
  };

  class GMesh
  {
    kgmMaterial* material;
    kgmList<Mesh> meshes;
    kgmOctTree<Mesh*> tree;
  };

public:
  struct Settings
  {
    char texture_quality;
    char shadow_quality;

    bool lods;
  };

private:
  kgmIGraphics* gc;
  kgmIResources* rc;

  kgmFont* font;

  Camera   m_camera;

  kgmList<Mesh*>        m_meshes;
  kgmList<kgmMaterial*> m_materials;
  kgmList<Light>        m_lights;
  kgmList<kgmVisual*>   m_visuals;
  kgmList<kgmVisual*>   m_vis_mesh;
  kgmList<kgmVisual*>   m_vis_text;
  kgmList<kgmVisual*>   m_vis_blend;
  kgmList<kgmVisual*>   m_vis_sprite;
  kgmList<kgmVisual*>   m_vis_particles;
  kgmList<kgmGui*>      m_guis, m_tguis;

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
public:
  kgmGameGraphics(kgmIGraphics*, kgmIResources*);
  ~kgmGameGraphics();

  void clear();
  void build();
  void render();
  void update();
  void resize(float, float);

  void setGuiStyle(kgmGuiStyle*);
  void setDefaultFont(kgmFont*);

private:
  void render(Mesh*);
  void render(kgmGui*);
  void render(kgmMesh*);
  void render(kgmVisual*);
  void render(kgmShader*);
  void render(kgmMaterial*);
  void render(kgmParticles*);
  void gcDrawText(kgmFont*, u32, u32, u32, kgmGui::Rect, kgmString&);
  void gcDrawRect(kgmGui::Rect, u32, kgmTexture*);

  void trash();
public:
  void add(kgmMaterial* mtl)
  {
    if(mtl)
    {
      m_materials.add(mtl);
    }
  }

  void add(kgmLight* lgt)
  {
    if(lgt)
      lgt->increment();

    Light light;
    light.light = lgt;
    m_lights.add(light);
  }

  void add(kgmMesh* mesh, kgmMaterial* material)
  {
    Mesh* m = new Mesh(mesh, material);

    m_meshes.add(m);
  }

  void add(kgmVisual* a){
    if(!a)
      return;

    a->increment();

    switch(a->m_typerender)
    {
    case kgmVisual::RenderNone:
    case kgmVisual::RenderMesh:
      if(a->m_visuals.size() > 0 &&
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
    };
  }

  void add(kgmGui* gui, bool tmp = false){
    if(gui){
      gui->increment();
      if(tmp)
       m_tguis.add(gui);
      else
        m_guis.add(gui);
    }
  }

  bool get(kgmString name, kgmMaterial** mtl)
  {
    for(int i = 0; i < m_materials.size(); i++)
    {
      kgmMaterial* m = m_materials[i];

      if(m && m->m_id == name)
      {
        *mtl = m;

        return true;
      }
    }

    return false;
  }

  bool set(kgmMesh* msh, kgmMaterial* mtl)
  {
    for(int i = 0; i < m_meshes.size(); i++)
    {
      kgmMesh* m = m_meshes[i]->m_mesh;

      if(m == msh)
      {
        m_meshes[i]->m_mtrl = mtl;

        return true;
      }
    }

    return false;
  }

  void setTo(kgmMesh* mesh, kgmMaterial* mtl){
    if(!mesh || !mtl)
      return;

    for(int i = 0; i < m_meshes.size(); i++){
      if(mesh == m_meshes[i]->m_mesh){
        m_meshes[i]->m_mtrl = mtl;
        break;
      }
    }
  }

  kgmCamera& camera(){
    return m_camera.camera;
  }

  void linkCamera(kgmVisual* o, float zdiff, float dist)
  {
    if(o)
    {
      m_camera.object = o;
      m_camera.zdiff  = zdiff;
      m_camera.dist   = dist;
    }
    else
    {
      m_camera.object = null;
      m_camera.zdiff  = 0.0f;
      m_camera.dist   = 0.0f;
      m_camera.camera.set(PI / 6, 1, 1, 1000, vec3(0, 0, 1), vec3(-1, 0, 0), vec3(0, 0, 1));
    }
  }
};
