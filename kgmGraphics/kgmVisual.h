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

 struct Group
 {
   kgmMaterial*  material;
   kgmMesh*      mesh;
   kgmSkeleton*  skeleton;
   mtx4*         transform;
 };

public:
 bool         m_visible;
 mtx4         m_transform;
 TypeShadow   m_typeshadow;
 TypeRender   m_typerender;

 kgmList<kgmMaterial*>	m_materials;	//material maps
 kgmList<Group*>        m_groups;
 Group*                 m_group;

public:
 kgmVisual()
 {
  m_visible = true;
  m_typeshadow = ShadowNone;
  m_typerender = RenderMesh;
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

 bool setGroup(u32 i)
 {
   if(i > m_groups.size() || i == m_groups.size())
   {
     return false;
   }

   m_group = m_groups[i];
 }

 Group* getGroup()
 {
   return m_group;
 }

 void addGroup(kgmMesh* msh, kgmMaterial* mtl, kgmSkeleton* skel, mtx4* tran)
 {
   Group* group = new Group();
   memset(group, 0, sizeof(Group));

   if(msh)  { group->mesh = msh; msh->increment(); }
   if(mtl)  { group->material = mtl; mtl->increment(); }
   if(skel) { group->skeleton = skel; skel->increment(); }
   if(tran) { group->transform = new mtx4(); *group->transform = *tran; }
 }
};

