#include "kgmGuiMenu.h"
KGMOBJECT_IMPLEMENT(kgmGuiMenu, kgmGui)

u32 kgmGuiMenu::ItemHeight = 20;

kgmGuiMenu::kgmGuiMenu(kgmGui* parent)
  :kgmGui(parent, 0, 0, 0, 0)
{
  m_rect.x = m_rect.y = 0;

  item = new Item(-1, "", false);
}

kgmGuiMenu::Item* kgmGuiMenu::add(u32 id, kgmString title)
{
  Item* im = item->add(id, title);

  if(!im)
    return null;

  u32 x = m_rect.x + m_rect.w + 1;

  m_rect = item->getRect();

  im->setPosition(x, 0);

  return im;
}

kgmGuiMenu::~kgmGuiMenu()
{
  if(item)
    delete item;
}

void kgmGuiMenu::onMsLeftUp(int key, int x, int y)
{
  if(m_rect.inside(x, y))
  {
    item->setExpand(!item->getExpand());
  }
}

void kgmGuiMenu::onMsMove(int key, int x, int y)
{
  item->movePointer(x, y);
}
