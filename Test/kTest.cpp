#include "../kgmSystem/kgmSystem.h"
#include "../kgmGame/kgmGameApp.h"
#include "../kgmGame/kgmGameBase.h"

#include "../kgmBase/kgmXml.h"
#include "../kgmBase/kgmLog.h"
#include "../kgmBase/kgmPointer.h"
#include "../kgmGraphics/kgmGuiButton.h"
#include "../kgmSystem/kgmVulkan.h"

#include "kGlobals.h"
#include "kGui.h"

/*
 * valgrind --leak-check=full --show-leak-kinds=definite --track-origins=yes --error-limit=no -v  ./kTest > ktest.vlg 2>&1
*/

bool g_ms_camera = true;

const char* maps[] =
{
  "map test0", "map000.map",
  "map test1", "map001.map",
  "map test2", "map002.map",
  "map test3", "map003.map",
  "map test4", "map004.map",
  "map test5", "map005.map",
};

class kGame: public kgmGameBase
{
  struct GameData
  {
    u16 sig;
    u8  maps;
    u8  cmap;
  };

  kGui *gui = null;

  //GameData data;

public:
  kGame(bool edit)
    :kgmGameBase(edit)
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

  void onKeyDown(int k)
  {
    kgmGameBase::onKeyDown(k);
  }

  void onMsLeftUp(int k, int x, int y)
  {
    kgmGameBase::onMsLeftUp(k, x, y);
  }

  void onMsLeftDown(int k, int x, int y)
  {
    kgmGameBase::onMsLeftDown(k, x, y);
  }

  void onMsMove(int k, int x, int y)
  {
    kgmGameBase::onMsMove(k, x, y);
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

  /*int main(int argc, char **argv)
  {
    u32 w, h;
    kgmSystem::getDesktopDimension(w, h);
    m_game = game = new kGame();

    game->setRect(0, 0, w, h);
    game->guiShow(false);
    game->loop();
    game->release();

    return 0;
  }*/

  void gameInit(bool edit)
  {
    u32 w, h;
    kgmSystem::getDesktopDimension(w, h);

    kgm_log() << "CPU COUNT: " << kgmSystem::getCpuConcurrency() << "\n";

    //game = kgm_ptr<kGame>(new kGame(edit));
    game = new kGame(edit);

    m_game = ((kGame*)game);

    game->gInit();

    game->setRect(0, 0, w, h);

#ifdef VULKAN
    //kgmVulkan *vk = new kgmVulkan(game);

    //delete vk;
#endif
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

  void gameEdit()
  {
    game->edit();
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

//main object
kApp theApp;
//////////////

//FOR ANDROID
#ifdef ANDROID

#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <EGL/egl.h>
#include <GLES/gl.h>

kApp*          m_app  = null;

bool kgm_android_init_app()
{
  m_app = new kApp();

  return true;
}

const char*  kgm_android_classname()
{
  return "com/example/Test/Test";
}

extern "C"
{
JNIEXPORT void  JNICALL Java_com_example_Test_TestLib_init(JNIEnv * env, jobject obj,  jint width, jint height,
                                                           jobject am, jobject surface);
JNIEXPORT void  JNICALL Java_com_example_Test_TestLib_quit(JNIEnv * env, jobject obj);
JNIEXPORT void  JNICALL Java_com_example_Test_TestLib_idle(JNIEnv * env, jobject obj);
JNIEXPORT void  JNICALL Java_com_example_Test_TestLib_onKeyboard(JNIEnv * env, jobject obj, jint a, jint key);
JNIEXPORT void  JNICALL Java_com_example_Test_TestLib_onTouch(JNIEnv * env, jobject obj,  jint act, jint x, jint y);
};

JNIEXPORT void JNICALL Java_com_example_Test_TestLib_init(JNIEnv* env, jobject obj,  jint width, jint height, jobject am,
                                                          jobject surface)
{
  if(kgmGameApp::gameApplication()->game())
  {
    kgmGameApp::gameApplication()->game()->getWindow()->setRect(0, 0, width, height);
  }
  else
  {
    kgm_android_init_app();
    kgmGameApp::gameApplication()->android_init(env, obj, width, height, am, surface);
  }
}

JNIEXPORT void JNICALL Java_com_example_Test_TestLib_quit(JNIEnv * env, jobject obj)
{
  kgmGameApp::gameApplication()->android_quit(env, obj);
}

JNIEXPORT void JNICALL Java_com_example_Test_TestLib_idle(JNIEnv * env, jobject obj)
{
  kgmGameApp::gameApplication()->android_idle(env, obj);
}

JNIEXPORT void JNICALL Java_com_example_Test_TestLib_onKeyboard(JNIEnv * env, jobject obj, jint a, jint key)
{
  kgmGameApp::gameApplication()->android_onKeyboard(env, obj, a, key);
}

JNIEXPORT void JNICALL Java_com_example_Test_TestLib_onTouch(JNIEnv * env, jobject obj, jint a, jint x, jint y)
{
  kgmGameApp::gameApplication()->android_onTouch(env, obj,  a, x, y);
}
#endif
