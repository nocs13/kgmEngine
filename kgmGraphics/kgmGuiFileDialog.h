#ifndef KGMGUIFILEDIALOG_H
#define KGMGUIFILEDIALOG_H

#include "kgmGui.h"
#include "kgmGuiText.h"
#include "kgmGuiList.h"
#include "kgmGuiFrame.h"
#include "kgmGuiButton.h"

class kgmGuiFileDialog : public kgmGuiFrame
{
  KGM_OBJECT(kgmGuiFileDialog);

private:
  static kgmGuiFileDialog* single;

  kgmGuiList* list;
  kgmGuiText* text;
  kgmGuiButton* btnCmd;

  static kgmString DIRCON;

  kgmString pathFolder;
  kgmString filePath;
  kgmString filter;

  bool      modeSave;
  bool      localable;
  bool      allsee;

  Slot<kgmGuiFileDialog, int> slotSelect, slotFail;

public:
  Signal<kgmGuiFileDialog*> sigSelect, sigFail;

protected:
  kgmGuiFileDialog();
  ~kgmGuiFileDialog();

public:
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

  static kgmGuiFileDialog* getDialog();

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

#endif // KGMGUIFILEDIALOG_H
