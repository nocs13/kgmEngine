#pragma once

#include "../kgmMain/kgmBase.h"
#include "../kgmMain/kgmList.h"
#include "../kgmMain/kgmIGraphics.h"

#include "kgmMesh.h"
#include "kgmLight.h"
#include "kgmSkeleton.h"

//Graphical Object
class kgmGraphical{
// KGM_OBJECT
public:
 enum TypeRender{
  RenderMesh,
  RenderText,
  RenderSprite,
  RenderParticles,
 };

 enum  TypeShadow{
  ShadowNone,
  ShadowCast,
  ShadowRecv,
  ShadowFull,
 };
public:
 bool					m_visible; 
 mtx4					m_transform;
 TypeShadow				m_typeshadow; 
 TypeRender				m_typerender; 
 char*					m_name;

 kgmMaterial*			m_material;		//active material
 kgmList<kgmMaterial*>	m_materials;	//material maps

 kgmMesh*				m_mesh;			//active mesh
public:
 kgmGraphical(){
  m_visible = true;
  m_typeshadow = ShadowNone;
  m_typerender = RenderMesh;
  m_material = 0;
  m_mesh = 0;
  m_name = new char[64]; memset(m_name, 0, 64);
 }
 virtual ~kgmGraphical(){
  int i = 0;
  int size = m_materials.size();
  for(i = 0; i < size; i++)
	  m_materials[i]->release();
  m_materials.clear();
  if(m_mesh) 
	  m_mesh->release();
  if(m_material) 
	  m_material->release();
  delete [] m_name;
 }
 //Materials
 int numMaterials(){
  return m_materials.size();
 }

 bool add(kgmMaterial* m){
  if(m == 0) 
	  return false;
  m_materials.add(m);
  m->increment();
  set(m);
  return true;
 }	
 void set(kgmMesh* m){
  if(m) 
	m_mesh = m;
 }
 void set(kgmMaterial* m){
  if(m_material){
   m_material->release();
   m_material = 0;
  }
  if(m){
   m_material = m;
   m_material->increment();
  }
 }
	
 kgmMaterial* material(int i){
  if(i >= m_materials.size())
	return 0;
  if(i < 0)
	  return m_material;
  return m_materials[i];
 }
};

