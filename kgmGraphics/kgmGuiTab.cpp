#include "kgmGuiTab.h"

KGMOBJECT_IMPLEMENT(kgmGuiTab, kgmGui)

kgmGuiTab::kgmGuiTab()
{
  m_index  = 0;

  tab_height = 20;

  client = null;
}

kgmGuiTab::kgmGuiTab(kgmGui *par, int x, int y, u32 w, u32 h)
:kgmGui(par, x, y, w, h)
{
  m_index  = 0;

  tab_height = 20;

  client = new kgmGui(this, 0, tab_height, w, h - tab_height);
}

kgmGuiTab::~kgmGuiTab()
{
  for(int i = tabs.length(); i > 0; i--)
  {
    tabs[i]->release();
  }

  tabs.clear();
}


u32 kgmGuiTab::set(u32 k)
{
  if((k < 0) || (k >= tabs.size()))
    return m_index;
  
  for(int i = 0; i < tabs.size(); i++)
  {
    if(i == k)
    {
      m_index = i;
      tabs[i]->show();
    }
    else
    {
      tabs[i]->hide();
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
  if(tabs.size() < 1)
    return null;

  if(m_index < 0 || m_index >= tabs.size())
    return null;

  return tabs[m_index];
}

kgmGui* kgmGuiTab::addTab(kgmString title)
{

}
