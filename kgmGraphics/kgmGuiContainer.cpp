#include "kgmGuiContainer.h"


kgmGuiContainer::kgmGuiContainer()
    :kgmGui(){
}

kgmGuiContainer::kgmGuiContainer(kgmGui *par, u32 x, u32 y, u32 w, u32 h)
  :kgmGui(par, x, y, w, h)
{
}

kgmGuiContainer::~kgmGuiContainer()
{
}

void kgmGuiContainer::add(kgmGui* g, CellSize cw, Align al)
{
  if (isChild(g))
    return;

  // If g is null, then separator.

  Cell c = {g, cw, CellSize_1, al};

  _cells.add(c);
}

void kgmGuiContainer::update() {

  Rect r = getRect();

  u32 cellWidth = r.w / cols_per_page;

  u32 itemHeight = 40;
  u32 row = 1;

  auto i = _cells.begin();

  while(!i.end()) {

  }
}


