#pragma once
#include "../kgmGraphics/kgmNode.h"

class kgmUnit;

class kgmGNode : public kgmNode
{
  kgmUnit*   m_unit;

public:
  kgmGNode(kgmUnit* u, kgmObject* n, kgmIGraphics::TypeNode t);
  ~kgmGNode();

  box3       getNodeBound()     override;
  vec3       getNodePosition()  override ;
  mtx4       getNodeTransform() override ;
};
