#pragma once

#include "../kgmBase/kgmBase.h"
#include "../kgmBase/kgmList.h"
#include "../kgmBase/kgmIGraphics.h"

#include "kgmMesh.h"
#include "kgmLight.h"
#include "kgmSkeleton.h"

//Graphical Object
class kgmVisual: public kgmObject
{
  KGM_OBJECT(kgmVisual)
public:
 enum TypeRender
 {
  RenderMesh,
  RenderText,
  RenderSprite,
  RenderParticles,
 };

 enum  TypeShadow
 {
  ShadowNone,
  ShadowCast,
  ShadowRecv,
  ShadowFull,
 };

 struct Mesh
 {
   kgmMesh::Vertex* vertices;
   kgmMesh::Face*   faces;

   u32              vcount;
   u32              vcount;
   u32              fvf;
   u32              fff;
 };

 struct Group
 {
   kgmMaterial*  material;
   kgmMesh*      mesh;
   mtx4*         transform;
   bool          skin;
 };

public:
 bool         m_visible;
 mtx4         m_transform;
 TypeShadow   m_typeshadow;
 TypeRender   m_typerender;

 kgmList<kgmMaterial*>	 m_materials;	//material maps
 kgmList<Group*>        m_groups;
 Group*                 m_group;

 kgmSkeleton*           m_skeleton;
 kgmAnimation*          m_animation;
 bool                   m_floop;
 u32                    m_fstart;
 u32                    m_fend;

public:
 kgmVisual()
 {
  m_visible = true;
  m_typeshadow = ShadowNone;
  m_typerender = RenderMesh;

  m_animation = null;
  m_fstart    = m_fend = 0;
  m_floop     = false;

  m_group = null;
 }

 virtual ~kgmVisual()
 {
  int i = 0;

  for(int i = 0; i < m_materials.size(); i++)
    m_materials[i]->release();
  m_materials.clear();

  for(int i = 0; i < m_groups.size(); i++)
  {
    Group* g = m_groups[i];

    if(g->mesh)      g->mesh->release();
    if(g->material)  g->material->release();
    if(g->skeleton)  g->skeleton->release();
    if(g->transform) delete g->transform;

    delete g;
  }
  m_groups.clear();
}

 //Materials
 int numMaterials(){
  return m_materials.size();
 }

 bool add(kgmMaterial* m)
 {
  if(m == 0) 
	  return false;

  m_materials.add(m);
  m->increment();

  return true;
 }	

 void setAnimation(kgmAnimation* a, u32 start, u32 end, bool loop)
 {
     m_animation = a;
     m_fstart    = start;
     m_fend      = end;
     m_floop     = loop;
 }

 void setSkeleton(kgmSkeleton* skel)
 {
   if(skel)
   {
     m_skeleton = skel;
     skel->increment();
   }
 }

 void addGroup(kgmMesh* msh, kgmMaterial* mtl, mtx4* tran)
 {
   Group* group = new Group();
   memset(group, 0, sizeof(Group));

   if(msh)  { group->mesh = msh; msh->increment(); }
   if(mtl)  { group->material = mtl; mtl->increment(); }
   if(tran) { group->transform = new mtx4(); *group->transform = *tran; }

   m_groups.add(group);
 }
};

