#include "kNode.h"
#include "../../kgmGame/kgmGameApp.h"
#include "../../kgmGame/kgmGameBase.h"

using namespace kgmGameEditor;

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
    ((kgmGameBase*)kgmGameApp::gameApplication()->game())->getRender()->set(geo, mtrn);

  switch(typ)
  {
  case LIGHT:
    lgt->position = v;
    lgt->direction = vec3(1, 0, 0);
    lgt->direction = getMatrix() * lgt->direction;
    break;
  case MESH:
  {
    ((kgmGameBase*)kgmGameApp::gameApplication()->game())->getRender()->set(msh, mtrn);
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
    ((kgmGameBase*)kgmGameApp::gameApplication()->game())->getRender()->set(geo, mtrn);

  switch(typ)
  {
  case LIGHT:
    lgt->direction = vec3(1, 0, 0);
    lgt->direction = getMatrix() * lgt->direction;
    break;
  case MESH:
  {
    ((kgmGameBase*)kgmGameApp::gameApplication()->game())->getRender()->set(msh, mtrn);
  }
    break;
  }
}

void kNode::setMaterial(kgmString m)
{
  if(typ == MESH)// || typ == ACTOR
  {
    kgmMaterial* mtl = ((kgmGameBase*)kgmGameApp::gameApplication()->game())->getResources()->getMaterial(m.data());

    ((kgmGameBase*)kgmGameApp::gameApplication()->game())->getRender()->set(msh, mtl);

    mat = m;
  }
}
