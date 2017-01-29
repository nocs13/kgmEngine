#pragma once
#include "kgmGui.h"
#include "kgmGuiMenu.h"

class kgmGuiTab: public kgmGui
{
  KGM_OBJECT(kgmGuiTab);

private:

  class Labels: public kgmGuiMenu
  {
  public:
    Signal<u32> sigSwitch;

  public:
    Labels(kgmGui* parent): kgmGuiMenu(parent)
    {

    }

    void onMsLeftUp(int key, int x, int y)
    {
      Item* selected = root->clickPointer(x, y);

      if(selected)
      {
        for(int i = 0; i < root->getItemsCount(); i++)
        {
          Item* citem = root->getItem(i);

          if(citem == selected) {
            choose = selected;
            sigSwitch(i);
          }
        }
      }
    }
  };

private:
  u32  m_index;
  u32  tab_height;

  kgmGui*     client;
  Labels*     labels;

  u32 tabs;

  Slot<kgmGuiTab, u32> slotLabel;

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
