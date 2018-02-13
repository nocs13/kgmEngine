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

#ifdef EDITOR

#include "editor/kArrow.h"
#include "editor/kPivot.h"
#include "editor/kGridline.h"

using namespace  kgmGameEditor;

#endif

class kgmIGame;
class kgmUnit;

class kgmGameGraphics: public kgmGraphics
{
  KGM_OBJECT(kgmGameGraphics);

#ifdef EDITOR
  kPivot*    m_pivot    = null;
  kGridline* m_gridline = null;
#endif

  kgmIGame* m_game = null;

public:
  kgmGameGraphics(kgmIGC*, kgmIResources*);
  ~kgmGameGraphics();

  void render_3d();
  void render_2d();

#ifdef EDITOR
  void drawLight(kgmUnit*);

  void set(kPivot* p)
  {
    m_pivot = p;
  }

  void set(kGridline* g)
  {
    m_gridline = g;
  }
#endif
};
