#include "../kgmSystem/kgmSystem.h"
#include "../kgmGame/kgmGameApp.h"
#include "../kgmGame/kgmGameBase.h"

#include "../kgmBase/kgmXml.h"
#include "../kgmBase/kgmLog.h"
#include "../kgmBase/kgmPointer.h"
#include "../kgmGraphics/kgmGuiButton.h"

/*
 * valgrind --leak-check=full --show-leak-kinds=definite --track-origins=yes --error-limit=no -v  ./kTest > ktest.vlg 2>&1
*/

class kGame: public kgmGameBase
{
  KGM_OBJECT(kGame);
public:
  kGame()
  {
    setMsAbsolute(true);
  }

  ~kGame()
  {
#ifdef DEBUG
  kgm_log() << "kGame::~kGame.\n";
#endif
  }
};

class kApp: public kgmGameApp
{
  KGM_OBJECT(kApp);

#ifdef ANDROID
public:
#endif

  kGame* game;

  bool failed;

public:
  kApp()
  {
    game = null;

    failed = false;
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

    m_game = game = new kGame();

    if (!game->gInit())
    {
      failed = true;

      return;
    }

    game->setRect(0, 0, w, h);

    kgmGameApp::Options* options = kgmGameApp::gameApplication()->options();

    if (options->map)
      game->gLoad(options->mapid);

    if (options->edit)
      game->gSwitch(kgmIGame::State_Edit);

    m_game = game;
  }

  void gameLoop()
  {
    if(game != null && failed != true)
    {
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
