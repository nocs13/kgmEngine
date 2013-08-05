#include "../kgmSystem/kgmSystem.h"
#include "../kgmGame/kgmGameApp.h"
#include "../kgmGame/kgmGameBase.h"

#include "../kgmBase/kgmXml.h"
#include "../kgmBase/kgmLog.h"
#include "../kgmGraphics/kgmGuiButton.h"

#include "kGlobals.h"
#include "kGui.h"
#include "Actors/ATaoRen.h"
#include "Actors/AKomble.h"
#include "Actors/ASp_Gui.h"
#include "Actors/ASpacerObjects.h"
#include "Actors/ASpacer.h"
#include "Actors/ASp_Gun.h"
#include "Actors/ASp_Spacer.h"
#include "Actors/ASp_Spaceship.h"

KGMOBJECT_IMPLEMENT(AKomble, kgmActor);
KGMOBJECT_IMPLEMENT(ASpacer, kgmActor);
KGMOBJECT_IMPLEMENT(ASp_Gui, kgmObject);
KGMOBJECT_IMPLEMENT(ASp_Gun, kgmActor);
KGMOBJECT_IMPLEMENT(ASp_GunA, ASp_Gun);
KGMOBJECT_IMPLEMENT(ASp_GunFA, ASp_Gun);
KGMOBJECT_IMPLEMENT(ASp_Spacer, kgmActor);
KGMOBJECT_IMPLEMENT(ASp_SpacerA, kgmActor);
KGMOBJECT_IMPLEMENT(ASp_Result, kgmGameObject);
KGMOBJECT_IMPLEMENT(ASp_Skybox, kgmGameObject);
KGMOBJECT_IMPLEMENT(ASp_MotorA, kgmGameObject);
KGMOBJECT_IMPLEMENT(ASp_Laser, kgmGameObject);
KGMOBJECT_IMPLEMENT(ASp_LaserA, ASp_Laser);
KGMOBJECT_IMPLEMENT(ASp_LaserB, ASp_Laser);
KGMOBJECT_IMPLEMENT(ASp_Flame, kgmGameObject);
KGMOBJECT_IMPLEMENT(ASp_FlameA, ASp_Flame);
KGMOBJECT_IMPLEMENT(ASp_Smoke, kgmGameObject);
KGMOBJECT_IMPLEMENT(ASp_SmokeA, ASp_Smoke);
KGMOBJECT_IMPLEMENT(ASp_Explode, kgmGameObject);
KGMOBJECT_IMPLEMENT(ASp_ExplodeA, ASp_Explode);
KGMOBJECT_IMPLEMENT(ASp_ExplodeB, ASp_Explode);
KGMOBJECT_IMPLEMENT(ASp_ExplodeC, ASp_Explode);
KGMOBJECT_IMPLEMENT(ASp_AsteroidSpawner, kgmGameObject);
KGMOBJECT_IMPLEMENT(ASp_SpacerSpawner, kgmGameObject);
KGMOBJECT_IMPLEMENT(ASp_Spaceship, kgmActor);
KGMOBJECT_IMPLEMENT(ASp_SpaceshipA, ASp_Spaceship);

bool g_ms_camera = false;


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

  void prepare()
  {
    enemies = 1;
    kgmList<kgmGameObject*> objs;
    kgmGameLogic::prepare();
    this->getObjectsByType(ASp_Spaceship::Class, objs);

    enemies = objs.size();
    objs.clear();
  }


  void action(kgmILogic::ACTION type, kgmObject* src, kgmString arg)
  {
    if(type == kgmILogic::ACTION_GAMEOBJECT)
    {
      if(arg == "die")
      {
        if(src->isType(ASp_Spaceship::Class))
        {
          enemies--;

          if(enemies == 0)
          {
              ASp_Result* res = new ASp_Result(game, 3000, 1, "Success");
              game->gAppend(res);
          }
        }
        else if(src->isType(ASpacer::Class))
        {
            ASp_Result* res = new ASp_Result(game, 3000, 0, "Failed");
            game->gAppend(res);
        }
      }
      else if(arg == "result")
      {
        if(src->isClass(ASp_Result::Class))
        {
          if(((ASp_Result*)src)->getResult())
            game->gCommand("gameover_success");
          else
            game->gCommand("gameover_fail");
        }
      }
    }
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

    if(os->isType(ASp_Laser::Class) && !od->isType(ASp_Laser::Class))
    {
      if(os->getGroup() != od->getGroup())
      {
        kgmString t = kgmString("Logic: ") + os->getId() + kgmString(" ") + od->getId();
        //vtext->m_text->m_text = t;

        ASp_Laser* laser = (ASp_Laser*)os;

        if(od->isType(kgmActor::Class))
        {
          kgmActor* actor = (kgmActor*)od;

          actor->m_health -= laser->power;
        }

        os->remove();
      }
    }
  }
};

