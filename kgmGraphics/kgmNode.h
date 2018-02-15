#ifndef KGMNODE_H
#define KGMNODE_H

#include "kgmIGraphics.h"
#include "../kgmBase/kgmObject.h"
#include "../kgmMath/kgmMatrix3d.h"
#include "../kgmMath/kgmVector3d.h"
#include "../kgmMath/kgmBox3d.h"

class kgmNode: public kgmIGraphics::INode, public kgmObject
{
  KGM_OBJECT(kgmNode);

protected:
  mtx4 m_transform;
  vec3 m_position;

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

  vec3 getNodePosition()
  {
    return m_position;
  }

  mtx4 getNodeTransform()
  {
    return m_transform;
  }

  box3 getNodeBound()
  {
    box3 bb;

    return bb;
  }

  void setNodeMaterial(kgmMaterial*)
  {

  }

  kgmMaterial* getNodeMaterial()
  {
    return null;
  }
};

#endif // KGMNODE_H
