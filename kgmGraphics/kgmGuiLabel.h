#pragma once
#include "kgmGui.h"


class kgmGuiLabel: public kgmGui
{
  KGM_OBJECT(kgmGuiLabel);

  u32 font_width;
  u32 font_height;

public:
  kgmGuiLabel();
  kgmGuiLabel(kgmGui *par, u32 x, u32 y, u32 w, u32 h);
  virtual ~kgmGuiLabel();

  void setFontWidth(u32 w)  { font_width  = w; }
  void setFontHeight(u32 h) { font_height = h; }

  u32 getFontWidth() const { return font_width; }
  u32 getFontHeight() const { return font_height; }
};
