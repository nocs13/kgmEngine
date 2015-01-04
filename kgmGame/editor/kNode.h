#ifndef KNODE_H
#define KNODE_H

#include "../../kgmBase/kgmObject.h"
#include "../../kgmGraphics/kgmMesh.h"
#include "../../kgmGraphics/kgmLight.h"
#include "../../kgmGraphics/kgmMaterial.h"
#include "../../kgmGraphics/kgmGraphics.h"

#include "../../kgmGame/kgmUnit.h"
#include "../../kgmGame/kgmActor.h"
#include "../../kgmGame/kgmEffect.h"
#include "../../kgmGame/kgmSensor.h"
#include "../../kgmGame/kgmTrigger.h"

#include "../../kgmPhysics/kgmObstacle.h"

namespace kgmGameEditor
{

class kNode : public kgmObject
{
  KGM_OBJECT(kNode);
public:
  enum Type
  {
    NONE,
    MESH,
    UNIT,
    LIGHT,
    ACTOR,
    EFFECT,
    SENSOR,
    TRIGGER,
    OBSTACLE
  };

  union
  {
    kgmUnit*        unt;
    kgmLight*       lgt;
    kgmActor*       act;
    kgmVisual*      msh;
    kgmEffect*      eff;
    kgmSensor*      sns;
    kgmTrigger*     trg;
    kgmObstacle*    obs;
    kgmGameObject*  obj;
  };

  Type typ;

  vec3 pos;
  vec3 rot;

  box3 bnd;

  kgmString nam;
  kgmString lnk;
  kgmString mat;
  kgmString shd;
  kgmString shp;
  kgmString ini;

  bool col, lock;

  kgmGraphics::Icon* icn;
  kgmVisual*         geo;

  kNode()
  {
    typ = NONE;
    msh = null;
    icn = null;
    geo = null;
    col = false;
    lock = false;
  }

  kNode(kgmMesh* m)
  {
    typ = MESH;
    msh = new kgmVisual();
    msh->set(m);
    icn = null;
    geo = null;
    col = false;
    lock = false;

    obj->increment();
  }

  kNode(kgmUnit* u)
  {
    typ = UNIT;
    unt = u;
    icn = null;
    geo = null;
    col = false;
    lock = false;

    obj->increment();
  }

  kNode(kgmLight* l)
  {
    typ = LIGHT;
    lgt = l;
    icn = null;
    geo = null;
    col = false;
    lock = false;

    obj->increment();
  }

  kNode(kgmActor* a)
  {
    typ = ACTOR;
    act = a;
    icn = null;
    geo = null;
    col = false;
    lock = false;

    obj->increment();
  }

  kNode(kgmEffect* e)
  {
    typ = SENSOR;
    eff = e;
    icn = null;
    geo = null;
    col = false;
    lock = false;

    obj->increment();
  }

  kNode(kgmSensor* s)
  {
    typ = SENSOR;
    sns = s;
    icn = null;
    geo = null;
    col = false;
    lock = false;

    obj->increment();
  }

  kNode(kgmTrigger* t)
  {
    typ = TRIGGER;
    trg = t;
    icn = null;
    geo = null;
    col = false;
    lock = false;

    obj->increment();
  }

  kNode(kgmObstacle* o)
  {
    typ = OBSTACLE;
    obs = o;
    icn = null;
    geo = null;
    col = true;
    lock = false;

    obj->increment();
  }

  kNode(const kNode& n)
  {
    typ = n.typ;
    col = n.col;
    shp = n.shp;
    shd = n.shd;
    pos = n.pos;
    rot = n.rot;
    bnd = n.bnd;
    lnk = n.lnk;
    lock = n.lock;

    obj = null;
    icn = null;
    geo = null;

    if(n.obj)
      obj = (kgmGameObject*)n.obj->clone();
  }

  ~kNode()
  {
    kgmObject* o = (kgmObject*)msh;

    if(o)
      o->release();

    if(icn)
      icn->release();

    if(geo)
      geo->release();
  }

  mtx4 getMatrix();
  void setPosition(vec3 v);
  void setRotation(vec3 r);
  void setMaterial(kgmString m);
  void setShader(kgmString s);
  void setConvex(kgmString s);
};

}

#endif // KNODE_H
