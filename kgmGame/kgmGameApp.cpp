#include "kgmGameApp.h"
#include "../kgmBase/kgmEvent.h"
#include "../kgmSystem/kgmWindow.h"

//FOR ANDROID
#ifdef ANDROID

static AAssetManager* g_assetManager = NULL;
static kgmIGame*      g_game = null;
static JavaVM*        jvm = null;

void kgmGameApp::android_init(JNIEnv* env, jobject obj, jint width, jint height, jobject am, jobject surface)
{
  LOGI("Game init\n");
  LOGI("args %i %i\n", am, env);
  AAssetManager* mgr = AAssetManager_fromJava(env, am);
  g_assetManager = mgr;
  env->NewGlobalRef(am);
  LOGI("Game init asset manager\n");
  env->GetJavaVM(&jvm);

  kgmGameApp::gameApplication()->setMainWindow(ANativeWindow_fromSurface(env, surface));
  LOGI("Game init native widnow\n");

  kgmString spath;
  g_game = kgmGameApp::gameApplication()->android_init_game();

  if(g_game)
  {
    g_game->getWindow()->setRect(0, 0, width, height);

    LOGI("Game inited\n");
  }
}

void kgmGameApp::android_quit(JNIEnv* env, jobject obj)
{
  LOGI("Game quit\n");

  if(g_game)
  {
    g_game->getWindow()->onClose();
  }

  g_game = null;
}

void kgmGameApp::android_exit()
{
  LOGI("kgmEngine send quit request\n");

  JNIEnv* env;
  jvm->AttachCurrentThread(&env, NULL);
  jclass cls = env->FindClass(kgm_android_classname());
  jmethodID mid = env->GetStaticMethodID(cls, "kgmAppFinish", "()V");
  env->CallStaticVoidMethod(cls, mid);
}

void kgmGameApp::android_idle(JNIEnv* env, jobject obj)
{
  if(g_game)
  {
    g_game->getWindow()->onIdle();
  }
}

void kgmGameApp::android_onKeyboard(JNIEnv* env, jobject obj, jint a, jint key)
{
#ifdef DEBUG
  LOGI("Game onKeyboard %i %i\n", a, key);
#endif

  if(g_game)
  {
    kgmEvent::Event evt;

    switch(a)
    {
    case 0:
      evt.event = evtKeyDown;
      evt.key = keyTranslate(key);
      g_game->getWindow()->onEvent(&evt);
      break;
    case 1:
      evt.event = evtKeyUp;
      evt.key = keyTranslate(key);
      g_game->getWindow()->onEvent(&evt);
      break;
    default:
      break;
    }
  }
}

void kgmGameApp::android_onTouch(JNIEnv* env, jobject obj,  jint a, jint x, jint y)
{
  static int  prev_x = 0, prev_y = 0;
  static bool set = false;

#ifdef DEBUG
  LOGI("Game onTouch %i %i %i\n", a, x, y);
#endif

  if(g_game)
  {
    kgmEvent::Event evt;

    switch(a)
    {
    case 0:
      evt.event = evtMsMove;

      if(kgmGameApp::gameApplication()->game()->getWindow()->getMsAbsolute())
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
      g_game->getWindow()->onEvent(&evt);
      break;
    case 1:
      evt.event = evtMsLeftDown;
      evt.msx = x;
      evt.msy = y;
      g_game->getWindow()->onEvent(&evt);
      break;
    case 2:
      evt.event = evtMsLeftUp;
      evt.msx = x;
      evt.msy = y;
      g_game->getWindow()->onEvent(&evt);
      break;
    default:
      break;
    }
  }
}

void kgmGameApp::android_onCompass(JNIEnv* env, jobject obj, jfloat x, jfloat y, jfloat z)
{

}

void kgmGameApp::android_onGyroscope(JNIEnv* env, jobject obj, jfloat x, jfloat y, jfloat z)
{

}

void kgm_android_init(JNIEnv * env, jobject am)
{

}

AAssetManager* kgm_android_getAssetManager()
{
  return g_assetManager;
}

JNIEXPORT void JNICALL Java_com_kgmEngine_Game_GameLib_init(JNIEnv* env, jobject obj,  jint width, jint height, jobject am,
                                                          jobject surface)
{
  if(kgmGameApp::gameApplication() && kgmGameApp::gameApplication()->game())
  {
    kgmGameApp::gameApplication()->game()->getWindow()->setRect(0, 0, width, height);
  }
  else
  {
    kgm_android_init_app();
    kgmGameApp::gameApplication()->android_init(env, obj, width, height, am, surface);
  }
}

JNIEXPORT void JNICALL Java_com_kgmEngine_Game_GameLib_quit(JNIEnv * env, jobject obj)
{
  kgmGameApp::gameApplication()->android_quit(env, obj);
}

JNIEXPORT void JNICALL Java_com_kgmEngine_Game_GameLib_idle(JNIEnv * env, jobject obj)
{
  kgmGameApp::gameApplication()->android_idle(env, obj);
}

JNIEXPORT void JNICALL Java_com_kgmEngine_Game_GameLib_onKeyboard(JNIEnv * env, jobject obj, jint a, jint key)
{
  kgmGameApp::gameApplication()->android_onKeyboard(env, obj, a, key);
}

JNIEXPORT void JNICALL Java_com_kgmEngine_Game_GameLib_onTouch(JNIEnv * env, jobject obj, jint a, jint x, jint y)
{
  kgmGameApp::gameApplication()->android_onTouch(env, obj, a, x, y);
}

JNIEXPORT void JNICALL Java_com_kgmEngine_Game_GameLib_onCompass(JNIEnv * env, jobject obj, jfloat x, jfloat y, jfloat z)
{
  kgmGameApp::gameApplication()->android_onGyroscope(env, obj, x, y, z);
}

JNIEXPORT void JNICALL Java_com_kgmEngine_Game_GameLib_onGyroscope(JNIEnv * env, jobject obj, jfloat x, jfloat y, jfloat z)
{
  kgmGameApp::gameApplication()->android_onGyroscope(env, obj, x, y, z);
}

#endif
