#include "kViewObjects.h"

using namespace kgmGameEditor;

kViewObjects::kViewObjects(kgmEvent* t, int x, int y, int w, int h)
  :kgmGuiFrame("Objects", x, y, w, h)
{
  target = t;

  list = new kgmGuiList(getClient(), 0, 0, getClient()->m_rect.width(), getClient()->m_rect.height());

  slotSelect.connect(this, (Slot<kViewObjects, u32>::FN) &kViewObjects::onSelectItem, &list->sigSelect);
}

void kViewObjects::onClose()
{
  close();
}

void kViewObjects::onSelectItem(u32 i)
{
  sigSelect(list->getItem(i));
  close();
}
