#include "kgmGuiList.h"

KGMOBJECT_IMPLEMENT(kgmGuiList, kgmGui)

kgmGuiList::kgmGuiList()
  :kgmGui(), callback(null, null)
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
  :kgmGui(par, x, y, w, h), callback(null, null)
{
  m_scroll = new kgmGuiScroll(this, x + w - 10, 0, 10, h);
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

int kgmGuiList::getItem(kgmString n)
{
  int ci = 0;

  for(Items::iterator i = m_items.begin(); i != m_items.end(); ++i)
  {
    if((*i) == n)
      return ci;

    ci++;
  }

  return -1;
}

kgmGui::Rect kgmGuiList::getItemRect(int i)
{
  Rect rc(0, 0, 0, 0);

  if(i < 0 || i >= m_items.length())
    return rc;

  rc.x = m_rect.x + 1;
  rc.w = m_rect.w - 2;
  rc.y = m_rect.y + m_itemHeight * (i - m_position) + 1;
  rc.h = m_itemHeight;

  return rc;
}

kgmGui::Icon kgmGuiList::getItemIcon(int i)
{
  return null;
}

void kgmGuiList::setSel(int sel)
{
  m_itemSel = sel;

  if(m_scroll)
    m_scroll->setPosition(sel);
}

int kgmGuiList::getSel()
{
  return m_itemSel;
}

int kgmGuiList::getFirstVisibleItem()
{
  return m_position;
}

int kgmGuiList::getVisibleItemsCount()
{
  return m_rect.h / m_itemHeight;
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

  m_position = 0;

  if(m_scroll)
    m_scroll->setPosition(0);
}

void kgmGuiList::sort()
{
  for(Items::iterator i = m_items.begin(); i != m_items.end(); ++i)
  {
    for(Items::iterator j = i; j != m_items.end(); ++j)
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

void kgmGuiList::onMsLeftDown(int k, int x, int y)
{
  kgmGui::onMsLeftDown(k, x, y);
  //if(m_scroll->visible() && m_scroll->m_rect.inside(x, y))
  //  return m_scroll->onMsLeftDown(k, x, y);
}

void kgmGuiList::onMsLeftUp(int k, int x, int y)
{
  kgmGui::onMsLeftUp(k, x, y);
  //if(m_scroll->visible() && m_scroll->m_rect.inside(x, y))
  //  return m_scroll->onMsLeftUp(k, x, y);

  Rect rect = Rect(0, 0, m_rect.w, m_rect.h);
  m_itemSel = m_position + (y - rect.y) / m_itemHeight;

  if((m_itemSel < m_items.size()))
  {
    if(callback.hasObject() && callback.hasFunction())
      callback(callback.getObject());
  }
}

void kgmGuiList::onMsMove(int k, int x, int y)
{
  if(m_scroll->visible() && m_scroll->m_rect.inside(x, y))
    return m_scroll->onMsMove(k, x - m_scroll->m_rect.x, y - m_scroll->m_rect.y);
  else if((y / m_itemHeight < m_items.size()) &&
          (y < (m_rect.y + m_rect.h - 1)))
    m_itemSel = getFirstVisibleItem() +  y / m_itemHeight;
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
  s32 iir = m_rect.h / m_itemHeight;

  switch(k)
  {
  case KEY_UP:
    if(m_itemSel > 0)
      m_itemSel--;
    if(m_itemSel < m_position)
      m_position = m_itemSel;
    m_scroll->setPosition(m_position);
    break;
  case KEY_DOWN:
    if(m_itemSel < (m_items.size() - 1))
      m_itemSel++;
    if(m_itemSel >= (m_position + iir))
      m_position++;
    m_scroll->setPosition(m_position);
    break;
  case KEY_ENTER:
    if(m_itemSel >= 0 && m_itemSel < m_items.size())
    {
      if(callback.hasObject() && callback.hasFunction())
        callback(callback.getObject());
    }
    break;
  }
}
