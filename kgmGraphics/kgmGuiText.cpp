#include "kgmGuiText.h"

KGMOBJECT_IMPLEMENT(kgmGuiText, kgmGui);

kgmGuiText::kgmGuiText()
{
  editable = false;
  shift = false;
  index = 0;
}

kgmGuiText::kgmGuiText(kgmGui *par, u32 x, u32 y, u32 w, u32 h)
 :kgmGui(par, x, y, w, h)
{
  editable = false;
  shift = false;
  index = 0;
}

kgmGuiText::~kgmGuiText()
{
}

void kgmGuiText::setEditable(bool e)
{
  editable = e;
}

void kgmGuiText::dropCursor()
{
  index = 0;
}

bool kgmGuiText::isReadOnly()
{
  return !editable;
}

u32 kgmGuiText::getCursor()
{
  return index;
}

void kgmGuiText::onChange()
{
  onAction(this, 0);
}

void kgmGuiText::onMsLeftDown(int k, int x, int y)
{
  kgmGui* top = getRoot();

  if(top)
    top->setFocus(this);
}

void kgmGuiText::onKeyDown(int k)
{
  if(!editable)
    return;

  switch (k)
  {
  case KEY_ENTER:
    break;
  case KEY_LSHIFT:
  case KEY_RSHIFT:
    shift = true;
    break;
  case KEY_LEFT:
    if(index > 0)
      index--;
    break;
  case KEY_RIGHT:
    if(index < (m_text.length() - 1))
      index++;
    break;
  case KEY_BACK:
  {
    kgmString pt1;
    kgmString pt2;

    if(index > 0)
      pt1 = kgmString(m_text.data(), index - 1);

    if(index < m_text.length())
      pt2 = kgmString(m_text.data() + index, m_text.length() - index);

    if(index > 0)
      index--;

    m_text = pt1 + pt2;

    onChange();
  }
    break;
  case KEY_DELETE:
  {
    kgmString pt1;
    kgmString pt2;

    if(!m_text.length())
      break;

    if(index != 0)
      pt1 = kgmString(m_text.data(), index);

    if(index < m_text.length())
      pt2 = kgmString(m_text.data() + index + 1, m_text.length() - index - 1);

    if(index >= m_text.length())
      index--;

    m_text = pt1 + pt2;

    onChange();
  }
    break;
  default:
  {
    kgmString pt1;
    kgmString pt2;
    kgmString pt3;

    if(KEY_NONE == toAnsii(shift, k))
      break;

    pt3.fromSym(toAnsii(shift, k));

    if(index != 0)
      pt1 = kgmString(m_text.data(), index);

    if(index < m_text.length())
      pt2 = kgmString(m_text.data() + index, m_text.length() - index);

    index++;

    m_text = pt1 + pt3 + pt2;

    onChange();
  }
    break;
  }
}

void kgmGuiText::onKeyUp(int k)
{
  switch(k)
  {
  case KEY_LSHIFT:
  case KEY_RSHIFT:
    shift = false;
    break;
  }
}

char kgmGuiText::toAnsii(bool shift, u16 key)
{
  if(key >= KEY_0 && key <= KEY_9)
  {
    if(!shift)
    {
      return 48 + key - KEY_0;
    }
    else
    {

    }
  }
  else if(key >= KEY_A && key <= KEY_Z)
  {
    if(!shift)
    {
      return 97 + key - KEY_A;
    }
    else
    {
      return 65 + key - KEY_A;
    }
  }
  else if(key == KEY_SUB)
  {
    if(shift)
      return 95;
    else
      return 45;
  }
  else if(key == KEY_COMMA)
  {
    if(shift)
      return 60;
    else
      return 44;
  }
  else if(key == KEY_DOT)
  {
    if(shift)
      return 62;
    else
      return 46;
  }

  return KEY_NONE;
}
