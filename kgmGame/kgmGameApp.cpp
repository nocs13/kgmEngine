#include "kgmGameApp.h"
#include "../kgmBase/kgmEvent.h"
#include "../kgmSystem/kgmWindow.h"

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


kgmIGame*      g_game = null;
AAssetManager* g_assetManager = NULL;
static JavaVM* jvm;

//Game object create function. Should be run from outside.
extern kgmIGame* kgm_android_init();
extern void      kgm_android_quit();
extern void      kgm_android_idle();
extern void      kgm_android_input(kgmEvent::Event*);
extern void      kgm_android_resize(int, int);
extern void      kgm_android_close();
extern void      kgm_android_release();
extern bool      kgm_android_msabsolute();

kgm_android_exit()
{
  LOGI("kgmEngine quit\n");

  JNIEnv* env;
  jvm->AttachCurrentThread(&env, NULL);
  jclass cls = env->FindClass("com/kgmEngine/Game/Game");
  jmethodID mid = env->GetStaticMethodID(cls, "GameFinish", "()V");
  env->CallStaticVoidMethod(cls, mid);
}

AAssetManager* kgm_android_getAssetManager()
{
    return g_assetManager;
}

extern "C"
{
  JNIEXPORT void  JNICALL Java_com_kgmEngine_Game_GameLib_init(JNIEnv * env, jobject obj,  jint width, jint height,
                                                                             jobject am,   jobject surface);
  JNIEXPORT void  JNICALL Java_com_kgmEngine_Game_GameLib_quit(JNIEnv * env, jobject obj);
  JNIEXPORT void  JNICALL Java_com_kgmEngine_Game_GameLib_idle(JNIEnv * env, jobject obj);
  JNIEXPORT void  JNICALL Java_com_kgmEngine_Game_GameLib_onKeyboard(JNIEnv * env, jobject obj, jint a, jint key);
  JNIEXPORT void  JNICALL Java_com_kgmEngine_Game_GameLib_onTouch(JNIEnv * env, jobject obj,  jint act, jint x, jint y);
};

JNIEXPORT void JNICALL Java_com_kgmEngine_Game_GameLib_init(JNIEnv* env, jobject obj,  jint width, jint height, jobject am,
                                                          jobject surface)
{
    if(g_game)
    {
      kgm_android_resize(width, height);

      return;
    }

    LOGI("kgmTest init\n");
    AAssetManager* mgr = AAssetManager_fromJava(env, am);
    g_assetManager = mgr;
    env->NewGlobalRef(am);
    LOGI("kgmEngine init asset manager\n");
    env->GetJavaVM(&jvm);

    //m_app = new KApp();
    //m_app->m_nativeWindow = ANativeWindow_fromSurface(env, surface);
    //LOGI("kgmTest init native widnow\n");

    kgmString spath;
    g_game = kgm_android_init();
    
    if(g_game)
    {
      kgm_android_resize(width, height);

      LOGI("kgmEngine inited\n");
    }
}

JNIEXPORT void JNICALL Java_com_kgmEngine_Game_GameLib_quit(JNIEnv * env, jobject obj)
{
    LOGI("kgmEngine quit\n");

    if(g_game)
    {
      LOGI("kgmEngine release game\n");
      kgm_android_close();
      kgm_android_release();
    }

    g_game = null;
}

JNIEXPORT void JNICALL Java_com_kgmEngine_Game_GameLib_idle(JNIEnv * env, jobject obj)
{
    if(g_game)
    {
      kgm_android_idle();
    }
}

JNIEXPORT void JNICALL Java_com_kgmEngine_Game_GameLib_onKeyboard(JNIEnv * env, jobject obj, jint a, jint key)
{
#ifdef DEBUG
    LOGI("kgmTest onKeyboard %i %i\n", a, key);
#endif

    if(g_game)
    {
      kgmEvent::Event evt;

      switch(a)
      {
      case 0:
          evt.event = evtKeyDown;
          evt.key = keyTranslate(key);
          kgm_android_input(&evt);
          break;
      case 1:
          evt.event = evtKeyUp;
          evt.key = keyTranslate(key);
          kgm_android_input(&evt);
          break;
      default:
          break;
      }
    }
}

JNIEXPORT void JNICALL Java_com_kgmEngine_Game_GameLib_onTouch(JNIEnv * env, jobject obj, jint a, jint x, jint y)
{
    static int  prev_x = 0, prev_y = 0;
    static bool set = false;

#ifdef DEBUG
    LOGI("kgmTest onTouch %i %i %i\n", a, x, y);
#endif

    if(g_game)
    {
      kgmEvent::Event evt;

      switch(a)
      {
      case 0:
          evt.event = evtMsMove;

          if(kgm_android_msabsolute())
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
          kgm_android_input(&evt);
          break;
      case 1:
          evt.event = evtMsLeftDown;
          evt.msx = x;
          evt.msy = y;
          kgm_android_input(&evt);
          break;
      case 2:
          evt.event = evtMsLeftUp;
          evt.msx = x;
          evt.msy = y;
          kgm_android_input(&evt);
          break;
      default:
          break;
      }
    }
}

#endif
