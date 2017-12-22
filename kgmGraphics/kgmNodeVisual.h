#ifndef KGMNODEVISUAL_H
#define KGMNODEVISUAL_H

#include "kgmNode.h"

class kgmNodeVisual: public kgmNode
{
public:
  kgmNodeVisual();

  kgmIGraphics::TypeNode getNodeType()
  {
    return kgmIGraphics::NodeVisual;
  }
};

#endif // KGMNODEVISUAL_H
