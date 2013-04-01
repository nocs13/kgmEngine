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

  gui = new kgmGui(m_guiMain, 0, 50, w, h - 50);
  gui->setSid("settings");
  gui->hide();

  gui = new kgmGui(m_guiMain, 0, 50, w, h - 50);
  gui->setSid("game");
  btn = new kgmGuiButton(gui, 1, 10, 100, 30);
  text = "Start";
  btn->setText(text);
  btn = new kgmGuiButton(gui, 1, 50, 100, 30);
  text = "Continue";
  btn->setText(text);

  kgmGuiList* lst = new kgmGuiList(gui, 0, 100, 200, 200);
  lst->setSid("levels");
  lst->addItem("level01.kgm");
  gui->show();

  game->addGui(m_guiMain);


  m_guiPause = new kgmGui(null, w / 2 - 100, h / 2 - 100, 200, 200);
  m_guiPause->addListener(this);
  m_guiPause->hide();

  btn = new kgmGuiButton(m_guiPause, 1, 10, 100, 30);
  btn->setText("Resume");
  btn->setSid("gameResume");

  btn = new kgmGuiButton(m_guiPause, 1, 50, 100, 30);
  btn->setText("Exit");
  btn->setSid("gameExit");

  game->addGui(m_guipause);
}

kGui::~kGui()
{
}

void kGui::onAction(kgmEvent* e, int a)
{
  kgmGui* gui = (kgmGui*)e;

  kgmString sid = gui->getSid();

  if(sid == "quit")
  {
    m_game->gQuit();
  }
  else if(sid == "levels")
  {
    kgmGuiList* lst = (kgmGuiList*)gui;
    int i = lst->getSel();

    kgmString map = lst->getItem(i);
    int res = m_game->gLoad(map);

    if(res == kgmIGame::State_Play)
    {
      m_guiMain->hide();
    }
  }
}
