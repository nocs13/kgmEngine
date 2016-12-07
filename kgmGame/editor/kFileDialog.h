#ifndef KFILEDIALOG_H
#define KFILEDIALOG_H

#include "../../kgmGraphics/kgmGui.h"
#include "../../kgmGraphics/kgmGuiText.h"
#include "../../kgmGraphics/kgmGuiList.h"
#include "../../kgmGraphics/kgmGuiFrame.h"
#include "../../kgmGraphics/kgmGuiButton.h"

namespace kgmGameEditor
{

class kFileDialog : public kgmGuiFrame
{
  kgmGuiList* list;
  kgmGuiText* text;
  kgmGuiButton* btnCmd;
  kgmGuiButton* btnFail;

  static kgmString DIRCON;

  kgmString pathFolder;
  kgmString filePath;
  kgmString filter;

  bool      modeSave;
  bool      localable;
  bool      allsee;

  Slot<kFileDialog, int> slotSelect, slotFail;

public:
  Signal<kFileDialog*> sigSelect, sigFail;

public:
  kFileDialog();

  void listFolder();

  void forOpen(kgmString dir);
  void forSave(kgmString dir);
  void setFilter(kgmString flt);

  void changeLocation(bool s);
  void showHidden(bool s);

  kgmString getFile();
  kgmString getPath();
  kgmString getFolder();

  virtual void onOpen(){}
  virtual void onSave(){}
  virtual void onFail(){}

protected:
  __stdcall void onFileSelect();
  __stdcall void onOpenSelect(int);
  __stdcall void onSaveSelect(int);
  __stdcall void onFailSelect();
  __stdcall void onEditFile(kgmString);

  void onAction(kgmGui*, u32);
  void onMsLeftUp(int, int, int);
  void onMsMove(int, int, int);
};

}

#endif // KFILEDIALOG_H
