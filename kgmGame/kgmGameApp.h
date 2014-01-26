#pragma once
#include "kgmIGame.h"
#include "../kgmBase/kgmEvent.h"
#include "../kgmSystem/kgmApp.h"

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

extern "C"
{
  JNIEXPORT void  JNICALL Java_com_kgmEngine_Game_GameLib_init(JNIEnv * env, jobject obj,  jint width, jint height,
                                                                             jobject am,   jobject surface);
  JNIEXPORT void  JNICALL Java_com_kgmEngine_Game_GameLib_quit(JNIEnv * env, jobject obj);
  JNIEXPORT void  JNICALL Java_com_kgmEngine_Game_GameLib_idle(JNIEnv * env, jobject obj);
  JNIEXPORT void  JNICALL Java_com_kgmEngine_Game_GameLib_onKeyboard(JNIEnv * env, jobject obj, jint a, jint key);
  JNIEXPORT void  JNICALL Java_com_kgmEngine_Game_GameLib_onTouch(JNIEnv * env, jobject obj,  jint act, jint x, jint y);
};

extern AAssetManager* kgm_android_getAssetManager();
extern const char*    kgm_android_classname();
extern bool           kgm_android_init_app();
extern void           kgm_android_init(JNIEnv * env, jobject am);
#endif


class kgmGameApp: public kgmApp
{
protected:
  kgmIGame*  m_game;

public:
  virtual ~kgmGameApp(){

  }

  kgmIGame* game(){
    return m_game;
  }

  static kgmGameApp* gameApplication(){
    return (kgmGameApp*)kgmApp::application();
  }

#ifdef ANDROID
  virtual kgmIGame* android_init_game() = 0
  ;
  void android_exit();
  void android_init(JNIEnv* env, jobject obj, jint width, jint height, jobject am, jobject surface);
  void android_quit(JNIEnv* env, jobject obj);
  void android_idle(JNIEnv* env, jobject obj);
  void android_onKeyboard(JNIEnv* env, jobject obj, jint a, jint key);
  void android_onTouch(JNIEnv* env, jobject obj,  jint act, jint x, jint y);
  void android_onCompass(JNIEnv* env, jobject obj, jfloat x, jfloat y, jfloat z);
  void android_onGyroscope(JNIEnv* env, jobject obj, jfloat x, jfloat y, jfloat z);
#endif
};
