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
  struct VBMesh
  {
    int   id;
    void* vb;
    int v_fmt, v_size, i_size, i_count;
     box3 box;
  };

  struct Mesh
  {
    kgmMesh*     mesh;
    kgmMaterial* material;
    box3         box;
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

  class GMesh{
    kgmMaterial* material;
    kgmList<Mesh> meshes;
    kgmOctTree<Mesh*> tree;
  };

  class ModAnimation
  {
  public:
      kgmMesh*      m_mesh;
      kgmSkeleton*  m_skel;
      kgmAnimation* m_anim;

      ModAnimation(kgmMesh* m, kgmAnimation* a, kgmSkeleton* s)
      {
         m_mesh = null;
         m_skel = null;
         m_anim = null;

         if(m)
             m_mesh = (kgmMesh*)m->clone();
         m_skel = s;
         m_anim = a;
      }

      void animate(u32 frame)
      {

      }

      kgmMesh* getAnimated()
      {
          return m_mesh;
      }
  };

  kgmIGraphics* gc;
  kgmIResources* rc;

  kgmFont* font;

  Camera   m_camera;

  kgmList<Mesh>         m_meshes;
  kgmList<kgmMaterial*> m_materials;
  kgmList<Light>        m_lights;
  kgmList<kgmVisual*>   m_visuals;
  kgmList<kgmGui*>      m_guis, m_tguis;

  kgmGuiStyle* gui_style;

  void* tdiffuse;
  void* tspecular;
  void* tnormal;
  void* tdepth;
  void* shader;

  kgmTab<u16, kgmShader*>  shaders;
  kgmTab<u16, kgmTexture*> textures;

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
  void render(kgmShader*, u32);
  void render(kgmMaterial*);
  void gcDrawText(kgmFont*, u32, u32, u32, kgmGui::Rect, kgmString&);
  void gcDrawRect(kgmGui::Rect, u32, kgmTexture*);

  void trash();

public:
  void add(kgmMesh* mesh, kgmMaterial* material){
    if(mesh)
      mesh->increment();
    if(material)
      material->increment();

    Mesh m;
    m.mesh = mesh;
    m.material = material;
    m.box = mesh->bound();
    m_meshes.add(m);
  }

  void add(kgmMaterial* mtl){
    if(mtl){
      m_materials.add(mtl);
    }
  }

  void add(kgmLight* lgt){
    if(lgt)
      lgt->increment();

    Light light;
    light.light = lgt;
    m_lights.add(light);
  }

  void add(kgmVisual* a){
    if(a)
        a->increment();
    m_visuals.add(a);
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
      kgmMesh* m = m_meshes[i].mesh;

      if(m == msh)
      {
        m_meshes[i].material = mtl;

        return true;
      }
    }

    return false;
  }

  void setTo(kgmMesh* mesh, kgmMaterial* mtl){
    if(!mesh || !mtl)
      return;

    for(int i = 0; i < m_meshes.size(); i++){
      if(mesh == m_meshes[i].mesh){
        m_meshes[i].material = mtl;
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
