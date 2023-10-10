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
  mtx4 m;

  vec3 v = m_unit->transform().position();
  vec3 s = m_unit->transform().scaled();

  m.identity();
  m.translate(v);
  m.scale(s);

  box3 b = m_unit->getBound();

  b.min = m * b.min;
  b.max = m * b.max;

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
