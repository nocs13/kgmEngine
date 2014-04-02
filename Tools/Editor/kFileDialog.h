#ifndef KFILEDIALOG_H
#define KFILEDIALOG_H

#include "../../kgmGraphics/kgmGui.h";
#include "../../kgmGraphics/kgmGuiList.h";
#include "../../kgmGraphics/kgmGuiButton.h";

class kFileDialog : public kgmGui
{
  kgmGuiList* list;
  kgmGuiButton* button;

  static kgmString DIRCON;

  kgmString pathFolder;

public:
  kFileDialog();

  void listFolder();

  void forRead(kgmString dir);

  void onAction(kgmGui*, u32);
  void onMsLeftUp(int, int, int);
  void onMsMove(int, int, int);
};

#endif // KFILEDIALOG_H
