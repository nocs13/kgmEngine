#include "kViewObjects.h"

using namespace kgmGameEditor;

kViewObjects::kViewObjects(kgmEvent* t, int x, int y, int w, int h)
  :callClose(null, null),
  kgmGui(null, x, y, w, h)
{
  target = t;

  btn_close = new kgmGuiButton(this, w - 21, 1, 20, 20);
  btn_close->setText("X");
  btn_close->setClickCallback(kgmGuiButton::ClickEventCallback(this, (kgmGuiButton::ClickEventCallback::Function)&kViewObjects::onCloseObjects));

  list = new kgmGuiList(this, 1, 21, w - 2, h - 23);
}

void kViewObjects::onCloseObjects()
{
  if(callClose.hasObject() && callClose.hasFunction())
    callClose(callClose.getObject());

  erase();
  release();
}
