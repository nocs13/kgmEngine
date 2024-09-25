#include "kgmGuiTab.h"
/*
kgmGuiTab::kgmGuiTab()
{
  m_index  = 0;

  tab_height = 20;

  labels = null;
  client = null;
}

kgmGuiTab::kgmGuiTab(kgmGui *par, int x, int y, u32 w, u32 h)
:kgmGui(par, x, y, w, h)
{
  m_index  = 0;

  tab_height = 20;

  labels = new Labels(this);
  client = new kgmGui(this, 0, tab_height, w, h - tab_height);

  labels->setSid("tab_labels");
  labels->m_rect.w = w;
  labels->m_rect.h = tab_height;

  slotLabel.connect(this, (Slot<kgmGuiTab, u32>::FN) &kgmGuiTab::select, &labels->sigSwitch);
}

kgmGuiTab::~kgmGuiTab()
{
}

u32 kgmGuiTab::set(u32 k)
{
  if(k >= (u32) client->m_childs.length())
    return m_index;

  for(s32 i = 0; i < client->m_childs.length(); i++)
  {
    if((u32) i == k)
    {
      m_index = i;
      client->m_childs[i]->show();
    }
    else
    {
      client->m_childs[i]->hide();
    }
  }

  for(s32 i = 0; i < labels->getItem()->getItemsCount(); i++)
  {
    kgmGuiMenu::Item* it = labels->getItem()->getItem(i);

    if (it->getId() == k)
      labels->getItem()->getItem(i)->select(true);
    else
      labels->getItem()->getItem(i)->select(false);
  }

  return m_index;
}

u32 kgmGuiTab::next()
{
  return set(m_index + 1);
}

u32 kgmGuiTab::previous()
{
  return set(m_index - 1);
}

kgmGui* kgmGuiTab::active()
{
  if(client->m_childs.length() < 1)
    return null;

  if(m_index >= (u32) client->m_childs.length())
    return null;

  return client->m_childs[m_index];
}

kgmGui* kgmGuiTab::addTab(kgmString title)
{
  kgmGui* gui = new kgmGui(client, 0, 0, client->getRect().width(), client->getRect().height());

  gui->setText(title);

  if(active() != null)
    active()->hide();

  kgmGuiMenu::Item* item = labels->add(client->m_childs.length(), title);

  item->setId(client->m_childs.length());

  gui->show();

  return gui;
}

void kgmGuiTab::select(u32 i)
{
  if(i >= (u32) client->m_childs.length())
    return;

  set(i);
}
*/