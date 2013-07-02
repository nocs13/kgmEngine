#include "../kgmSystem/kgmApp.h"
#include "../kgmSystem/kgmSystem.h"
#include "../kgmGame/kgmGameBase.h"

#include "../kgmBase/kgmXml.h"
#include "../kgmBase/kgmLog.h"

#include "kGui.h"
#include "Actors/ATaoRen.h"
#include "Actors/AKomble.h"
#include "Actors/ASpacerObjects.h"
#include "Actors/ASpacer.h"
#include "Actors/ASp_Spacer.h"

KGMOBJECT_IMPLEMENT(AKomble, kgmActor);
KGMOBJECT_IMPLEMENT(ASpacer, kgmActor);
KGMOBJECT_IMPLEMENT(ASp_Spacer, kgmActor);
KGMOBJECT_IMPLEMENT(ASp_SpacerA, kgmActor);
KGMOBJECT_IMPLEMENT(ASp_Skybox, kgmGameObject);
KGMOBJECT_IMPLEMENT(ASp_MotorA, kgmGameObject);
KGMOBJECT_IMPLEMENT(ASp_LaserA, kgmGameObject);
KGMOBJECT_IMPLEMENT(ASp_Asteroid, kgmGameObject);
KGMOBJECT_IMPLEMENT(ASp_AsteroidSpawner, kgmGameObject);
KGMOBJECT_IMPLEMENT(ASp_SpacerSpawner, kgmGameObject);

class kLogic: public kgmGameLogic
{
  kgmIGame*  game;
  kgmVisual* vtext;
public:
  kLogic(kgmIGame* g)
  :kgmGameLogic()
  {
    game  = g;
    vtext = null;
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
    }
    else
    {
    }

    if(os->isClass(ASp_LaserA::Class))
    {
      if(os->getGroup() != od->getGroup())
      {
        kgmString t = kgmString("Logic: ") + os->getId() + kgmString(" ") + od->getId();
        vtext->m_text->m_text = t;

        os->remove();

        if(os->getBody())
          os->getBody()->remove();

        if(os->getVisual())
          os->getVisual()->remove();
      }
    }
  }
};

class kGame: public kgmGameBase{
  kGui*      gui;
  kLogic*    logic;
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
      m_logic = new kLogic(this);
    }
  }

  ~kGame(){
  }

private:
  void  initLogic()
  {
    m_logic = new kLogic(this);
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
    else if(t == "KSpacerA")
    {
      return new ASpacer(this);
    }
    else if(t == "KSpacerB")
    {
      return new ASpacer(this);
    }
    else if(t == "KSpacerC")
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
    else if(t == "ASp_SpacerA")
    {
      return new ASp_SpacerA(this);
    }
    else if(t == "KSpacerSpawner")
    {
      return new ASp_SpacerSpawner(this);
    }

    return kgmGameBase::gObject(t);
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

/////////////////////
/// \brief The KApp class
#include <stdio.h>
#include <stdlib.h>

#include <EGL/egl.h>
#include <GLES/gl.h>

struct ANativeWindow{
  int width, height, format;

  ANativeWindow()
  {
    width  = 400;
    height = 177;
    format = 1;
  }
};
NativeWindowType displayWindow;

const EGLint config16bpp[] =
{
EGL_RED_SIZE, 5,
EGL_GREEN_SIZE, 6,
EGL_BLUE_SIZE, 5,
EGL_NONE
};

GLfloat colors[3][4] =
{
    {1.0f, 0.0f, 0.0f, 1.0f},
    {0.0f, 1.0f, 0.0f, 1.0f},
    {0.0f, 0.0f, 1.0f, 1.0f}
};

GLfloat vertices[3][3] =
{
    {0.0f, 0.7f, 0.0f},
    {-0.7f, -0.7f, 0.0f},
    {0.7f, -0.7f, 0.0f}
};


void draw_tri()
{
    glViewport(0,0,displayWindow->width,displayWindow->height);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);

    glColorPointer(4, GL_FLOAT, 0, colors);
    glVertexPointer(3, GL_FLOAT, 0, vertices);

    // Draw the triangle (3 vertices)
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
}


int main(int argc, char** argv)
{
    EGLint majorVersion, minorVersion;
    EGLContext eglContext;
    EGLSurface eglSurface;
    EGLConfig eglConfig;
    EGLDisplay eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    int numConfigs;

    // Window surface that covers the entire screen, from libui.
    //displayWindow = android_createDisplaySurface();
    displayWindow = new ANativeWindow();

    eglInitialize(eglDisplay, &majorVersion, &minorVersion);
    printf("GL version: %d.%d\n",majorVersion,minorVersion);


    printf("Window specs: %d*%d format=%d\n",
     displayWindow->width,
     displayWindow->height,
     displayWindow->format);

    if (!eglChooseConfig(eglDisplay, config16bpp, &eglConfig, 1, &numConfigs))
    {
     printf("eglChooseConfig failed\n");
     if (eglContext==0) printf("Error code: %x\n", eglGetError());
    }

    eglContext = eglCreateContext(eglDisplay,
     eglConfig,
     EGL_NO_CONTEXT,
     NULL);
    printf("GL context: %x\n", eglContext);
    if (eglContext==0) printf("Error code: %x\n", eglGetError());

    eglSurface = eglCreateWindowSurface(eglDisplay,
     eglConfig,
     displayWindow,
     NULL);
    printf("GL surface: %x\n", eglSurface);
    if (eglSurface==0) printf("Error code: %x\n", eglGetError());

    eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext);


    while (1)
    {
     draw_tri();
     eglSwapBuffers(eglDisplay, eglSurface);
    }


    return 0;
}

extern "C" jint JNI_OnLoad(JavaVM* vm,void* reserved) {
    return JNI_VERSION_1_6;
}
/////////////////////////

/*class KApp: public kgmApp{
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

AAssetManager* kgm_getAssetManager()
{
  return null;
  //return g_assetManager;
}

#endif
