#ifndef KNODE_H
#define KNODE_H

#include "../../kgmBase/kgmObject.h"
#include "../../kgmGraphics/kgmMesh.h"
#include "../../kgmGraphics/kgmLight.h"
#include "../../kgmGraphics/kgmMaterial.h"
#include "../../kgmGraphics/kgmGraphics.h"

#include "../../kgmGame/kgmSensor.h"
#include "../../kgmGame/kgmTrigger.h"

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
    TRIGGER,
    MATERIAL
  };

  union
  {
    kgmMesh*      msh;
    kgmUnit*      act;
    kgmLight*     lgt;
    kgmSensor*    sns;
    kgmTrigger*   trg;
    kgmMaterial*  mtl;
  };

  Type typ;
  vec3 pos;
  vec3 rot;
  box3 bnd;
  kgmString nam;
  kgmString lnk;

  kNode()
  {
    typ = NONE;
    msh = null;
  }

  kNode(kgmMesh* m)
  {
    typ = MESH;
    msh = m;
  }

  kNode(kgmLight* l)
  {
    typ = LIGHT;
    lgt = l;
  }

  kNode(kgmUnit* a)
  {
    typ = ACTOR;
    act = a;
  }

  kNode(kgmSensor* s)
  {
    typ = SENSOR;
    sns = s;
  }

  kNode(kgmTrigger* t)
  {
    typ = TRIGGER;
    trg = t;
  }

  kNode(kgmMaterial* m)
  {
    typ = MATERIAL;
    mtl = m;
  }

  ~kNode()
  {
    kgmObject* o = (kgmObject*)msh;

    if(o)
      o->release();
  }

  void setPosition(vec3 v);
  void setRotation(vec3 r);
};

#endif // KNODE_H
