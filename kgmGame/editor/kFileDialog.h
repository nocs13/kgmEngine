#ifndef KFILEDIALOG_H
#define KFILEDIALOG_H

#include "../../kgmGraphics/kgmGui.h";
#include "../../kgmGraphics/kgmGuiText.h";
#include "../../kgmGraphics/kgmGuiList.h";
#include "../../kgmGraphics/kgmGuiButton.h";

namespace kgmGameEditor
{

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
  typedef kgmCallback<void(void*, kgmString)> ClickEventCallback;

private:
  ClickEventCallback callback;
  kgmGuiButton::ClickEventCallback fail_callback;

public:
  kFileDialog();

  void listFolder();

  void forOpen(kgmString dir, ClickEventCallback call);
  void forSave(kgmString dir, ClickEventCallback call);
  void setFilter(kgmString flt);
  void setFailCallback(kgmGuiButton::ClickEventCallback);

  void changeLocation(bool s);
  void showHidden(bool s);

  kgmString getFile();
  kgmString getPath();
  kgmString getFolder();

  virtual void onOpen(){}
  virtual void onSave(){}
  virtual void onFail(){}

protected:
  void onFileSelect();
  void onOpenSelect();
  void onSaveSelect();
  void onFailSelect();
  void onEditFile(kgmString);
  void onAction(kgmGui*, u32);
  void onMsLeftUp(int, int, int);
  void onMsMove(int, int, int);
};

}

#endif // KFILEDIALOG_H
