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

  class MeshMove: public Mesh
  {
  public:
    mtx4* m_tran;

    MeshMove()
    {
    }

    MeshMove(kgmMesh* m, kgmMaterial *l, mtx4* t)
      :Mesh(m, l)
    {
      m_tran = t;
    }
  };

  class MeshSkinned: public MeshMove
  {
  public:
      kgmMesh*      m_mesh;
      kgmMaterial*  m_mtrl;
      kgmSkeleton*  m_skel;
      kgmAnimation* m_anim;

      mtx4*         m_tran;

      kgmMesh::Vertex_P_N_C_T_BW_BI* m_verts;

      u32 frame;

      MeshSkinned()
      {
        m_verts = null;
        frame   = 0;
      }

      MeshSkinned(kgmMesh* m, kgmMaterial* l,  mtx4* t, kgmSkeleton* s, kgmAnimation* a)
        :MeshMove(m, l, t)
      {
         m_mesh = null;
         m_skel = null;
         m_anim = null;
         m_mtrl = null;
         m_tran = null;

         m_mesh = m;
         m_skel = s;
         m_anim = a;
         m_mtrl = l;
         m_tran = t;

         m_verts = new kgmMesh::Vertex_P_N_C_T_BW_BI[m_mesh->vcount()];
         memcpy(m_verts, m_mesh->vertices(),
                sizeof(kgmMesh::Vertex_P_N_C_T_BW_BI) * m_mesh->vcount());
      }

      ~MeshSkinned()
      {
        if(m_verts) delete [] m_verts;
      }

      void animate(u32 frame)
      {
        if(!m_skel || !m_anim)
          return;

         kgmMesh::Vertex_P_N_C_T_BW_BI* vbase = (kgmMesh::Vertex_P_N_C_T_BW_BI*)m_mesh->vertices();
         mtx4* j_anim = new mtx4[m_skel->m_joints.size()];

         for(int i = 0; i < m_skel->m_joints.size(); i++)
         {
           kgmAnimation::Animation* a = m_anim->getNode(m_skel->m_joints[i]->n);

           if(!a)
           {
             j_anim[i].identity();

             continue;
           }

           mtx4 jframe;
           a->getFrame(frame, jframe);

           j_anim[i] = m_skel->m_matrices[i] * m_skel->m_imatrices[i] * jframe;
         }

         for(int i = 0; i < m_mesh->vcount(); i++)
         {
           vec3   bpos(0, 0, 0);
           float* wght = (float*)(&vbase[i].bw);
           int*   indx = (int*)(&vbase[i].bi);

           for(int j = 0; j < 4; j++)
           {
             if(indx[j] < 0)
               break;

             bpos = bpos + j_anim[indx[j]] * vbase[i].pos * wght[j];
           }

           m_verts[i].pos = bpos;
         }

         delete [] j_anim;
      }

      kgmMesh::Vertex* getVertices()
      {
        return m_verts;
        //return m_mesh->vertices();
      }
  };

  class GMesh{
    kgmMaterial* material;
    kgmList<Mesh> meshes;
    kgmOctTree<Mesh*> tree;
  };

  kgmIGraphics* gc;
  kgmIResources* rc;

  kgmFont* font;

  Camera   m_camera;

  kgmList<Mesh*>        m_meshes;
  kgmList<MeshMove*>    m_mmeshes;
  kgmList<MeshSkinned*> m_smeshes;
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
  mtx4  location;

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
  void transform(mtx4&);
  void gcDrawText(kgmFont*, u32, u32, u32, kgmGui::Rect, kgmString&);
  void gcDrawRect(kgmGui::Rect, u32, kgmTexture*);

  void trash();

public:
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

  void add(kgmMesh* mesh, kgmMaterial* material){
    Mesh* m = new Mesh(mesh, material);
    m_meshes.add(m);
  }

  void add(kgmVisual* a){
    if(!a)
      return;

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
