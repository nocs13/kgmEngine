#pragma once
#include "kgmGui.h"
#include "kgmGuiMenu.h"

class kgmGuiTab: public kgmGui
{
  KGM_OBJECT(kgmGuiTab);

private:

  class Menu: public kgmGuiMenu
  {
  public:
    typedef kgmCallback<void, kgmObject*, u32> ClickEventCallback;

    ClickEventCallback callback;
  public:
    Menu(kgmGui* parent)
      :kgmGuiMenu(parent), callback(null, null)
    {

    }

    void onMsLeftUp(int key, int x, int y)
    {
      Item* selected = item->clickPointer(x, y);

      if(selected)
      {
        if(callback.hasObject())
          callback(item->getSelected());
      }
    }
  };

private:
  u32  m_index;
  u32  tab_height;

  kgmGui*     client;
  kgmGuiMenu* labels;

  kgmList<kgmGui*> tabs;

public:
 kgmGuiTab();
 kgmGuiTab(kgmGui *par, int x, int y, u32 w, u32 h);
 virtual ~kgmGuiTab();

 u32 set(u32 i);
 u32 next();
 u32 previous();

 kgmGui* active();
 kgmGui* addTab(kgmString title);

 __stdcall void select(u32);
};
