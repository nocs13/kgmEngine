#pragma once
#include "../kgmBase/kgmObject.h"
#include "../kgmBase/kgmOctTree.h"

#include "../kgmGraphics/kgmGui.h"
#include "../kgmGraphics/kgmGuiStyle.h"

#include "../kgmGraphics/kgmMesh.h"
#include "../kgmGraphics/kgmLight.h"
#include "../kgmGraphics/kgmCamera.h"
#include "../kgmGraphics/kgmMaterial.h"
#include "../kgmGraphics/kgmGraphics.h"

class kgmIGame;
class kgmUnit;

class kgmGameGraphics: public kgmGraphics
{
  KGM_OBJECT(kgmGameGraphics);

  kgmIGame* m_game = null;

public:
  kgmGameGraphics(kgmIGC*, kgmIResources*);
  ~kgmGameGraphics();

  void render_3d();
  void render_2d();

  void setBgColor(kgmColor c);
};
