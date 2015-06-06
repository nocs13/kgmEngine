#include "kgmGuiTab.h"

KGMOBJECT_IMPLEMENT(kgmGuiTab, kgmGui)

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

  labels->setLabelsCallback(Labels::ClickEventCallback(this, (Labels::ClickEventCallback::Function)&kgmGuiTab::select));
}

kgmGuiTab::~kgmGuiTab()
{
}

u32 kgmGuiTab::set(u32 k)
{
  if((k < 0) || (k >= client->m_childs.length()))
    return m_index;
  
  for(int i = 0; i < client->m_childs.length(); i++)
  {
    if(i == k)
    {
      m_index = i;
      client->m_childs[i]->show();
    }
    else
    {
      client->m_childs[i]->hide();
    }
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

  if(m_index < 0 || m_index >= client->m_childs.length())
    return null;

  return client->m_childs[m_index];
}

kgmGui* kgmGuiTab::addTab(kgmString title)
{
  kgmGui* gui = new kgmGui(client, 0, 0, client->getRect().width(), client->getRect().height());

  gui->setText(title);

  if(active() != null)
    active()->hide();

  labels->add(client->m_childs.length(), title);
  gui->show();
}

void kgmGuiTab::select(u32 i)
{
  if(i >= client->m_childs.length())
    return;

  set(i);
}
