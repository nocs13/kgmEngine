#include "kgmGuiContainer.h"
#include "../kgmSystem/kgmWindow.h"

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
  g->setParent(this);

  update();
}

void kgmGuiContainer::setWindow(kgmWindow* w)
{
  _wnd = w;
}

void kgmGuiContainer::update()
{
  Rect pr;

  if (m_parent) {
    pr = m_parent->getRect();
  } else if (_wnd) {
    s32 x, y, w, h;

    _wnd->getRect(x, y, w, h);

    pr = Rect(x, y, w, h);
  }

  if ((pr.w > getRect().w) || (pr.h > getRect().h))
    align(null, pr);

  Rect r = getRect();

  u32 cellWidth = r.w / cols_per_page;

  u32 itemHeight = 40;
  u32 row = 0;
  u32 cols = 0;

  auto i = _cells.begin();

  while(!i.end()) {
    if (cols >= (u32) CellSize_5) {
      row ++;
      cols = 0;
    }

    u32 x = cols * cellWidth;
    u32 y = row * itemHeight;
    u32 w = (*i).width;
    u32 h = itemHeight;

    if ((cols + w) > CellSize_5) {
      w = CellSize_5 - cols;
    }

    if (w < 1) {
      i.next();

      continue;
    }

    if ((*i).gui) {
      Rect rc(x, y, w * cellWidth, h);

      (*i).gui->setRect(rc);
    }

    cols += w;
    i.next();
  }
}

void kgmGuiContainer::align(kgmGui* g, Rect inside)
{
  if (g == null) {
    s32 w = getRect().w;
    s32 h = getRect().h;
    s32 x = inside.w / 2 - w / 2;
    s32 y = inside.h / 2 - h / 2;

    setRect(x, y, w, h);
  }
}