class kGame: public kgmGameBase{
  kGui*      gui;
  //kgmSound* snd;

public:
  kGame(){
    gui = new kGui(this);
    m_msAbs = false;
    m_gamemode = true;

    //m_logic->add("RenTao", new AITaoRen(this));
    /*snd = m_game->getResources()->getSound("1.wav");
      if(snd && snd->getSound())
      {
        snd->getSound()->play(true);
      }*/

    if(m_physics)
      m_physics->m_gravity = 1.0f;

    if(m_logic)
    {
      m_logic->release();
      m_logic = new ASp_Logic(this);
    }
  }

  ~kGame(){
  }

//protected:
  void  initLogic();

public:
  void onIdle()
  {
    kgmGameBase::onIdle();

    if(m_state == State_Play)
    {

    }
  }

  void onKeyUp(int k){
    kgmGameBase::onKeyUp(k);

    if(k == KEY_ESCAPE)
    {
      if(gState() == State_Play)
      {
        gPause(true);
        gui->m_guiPause->show();
        m_msAbs = false;
      }
      else if(gState() == State_Pause)
      {
        gPause(false);
        gui->m_guiPause->hide();
        m_msAbs = true;
      }
    }
  }

  void onKeyDown(int k){
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

    /*if(snd)
    {
      vec3 pos(x, y, 0), vel(0,0,0);
      snd->getSound()->emit(pos, vel);
    }*/
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

    return res;
  }

  kgmGameObject* gObject(kgmString t)
  {
    if(t == "RenTao")
    {
      return new ATaoRen(this);
    }
    else if(t == "HyugaNeji")
    {

    }
    else if(t == "Komble")
    {
      return new AKomble(this);
    }
    else if(t == "KSpacer")
    {
      return new ASpacer(this);
    }
    else if(t == "MotorA")
    {
      return new ASp_MotorA(this);
    }
    else if(t == "KAsteroidSpawner")
    {
      return new ASp_AsteroidSpawner(this);
    }
    else if(t == "GunFA")
    {
      return new ASp_GunFA(this);
    }
    else if(t == "KSpacerA")
    {
      return new ASp_SpacerA(this);
    }
    else if(t == "KSpaceshipA")
    {
      return new ASp_SpaceshipA(this);
    }
    else if(t == "KSpacerSpawner")
    {
      return new ASp_SpacerSpawner(this);
    }
    else if(t == "KFlameA")
    {
      return new ASp_FlameA(this);
    }

    return kgmGameBase::gObject(t);
  }
};

void  kGame::initLogic()
{
  m_logic = new ASp_Logic(this);
}


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

