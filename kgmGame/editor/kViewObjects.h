#ifndef KVIEWOBJECTS_H
#define KVIEWOBJECTS_H

#include "../../kgmGraphics/kgmGuiFrame.h"
#include "../../kgmGraphics/kgmGuiList.h"
#include "../../kgmGraphics/kgmGuiButton.h"

namespace kgmGameEditor
{

class kViewObjects : public kgmGuiFrame
{
public:
  typedef kgmCallback<void, kgmObject*> CloseCallback;
  typedef kgmCallback<void, kgmObject*, kgmString> SelectCallback;

private:
  CloseCallback  callClose;
  CloseCallback  callEclose;
  SelectCallback callSelect;

  kgmEvent* target;
  kgmGuiList* list;

  bool closeOnSelect;

public:
  kViewObjects(kgmEvent* tar, int x, int y, int w, int h, bool selclose = true);

  void onClose();
  void onSelectItem();

  void setCloseCallback(CloseCallback cc)
  {
    callEclose = cc;
  }

  void setSelectCallback(SelectCallback sc)
  {
    callSelect = sc;
  }

  void onAction(kgmGui* g, u32 id)
  {
    target->onAction(this, id);
  }

  bool addItem(kgmString item)
  {
    if(hasItem(item))
      return false;

    list->addItem(item);

    return true;
  }

  bool hasItem(kgmString item)
  {
    for(int i = 0; i < list->m_items.length(); i++)
    {
      if(item == list->m_items[i])
        return true;
    }

    return false;
  }
};

}

#endif // KVIEWOBJECTS_H
