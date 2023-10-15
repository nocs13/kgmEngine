#include "kgmGNode.h"
#include "kgmUnit.h"

kgmGNode::kgmGNode(kgmUnit* u, kgmObject* n, kgmIGraphics::TypeNode t)
: kgmNode(n, t)
{
  m_unit = u;
  m_valid = true;
}

kgmGNode::~kgmGNode()
{
}

bool kgmGNode::isNodeValid()
{
  return m_valid;
}

box3 kgmGNode::getNodeBound()
{
  box3 b = m_unit->getBound();
  vec3 s = m_unit->scale();

  b.min.set(b.min.x * s.x, b.min.y * s.y, b.min.z * s.z);
  b.max.set(b.max.x * s.x, b.max.y * s.y, b.max.z * s.z);

  return b;
}

vec3 kgmGNode::getNodePosition()
{
  return m_unit->position();
}

mtx4 kgmGNode::getNodeTransform()
{
  return m_unit->transform();
}

void kgmGNode::setValidity(bool v)
{
  m_valid = v;
}