/*
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
  if(!m_app)
    kgm_android_init_app();
}

JNIEXPORT void JNICALL Java_com_example_Test_TestLib_quit(JNIEnv * env, jobject obj)
{
  if(m_app)
    m_app->android_quit();
}

JNIEXPORT void JNICALL Java_com_example_Test_TestLib_idle(JNIEnv * env, jobject obj)
{
  if(m_app)
    m_app->android_idle();
}

JNIEXPORT void JNICALL Java_com_example_Test_TestLib_onKeyboard(JNIEnv * env, jobject obj, jint a, jint key)
{
}

JNIEXPORT void JNICALL Java_com_example_Test_TestLib_onTouch(JNIEnv * env, jobject obj, jint a, jint x, jint y)
{
}

class KApp: public kgmApp{
public:
  void main(){

  }
};

KApp*          m_app  = null;
kgmIGame*      m_game = null;
AAssetManager* g_assetManager = NULL;
static JavaVM* jvm;

kgmIGame* kgm_android_init_game()
{
  return new kGame();
}

kgm_android_exit()
{
  LOGI("kgmTest quit\n");

  if(m_game)
    delete m_game;

  if(m_app)
    delete m_app;

  JNIEnv* env;
  jvm->AttachCurrentThread(&env, NULL);
  jclass cls = env->FindClass("com/example/Test/Test");
  jmethodID mid = env->GetStaticMethodID(cls, "TestFinish", "()V");
  env->CallStaticVoidMethod(cls, mid);
}

extern "C"
{
JNIEXPORT void  JNICALL Java_com_example_Test_TestLib_init(JNIEnv * env, jobject obj,  jint width, jint height,
                                                           jobject am, jobject surface);
JNIEXPORT void  JNICALL Java_com_example_Test_TestLib_quit(JNIEnv * env, jobject obj);
JNIEXPORT void  JNICALL Java_com_example_Test_TestLib_idle(JNIEnv * env, jobject obj);
JNIEXPORT void  JNICALL Java_com_example_Test_TestLib_onKeyboard(JNIEnv * env, jobject obj, jint a, jint key);
JNIEXPORT void  JNICALL Java_com_example_Test_TestLib_onTouch(JNIEnv * env, jobject obj,  jint act, jint x, jint y);
JNIEXPORT jstring  JNICALL Java_com_example_Test_TestLib_stringFromJNI(JNIEnv * env, jobject obj);
};

JNIEXPORT void JNICALL Java_com_example_Test_TestLib_init(JNIEnv* env, jobject obj,  jint width, jint height, jobject am,
jobject surface)
{
  if(m_game)
  {
    return;
  }

  LOGI("kgmTest init\n");
  AAssetManager* mgr = AAssetManager_fromJava(env, am);
  assert(NULL != mgr);
  g_assetManager = mgr;
  env->NewGlobalRef(am);
  LOGI("kgmTest init asset manager\n");
  env->GetJavaVM(&jvm);

  m_app = new KApp();
  m_app->setMainWindow(ANativeWindow_fromSurface(env, surface));
  LOGI("kgmTest init native widnow\n");

  kgmString spath;
  m_game = kgm_android_init_game();
  m_game->getWindow()->setRect(0, 0, width, height);
  LOGI("kgmTest inited\n");
}

JNIEXPORT void JNICALL Java_com_example_Test_TestLib_quit(JNIEnv * env, jobject obj)
{
  LOGI("kgmTest quit\n");

  if(m_game)
  {
    LOGI("kgmTest release gamet\n");
    m_game->getWindow()->onClose();
    //m_game->release();
  }

//  if(m_app)
//    delete m_app;

  m_game = null;
}

JNIEXPORT void JNICALL Java_com_example_Test_TestLib_idle(JNIEnv * env, jobject obj)
{
  //LOGI("kgmTest idle\n");
  if(m_game)
  {
    m_game->getWindow()->onIdle();

    /*
      struct AInputQueue queue;
      struct AInputEvent events[1];

      ASensorManager* sm = ASensorManager_getInstance();
      while(AInputQueue_hasEvents(&queue))
      {
        AInputEvent event;

        LOGI("kgmTest idle hasEvents\n");

        AInputQueue_getEvent(&queue, (AInputEvent**)&events);
        int32_t input_type = AInputEvent_getType(&event);

        switch(input_type)
        {
        case AINPUT_EVENT_TYPE_KEY:
          LOGI("kgmTest idle AINPUT_EVENT_TYPE_KEY\n");
          break;
        case AINPUT_EVENT_TYPE_MOTION:
          LOGI("kgmTest idle AINPUT_EVENT_TYPE_MOTION\n");
          break;
        default:
          break;
        }
      }
     //*/ /*
  }
}

JNIEXPORT void JNICALL Java_com_example_Test_TestLib_onKeyboard(JNIEnv * env, jobject obj, jint a, jint key)
{
  //int x = 0, y = 0;
  LOGI("kgmTest onKeyboard %i %i\n", a, key);
  if(m_game)
  {
    kgmEvent::Event evt;

    switch(a)
    {
    case 0:
      evt.event = evtKeyDown;
      evt.key = keyTranslate(key);
      m_game->getWindow()->onEvent(&evt);
      break;
    case 1:
      evt.event = evtKeyUp;
      evt.key = keyTranslate(key);
      m_game->getWindow()->onEvent(&evt);
      break;
    default:
      break;
    }
  }
}

JNIEXPORT void JNICALL Java_com_example_Test_TestLib_onTouch(JNIEnv * env, jobject obj, jint a, jint x, jint y)
{
  static int  prev_x = 0, prev_y = 0;
  static bool set = false;
  LOGI("kgmTest onTouch %i %i %i\n", a, x, y);
  if(m_game)
  {
    kgmEvent::Event evt;

    switch(a)
    {
    case 0:
      evt.event = evtMsMove;

      if(m_game->getWindow()->m_msAbs)
      {
        evt.msx = x - prev_x;
        evt.msy = y - prev_y;
        prev_x = x;
        prev_y = y;
      }
      else
      {
        evt.msx = x;
        evt.msy = y;
      }
      m_game->getWindow()->onEvent(&evt);
      break;
    case 1:
      evt.event = evtMsLeftDown;
      evt.msx = x;
      evt.msy = y;
      m_game->getWindow()->onEvent(&evt);
      break;
    case 2:
      evt.event = evtMsLeftUp;
      evt.msx = x;
      evt.msy = y;
      m_game->getWindow()->onEvent(&evt);
      break;
    default:
      break;
    }
  }
}

JNIEXPORT jstring  JNICALL Java_com_example_Test_TestLib_stringFromJNI(JNIEnv * env, jobject obj){
  return (env)->NewStringUTF("Hello from TEST JNI !");
}
*/
#endif
