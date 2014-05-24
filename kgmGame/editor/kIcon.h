#ifndef KICON_H
#define KICON_H

#include "../../kgmGraphics/kgmMesh.h"
#include "../../kgmGraphics/kgmMaterial.h"

namespace kgmGameEditor
{

class kIcon : public kgmMesh
{
  kgmMaterial* mrl;
public:
  kIcon();
};

}

#endif // KICON_H
