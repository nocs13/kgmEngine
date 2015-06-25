#include "kGui.h"
#include "kGlobals.h"
#include "../kgmBase/kgmLog.h"
#include "../kgmGame/kgmGameApp.h"

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

#ifdef DEBUG
  kgm_log() << "GameRect: " << w << " " << h << ".";
#endif

  m_guiMain = new kgmGui(null, 0, 0, w, h);

  kgmGui* gui = new kgmGui(m_guiMain, 0, 0, w, 50);

  kgmGuiButton* btn = new kgmGuiButton(gui, 0, 0, w/3, 50);
  text = "Game";
  btn->setSid("game");
  btn->setText(text);
  btn = new kgmGuiButton(gui, w/3, 0, w/3, 50);
  text = "Settings";
  btn->setSid("settings");
  btn->setText(text);
  btn = new kgmGuiButton(gui, 2 * w/3, 0, w/3, 50);
  text = "Quit";
  btn->setText(text);
  btn->setSid("quit");

  gui = new kgmGui(m_guiMain, 0, 50, w, h - 50);
  gui->setSid("guiSettings");
  gui->hide();

  gui = new kgmGuiText(gui, 10, 10, 300, 40);
  gui->setText("Developer: Karakal_13");
  gui->show();

  gui = new kgmGui(m_guiMain, 0, 50, w, h - 50);
  gui->setSid("guiGame");
  btn = new kgmGuiButton(gui, 1, 10, 100, 30);
  text = "Start";
  btn->setText(text);
  btn = new kgmGuiButton(gui, 1, 50, 100, 30);
  text = "Continue";
  btn->setText(text);

  kgmGuiList* lst = new kgmGuiList(gui, w / 2, 10, w / 2 - 2, h - 100);
  lst->m_itemHeight = 40;
  lst->setSid("levels");
  lst->addItem("map000.kgm");
  lst->addItem("map001.kgm");
  lst->addItem("map002.kgm");
  lst->addItem("map003.kgm");
  lst->addItem("map004.kgm");
  lst->addItem("map005.kgm");
  gui->show();

  game->guiAdd(m_guiMain);


  m_guiPause = new kgmGui(null, w / 2 - 50, h / 2 - 50, 100, 100);
  m_guiPause->hide();

  btn = new kgmGuiButton(m_guiPause, 1, 1, 98, 48);
  btn->setText("Resume");
  btn->setSid("gameResume");

  btn = new kgmGuiButton(m_guiPause, 1, 51, 98, 48);
  btn->setText("Exit");
  btn->setSid("gameExit");

  game->guiAdd(m_guiPause);
}

kGui::~kGui()
{
  m_guiMain->release();
  m_guiPause->release();

#ifdef DEBUG
  kgm_log() << "kGui::~kGui.\n";
#endif
}

void kGui::onAction(kgmEvent* e, int a)
{
  kgmGui* gui = (kgmGui*)e;

  kgmString sid = gui->getSid();

  if(sid == "quit")
  {
    m_game->gQuit();
#ifdef ANDROID
    kgmGameApp::gameApplication()->android_exit();
#endif
  }
  else if(sid == "game")
  {
    m_guiGame->show();
    m_guiSettings->hide();
  }
  else if(sid == "settings")
  {
    m_guiGame->hide();
    m_guiSettings->show();
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

      if(g_ms_camera)
        m_game->setMsAbsolute(false);
    }
  }
  else if(sid == "gameExit")
  {
    m_game->gSwitch(kgmIGame::State_Clean);
    m_guiPause->hide();
    m_guiMain->show();

    if(g_ms_camera)
      m_game->setMsAbsolute(true);
  }
  else if(sid == "gameResume")
  {
    m_guiPause->hide();
    m_game->gPause(false);

    if(g_ms_camera)
      m_game->setMsAbsolute(false);
  }
}

void kGui::viewAgain()
{
  m_guiMain->show();

  if(g_ms_camera)
    m_game->setMsAbsolute(true);
}
