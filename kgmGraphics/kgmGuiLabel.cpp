#include "kgmGuiLabel.h"

KGMOBJECT_IMPLEMENT(kgmGuiLabel, kgmGui)

kgmGuiLabel::kgmGuiLabel()
{
  font_width = 8;
  font_height = 13;
}

kgmGuiLabel::kgmGuiLabel(kgmGui *par, u32 x, u32 y, u32 w, u32 h)
 :kgmGui(par, x, y, w, h)
{
  font_width = 8;
  font_height = 13;
}

kgmGuiLabel::~kgmGuiLabel()
{
}
