#pragma once
#include "../kgmBase/kgmObject.h"
#include "../kgmBase/kgmOctTree.h"

#include "../kgmGraphics/kgmGui.h"
#include "../kgmGraphics/kgmGuiStyle.h"

#include "../kgmGraphics/kgmMesh.h"
#include "../kgmGraphics/kgmLight.h"
#include "../kgmGraphics/kgmCamera.h"
#include "../kgmGraphics/kgmVisual.h"
#include "../kgmGraphics/kgmMaterial.h"
#include "../kgmGraphics/kgmGraphics.h"

class kgmGameGraphics: public kgmGraphics
{
  KGM_OBJECT(kgmGameGraphics);

public:
  kgmGameGraphics(kgmIGraphics*, kgmIResources*);
  ~kgmGameGraphics();
};
