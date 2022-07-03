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
  u32 cols = 0;

  auto i = _cells.begin();

  while(!i.end()) {
    if (cols >= (u32) CellSize_5) {
      row ++;
      cols = 0;
    }

    u32 x = row * itemHeight + cols * cellWidth;
    u32 y = row * itemHeight;
    u32 w = (*i).width;
    u32 h = itemHeight;

    if ((cols + w) > CellSize_5) {
      w = CellSize_5 - cols;
    }

    if (w < 1)
      continue;

    if ((*i).gui) {
      Rect rc(x, y, w * cellWidth, h);

      (*i).gui->setRect(rc);
    }

    cols += w;
  }
}


