#ifndef KNODE_H
#define KNODE_H

#include "../../kgmBase/kgmObject.h"
#include "../../kgmGraphics/kgmMesh.h"
#include "../../kgmGraphics/kgmLight.h"
#include "../../kgmGraphics/kgmMaterial.h"
#include "../../kgmGraphics/kgmGraphics.h"

#include "../../kgmGame/kgmActor.h"
#include "../../kgmGame/kgmSensor.h"
#include "../../kgmGame/kgmTrigger.h"

namespace kgmGameEditor
{

class kNode : public kgmObject
{
public:
  enum Type
  {
    NONE,
    MESH,
    LIGHT,
    ACTOR,
    SENSOR,
    OBJECT,
    TRIGGER
  };

  union
  {
    kgmMesh*        msh;
    kgmLight*       lgt;
    kgmActor*       act;
    kgmSensor*      sns;
    kgmTrigger*     trg;
    kgmGameObject*  obj;
  };

  Type typ;

  vec3 pos;
  vec3 rot;

  box3 bnd;

  kgmString nam;
  kgmString lnk;
  kgmString mat;

  bool col, lock;

  kgmGraphics::Icon* icn;
  kgmMesh*           geo;

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
    msh = m;
    icn = null;
    geo = null;
    col = false;
    lock = false;
  }

  kNode(kgmLight* l)
  {
    typ = LIGHT;
    lgt = l;
    icn = null;
    geo = null;
    col = false;
    lock = false;
  }

  kNode(kgmActor* a)
  {
    typ = ACTOR;
    act = a;
    icn = null;
    geo = null;
    col = false;
    lock = false;
  }

  kNode(kgmSensor* s)
  {
    typ = SENSOR;
    sns = s;
    icn = null;
    geo = null;
    col = false;
    lock = false;
  }

  kNode(kgmTrigger* t)
  {
    typ = TRIGGER;
    trg = t;
    icn = null;
    geo = null;
    col = false;
    lock = false;
  }

  kNode(kgmGameObject* o)
  {
    typ = OBJECT;
    obj = o;
    icn = null;
    geo = null;
    col = false;
    lock = false;
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
};

}

#endif // KNODE_H
