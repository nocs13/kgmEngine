#include "kGui.h"

kGui::kGui(kgmGameBase* game)
{
  m_guiMain = null;
  kgmString text;

  if(!game)
    return;

  int x, y, w, h;

  game->getRect(x, y, w, h);

  m_guiMain = new kgmGui(null, 0, 0, w, h);

  kgmGui* gui = new kgmGui(m_guiMain, 0, 0, w, 50);

  kgmGuiButton* btn = new kgmGuiButton(gui, 0, 0, w/3, 50);
  text = "Game";
  btn->setText(text);
  btn = new kgmGuiButton(gui, w/3, 0, w/3, 50);
  text = "Settings";
  btn->setText(text);
  btn = new kgmGuiButton(gui, 2 * w/3, 0, w/3, 50);
  text = "Quit";
  btn->setText(text);

  game->addGui(m_guiMain);
}
