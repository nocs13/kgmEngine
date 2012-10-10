#pragma once
#include "../kgmBase/kgmObject.h"
#include "../kgmBase/kgmTime.h"
#include "kgmAnimation.h"
#include "kgmMesh.h"


class kgmAnimator: public kgmObject
{
 KGM_OBJECT(kgmAnimator)
public:
 uint			   m_type;
 kgmMesh*          m_mesh;
 mtx4*             m_imtx;
 kgmAnimation*     m_canim; // current animation
 kgmArray<vec3>    m_mverts;
 uint              m_stick, m_atick;
 uint              m_objects;     //animated objects count
 kgmAnimation::Framed** m_oframed; //animated objects
public:
 kgmAnimator(kgmMesh* m){
  int i = 0;
  m_mesh = 0;
  m_canim = 0;
  m_stick = 0;
  m_oframed = 0;

  if(!m || !m->v.count() ||
	 !m->sv.count() || !m->bn.size())
	  return;

  m_mesh = m;

  m_mverts.realloc(m_mesh->v.count());
  for(i = 0; i < m_mesh->v.count(); i++)
	  m_mverts[i] = m_mesh->v[i].pos;

  m_objects =  m_mesh->bn.size();
  m_imtx = new mtx4[m_objects];
  for(i = 0; i < m_objects; i++){
   m_imtx[i] = mtx4(m_mesh->bn[i].r, m_mesh->bn[i].v);
   m_imtx[i].fast_invert();
  }

//  m_oframed = new 
 }
 virtual ~kgmAnimator(){
  if(m_mesh)
   for(int i = 0; i < m_mesh->v.count(); i++)
     m_mesh->v[i].pos = m_mverts[i];
  m_mverts.clear();
  if(m_imtx)
	  delete m_imtx;
  if(m_oframed)
	  delete m_oframed;
 }
 ///*
 void animate(){
  uint ctick = kgmTime::getTicks();
  uint dtick = ctick - m_stick;
  mtx4 *mtx;

  if(!m_mesh || !m_canim)
	  return;

  if(ctick - m_atick < 30)
	  return;

  if(dtick > (m_canim->m_end - m_canim->m_start)){
   dtick = 0;
   m_stick = kgmTime::getTicks();
  }

  mtx = new mtx4[m_objects];
  int i = 0;
  for(i = 0; i < m_objects; i++){
   kgmAnimation::Framed* framed = m_canim->getFramed(m_mesh->bn[i].name);
   if(!framed)
	   mtx[i] = mtx4();
   else
       framed->getFrame(&mtx[i], dtick);
  }

  for(i = 0; i < m_mverts.count(); i++){
   vec3 v; 
   for(int j = 0; j < m_mesh->sv[i].nw; j++){
    int i_bn = m_mesh->sv[i].i[j];
	float w_bn = m_mesh->sv[i].w[j];
    vec3 bw_pos;
    quat bw_rot;
    
	if(i_bn >= m_mesh->bn.size())
		continue;

	bw_pos = m_imtx[i_bn] * m_mverts[i];
	bw_pos = mtx[i_bn] * bw_pos;
	bw_pos = bw_pos * w_bn;
    v = v +  bw_pos;
   }
   
   m_mesh->v[i].pos = v;
//   uint bi = m_mesh->sv[i].i[0];
//   if(bi >= m_objects)
//	   continue;
//   mtx4 m  = m_imtx[bi] * mtx[bi];
//   vec3 vv = m_mverts[i];
//   m_mesh->v[i].pos = m * m_mverts[i];
  }

  delete [] mtx;
  m_atick = kgmTime::getTicks();
 }
//*/
// void animate(){
// }
 void setAnimation(kgmAnimation* anim){
  m_canim = anim;
  m_stick = kgmTime::getTicks();
 }
};
