#ifndef KNODE_H
#define KNODE_H

#include "../../kgmBase/kgmObject.h"
#include "../../kgmGraphics/kgmMesh.h"
#include "../../kgmGraphics/kgmLight.h"
#include "../../kgmGraphics/kgmMaterial.h"
#include "../../kgmGraphics/kgmGraphics.h"

#include "../../kgmGame/kgmActor.h"

class kNode : public kgmObject
{
public:
  enum Type
  {
    NONE,
    MESH,
    LIGHT,
    ACTOR,
    MATERIAL
  };

  union
  {
    kgmMesh*      msh;
    kgmLight*     lgt;
    kgmActor*     act;
    kgmMaterial*  mtl;
  };

  Type typ;
  vec3 pos;
  vec3 rot;
  box3 bnd;
  kgmString nam;
  kgmString lnk;

  kgmGraphics::Icon* icn;

  kNode()
  {
    typ = NONE;
    msh = null;
    icn = null;
  }

  kNode(kgmMesh* m)
  {
    typ = MESH;
    msh = m;
    icn = null;
  }

  kNode(kgmLight* l)
  {
    typ = LIGHT;
    lgt = l;
    icn = null;
  }

  kNode(kgmActor* a)
  {
    typ = ACTOR;
    act = a;
    icn = null;
  }

  kNode(kgmMaterial* m)
  {
    typ = MATERIAL;
    mtl = m;
    icn = null;
  }

  ~kNode()
  {
    kgmObject* o = (kgmObject*)msh;

    if(o)
      o->release();

    if(icn)
      icn->release();
  }

  void setPosition(vec3 v)
  {
    pos = v;

    if(icn)
      icn->setPosition(v);

    switch(typ)
    {
    case LIGHT:
      lgt->position = v;
      break;
    }
  }

  void setRotation(vec3 r)
  {
    rot = r;
  }
};

#endif // KNODE_H
