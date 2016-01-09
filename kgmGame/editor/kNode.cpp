#include "kNode.h"
#include "../../kgmGame/kgmGameApp.h"
#include "../../kgmGame/kgmGameBase.h"

using namespace kgmGameEditor;

KGMOBJECT_IMPLEMENT(kNode, kgmObject);

void kNode::update()
{
  switch(typ)
  {
  case UNIT:
  case ACTOR:
  case EFFECT:
  case SENSOR:
  case TRIGGER:

    if(unt->getBody())
    {
      pos = unt->getPosition();
      rot = unt->getRotation();
    }

    if(icn)
      icn->setPosition(pos);

    break;
  }
}

mtx4 kNode::getMatrix()
{
  mtx4 mrot, mpos;

  mrot.rotate(rot);
  mpos.translate(pos);

  return (mrot * mpos);
}

void kNode::setPosition(vec3 v)
{
  pos = v;

  mtx4 mrot, mpos, mtrn;

  mrot.identity();
  mrot.rotate(rot);

  mpos.identity();
  mpos.translate(pos);
  mtrn = mrot * mpos;

  if(icn)
    icn->setPosition(v);

  switch(typ)
  {
  case LIGHT:
    lgt->position = v;
    lgt->direction = vec3(1, 0, 0);
    lgt->direction = getMatrix() * lgt->direction;
    break;
  case VISUAL:
    vis->set(mtrn);
    break;
  case UNIT:
  case ACTOR:
  case EFFECT:
  case SENSOR:
  case TRIGGER:
    unt->setPosition(pos);
    unt->setRotation(rot);

    if(unt->getVisual())
    {
      kgmVisual* vis = unt->getVisual();

      vis->getTransform() = mtrn;
    }
    break;
  case OBSTACLE:
    obs->set(mtrn);
    break;
  }
}

void kNode::setRotation(vec3 r)
{
  rot = r;

  mtx4 mrot, mpos, mtrn;

  mrot.identity();
  mrot.rotate(rot);

  mpos.identity();
  mpos.translate(pos);
  mtrn = mrot * mpos;

  switch(typ)
  {
  case LIGHT:
    lgt->direction = vec3(1, 0, 0);
    lgt->direction = getMatrix() * lgt->direction;
    break;
  case VISUAL:
    vis->set(mtrn);
    break;
  case UNIT:
  case ACTOR:
  case EFFECT:
  case SENSOR:
  case TRIGGER:
    unt->setPosition(pos);
    unt->setRotation(rot);

    if(unt->getVisual())
    {
      kgmVisual* vis = unt->getVisual();

      vis->getTransform() = mtrn;
    }
    break;
  case OBSTACLE:
    obs->set(mtrn);
    break;
  }
}

vec3 kNode::getPosition()
{
  return pos;
}

vec3 kNode::getRotation()
{
  return rot;
}

void kNode::setConvex(kgmString s)
{
}

kgmMaterial* kNode::getMaterial()
{
  switch(typ)
  {
  case VISUAL:
  {
    return vis->getMaterial();

    break;
  }
  case UNIT:
  case ACTOR:
  case EFFECT:
  case SENSOR:
  case TRIGGER:
  {
    kgmVisual* v = unt->getVisual();

    if(v)
      v->getMaterial();

    break;
  }
  default:
    return null;
  }

  return null;
}

void kNode::setMaterial(kgmMaterial* m)
{
  switch(typ)
  {
  case VISUAL:
  {
    return vis->set(m);

    break;
  }
  case UNIT:
  case ACTOR:
  case EFFECT:
  case SENSOR:
  case TRIGGER:
  {
    kgmVisual* v = unt->getVisual();

    if(v)
      v->set(m);

    break;
  }
  default:
    break;
  }
}
