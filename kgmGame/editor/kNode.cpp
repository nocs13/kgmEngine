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
  case ACTOR:
  case SENSOR:
  case TRIGGER:
    trg->setPosition(pos);
    trg->setRotation(rot);

    if(obj->getVisual())
    {
      kgmVisual* vis = obj->getVisual();

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
  case ACTOR:
  case SENSOR:
  case TRIGGER:
    trg->setPosition(pos);
    trg->setRotation(rot);

    if(obj->getVisual())
    {
      kgmVisual* vis = obj->getVisual();

      vis->getTransform() = mtrn;
    }
    break;
  }
}

void kNode::setMaterial(kgmString m)
{
  if(typ == MESH)
  {
    kgmMaterial* mtl = ((kgmGameBase*)kgmGameApp::gameApplication()->game())->getResources()->getMaterial(m.data());

    if(msh && mtl)
    {
      msh->set(mtl);
      mat = m;
    }
  }
  else if(typ == ACTOR)
  {

  }
}

void kNode::setShader(kgmString s)
{
  if(typ == MESH)
  {
    kgmMaterial* mtl = ((kgmGameBase*)kgmGameApp::gameApplication()->game())->getResources()->getMaterial(mat.data());

    if(mtl)
    {
      mtl->setShader(kgmShader::toType(s));
      shd = s;
    }
  }
  else if(typ == ACTOR)
  {

  }
}

void kNode::setConvex(kgmString s)
{
  if(typ == OBSTACLE)
  {
  }
}
