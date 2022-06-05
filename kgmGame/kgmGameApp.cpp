#include "kgmGameApp.h"
#include "../kgmBase/kgmLog.h"
#include "../kgmBase/kgmFile.h"
#include "../kgmBase/kgmEvent.h"
#include "../kgmSystem/kgmWindow.h"

#include "kgmUnit.h"

s32 kgmGameApp::exec(s32 argc, s8 **argv)
{
  memset(&m_options, 0, sizeof(Options));

  for (int i = 1; i < argc; i++) {
    if (!strcmp("edit",  (char*) argv[i]) || !strcmp("--edit",  (char*) argv[i])) {
      if (!m_options.edit)
        m_options.edit = true;
    } else if (!strcmp("map",  (char*) argv[i]) || !strcmp("--map",  (char*) argv[i])) {
      if (argc > ( i + 1) && !m_options.map) {
        m_options.map = true;
        m_options.mapid = (char*) argv[i + 1];
      }
    }
  }

  gameInit();

  gameLoop();

  gameFree();

  return 0;
}

void kgmGameApp::abort()
{
  if (!game())
    return;

  game()->gQuit();
}

#ifdef ANDROID

static AAssetManager* g_assetManager = NULL;
static kgmIGame*      g_game = null;
static JavaVM*        jvm = null;

void kgmGameApp::android_init(JNIEnv* env, jobject obj, jint width, jint height, jobject am, jobject surface)
{
  LOGI("Game init\n");
  LOGI("args %p %p\n", am, env);
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

AAssetManager* kgm_android_getAssetManager()
{
  return g_assetManager;
}

void kgm_android_init(JNIEnv * env, jobject am)
{

}

#endif
