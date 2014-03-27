#include "kgmGuiMenu.h"
KGMOBJECT_IMPLEMENT(kgmGuiMenu, kgmGui)

kgmGuiMenu::kgmGuiMenu(kgmGui* parent, kgmString title)
  :kgmGui(parent, 0, 0, 70, 20)
{
  m_rect.x = m_rect.y = 0;

  item = new Item(-1, title, false);
}

kgmGuiMenu::~kgmGuiMenu()
{
  delete item;
}

iRect kgmGuiMenu::getMenuRect()
{
  return item->getRect();
}

void kgmGuiMenu::setPosition(int x, int y)
{
  item->setPosition(x, y);
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

}
