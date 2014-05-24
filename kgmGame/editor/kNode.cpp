#include "kNode.h"
#include "../../kgmGame/kgmGameApp.h"
#include "../../kgmGame/kgmGameBase.h"

using namespace kgmGameEditor;

void kNode::setPosition(vec3 v)
{
  pos = v;

  if(icn)
    icn->setPosition(v);

  switch(typ)
  {
  case LIGHT:
    lgt->position = v;
    break;
  case MESH:
  {
    mtx4 mrot, mpos, mtrn;

    mrot.rotate(rot);
    mpos.translate(pos);
    mtrn = mrot * mpos;

    ((kgmGameBase*)kgmGameApp::gameApplication()->game())->getRender()->set(msh, mtrn);
  }
    break;
  }
}

void kNode::setRotation(vec3 r)
{
  rot = r;

  switch(typ)
  {
  case LIGHT:
    break;
  case MESH:
  {
    mtx4 mrot, mpos, mtrn;

    mrot.rotate(rot);
    mpos.translate(pos);
    mtrn = mrot * mpos;

    ((kgmGameBase*)kgmGameApp::gameApplication()->game())->getRender()->set(msh, mtrn);
  }
    break;
  }
}
