#include "kgmGuiText.h"
#include "../kgmBase/kgmLog.h"

kgmGuiText::kgmGuiText()
  :kgmGui()
{
  editable = false;
  numeric = false;
  hexnum  = false;
  shift = false;
  index = 0;
  place = 0;
  fwidth = 10;
  fheight = 15;
}

kgmGuiText::kgmGuiText(kgmGui *par, u32 x, u32 y, u32 w, u32 h)
 :kgmGui(par, x, y, w, h)
{
  editable = false;
  numeric = false;
  hexnum  = false;
  shift = false;
  index = 0;
  place = 0;
  fwidth = 10;
  fheight = 15;
}

kgmGuiText::~kgmGuiText()
{
}

void kgmGuiText::setEditable(bool e)
{
  editable = e;
}

void kgmGuiText::setNumeric(bool n)
{
  numeric = n;

  if (numeric)
    hexnum = false;
}

void kgmGuiText::setHexnum(bool n)
{
  hexnum = n;

  if (hexnum)
    numeric = false;
}

void kgmGuiText::dropCursor()
{
  index = 0;
}

bool kgmGuiText::isReadOnly()
{
  return !editable;
}

bool kgmGuiText::isNumeric()
{
  return numeric;
}

u32 kgmGuiText::getCursor()
{
  return place;
}

u32 kgmGuiText::getIndex()
{
  return index;
}

void kgmGuiText::moveLeft()
{
  if(index > 0)
    index--;

  if(place > 0)
    place--;
}

void kgmGuiText::moveRight()
{
  if(index < (u32) m_text.length())
    index++;

  u32 cpw = m_rect.width() / fwidth; // chars per width

  if(place < cpw && place < (u32) m_text.length())
    place++;
}

void kgmGuiText::delLeft()
{
  kgmString pt1;
  kgmString pt2;

  if(!m_text.length())
    return;

  if(index > 0)
    pt1 = kgmString(m_text.data(), index - 1);

  if(index < (u32) m_text.length())
    pt2 = kgmString(m_text.data() + index, m_text.length() - index);

  moveLeft();

  m_text = pt1 + pt2;

  kgm_log() << "kgmGuiTex::delLeft index " << (s32)index << " place " << (s32)place << "\n"; 
}

void kgmGuiText::delRight()
{
  kgmString pt1;
  kgmString pt2;

  if(!m_text.length() || (index == (u32) m_text.length()))
    return;

  if(index != 0)
    pt1 = kgmString(m_text.data(), index);

  if(index < (u32) m_text.length())
    pt2 = kgmString(m_text.data() + index + 1, m_text.length() - index - 1);

  if(index != 0 && index >= (u32) m_text.length())
    index--;

  m_text = pt1 + pt2;

  kgm_log() << "kgmGuiTex::delRight index " << (s32)index << " place " << (s32)place << "\n"; 
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

  //FIXME

  switch (k)
  {
  case KEY_ENTER:
    break;
  case KEY_LSHIFT:
  case KEY_RSHIFT:
    shift = true;
    break;
  case KEY_LEFT:
    moveLeft();
    break;
  case KEY_RIGHT:
    moveRight();
    break;
  case KEY_BACK:
  {
    delLeft();

    sigChange(getText());
  }
    break;
  case KEY_DELETE:
  {
    delRight();

    sigChange(getText());
  }
    break;
  default:
  {
    kgmString pt1;
    kgmString pt2;
    kgmString pt3;

    u16 akey = toAnsii(shift, k);

    if(KEY_NONE == akey)
      break;

    if(numeric)
    {
      if((akey < '0' || akey > '9'))
      {
        if(akey == '.')
        {
          char dot = '.';

          if(getText().exist(dot))
          {
            return;
          }
        }
        else if(akey == '-')
        {
          if(index != 0)
            return;
        }
        else
        {
          return;
        }
      }
    }
    else if (hexnum)
    {
      bool pass = false;

      if(akey >= '0' && akey <= '9')
        pass = true;

      if (akey >= 'a' && akey <= 'f')
        pass = true;

      if (akey >= 'A' && akey <= 'F')
        pass = true;

      if (!pass)
        return;
    }

    pt3.fromSym(akey);

    if(index != 0)
      pt1 = kgmString(m_text.data(), index);

    if(index < (u32) m_text.length())
      pt2 = kgmString(m_text.data() + index, m_text.length() - index);

    m_text = pt1 + pt3 + pt2;

    moveRight();

    sigChange(getText());

    kgm_log() << "kgmGuiTex::delLeft index " << (s32)index << " place " << (s32)place << "\n";
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
