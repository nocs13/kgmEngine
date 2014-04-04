#include "kgmGuiList.h"

KGMOBJECT_IMPLEMENT(kgmGuiList, kgmGui)

kgmGuiList::kgmGuiList()
  :kgmGui()
{
  m_scroll = new kgmGuiScroll(this, 0, 0, 0, 0);
  m_scroll->show();
  m_scroll->setRange(1);
  m_itemSel = -1;
  m_itemStart = 0;
  m_itemHeight = 20;
  m_position = 0;
  m_range    = 0;
}

kgmGuiList::kgmGuiList(kgmGui *par, int x, int y, int w, int h)
  :kgmGui(par, x, y, w, h)
{
  m_scroll = new kgmGuiScroll(this, x + w - 10, y, 10, h);
  m_scroll->show();
  m_scroll->setRange(1);
  m_itemSel = -1;
  m_itemStart = 0;
  m_itemHeight = 20;
  m_position = 0;
  m_range    = 0;
}


kgmGuiList::~kgmGuiList()
{
  clear();
  //delete m_scroll;
}

void kgmGuiList::addItem(kgmString s, u32 icon)
{
  if(!s.length())
    return;

  int vCnt = m_rect.h / m_itemHeight;
  //int sRange = m_items.size() - vCnt;

  m_items.push_back(s);

  m_scroll->setRange(m_items.length());
}

void kgmGuiList::remItem(int s)
{
  if(s >= 0 && s < m_items.length())
    m_items.erase(s);

  m_scroll->setRange(m_items.length());
}

kgmString kgmGuiList::getItem(int i)
{
  if(i < 0 || i > m_itemSel)
    return kgmString();

  return m_items[i];
}

kgmGui::Rect kgmGuiList::getItemRect(int i)
{

}

kgmGui::Icon kgmGuiList::getItemIcon(int i)
{
  return null;
}

void kgmGuiList::setSel(int sel)
{
  m_itemSel = sel;
}

int kgmGuiList::getSel()
{
  return m_itemSel;
}

char* kgmGuiList::getSelectedItem()
{
  if(m_itemSel < 0)
    return 0;

  return getItem(m_itemSel);
}

void kgmGuiList::clear()
{
  for(int i = 0; i < m_items.length(); i++)
    m_items[i].clear();

  m_items.clear();
}

void kgmGuiList::sort()
{
  for(Items::iterator i = m_items.begin(); i != m_items.end(); i++)
  {
    for(Items::iterator j = i; j != m_items.end(); j++)
    {
      if(*i == *j)
        continue;

      for(int k = 0; k < (*i).length(); k++)
      {
        if((k >= (*j).length()) || ((*i)[k] > (*j)[k]))
        {
          kgmString tmp = (*j);

          (*j) = (*i);

          (*i) = tmp;

          break;
        }
        else if((*i)[k] < (*j)[k])
        {
          break;
        }
      }
    }
  }
}

void kgmGuiList::onSelect(u32 sel)
{
  m_parent->onAction(this, sel);
}

void kgmGuiList::onMsLeftDown(int k, int x, int y)
{
  if(m_scroll->visible() && m_scroll->m_rect.inside(x, y))
    return m_scroll->onMsLeftDown(k, x, y);
}

void kgmGuiList::onMsLeftUp(int k, int x, int y)
{
  if(m_scroll->visible() && m_scroll->m_rect.inside(x, y))
    return m_scroll->onMsLeftUp(k, x, y);

  Rect rect = Rect(0, 0, m_rect.w, m_rect.h);
  m_itemSel = m_position + (y - rect.y) / m_itemHeight;

  if(m_parent && (m_itemSel < m_items.size()))
    onSelect(m_itemSel);
}

void kgmGuiList::onMsMove(int k, int x, int y)
{
  Rect rect = Rect(0, 0, m_rect.w, m_rect.h);

  if(m_scroll->visible() && m_scroll->m_rect.inside(x, y))
    return m_scroll->onMsMove(k, x, y);
  else if(y / m_itemHeight < m_items.size())
    m_itemSel = y / m_itemHeight;
}

void kgmGuiList::onMsWheel(int k, int x, int y, int z)
{
  kgmGui::onMsWheel(k, x, y, z);

  if(z > 0)
    onKeyDown(KEY_UP);
  else
    onKeyDown(KEY_DOWN);
}

void kgmGuiList::onKeyDown(int k)
{
  switch(k)
  {
  case KEY_UP:
    if(m_itemSel > 0)
      m_itemSel--;
    break;
  case KEY_DOWN:
    if(m_itemSel < (m_items.size() - 1))
      m_itemSel++;
    break;
  case KEY_ENTER:
    if(m_itemSel >= 0 && m_itemSel < m_items.size())
      onSelect(m_itemSel);
    break;
  }
}

void kgmGuiList::onAction(kgmGui *gui, u32 a)
{
  if(gui == m_scroll)
  {
    m_position = a;
  }
}
