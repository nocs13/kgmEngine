#ifndef KGMNODE_H
#define KGMNODE_H

#include "kgmIGraphics.h"
#include "../kgmBase/kgmObject.h"
#include "../kgmMath/kgmMatrix3d.h"

class kgmNode: public kgmIGraphics::INode, public kgmObject
{
  KGM_OBJECT(kgmNode);

protected:
  mtx4 m_transform;

public:
  kgmNode();

  kgmObject* getNodeObject()
  {
    return null;
  }

  kgmIGraphics::TypeNode getNodeType()
  {
    return kgmIGraphics::NodeNone;
  }

  bool isNodeValid()
  {
    return false;
  }

  mtx4 getNodeTransform()
  {
    return m_transform;
  }
};

#endif // KGMNODE_H
