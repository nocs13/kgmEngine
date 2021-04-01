#include "../kgmGraphics/kgmNode.h"

class kgmUnit;

class kgmGNode : public kgmNode
{
  kgmUnit*   m_unit;

public:
  kgmGNode(kgmUnit* u, kgmObject* n, kgmIGraphics::TypeNode t);
  ~kgmGNode();

  kgmIGraphics::TypeNode getNodeType()
  {
    return kgmIGraphics::NodeLight;
  }
};