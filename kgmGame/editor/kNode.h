#ifndef KNODE_H
#define KNODE_H

#include "../../kgmBase/kgmObject.h"
#include "../../kgmBase/kgmPointer.h"

#include "../../kgmGraphics/kgmMesh.h"
#include "../../kgmGraphics/kgmLight.h"
#include "../../kgmGraphics/kgmVisual.h"
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
    UNIT,
    LIGHT,
    ACTOR,
    VISUAL,
    EFFECT,
    SENSOR,
    TRIGGER,
    OBSTACLE
  };

  union
  {
    kgm_ptr<kgmUnit>      unt;
    kgm_ptr<kgmLight>     lgt;
    kgm_ptr<kgmActor>     act;
    kgm_ptr<kgmVisual>    vis;
    kgm_ptr<kgmEffect>    eff;
    kgm_ptr<kgmSensor>    sns;
    kgm_ptr<kgmTrigger>   trg;
    kgm_ptr<kgmObstacle>  obs;

    kgm_ptr<kgmObject>    obj;
  };

  Type typ;

  vec3 pos;
  vec3 rot;

  box3 bnd;

  kgmString nam;
  kgmString lnk;
  kgmString shp;
  kgmString ini;

  bool col, lock;

  kgm_ptr<kgmIcon>   icn;
  kgm_ptr<kgmVisual> geo;
  //kgm_ptr<kgmIcon> icn;
  //kgm_ptr<kgmVisual*> geo;

private:
  ~kNode()
  {
  }

public:
  kNode()
  {
    typ  = NONE;
    obj  = null;
    col  = false;
    lock = false;
  }

  kNode(kgm_ptr<kgmVisual> v)
  {
    typ = VISUAL;
    vis = v;
    col = false;
    lock = false;
  }

  kNode(kgm_ptr<kgmUnit> u)
  {
    typ  = UNIT;
    unt  = u;
    col  = false;
    lock = false;
  }

  kNode(kgm_ptr<kgmLight> l)
  {
    typ  = LIGHT;
    lgt  = l;
    col  = false;
    lock = false;
  }

  kNode(kgm_ptr<kgmActor> a)
  {
    typ = ACTOR;
    act = a;
    col = false;
    lock = false;
  }

  kNode(kgm_ptr<kgmEffect> e)
  {
    typ = EFFECT;
    eff = e;
    col = false;
    lock = false;
  }

  kNode(kgm_ptr<kgmSensor> s)
  {
    typ = SENSOR;
    sns = s;
    col = false;
    lock = false;
  }

  kNode(kgm_ptr<kgmTrigger> t)
  {
    typ = TRIGGER;
    trg = t;
    col = false;
    lock = false;
  }

  kNode(kgm_ptr<kgmObstacle> o)
  {
    typ = OBSTACLE;
    obs = o;
    col = true;
    lock = false;
  }

  kNode(const kNode& n)
  {
    typ = n.typ;
    col = n.col;
    shp = n.shp;
    pos = n.pos;
    rot = n.rot;
    bnd = n.bnd;
    lnk = n.lnk;
    lock = n.lock;

    if(n.icn != null)
      icn = kgm_ptr<kgmIcon>((kgmIcon*)n.geo->clone());

    if(n.geo != null)
      geo = kgm_ptr<kgmVisual>((kgmVisual*)n.geo->clone());

    if(n.obj)
    {
      switch(n.typ)
      {
      case UNIT:
        unt = kgm_ptr<kgmUnit>((kgmUnit*)n.unt->clone());
        break;
      case LIGHT:
        lgt = kgm_ptr<kgmLight>((kgmLight*)n.lgt->clone());
        break;
      case ACTOR:
        act = kgm_ptr<kgmActor>((kgmActor*)n.act->clone());
        break;
      case VISUAL:
        vis = kgm_ptr<kgmVisual>((kgmVisual*)n.vis->clone());
        break;
      case EFFECT:
        eff = kgm_ptr<kgmEffect>((kgmEffect*)n.eff->clone());
        break;
      case SENSOR:
        sns = kgm_ptr<kgmSensor>((kgmSensor*)n.sns->clone());
        break;
      case TRIGGER:
        trg = kgm_ptr<kgmTrigger>((kgmTrigger*)n.trg->clone());
        break;
      case OBSTACLE:
        obs = kgm_ptr<kgmObstacle>((kgmObstacle*)n.obs->clone());
        break;
      };
    }
  }

  mtx4 getMatrix();
  vec3 getPosition();
  vec3 getRotation();
  void setPosition(vec3 v);
  void setRotation(vec3 r);
  void setConvex(kgmString s);

  void setMaterial(kgmMaterial* m);
  kgmMaterial* getMaterial();
};

}

#endif // KNODE_H
