#pragma once
#include "../kgmMain/kgmTime.h"

#include "../kgmGraphics/kgmGraphical.h"
#include "../kgmGraphics/kgmMesh.h"
#include "../kgmGraphics/kgmPolygon.h"
#include "../kgmGraphics/kgmTexture.h"
#include "../kgmGraphics/kgmShader.h"
#include "../kgmGraphics/kgmMaterial.h"

#include "../kgmGraphics/kgmAnimation.h"
#include "../kgmGraphics/kgmDummy.h"
#include "../kgmGraphics/kgmCamera.h"

#include "../kgmMain/kgmSound.h"

#include "../kgmPhysics/kgmBody.h"
//#include "../kgmGraphics/kgmConstraint.h"
#include "kgmGamePhysics.h"



class kgmActor: public kgmObject, public kgmGraphical, public kgmBody{
  KGM_OBJECT(kgmActor)

public:
#ifdef KGM_VBO_MESH
  typedef struct {
    void* mesh;
    uint vertices, faces, vsize, fsize;
  } VBOMesh;
#endif

  struct Section{
    kgmMaterial*      material;
#ifdef KGM_VBO_MESH
    kgmList<VBOMesh>  meshes;
#else
    kgmList<kgmMesh*> meshes;
#endif

    kgmAnimation*     animation;

    kgmSkeleton*      skeleton;
    kgmList<mtx4>     mjoints;

    void clear(){

    }
  };

protected:
  kgmActor* 	     m_parent;
  kgmList<kgmActor*> m_childs;

public:
  uint	 m_uid;
  uint	 m_type;
  uint	 m_state;
  uint	 m_bearing;

  bool   m_enable;
  bool	 m_active;
  bool	 m_remove;
  bool	 m_culled;

  float	 m_life;
  float  m_birth;

  kgmString m_id;
  kgmString m_name;

  kgmList<Section>    m_sections;
  kgmList<kgmDummy*>  m_dummies;

  bool                m_animation, m_animation_loop;
  float               m_animation_start, m_animation_end, m_animation_frame;

public:
  kgmActor();
  virtual ~kgmActor();

  virtual void update(float time);
  virtual void prepare();		//call before game level starting

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

  void setParent(kgmActor* a);
  void addChild(kgmActor* a);
  void delChild(kgmActor* a);

  void addSection(){
    Section s;
    s.material  = 0;
    s.animation = 0;
    s.skeleton  = 0;

    m_sections.add(s);
  }

  void set(uint sid, kgmSkeleton* s){
    if(!s || sid >= m_sections.size())
      return;

    Section& p = m_sections[sid];
    p.skeleton = s;
    for(int i = 0; i < s->m_joints.size(); i++)
      p.mjoints.add(mtx4());
  }

  void set(uint sid, kgmAnimation* a){
    if(!a || sid >= m_sections.size())
      return;

    m_sections[sid].animation = a;
  }

  void set(uint sid, kgmMaterial* m){
    if(!m || sid >= m_sections.size())
      return;

    m_sections[sid].material = m;
  }

  void add(kgmDummy* m){
    if(m)
      m_dummies.add(m);
  }

  void add(uint sid, kgmMesh* m){
    if(!m || sid >= m_sections.size())
      return;

    m_sections[sid].meshes.add(m);
  }

  void animate(float sf=0, float ef=0, bool loop=false){
   m_animation_start   = m_animation_frame = sf;
   m_animation_end     = ef;
   m_animation_loop    = loop;
   m_animation         = true;
  }

  uint sections(){
    return m_sections.size();
  }

  Section& section(uint i){
    return m_sections[i];
  }
};

typedef kgmList<kgmActor*> kgmActors;
typedef kgmList<kgmActor*> kgmActorList;
