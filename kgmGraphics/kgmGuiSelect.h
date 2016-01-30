#pragma once

#include "kgmGui.h"
#include "kgmGuiText.h"
#include "kgmGuiList.h"

class kgmGuiSelect : public kgmGui
{
  KGM_OBJECT(kgmGuiSelect);

  kgmGuiText* m_text;
  kgmGuiList* m_list;

  Slot<kgmGuiSelect, u32> slotSelect;

public:
  kgmGuiSelect();
  kgmGuiSelect(kgmGui *par, int x, int y, int w, int h);

  void add(kgmString);

  kgmString getText();

protected:
  void onMsLeftDown(int k, int x, int y);

private:
  __stdcall void onSelect(u32);
};
