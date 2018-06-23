#include "kgmGuiMenu.h"
#include "../kgmBase/kgmLog.h"

u32 kgmGuiMenu::ItemHeight = 20;
u32 kgmGuiMenu::FontWidth  = 10;

kgmGuiMenu::kgmGuiMenu(kgmGui* parent)
  :kgmGui(parent, 0, 0, 1, 1)
{
  root = new Item(null, "", true);

  m_onpoint = true;

  choose = null;
}

kgmGuiMenu::Item* kgmGuiMenu::add(kgmString title)
{
  Item* im = root->add(title);

  m_rect = root->getSubRect();

  return im;
}

kgmGuiMenu::Item* kgmGuiMenu::add(u32 id, kgmString title)
{
  Item* im = root->add(title);

  m_rect = root->getSubRect();

  return im;
}

kgmGuiMenu::~kgmGuiMenu()
{
  delete root;
}

void kgmGuiMenu::onMsLeftUp(int key, int x, int y)
{
  if(!m_view || m_freeze)
    return;

  Item* selected = root->clickPointer(x, y);

  if(selected && selected->getType() == Item::TypeItem)
  {
    sigChoose(selected->getId());
  }
}

void kgmGuiMenu::onMsMove(int key, int x, int y)
{
  if(!m_view || m_freeze)
    return;

  root->movePointer(x, y);
}
