#include "kViewOptions.h"

kViewOptions::kViewOptions(int x, int y, int w, int h)
  :kgmGui(null, x, y, w, h)
{
  tab = new kgmGuiTab(this, 0, 0, w, h);
}
