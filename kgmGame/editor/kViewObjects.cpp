#include "kViewObjects.h"

using namespace kgmGameEditor;

kViewObjects::kViewObjects(kgmEvent* t, int x, int y, int w, int h, bool selclose)
  :callClose(null, null), callEclose(null, null), callSelect(null, null),
    kgmGuiFrame("Objects", x, y, w, h)
{
  target = t;

  list = new kgmGuiList(getClient(), 0, 0, getClient()->m_rect.width(), getClient()->m_rect.height());
  list->setSelectCallback(kgmGuiList::SelectEventCallback(this, (kgmGuiList::SelectEventCallback::Function)&kViewObjects::onSelectItem));

  closeOnSelect = selclose;
}

void kViewObjects::onClose()
{
  if(callEclose.hasObject() && callEclose.hasFunction())
    callEclose(callEclose.getObject());
}

void kViewObjects::onSelectItem()
{

  if(callSelect.hasObject() && callSelect.hasFunction())
    callSelect(callSelect.getObject(), list->getItem(list->getSel()));

  if(closeOnSelect)
    close();
}
