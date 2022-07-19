#include "kgmGNode.h"
#include "kgmUnit.h"

kgmGNode::kgmGNode(kgmUnit* u, kgmObject* n, kgmIGraphics::TypeNode t)
: kgmNode(n, t)
{
  m_unit = u;
}

kgmGNode::~kgmGNode()
{
}

box3 kgmGNode::getNodeBound()
{
  return m_unit->getBound();
}

vec3 kgmGNode::getNodePosition()
{
  return m_unit->position();
}

mtx4 kgmGNode::getNodeTransform()
{
  return m_unit->transform();
}
