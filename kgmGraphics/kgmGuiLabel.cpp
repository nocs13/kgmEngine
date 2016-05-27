#include "kgmGuiLabel.h"

kgmGuiLabel::kgmGuiLabel()
{
  font_width = 6;
  font_height = 12;
}

kgmGuiLabel::kgmGuiLabel(kgmGui *par, u32 x, u32 y, u32 w, u32 h)
 :kgmGui(par, x, y, w, h)
{
  font_width = 6;
  font_height = 12;
}

kgmGuiLabel::~kgmGuiLabel()
{
}
