#include "kgmGuiMenu.h"
#include "../kgmBase/kgmLog.h"

u32 kgmGuiMenu::ItemHeight = 20;
u32 kgmGuiMenu::FontWidth = 10;

bool kgmGuiMenu::Item::havePointer(int x, int y)
{
  /*
  if (!parent && this->title.length() < 1)
    kgm_log() << "ROOT havePointer:\n";
  else
    kgm_log() << this->title.data() << " havePointer:\n";
  */

  /*
  kgm_log() << "  havePointer x: " << x << " y: " << y << "\n";
  kgm_log() << "    havePointer rc.x: " << rect.x << " rc.y: " << rect.y << "\n";
  kgm_log() << "    havePointer rc.w: " << rect.w << " rc.h: " << rect.h << "\n";
  */

  if (m_active)
  {
    for (int i = 0; i < items.length(); i++)
    {
      if (items[i]->havePointer(x, y))
      {
        m_active = true;

        return true;
      }
    }
  }

  if (rect.inside(x, y))
  {
    m_active = true;

    Item *root = getRoot();

    if (root)
      root->m_selected = this;

    return true;
  }

  m_active = false;

  return false;
}

kgmGuiMenu::kgmGuiMenu(kgmGui *parent)
    : kgmGui(parent, 0, 0, 1, 1)
{
  root = new Item(null, "", true);

  m_onpoint = true;

  choose = null;
}

kgmGuiMenu::Item *kgmGuiMenu::add(kgmString title)
{
  Item *im = root->add(title);

  m_rect = root->getSubRect();

  return im;
}

kgmGuiMenu::Item *kgmGuiMenu::add(u32 id, kgmString title)
{
  Item *im = root->add(title);

  m_rect = root->getSubRect();

  return im;
}

kgmGuiMenu::~kgmGuiMenu()
{
  delete root;
}

void kgmGuiMenu::onMsLeftUp(int key, int x, int y)
{
  if (!m_view || m_freeze)
    return;

  //Item *selected = root->clickPointer(x, y);

  //if (selected && selected->getType() == Item::TypeItem)
  //{
  //  sigChoose(selected->getId());
  //}
}

void kgmGuiMenu::onMsMove(int key, int x, int y)
{
  if (!m_view || m_freeze)
    return;
  //kgm_log() << __FUNCTION__ << x << " y: " << y << "\n";

  // root->movePointer(x, y);
  //root->havePointer(x, y);

  for (int i = 0; i < root->getItemsCount(); i++)
  {
    Item* it = root->getItem(i);

    it->havePointer(x, y);
    //kgm_log() << it->getTitle().data() << " active: " << (int) it->active()  << "\n";
  }
}

void kgmGuiMenu::showItems(int x, int y)
{
}
