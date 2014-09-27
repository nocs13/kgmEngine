#include "../kgmSystem/kgmSystem.h"
#include "../kgmGame/kgmGameApp.h"
#include "../kgmGame/kgmGameBase.h"
#include "../kgmGame/kgmGameScript.h"

#include "../kgmBase/kgmXml.h"
#include "../kgmBase/kgmLog.h"
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

  bool add(kgmGameObject *gobj, bool input)
  {
    if(gobj->isType(kgmActor::Class) && ((kgmActor*)gobj)->m_gameplayer)
      ((kgmActor*)gobj)->m_gameplayer = false;

    return kgmGameLogic::add(gobj);
  }

  void prepare()
  {
    enemies = 1;
    kgmList<kgmGameObject*> objs;
    kgmGameLogic::prepare();

    enemies = objs.size();
    objs.clear();
  }


  void action(kgmILogic::ACTION type, kgmObject* src, kgmString arg)
  {
  }

  void collide(kgmGameObject *os, kgmGameObject *od)
  {
    if(!vtext)
    {
      vtext = new kgmVisual();

      kgmText* text = new kgmText();
      text->m_rect  = uRect(10, 250, 500, 200);

      vtext->set(text);
      ((kgmGameBase*)game)->m_render->add(vtext);
      text->release();
      vtext->release();
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

  kGui*      gui;
  GameData   data;

public:
  kGame()
  {
    gui = new kGui(this);

    kgmGameObject::goRegister("kInput",  kgmGameObject::GoSensor, (kgmGameObject::GenGo)&kInput::New);
    kgmGameObject::goRegister("ACamera",  kgmGameObject::GoActor, (kgmGameObject::GenGo)&ACamera::New);

#ifdef EDITOR
    gui->m_guiMain->hide();
    m_gamemode = true;
#endif

    setMsAbsolute(true);

    if(m_physics)
      m_physics->m_gravity = 1.0f;

    if(m_logic)
      m_logic->release();

    m_logic = new ASp_Logic(this);

    readData();
    saveData();

    kgmSound* snd = m_resources->getSound("mail.wav");

    if(snd && snd->getSound())
    {
      snd->getSound()->play(false);
    }
  }

  ~kGame()
  {
    saveData();
  }

public:
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
      m_state = kgmIGame::State_Stop;
      gui->viewAgain();
    }
    else if(s == "gameover_success")
    {
      m_state = kgmIGame::State_Stop;
      gui->viewAgain();
    }
  }

  int gLoad(kgmString s)
  {
    int res = kgmGameBase::gLoad(s);

    if(res)
    {
      ACamera* camera = new ACamera(this);
      m_logic->add((kgmGameObject*)camera);
      camera->release();

      kInput* input = new kInput(this);
      m_logic->add((kgmGameObject*)input);
      input->release();
    }

    return res;
  }

  kgmGameObject* gObject(kgmString t)
  {
    return kgmGameBase::gObject(t);
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

    if(f.m_file)
      f.write(&data, sizeof(data));

    f.close();
  }
};

class kApp: public kgmGameApp{
#ifdef ANDROID
public:
#endif
  kGame* game;
public:
  kApp()
  {
  }

  ~kApp(){
  }

  void main()
  {
    u32 w, h;
    kgmSystem::getDesktopDimension(w, h);
    m_game = game = new kGame();

#ifndef ANDROID
    //kgmLuaOpen();
    //kgmMemory<s8> mem;
    //m_game->getResources()->getFile("main.lua", mem);
    //kgmString str(mem.data(), mem.length());
    //kgmLuaRun(str);
    //kgmLuaClose();
#endif

    game->setRect(0, 0, w, h);
    game->loop();
    game->release();
  }

#ifdef ANDROID
  kgmIGame* android_init_game()
  {
    u32 w, h;
    kgmSystem::getDesktopDimension(w, h);
    m_game = game = new kGame();
    game->setRect(0, 0, w, h);

    return game;
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
