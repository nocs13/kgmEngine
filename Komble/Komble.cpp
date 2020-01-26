#include "../kgmSystem/kgmSystem.h"
#include "../kgmGame/kgmGameApp.h"
#include "../kgmGame/kgmGameBase.h"

#include "../kgmBase/kgmXml.h"
#include "../kgmBase/kgmLog.h"
#include "../kgmBase/kgmPointer.h"
#include "../kgmGraphics/kgmGuiButton.h"

#ifdef VULKAN
#include "../kgmSystem/kgmVulkan.h"
#endif

/*
 * valgrind --leak-check=full --show-leak-kinds=definite --track-origins=yes --error-limit=no -v  ./kTest > ktest.vlg 2>&1
*/

class kGame: public kgmGameBase
{
  kGui *gui = null;

public:
  kGame()
  {
    setMsAbsolute(true);

    if(m_physics)
      m_physics->m_gravity = 1.0f;

    kgmMap<u32, u32> mm;

    for (u32 i = 0; i < 10; i++)
      mm.set(i, i);

    u32 h = mm.height();
    printf("Key val depth %i\n", h);

    kgmMap<u32, u32>::iterator i = mm.get(23);
    if (!i.isEnd())
      printf("Key val %i/%i\n", i.key(), i.data());
    mm.print();
    //while(i != mm.end()) {
      //printf("Key val %i/%i\n", i.key(), i.data());
      //++i;
    //}
  }

  ~kGame()
  {
#ifdef DEBUG
  kgm_log() << "kGame::~kGame.\n";
#endif
  }

public:
  void edit()
  {
#ifdef EDITOR
    m_state = State_Edit;
#endif
  }

  void guiShow(bool s)
  {
  }

  void onIdle()
  {
    kgmGameBase::onIdle();
  }

  void onKeyUp(int k)
  {
    kgmGameBase::onKeyUp(k);

#ifdef EDITOR
#else
    if(k == KEY_ESCAPE)
    {
      if(gState() == State_Play)
      {
        gPause(true);
        gui->m_guiPause->show();

        if(g_ms_camera)
          setMsAbsolute(true);
      }
      else if(gState() == State_Pause)
      {
        gPause(false);
        gui->m_guiPause->hide();
        m_msAbs = true;
      }
    }
#endif
  }

  int gCommand(kgmString s)
  {
    if(s == "gameover_fail")
    {
      m_state = kgmIGame::State_Clean;
      gUnload();
      m_state = kgmIGame::State_Idle;
      ((kGui*)gui)->viewAgain();
    }
    else if(s == "gameover_success")
    {
      m_state = kgmIGame::State_Clean;
      gUnload();
      m_state = kgmIGame::State_Idle;
      ((kGui*)gui)->viewAgain();
    }

    return 0;
  }
};

class kApp: public kgmGameApp
{
  KGM_OBJECT(kApp);

#ifdef ANDROID
public:
#endif

  kGame* game;

public:
  kApp()
  {
    game = null;
  }

  ~kApp()
  {
    if (game)
    {
      delete game;
    }
#ifdef DEBUG
    kgm_log() << "kApp::~kApp.\n";
#endif
  }

  void gameInit()
  {
    u32 w, h;
    kgmSystem::getDesktopDimension(w, h);

    game = new kGame();

    m_game = game;

    game->gInit();

    game->setRect(0, 0, w, h);

    kgmGameApp::Options* options = kgmGameApp::gameApplication()->options();

    if (options->map)
      game->gLoad(options->mapid);

    if (options->edit)
      game->gSwitch(kgmIGame::State_Edit);
  }

  void gameLoop()
  {
    if(game != null)
    {
      if(game->gState() == kgmIGame::State_Play)
        game->guiShow(false);

      game->loop();
    }
  }

  void gameFree()
  {
    if(game)
    {
      delete game;

      game = null;
    }
  }

#ifdef ANDROID
  kgmIGame* android_init_game()
  {
    u32 w, h;
    kgmSystem::getDesktopDimension(w, h);
    game = kgm_ptr<kGame>(new kGame(true));

    m_game = ((kGame*)game);
    ((kGame*)game)->setRect(0, 0, w, h);

    return ((kGame*)game);
  }
#endif
};

#ifdef ANDROID

KGM_ANDROID_INIT(Komble)

#else

int main(int argc, char** argv)
{
  kApp app;

  return  app.exec(argc, argv);
}

#endif
