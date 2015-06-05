#include "kNode.h"
#include "../../kgmGame/kgmGameApp.h"
#include "../../kgmGame/kgmGameBase.h"

using namespace kgmGameEditor;

KGMOBJECT_IMPLEMENT(kNode, kgmObject);

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

  if(geo)
    geo->set(mtrn);

  switch(typ)
  {
  case LIGHT:
    lgt->position = v;
    lgt->direction = vec3(1, 0, 0);
    lgt->direction = getMatrix() * lgt->direction;
    break;
  case MESH:
    msh->set(mtrn);
    break;
  case UNIT:
  case ACTOR:
  case EFFECT:
  case SENSOR:
  case TRIGGER:
    trg->setPosition(pos);
    trg->setRotation(rot);

    if(trg->getVisual())
    {
      kgmVisual* vis = trg->getVisual();

      vis->getTransform() = mtrn;
    }
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

  if(geo)
    geo->set(mtrn);

  switch(typ)
  {
  case LIGHT:
    lgt->direction = vec3(1, 0, 0);
    lgt->direction = getMatrix() * lgt->direction;
    break;
  case MESH:
    msh->set(mtrn);
    break;
  case UNIT:
  case ACTOR:
  case EFFECT:
  case SENSOR:
  case TRIGGER:
    trg->setPosition(pos);
    trg->setRotation(rot);

    if(trg->getVisual())
    {
      kgmVisual* vis = trg->getVisual();

      vis->getTransform() = mtrn;
    }
    break;
  }
}

void kNode::setConvex(kgmString s)
{
  if(typ == OBSTACLE)
  {
  }
}

kgmMaterial* kNode::getMaterial()
{
  switch(typ)
  {
   case MESH:
    {
      return msh->getMaterial();

      break;
    }
   case UNIT:
   case ACTOR:
   case EFFECT:
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
    case MESH:
    {
      return msh->set(m);

      break;
    }
    case UNIT:
    case ACTOR:
    case EFFECT:
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
