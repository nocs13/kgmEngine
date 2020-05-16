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

class kTestGame: public kgmWindow, public kgmIGame
{
  KGM_OBJECT(kTestGame);

public:
  kTestGame()
  :kgmWindow(0, "kTestGame", 0, 0, 800, 600, 24, false)
  {
    //setMsAbsolute(true);
  }

  ~kTestGame()
  {
#ifdef DEBUG
    kgm_log() << "kTestGame::~kTestGame [" << (void*)this << "]\n";
#endif
  }

  int            gQuit()              { return 0; }
  int            gInit()              { return 1; }
  int            gLoad(kgmString)     { return 0; }
  int            gUnload()            { return 0; }
  int            gButton(game_button) { return 0; }
  u32            gState()             { return 0; }
  int            gSwitch(u32)         { return 0; }

  bool           gUnitRegister(kgmString, NEW_UNIT) { return 0; }

  kgmUnit*       gSpawn(kgmString)    { return null; }
  bool           gAppend(kgmUnit*) { return 0; }
  kgmUnit*       gUnit(kgmString)  { return 0; }
  Iterator*      gUnits()          { return 0; }

  void           guiAdd(kgmGui* g) {}

  kgmIPhysics*    getPhysics() { return 0; }
  kgmIAudio*      getAudio() { return 0; }
  kgmIVideo*      getVideo() { return 0; }
  kgmILogic*      getLogic() { return 0; }
  kgmIScript*     getScript() { return 0; }
  kgmIGraphics*   getGraphics() { return 0; }
  kgmIResources*  getResources() { return 0; }
  kgmWindow*      getWindow() { return 0; }
  kgmSystem*      getSystem() { return 0; }

  u32             timeUpdate() { return 0; }

  //void loop() {}

protected:
  void  initResources() {}
  void  initGraphycs()  {}
  void  initPhysics()  {}
  void  initSystem()  {}
  void  initAudio()  {}
  void  initLogic()  {}
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
  //kTestGame* game;

  //kgmWindow* wnd = null;

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
      delete game;
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
    //game = new kTestGame();

    m_game = static_cast<kgmIGame*> (game);
    //m_game = reinterpret_cast<kgmIGame*>(game);

    setMainWindow(game);

    if (game && !game->gInit())
    {
      failed = true;
    }

    //wnd = new kgmWindow((kgmWindow*) null, "kgm", 0, 0, 800, 600, 24, false);
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
      delete game;

      m_game = null;
      game   = null;
    }

    //delete wnd;
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
