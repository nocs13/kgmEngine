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

extern AAssetManager* kgm_android_getAssetManager();
extern const char*    kgm_android_classname();
extern const char*    kgm_android_datapath();
extern bool           kgm_android_init_app();
extern void           kgm_android_init(JNIEnv * env, jobject am);

#define KGM_ANDROID_INIT(proj) \
extern "C" \
{ \
  JNIEXPORT void  JNICALL Java_com_kgmEngine_##proj##_##proj##Lib_init(JNIEnv * env, jobject obj,  jint width, jint height, jobject am,   jobject surface); \
  JNIEXPORT void  JNICALL Java_com_kgmEngine_##proj##_##proj##Lib_quit(JNIEnv * env, jobject obj); \
  JNIEXPORT void  JNICALL Java_com_kgmEngine_##proj##_##proj##Lib_idle(JNIEnv * env, jobject obj); \
  JNIEXPORT void  JNICALL Java_com_kgmEngine_##proj##_##proj##Lib_onKeyboard(JNIEnv * env, jobject obj, jint a, jint key); \
  JNIEXPORT void  JNICALL Java_com_kgmEngine_##proj##_##proj##Lib_onTouch(JNIEnv * env, jobject obj,  jint act, jint x, jint y); \
}; \
   \
JNIEXPORT void JNICALL Java_com_kgmEngine_##proj##_##proj##Lib_init(JNIEnv* env, jobject obj,  jint width, jint height, jobject am, jobject surface) \
{ \
  if(kgmGameApp::gameApplication() && kgmGameApp::gameApplication()->game())  \
  { \
    kgmGameApp::gameApplication()->game()->getWindow()->setRect(0, 0, width, height); \
  } \
  else  \
  { \
    kgm_android_init_app(); \
    kgmGameApp::gameApplication()->android_init(env, obj, width, height, am, surface);  \
  } \
} \
  \
JNIEXPORT void JNICALL Java_com_kgmEngine_##proj##_##proj##Lib_quit(JNIEnv * env, jobject obj)  \
{ \
  kgmGameApp::gameApplication()->android_quit(env, obj);  \
} \
  \
JNIEXPORT void JNICALL Java_com_kgmEngine_##proj##_##proj##Lib_idle(JNIEnv * env, jobject obj)  \
{ \
  kgmGameApp::gameApplication()->android_idle(env, obj);  \
} \
  \
JNIEXPORT void JNICALL Java_com_kgmEngine_##proj##_##proj##Lib_onKeyboard(JNIEnv * env, jobject obj, jint a, jint key)  \
{ \
  kgmGameApp::gameApplication()->android_onKeyboard(env, obj, a, key);  \
} \
  \
JNIEXPORT void JNICALL Java_com_kgmEngine_##proj##_##proj##Lib_onTouch(JNIEnv * env, jobject obj, jint a, jint x, jint y) \
{ \
  kgmGameApp::gameApplication()->android_onTouch(env, obj, a, x, y);  \
} \
  \
JNIEXPORT void JNICALL Java_com_kgmEngine_##proj##_##proj##Lib_onCompass(JNIEnv * env, jobject obj, jfloat x, jfloat y, jfloat z) \
{ \
  kgmGameApp::gameApplication()->android_onGyroscope(env, obj, x, y, z);  \
} \
  \
JNIEXPORT void JNICALL Java_com_kgmEngine_##proj##_##proj##Lib_onGyroscope(JNIEnv * env, jobject obj, jfloat x, jfloat y, jfloat z) \
{ \
  kgmGameApp::gameApplication()->android_onGyroscope(env, obj, x, y, z);  \
}
#endif

#ifdef DARWIN
void kgm_darwin_init(int width, int height);
void kgm_darwin_idle();
void kgm_darwin_quit();
#endif


class kgmGameApp: public kgmApp
{
public:
  typedef struct {
    bool  edit;
    bool  map;
    char* mapid;
  } Options;

protected:
  kgmIGame* m_game;
  Options   m_options;

  kgmString m_name;

public:
  virtual ~kgmGameApp()
  {
  }

  virtual void gameInit() = 0;
  virtual void gameLoop() = 0;
  virtual void gameFree() = 0;

  kgmIGame* game()
  {
    return m_game;
  }

  Options* options() const
  {
    return (Options*) &m_options;
  }

  s32 exec(s32 argc, s8 **argv);

  void abort();

  static kgmGameApp* gameApplication()
  {
    return (kgmGameApp*)kgmApp::application();
  }

  static kgmGameApp* gameApp()
  {
    return (kgmGameApp*) kgmApp::application();
  }

  bool exportProject(kgmString path);

#ifdef ANDROID
  virtual kgmIGame* android_init_game() = 0;
  void android_exit();
  void android_init(JNIEnv* env, jobject obj, jint width, jint height, jobject am, jobject surface);
  void android_quit(JNIEnv* env, jobject obj);
  void android_idle(JNIEnv* env, jobject obj);
  void android_onKeyboard(JNIEnv* env, jobject obj, jint a, jint key);
  void android_onTouch(JNIEnv* env, jobject obj,  jint act, jint x, jint y);
  void android_onCompass(JNIEnv* env, jobject obj, jfloat x, jfloat y, jfloat z);
  void android_onGyroscope(JNIEnv* env, jobject obj, jfloat x, jfloat y, jfloat z);
#endif
  
#ifdef DARWIN
#endif
};
