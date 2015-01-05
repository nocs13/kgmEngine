#include "kgmNode.h"

KGMOBJECT_IMPLEMENT(kgmNode, kgmObject);

kgmNode::kgmNode(kgmNode *parent)
{
  m_parent = parent;
}

kgmNode::~kgmNode()
{
}
