#include "../kgmSystem/kgmSystem.h"
#include "../kgmGame/kgmGameApp.h"
#include "../kgmGame/kgmGameBase.h"

#include "../kgmBase/kgmXml.h"
#include "../kgmBase/kgmLog.h"
#include "../kgmBase/kgmPointer.h"
#include "../kgmBase/kgmEncryptor.h"
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
    //setMsAbsolute(true);
  }

  ~kGame()
  {
#ifdef DEBUG
    kgm_log() << "kGame::~kGame [" << (void*)this << "]\n";
#endif
  }
};

class kApp: public kgmGameApp
{
  KGM_OBJECT(kApp);

#ifdef ANDROID
public:
#endif

  bool failed;

  //kGame* game;
  kgmGameBase* game;

public:
  kApp()
  {
    m_game = game = null;

    failed = false;

    //kgmEncryptor e;

    //e.init();
  }

  ~kApp()
  {
    if (game)
    {
      game->release();
    }

    //delete wnd;
#ifdef DEBUG
    kgm_log() << "kApp::~kApp [" << (void*)this << "].\n";
#endif
  }

  void gameInit()
  {
    u32 w, h;
    kgmSystem::getDesktopDimension(w, h);

    //game = new kGame();
    game = new kgmGameBase();

    m_game = static_cast<kgmIGame*> (game);
    //m_game = reinterpret_cast<kgmIGame*>(game);

    setMainWindow(game);

    if (game && !game->gInit())
    {
      failed = true;
    }
  }

  void gameLoop()
  {
    if(game != null && failed != true)
    {
      game->loop();
      //kgmWindow* w = (kgmWindow*) game;
      //kgmIGame*  g = (kgmIGame*) game;

      //w->loop();
    }
  }

  void gameFree()
  {
    if(game)
    {
      game->release();

      m_game = null;
      game   = null;
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
