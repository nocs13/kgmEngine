#include "kViewObjects.h"

using namespace kgmGameEditor;

kViewObjects::kViewObjects(kgmEvent* t, int x, int y, int w, int h)
  :kgmGui(null, x, y, w, h)
{
  target = t;
  list = new kgmGuiList(this, 1, 1, w - 2, h - 2);
}
