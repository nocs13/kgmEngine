#ifndef KNODE_H
#define KNODE_H

#include "../../kgmBase/kgmObject.h"
#include "../../kgmBase/kgmPointer.h"

#include "../../kgmGraphics/kgmMesh.h"
#include "../../kgmGraphics/kgmLight.h"
#include "../../kgmGraphics/kgmVisual.h"
#include "../../kgmGraphics/kgmMaterial.h"
#include "../../kgmGraphics/kgmGraphics.h"

#include "../../kgmGame/kgmIGame.h"
#include "../../kgmGame/kgmUnit.h"
#include "../../kgmGame/kgmActor.h"
#include "../../kgmGame/kgmEffect.h"
#include "../../kgmGame/kgmSensor.h"
#include "../../kgmGame/kgmTrigger.h"

#include "../../kgmPhysics/kgmObstacle.h"

namespace kgmGameEditor
{

#define BODY_ICO     "body_ico.tga"
#define UNIT_ICO     "unit_ico.tga"
#define LIGHT_ICO    "light_ico.tga"
#define ACTOR_ICO    "actor_ico.tga"
#define VISUAL_ICO   "visual_ico.tga"
#define EFFECT_ICO   "effect_ico.tga"
#define SENSOR_ICO   "sensor_ico.tga"
#define TRIGGER_ICO  "trigger_ico.tga"
#define OBSTACLE_ICO "obstacle_ico.tga"

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
    kgmUnit*      unt;
    kgmLight*     lgt;
    kgmActor*     act;
    kgmVisual*    vis;
    kgmEffect*    eff;
    kgmSensor*    sns;
    kgmTrigger*   trg;
    kgmObstacle*  obs;

    kgmObject*  obj = null;
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

  kgmIcon*   icn = null;

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

    icn = null;
  }

  kNode(kgmVisual* v)
  {
    typ = VISUAL;
    vis = v;
    col = false;
    lock = false;

    icn = new kgmIcon(kgmIGame::getGame()->getResources()->getTexture(VISUAL_ICO));
  }

  kNode(kgmUnit* u)
  {
    typ  = UNIT;
    unt  = u;
    col  = false;
    lock = false;

    icn = new kgmIcon(kgmIGame::getGame()->getResources()->getTexture(UNIT_ICO));
  }

  kNode(kgmLight* l)
  {
    typ  = LIGHT;
    lgt  = l;
    col  = false;
    lock = false;

    icn = new kgmIcon(kgmIGame::getGame()->getResources()->getTexture(LIGHT_ICO));
  }

  kNode(kgmActor* a)
  {
    typ = ACTOR;
    act = a;
    col = false;
    lock = false;

    icn = new kgmIcon(kgmIGame::getGame()->getResources()->getTexture(ACTOR_ICO));
  }

  kNode(kgmEffect* e)
  {
    typ = EFFECT;
    eff = e;
    col = false;
    lock = false;

    icn = new kgmIcon(kgmIGame::getGame()->getResources()->getTexture(EFFECT_ICO));
  }

  kNode(kgmSensor* s)
  {
    typ = SENSOR;
    sns = s;
    col = false;
    lock = false;

    icn = new kgmIcon(kgmIGame::getGame()->getResources()->getTexture(SENSOR_ICO));
  }

  kNode(kgmTrigger* t)
  {
    typ = TRIGGER;
    trg = t;
    col = false;
    lock = false;

    icn = new kgmIcon(kgmIGame::getGame()->getResources()->getTexture(TRIGGER_ICO));
  }

  kNode(kgmObstacle* o)
  {
    typ = OBSTACLE;
    obs = o;
    col = true;
    lock = false;

    icn = new kgmIcon(kgmIGame::getGame()->getResources()->getTexture(OBSTACLE_ICO));
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
      icn = (kgmIcon*) n.icn->clone();

    //if(n.geo != null)
    //  geo = (kgmVisual*) n.geo->clone();

    if(n.obj)
    {
      switch(n.typ)
      {
      case UNIT:
        unt = (kgmUnit*) n.unt->clone();
        break;
      case LIGHT:
        lgt = (kgmLight*) n.lgt->clone();
        break;
      case ACTOR:
        act = (kgmActor*) n.act->clone();
        break;
      case VISUAL:
        vis = (kgmVisual*) n.vis->clone();
        break;
      case EFFECT:
        eff = (kgmEffect*) n.eff->clone();
        break;
      case SENSOR:
        sns = (kgmSensor*) n.sns->clone();
        break;
      case TRIGGER:
        trg = (kgmTrigger*) n.trg->clone();
        break;
      case OBSTACLE:
        obs = (kgmObstacle*) n.obs->clone();
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
