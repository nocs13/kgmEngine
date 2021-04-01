#include "kgmGNode.h"

kgmGNode::kgmGNode(kgmUnit* u, kgmObject* n, kgmIGraphics::TypeNode t)
: kgmNode(n, t)
{
  m_unit = u;
}

kgmGNode::~kgmGNode()
{
}