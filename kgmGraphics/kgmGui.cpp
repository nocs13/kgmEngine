#include "kgmGui.h"
#include "kgmFont.h"
#include "../kgmBase/kgmIGC.h"
#include "kgmIGuiDraw.h"

KGMOBJECT_IMPLEMENT(kgmGui, kgmEvent)

kgmGui::kgmGui()
{
  m_id = 0;
  m_parent = 0;
  m_erase  = false;

  m_view     = false;
  m_freeze   = false;
  m_resize   = true;
  m_hasMouse = false;
  m_hasInput = false;
  m_hasAlpha = false;
  m_useStyle = true;
  m_focus    = null;

  m_draw       = 0;
  m_base.scale = true;

  m_color = 0xffffffff;
  m_image = null;
}

kgmGui::kgmGui(kgmGui *par, int x, int y, int w, int h)
{
  m_id = 0;
  m_parent = par;
  m_erase  = false;

  m_rect = m_base.rect = Rect(x, y, w, h);
  m_view = true;
  m_freeze   = false;
  m_resize   = true;
  m_hasMouse = false;
  m_hasInput = false;
  m_hasAlpha = false;
  m_useStyle = true;

  m_focus = null;

  if(par)
  {
    par->addChild(this);
    m_parent = par;
  }

  m_draw        = 0;
  m_base.scale  = true;

  m_color = 0xffffffff;
  m_image = null;
}

kgmGui::~kgmGui()
{
  for(int i = m_childs.size(); i > 0; i--)
    m_childs[i - 1]->release();

  m_childs.clear();

  if(m_parent)
    m_parent->delChild(this);
}

//public:
void kgmGui::show()
{
  m_view = true;
}

void kgmGui::hide()
{
  m_view = false;
}

void kgmGui::close()
{
  onClose();

  if(m_parent == null)
    erase();
}

void kgmGui::scale(float sw, float sh)
{
  m_rect.x = m_base.rect.x * sw;
  m_rect.y = m_base.rect.y * sh;
  m_rect.w = m_base.rect.w * sw;
  m_rect.h = m_base.rect.h * sh;

  for(int i = 0; i < m_childs.size(); i++)
    m_childs[i]->scale(sw, sh);
}

void kgmGui::resize(int w, int h)
{
  m_rect.w = w;
  m_rect.h = h;

  if(m_base.scale)
  {
    float sw = (float)w / (float)m_base.rect.w, sh = (float)h / (float)m_base.rect.h;

    scale(sw, sh);
  }

  onResize(w, h);
}

void kgmGui::repaint(kgmGui* gui)
{
  if(m_draw)
  {
    m_draw->update(gui);
  }
  else if(m_parent)
  {
    m_parent->repaint(gui);
  }
}

void kgmGui::setParent(kgmGui* pr)
{
  if(!pr || pr == m_parent)
    return;

  if(m_parent)
    m_parent->delChild(this);

  pr->addChild(this);
  m_parent = pr;
}

void kgmGui::setRect(kgmGui::Rect& rc)
{
  m_rect = m_base.rect = rc;
}

void kgmGui::setRect(u32 x, u32 y, u32 w, u32 h)
{
  m_rect = m_base.rect = Rect(x, y, w, h);
}

void kgmGui::getRect(kgmGui::Rect& rc, bool abs)
{
  if(abs)
  {
    rc = toAbsolute(this->m_rect);
  }
  else
  {
    rc = m_rect;
  }
}

void kgmGui::getRect(u32& x, u32& y, u32& w, u32& h, bool abs)
{
  if(abs)
  {
    Rect rc = toAbsolute(this->m_rect);
    x = rc.x;
    y = rc.y;
    w = rc.w;
    h = rc.h;
  }
  else
  {
    x = m_rect.x;
    y = m_rect.y;
    w = m_rect.w;
    h = m_rect.h;
  }
}

kgmGui* kgmGui::getRoot()
{
  kgmGui* root = this;

  while(root->m_parent)
    root = root->m_parent;

  return root;
}

kgmGui* kgmGui::getById(u32 id)
{
  if(m_id == id)
    return this;

  for(kgmList<kgmGui*>::iterator i = m_childs.begin(); i != m_childs.end(); ++i)
  {
    kgmGui* g = (*i)->getById(id);

    if(g)
      return g;
  }

  return null;
}

kgmGui* kgmGui::getBySid(kgmString sid)
{
  if(m_sid == sid)
    return this;

  for(kgmList<kgmGui*>::iterator i = m_childs.begin(); i != m_childs.end(); ++i)
  {
    kgmGui* g = (*i)->getBySid(sid);

    if(g)
      return g;
  }

  return null;
}

kgmGui* kgmGui::getFocus()
{
  kgmGui* root = getRoot();

  if(root)
    return root->m_focus;

  return m_focus;
}

bool kgmGui::setFocus(kgmGui* g)
{
  if(!g)
    return false;

  m_focus = g;

  return true;
}


// MESSAGE MANAGER
void kgmGui::onEvent(kgmEvent::Event* e)
{
  if(!e)
    return;

  kgmEvent::onEvent(e);
}

void kgmGui::onMsWheel(int k, int x, int y, int z)
{
  if(!m_view || m_freeze)
    return;

  if(!m_parent)
  {
    x = x - m_rect.x;
    y = y - m_rect.y;
  }

  for(int i = 0; i < m_childs.size(); i++)
  {
    if(m_childs[i]->m_view && !m_childs[i]->m_freeze)
    {
      m_childs[i]->onMsWheel(k, x, y, z);

      break;
    }
  }
}

