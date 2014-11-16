#include "kgmGuiMenu.h"
#include "../kgmBase/kgmLog.h"

KGMOBJECT_IMPLEMENT(kgmGuiMenu, kgmGui)

u32 kgmGuiMenu::ItemHeight = 20;

kgmGuiMenu::kgmGuiMenu(kgmGui* parent)
  :kgmGui(parent, 0, 0, 1, 1)
{
  item = new Item(null, "", true);
}

kgmGuiMenu::Item* kgmGuiMenu::add(kgmString title)
{
  Item* im = item->add(title);

  if(!im)
    return null;

  m_rect = item->getRect();

  return im;
}

kgmGuiMenu::Item* kgmGuiMenu::add(u32 id, kgmString title)
{
  Item* im = item->add(title);

  if(!im)
    return null;

  m_rect = item->getRect();

  return im;
}

kgmGuiMenu::~kgmGuiMenu()
{
  if(item)
    delete item;
}

void kgmGuiMenu::onMsLeftUp(int key, int x, int y)
{
  Item* selected = item->clickPointer(x, y);

  if(selected && selected->getType() == Item::TypeItem)
  {
    if(selected->callback.getObject())
      selected->callback();
  }
}

void kgmGuiMenu::onMsMove(int key, int x, int y)
{
#ifdef DEBUG
  kgm_log() << "kgmGuiMenu onMsMove: " << (s8*)m_sid << " " << x << " " << y << "\n";
#endif

  item->movePointer(x, y);
}
