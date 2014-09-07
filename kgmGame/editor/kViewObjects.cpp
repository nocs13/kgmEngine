#include "kViewObjects.h"

using namespace kgmGameEditor;

kViewObjects::kViewObjects(kgmEvent* t, int x, int y, int w, int h, bool selclose)
  :callClose(null, null), callEclose(null, null), callSelect(null, null),
  kgmGui(null, x, y, w, h)
{
  target = t;

  btn_close = new kgmGuiButton(this, w - 21, 1, 20, 20);
  btn_close->setText("X");
  btn_close->setClickCallback(kgmGuiButton::ClickEventCallback(this, (kgmGuiButton::ClickEventCallback::Function)&kViewObjects::onCloseObjects));

  list = new kgmGuiList(this, 1, 21, w - 2, h - 23);
  list->setSelectCallback(kgmGuiList::SelectEventCallback(this, (kgmGuiList::SelectEventCallback::Function)&kViewObjects::onSelectItem));

  closeOnSelect = selclose;
}

void kViewObjects::onCloseObjects()
{
  erase();
  release();

  if(callEclose.hasObject() && callEclose.hasFunction())
    callEclose(callEclose.getObject());
}

void kViewObjects::onSelectItem()
{

  if(callSelect.hasObject() && callSelect.hasFunction())
    callSelect(callSelect.getObject(), list->getItem(list->getSel()));

  if(closeOnSelect)
    onCloseObjects();
}