void kgmGui::onMsMove(int k, int x, int y)
{
  if(!m_view || m_freeze)
    return;

  if(!m_parent)
  {
    x = x - m_rect.x;
    y = y - m_rect.y;
  }

  for (int i = 0; i < m_childs.size(); i++)
  {
    kgmGui* guiPointed = null;
    kgmGui* child = m_childs[i];

    if (m_childs[i]->m_view && !m_childs[i]->m_freeze)
    {
      if(m_childs[i]->m_rect.inside(x, y))
      {
        if(!m_childs[i]->m_hasMouse)
        {
          m_childs[i]->onMsInside();
          m_childs[i]->m_hasMouse = true;
        }

        m_childs[i]->onMsMove(k, x - m_childs[i]->m_rect.x, y - m_childs[i]->m_rect.y);
      }
      else if(m_childs[i]->m_hasMouse)
      {
        m_childs[i]->onMsOutside();
        m_childs[i]->m_hasMouse = false;
      }
      else if(guiPointed = m_childs[i]->getPointed())
      {
        guiPointed->onMsOutside();
        guiPointed->m_hasMouse = false;
      }
    }
  }
}

void kgmGui::onMsLeftDown(int k, int x, int y)
{
  if(!m_view || m_freeze)
    return;
  
  if(!m_parent)
  {
    x = x - m_rect.x;
    y = y - m_rect.y;
  }

  for(int i = 0; i < m_childs.size(); i++)
  {
    if(m_childs[i]->m_view && !m_childs[i]->m_freeze)
    {
      if(m_childs[i]->m_rect.inside(x, y))
      {
        m_childs[i]->onMsLeftDown(k, x - m_childs[i]->m_rect.x, y - m_childs[i]->m_rect.y);

        break;
      }
    }
  }

  if(m_childs.size() < 1)
  {
    kgmGui* root = getRoot();

    if(root)
      root->setFocus(this);
    else
      m_focus = this;
  }
}

void kgmGui::onMsLeftUp(int k, int x, int y)
{
  if(!m_view || m_freeze)
    return;
  
  if(!m_parent)
  {
    x = x - m_rect.x;
    y = y - m_rect.y;
  }

  for (int i = 0; i < m_childs.size(); i++)
  {
    if (m_childs[i]->m_view && !m_childs[i]->m_freeze)
    {
      if (m_childs[i]->m_rect.inside(x, y))
      {
        m_childs[i]->onMsLeftUp(k, x - m_childs[i]->m_rect.x, y - m_childs[i]->m_rect.y);

        break;
      }
    }
  }
}

void kgmGui::onMsRightDown(int k, int x, int y)
{
  if(!m_view || m_freeze)
    return;
  
  if(!m_parent)
  {
    x = x - m_rect.x;
    y = y - m_rect.y;
  }

  for (int i = 0; i < m_childs.size(); i++)
  {
    if (m_childs[i]->m_view && !m_childs[i]->m_freeze)
    {
      if (m_childs[i]->m_rect.inside(x, y))
      {
        m_childs[i]->onMsRightDown(k, x - m_childs[i]->m_rect.x, y - m_childs[i]->m_rect.y);

        break;
      }
    }
  }
}

void kgmGui::onMsRightUp(int k, int x, int y)
{
  if(!m_view || m_freeze)
    return;

  if(!m_parent)
  {
    x = x - m_rect.x;
    y = y - m_rect.y;
  }

  for(int i = 0; i < m_childs.size(); i++)
  {
    if(m_childs[i]->m_view && !m_childs[i]->m_freeze)
    {
      if (m_childs[i]->m_rect.inside(x, y))
      {
        m_childs[i]->onMsRightUp(k, x - m_childs[i]->m_rect.x, y - m_childs[i]->m_rect.y);

        break;
      }
    }
  }
}

void kgmGui::onMsInside()
{
}

void kgmGui::onMsOutside()
{
}

void kgmGui::onKeyDown(int k)
{
  if(!m_view || m_freeze)
    return;
  
  if(m_focus && m_focus != this && !m_focus->m_freeze)
    m_focus->onKeyDown(k);
}

void kgmGui::onKeyUp(int k)
{
  if(!m_view || m_freeze)
    return;
  
  if(m_focus && m_focus != this && !m_focus->m_freeze)
    m_focus->onKeyUp(k);
}

void kgmGui::onResize(int w, int h)
{

}

void kgmGui::onIdle()
{
}

void kgmGui::addChild(kgmGui *e)
{
  if(!e)
    return;

  if(!isChild(e))
    m_childs.push_back(e);
}

void kgmGui::delChild(kgmGui *e)
{
  for(Guis::iterator i = m_childs.begin(); i != m_childs.end(); ++i)
  {
    if(e == *i)
      i = m_childs.erase(i);
  }
}

bool kgmGui::isChild(kgmGui *e)
{
  for(int i = 0; i < m_childs.size(); i++)
    if(m_childs[i] == e)
      return true;

  return false;
}

kgmGui* kgmGui::getFocus(Point pos)
{
  for(int i = 0; i < m_childs.size(); i++)
  {
    if(m_childs[i]->m_view && m_childs[i]->m_rect.inside(pos.x, pos.y))
      return m_childs[i];
  }

  return 0;
}

kgmGui* kgmGui::getPointed()
{
  if(m_hasMouse)
    return this;

  for(int i = 0; i < m_childs.size(); i++)
  {
    if(m_childs[i]->getPointed())
    {
      return m_childs[i];
    }
  }

  return null;
}
