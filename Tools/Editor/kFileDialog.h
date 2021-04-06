#ifndef KFILEDIALOG_H
#define KFILEDIALOG_H

#include "../../kgmGraphics/kgmGui.h"
#include "../../kgmGraphics/kgmGuiText.h"
#include "../../kgmGraphics/kgmGuiList.h"
#include "../../kgmGraphics/kgmGuiButton.h"

class kFileDialog : public kgmGui
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

public:
  typedef void (*ClickEventCallback)(void*);

private:
  ClickEventCallback fnCallback;
  void*              fnArg;

public:
  kFileDialog();

  void listFolder();

  void forOpen(kgmString dir, ClickEventCallback fn_call = null, void* arg = null);
  void forSave(kgmString dir, ClickEventCallback fn_call = null, void* arg = null);
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
  void onAction(kgmGui*, u32);
  void onMsLeftUp(int, int, int);
  void onMsMove(int, int, int);
};

#endif // KFILEDIALOG_H
