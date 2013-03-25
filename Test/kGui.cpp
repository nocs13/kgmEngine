#include "kGui.h"

kGui::kGui(kgmGameBase* game)
{
  m_guiMain = null;
  kgmString text;

  if(!game)
    return;
  else
    m_game = game;

  int x, y, w, h;

  game->getRect(x, y, w, h);

  m_guiMain = new kgmGui(null, 0, 0, w, h);
  m_guiMain->addListener(this);

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
  btn->setSid("quit");

  game->addGui(m_guiMain);
}

void kGui::onAction(kgmEvent* e, int a)
{
  kgmGui* gui = (kgmGui*)e;

  kgmString sid = gui->getSid();

  if(sid == "quit")
    m_game->gQuit();
}
