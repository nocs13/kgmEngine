#include "BaseRender.h"
#include "../../kgmBase/kgmIGC.h"
#include "../kgmGui.h"

class GlassRender: public BaseRender
{
 public:
  GlassRender(kgmGraphics* gr);

  void render();

};
