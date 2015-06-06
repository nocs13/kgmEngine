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
    typedef kgmCallback<void, kgmObject*, u32> ClickEventCallback;

  private:
    ClickEventCallback callback;

  public:
    Labels(kgmGui* parent)
      :kgmGuiMenu(parent), callback(null, null)
    {

    }

    void setLabelsCallback(ClickEventCallback call)
    {
      callback = call;
    }

    void onMsLeftUp(int key, int x, int y)
    {
      Item* selected = item->clickPointer(x, y);

      if(selected)
      {
        for(int i = 0; i < item->getItemsCount(); i++)
        {
          Item* citem = item->getItem(i);

          if(citem == selected && callback.hasObject())
            callback(i);
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
