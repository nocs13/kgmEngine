#pragma once
#include "../kgmBase/kgmTab.h"
#include "../kgmBase/kgmTime.h"

#include "../kgmGraphics/kgmGraphical.h"
#include "../kgmGraphics/kgmMesh.h"
#include "../kgmGraphics/kgmPolygon.h"
#include "../kgmGraphics/kgmTexture.h"
#include "../kgmGraphics/kgmShader.h"
#include "../kgmGraphics/kgmMaterial.h"

#include "../kgmGraphics/kgmAnimation.h"
#include "../kgmGraphics/kgmDummy.h"
#include "../kgmGraphics/kgmCamera.h"

#include "../kgmBase/kgmSound.h"

#include "../kgmPhysics/kgmBody.h"
#include "kgmGamePhysics.h"



class kgmActor: public kgmObject{
  KGM_OBJECT(kgmActor)
public:
  struct Mesh{
    void* mesh;
    u32 vertices,
         faces,
         vsize,
         fsize,
         fvf;
  };

  struct Section
  {
    kgmMaterial*      material;
    //kgmList<Mesh>     meshes;
    kgmMesh* mesh;
    kgmAnimation*     animation;

    kgmSkeleton*      skeleton;
    //kgmList<mtx4>     mjoints;

    void clear(){

    }
  };

  class State
  {
   public:
    kgmString id;
    u32       priopity;

    kgmTab<kgmString, kgmString> options;
  };

protected:
  kgmActor* 	        m_parent;
  kgmList<kgmActor*>  m_childs;

public:
  u32  m_uid;
  u32  m_type;
  u32  m_state;
  u32  m_bearing;

  bool    m_enable;
  bool    m_visible;
  bool    m_active;
  bool    m_remove;
  bool    m_culled;

  float   m_life;
  float   m_birth;

  mtx4    m_transform;

  kgmString m_id;
  kgmString m_name;

  kgmBody             m_body;

  kgmList<Section>    m_sections;
  kgmList<kgmDummy*>  m_dummies;

  bool                m_animation, m_animation_loop;
  float               m_animation_start, m_animation_end, m_animation_frame;

public:
  kgmActor();
  virtual ~kgmActor();

  virtual void init();
  virtual void exit();
  virtual void tick(u32);

  void remove();

  void enable(bool s);
  void active(bool a);
  void visible(bool a);

  bool enabled();
  bool removed();

  void setPosition(vec3& v);
  void setRotation(vec3& r);
  void setRotation(quat& r);
  void setDirection(vec3& d);
  void setForce(float f, vec3& dir);

  void setParent(kgmActor* a);
  void addChild(kgmActor* a);
  void delChild(kgmActor* a);

  void addSection(kgmMaterial* mtl = 0, kgmMesh* m = 0, kgmSkeleton* s = 0, kgmAnimation* a = 0){
    Section sec;
    sec.material  = mtl;
    sec.animation = a;
    sec.skeleton  = s;
    sec.mesh      = m;

    m_sections.add(sec);
  }

  void set(u32 sid, kgmSkeleton* s){
    if(!s || sid >= m_sections.size())
      return;

    Section& p = m_sections[sid];
    p.skeleton = s;
    //p.mjoints.clear();
    //for(int i = 0; i < s->m_joints.size(); i++)
    //  p.mjoints.add(mtx4());
  }

  void set(u32 sid, kgmAnimation* a){
    if(!a || sid >= m_sections.size())
      return;

    m_sections[sid].animation = a;
  }

  void set(u32 sid, kgmMaterial* m){
    if(!m || sid >= m_sections.size())
      return;

    m_sections[sid].material = m;
  }

  void add(kgmDummy* m){
    if(m)
      m_dummies.add(m);
  }

  void add(u32 sid, kgmMesh* m){
    if(!m || sid >= m_sections.size())
      return;

    //m_sections[sid].meshes.add(m);
  }

  void animate(float sf=0, float ef=0, bool loop=false){
   m_animation_start   = m_animation_frame = sf;
   m_animation_end     = ef;
   m_animation_loop    = loop;
   m_animation         = true;
  }

  u32 sections(){
    return m_sections.size();
  }

  Section& section(u32 i){
    return m_sections[i];
  }
};

typedef kgmList<kgmActor*> kgmActors;
typedef kgmList<kgmActor*> kgmActorList;
