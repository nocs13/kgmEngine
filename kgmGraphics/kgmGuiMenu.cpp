#include "kgmGuiMenu.h"
KGMOBJECT_IMPLEMENT(kgmGuiMenu, kgmGui)

u32 kgmGuiMenu::ItemHeight = 20;

kgmGuiMenu::kgmGuiMenu(kgmGui* parent)
  :kgmGui(parent, 0, 0, 0, 0)
{
  m_rect.x = m_rect.y = 0;

  item = new Item(null, "", true);
}

kgmGuiMenu::Item* kgmGuiMenu::add(kgmString title)
{
  Item* im = item->add(title);

  if(!im)
    return null;

  u32 x = m_rect.x + m_rect.w + 1;

  m_rect = item->getRect();

  return im;
}

kgmGuiMenu::Item* kgmGuiMenu::add(u32 id, kgmString title)
{
  Item* im = item->add(id, title);

  if(!im)
    return null;

  u32 x = m_rect.x + m_rect.w + 1;

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
    //onAction(this, selected->getId());
    if(selected->callback && selected->object)
      selected->callback(selected->object);
  }
}

void kgmGuiMenu::onMsMove(int key, int x, int y)
{
  item->movePointer(x, y);
}
