#pragma once
#include "kgmGuiLabel.h"

class kgmGuiCheck: public kgmGuiLabel
{
  KGM_OBJECT(kgmGuiCheck);
public:

  enum State
  {
    StateNone,
    StateFocus,
  };

private:
  State  m_state;
  bool   m_check;

  kgmList<kgmGuiCheck*> m_group;
  kgmGuiCheck*          m_group_owner;

public:
  Signal<bool> sigClick;

public:
  kgmGuiCheck();
  kgmGuiCheck(kgmGui *par, u32 x, u32 y, u32 w, u32 h);
  virtual ~kgmGuiCheck();

  State getState()
  {
    return m_state;
  }

  bool isCheck()
  {
    return m_check;
  }

  void setCheck(bool check)
  {
    m_check = check;

    if(m_check && (m_group_owner != null || m_group.length() > 0))
    {
      if(m_group_owner != null)
        m_group_owner->groupSelect(this);
      else if(m_group.length() > 0)
        groupSelect(this);
    }
  }

  void addGroup(kgmGuiCheck* gui)
  {
    if(gui == null)
      return;

    m_group.add(gui);
    gui->m_group_owner = this;
  }

  void groupSelect(kgmGuiCheck* gui)
  {
    for(s32 i = 0; i < m_group.length(); i++)
    {
      if (m_group[i] != gui)
      {
        m_group[i]->m_check = false;
      }
    }

    if(gui != this)
      m_check = false;
  }

private:
  void onMsLeftUp(int key, int x, int y);
  void onMsInside();
  void onMsOutside();
};
