#include "kgmGuiList.h"

#define SCROLL_WIDTH 20

kgmGuiList::kgmGuiList()
  :kgmGui()
{
  m_scroll = new kgmGuiScroll(this, 0, 0, 0, 0);
  m_scroll->show();
  m_scroll->setRange(1);
  m_itemSel = -1;
  m_itemPnt = -1;
  m_itemStart = 0;
  m_itemHeight = 12;
  m_position = 0;
  m_range    = 0;
}

kgmGuiList::kgmGuiList(kgmGui *par, int x, int y, int w, int h)
  :kgmGui(par, x, y, w, h)
{
  m_scroll = new kgmGuiScroll(this, x + w - SCROLL_WIDTH, 0, SCROLL_WIDTH, h);
  m_scroll->show();
  m_scroll->setRange(1);
  slotScroll.connect(this, (Slot<kgmGuiList, u32>::FN) &kgmGuiList::onScroll, &m_scroll->sigChange);
  m_itemSel = -1;
  m_itemPnt = -1;
  m_itemStart = 0;
  m_itemHeight = 12;
  m_position = 0;
  m_range    = 0;
}


kgmGuiList::~kgmGuiList()
{
  clear();
}

void kgmGuiList::addItem(kgmString s, u32 icon)
{
  if(!s.length())
    return;

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

  for(Items::iterator i = m_items.begin(); i != m_items.end(); i++)
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

int kgmGuiList::getPnt()
{
  return m_itemPnt;
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

bool kgmGuiList::hasItem(kgmString item)
{
  for(int i = 0; i < m_items.length(); i++)
    if(m_items[i] == item)
      return true;

  return false;
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
  for(Items::iterator i = m_items.begin(); i != m_items.end(); i++)
  {
    for(Items::iterator j = i; j != m_items.end(); j++)
    {
      if(*i == *j)
        continue;

      for(s32 k = 0; k < (*i).length(); k++)
      {
        if((k >= (*j).length()) || ((*i)[(u32) k] > (*j)[(u32) k]))
        {
          kgmString tmp = (*j);

          (*j) = (*i);

          (*i) = tmp;

          break;
        }
        else if((*i)[(u32) k] < (*j)[(u32) k])
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
}

void kgmGuiList::onMsLeftUp(int k, int x, int y)
{
  kgmGui::onMsLeftUp(k, x, y);

  Rect rect = Rect(0, 0, m_rect.w, m_rect.h);
  m_itemSel = m_position + (y - rect.y) / m_itemHeight;

  if((m_itemSel >= 0) && (m_itemSel < m_items.size()))
  {
    sigSelect(m_itemSel);
  }
}

void kgmGuiList::onMsMove(int k, int x, int y)
{
  if(m_scroll->visible() && m_scroll->m_rect.inside(x, y))
    return m_scroll->onMsMove(k, x - m_scroll->m_rect.x, y - m_scroll->m_rect.y);
  else if((y / (s32) m_itemHeight < m_items.size()) &&
          (y < (m_rect.y + m_rect.h - 1)))
    m_itemPnt = getFirstVisibleItem() +  y / m_itemHeight;
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
    if(m_itemSel < (s32) m_position)
      m_position = m_itemSel;
    m_scroll->setPosition(m_position);
    break;
  case KEY_DOWN:
    if(m_itemSel < (m_items.size() - 1))
      m_itemSel++;
    if(m_itemSel >= (s32) (m_position + iir))
      m_position++;
    m_scroll->setPosition(m_position);
    break;
  case KEY_ENTER:
    if(m_itemSel >= 0 && m_itemSel < m_items.size())
    {
      sigSelect(m_itemSel);
    }
    break;
  }
}

void kgmGuiList::onScroll(u32 n)
{
  m_position = n;
}
