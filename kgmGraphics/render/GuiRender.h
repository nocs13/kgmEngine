#pragma once

#include "../../kgmBase/kgmIGC.h"
#include "../kgmGui.h"
#include "../kgmMaterial.h"

class kgmGraphics;
class kgmFont;

class GuiRender
{
  kgmGraphics* gr;

  mtx4 mi;

  kgmMaterial mtl;

public:
  GuiRender(kgmGraphics* gr);

  void render();

private:
  void render(kgmGui* gui);
  void renderGuiMenuItem(kgmGui* menu, void *i);

  void gcDrawRect(kgmIGC* gc, kgmGui::Rect rc, u32 col, kgmTexture* tex);

  void gcDrawBorder(kgmIGC* gc, kgmGui::Rect rc, u32 col, kgmTexture* tex);

  void gcDrawText(kgmIGC* gc, kgmFont* font, u32 fwidth, u32 fheight, u32 fcolor,
                  kgmGui::Rect clip, kgmString& text);
};
