#include "../kgmSystem/kgmApp.h"
#include "../kgmSystem/kgmSystem.h"
#include "../kgmGame/kgmGameBase.h"
//#include "../kgmGame/kgmGameLogic.h"

#include "../kgmBase/kgmXml.h"
#include "../kgmBase/kgmLog.h"

#include "kGui.h"
#include "AI/AITaoRen.h"

class kGame: public kgmGameBase{
  kGui*   gui;
  //kgmSound* snd;

public:
  kGame(){
    gui = new kGui(this);
    m_msAbs = false;
    m_gamemode = true;

    m_logic->add("RenTao", new AITaoRen(this));
    /*snd = m_game->getResources()->getSound("1.wav");
      if(snd && snd->getSound())
      {
        snd->getSound()->play(true);
      }*/

    if(m_physics)
      m_physics->m_gravity = 1.0f;
  }

  ~kGame(){
  }

protected:
  virtual void initLogic()
  {
    m_logic = null; //new kLogic(this);
  }

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

  int gLoad(kgmString s)
  {
    int res = kgmGameBase::gLoad(s);

    return res;
  }
};


class kApp: public kgmApp{
#ifdef ANDROID
public:
#endif
  kGame*	m_game;
public:
  kApp(){
  }

  ~kApp(){
  }

  void main()
  {
    u32 w, h;
    kgmSystem::getDesktopDimension(w, h);
    m_game = new kGame();
    m_game->setRect(0, 0, w, h);
    m_game->loop();
    m_game->release();
  }
};

//main object
kApp theApp;
//////////////

//FOR ANDROID
#ifdef ANDROID

#include <jni.h>
#include <sys/types.h>
#include <android/log.h>
#include <android/input.h>
#include <android/sensor.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <android/native_window_jni.h>

#define  LOG_TAG    "kgmEngine"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

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
  m_app->m_nativeWindow = ANativeWindow_fromSurface(env, surface);
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

    /*struct AInputQueue queue;
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
      }*/
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

AAssetManager* kgm_getAssetManager()
{
  return g_assetManager;
}

#endif
