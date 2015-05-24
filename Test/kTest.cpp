#include "../kgmSystem/kgmSystem.h"
#include "../kgmGame/kgmGameApp.h"
#include "../kgmGame/kgmGameBase.h"
#include "../kgmGame/kgmGameScript.h"

#include "../kgmBase/kgmXml.h"
#include "../kgmBase/kgmLog.h"
#include "../kgmBase/kgmPointer.h"
#include "../kgmGraphics/kgmGuiButton.h"

#include "kGlobals.h"
#include "kGui.h"
#include "Actors/ACamera.h"
#include "Actors/kInput.h"

KGMOBJECT_IMPLEMENT(ACamera, kgmActor);
KGMOBJECT_IMPLEMENT(kInput, kgmSnInputListener);
bool g_ms_camera = true;

const char* maps[] =
{
  "map test0", "map000.map",
  "map test1", "map001.map",
  "map test2", "map002.map",
  "map test3", "map003.map",
  "map test4", "map004.map",
};

class kGame;

class ASp_Logic: public kgmGameLogic
{
  kgmIGame*  game;
  kgmVisual* vtext;
  kgmVisual* vresult;

  u32        enemies;

public:
  ASp_Logic(kgmIGame* g)
  :kgmGameLogic()
  {
    game    = g;
    vtext   = null;
    vresult = null;

    enemies = 1;
  }

  bool add(kgmActor *act, bool input)
  {
    if(!act)
      return false;

    kgmGameLogic::add((kgmActor*)act);

    if(act->m_gameplayer){}
  }

  void prepare()
  {
    enemies = 1;
    kgmList<kgmUnit*> objs;
    kgmGameLogic::build();

    enemies = objs.size();
    objs.clear();
  }


  void action(kgmILogic::ACTION type, kgmObject* src, kgmString arg)
  {
  }

  void collide(kgmUnit *os, kgmUnit *od)
  {
    if(!vtext)
    {
      vtext = new kgmVisual();

      kgmText* text = new kgmText();
      text->m_rect  = uRect(10, 250, 500, 200);

      vtext->set(text);
      ((kgmGameBase*)game)->m_render->add(vtext);
//      text->release();
//      vtext->release();
    }
    else
    {
    }
  }
};

class kGame: public kgmGameBase{
  struct GameData
  {
    u16 sig;
    u8  maps;
    u8  cmap;
  };

  kgm_ptr<kGui> gui;
  GameData   data;

public:
  kGame(bool edit)
    :kgmGameBase(edit)
  {
    gui = kgm_ptr<kGui>(new kGui(this));

    kgmUnit::unitRegister("kInput",  kgmUnit::GoSensor, (kgmUnit::GenGo)&kInput::New);
    kgmUnit::unitRegister("ACamera",  kgmUnit::GoActor, (kgmUnit::GenGo)&ACamera::New);

    setMsAbsolute(true);

    if(m_physics)
      m_physics->m_gravity = 1.0f;

    if(m_logic)
      delete m_logic;

    m_logic = new ASp_Logic(this);

    readData();
    saveData();

    return;

    kgmSound* snd = m_resources->getSound("1.wav");

    if(snd && snd->getSound())
    {
      getAudio()->play(snd->getSound(), true);
    }

    kgmSound* snd1 = m_resources->getSound("2.wav");

    if(snd1 && snd1->getSound())
    {
      getAudio()->play(snd1->getSound(), true);
    }

    kgmSound* snd2 = m_resources->getSound("3.wav");

    if(snd2 && snd2->getSound())
    {
      getAudio()->play(snd2->getSound(), true);
    }
  }

  ~kGame()
  {
    saveData();

    gui = (kGui*)null;

#ifdef DEBUG
  kgm_log() << "kGame::~kGame.\n";
#endif
  }

public:
  void edit()
  {
#ifdef EDITOR
    ((kGui*)gui)->m_guiMain->hide();
    m_state = State_Edit;
#endif
  }

  void guiShow(bool s)
  {
    if(s)
      ((kGui*)gui)->m_guiMain->show();
    else
      ((kGui*)gui)->m_guiMain->hide();
  }

  void onIdle()
  {
    kgmGameBase::onIdle();

    if(m_state == State_Play)
    {

    }
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
  }

private:
  void readData()
  {
    kgmString path;

#ifdef ANDROID
#else
    kgmSystem::getHomeDirectory(path);

    path += "/.kSpacer";

    if(!kgmSystem::isDirectory(path))
      return;

    path += "/data";
#endif;

    kgmFile f;

    f.open(path, kgmFile::Read);

    if(f.m_file)
      f.read(&data, sizeof(data));

    f.close();
  }

  void saveData()
  {
    kgmString path;

#ifdef ANDROID
#else
    kgmSystem::getHomeDirectory(path);

    path += "/.kSpacer";

    if(!kgmSystem::isDirectory(path))
      kgmFile::make_directory(path);

    path += "/data";
#endif;

    kgmFile f;

    f.open(path, kgmFile::Write | kgmFile::Create);

    if(f.m_file > 0)
    {
      f.write(&data, sizeof(data));
      f.close();
    }
  }
};

#include "../../kgmBase/kgmPointer.h"

class kApp: public kgmGameApp
{
#ifdef ANDROID
public:
#endif

  //kgm_ptr<kGame> game;
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

    //game = kgm_ptr<kGame>(new kGame(edit));
    game = new kGame(edit);

    m_game = ((kGame*)game);

    game->setRect(0, 0, w, h);
  }

  void gameLoop()
  {
//    if(game.valid())
    if(game != null)
    {
      if(game->gState() == kgmIGame::State_Play)
        game->guiShow(false);

      game->loop();
    }
  }

  void gameFree()
  {
//    if(game.valid())
    if(game)
    {
      delete game;
      game = (kGame*)null;
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
    game = kgm_ptr<kGame>(new kGame(edit));

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
