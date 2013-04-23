#include "../kgmSystem/kgmApp.h"
#include "../kgmSystem/kgmSystem.h"
#include "../kgmGame/kgmGameBase.h"
//#include "../kgmGame/kgmGameLogic.h"

#include "../kgmBase/kgmXml.h"
#include "../kgmBase/kgmLog.h"

#include "kGui.h"

class kGame: public kgmGameBase{
  kGui* gui;

public:
  kGame(){
      gui = new kGui(this);
  }

  ~kGame(){
  }

  void onKeyUp(int k){
    kgmGameBase::onKeyUp(k);

    if(k == KEY_ESCAPE)
    {
      if(gState() == State_Play)
      {
        gPause(true);
        gui->m_guiPause->show();
      }
      else if(gState() == State_Pause)
      {
        gPause(false);
        gui->m_guiPause->hide();
      }
    }    
  }

  void onMsLeftUp(int k, int x, int y)
  {
    kgmGameBase::onMsLeftUp(k, x, y);
  }

  void onMsLeftDown(int k, int x, int y)
  {
    kgmGameBase::onMsLeftDown(k, x, y);
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

 void print_xml(kgmXml::Node* n){
  if(!n)
   return;

  kgmString s, d;
  n->id(s);
  n->data(d);
  kgm_log() << "\n id=%s" << (char*)s;
  kgm_log() << "\n data=%s" << (char*)d;
  
  for(int i = 0; i < n->attributes(); i++){
    kgmString s, d;
    n->attribute(i, s, d);
    kgm_log() << "\n   attr: %s=%s" << (char*)s << " " << (char*)d;
  }
  
  for(int i = 0; i < n->nodes(); i++)
   print_xml(n->node(i));
 }
 
  /*void print_xml(char* path){
   kgmString s(path, strlen(path));
   kgmFile f;
   f.open(s, kgmFile::Read);
   kgmMemory<char> m;
   m.alloc(f.length());
   f.read(m.data(), f.length());
   f.close();
   kgmXml xml(m);
   print_xml(xml.m_node);
  }*/

 void print_xml(char* path){
  kgmString s(path, strlen(path));
  kgmFile f;
  f.open(s, kgmFile::Read);
  kgmMemory<char> m;
  m.alloc(f.length());
  f.read(m.data(), f.length());
  f.close();
  kgmXml xml;
  kgmXml::XmlState state;

  xml.open(m);

  while(state = xml.next())
  {
      if(state == kgmXml::XML_ERROR)
      {
          kgm_log() << "xml error \n";
          break;
      }
      else if(state == kgmXml::XML_FINISH)
      {
          kgm_log() << "xml finish \n";
          break;
      }
      else if(state == kgmXml::XML_TAG_OPEN)
      {
          kgm_log() << "xml tag open " << (char*)xml.m_tagName << "\n";
      }
      else if(state == kgmXml::XML_TAG_CLOSE)
      {
          kgm_log() << "xml tag close " << (char*)xml.m_tagName << "\n";
      }
      else if(state == kgmXml::XML_TAG_DATA)
      {
          kgm_log() << "xml data " << (char*)xml.m_tagData << "\n";
      }
  }
 }

 void main()
 {
  //print_xml("Data/level01.kgm");
  m_game = new kGame();
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
kGame*	       m_game = null;
AAssetManager* g_assetManager = NULL;
static JavaVM* jvm;

kgm_android_exit()
{
  LOGI("kgmTest quit\n");

  if(m_game)
    m_game->release();

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
    m_game = new kGame();
    m_game->setRect(0, 0, width, height);
    LOGI("kgmTest inited\n");
}

JNIEXPORT void JNICALL Java_com_example_Test_TestLib_quit(JNIEnv * env, jobject obj)
{
    LOGI("kgmTest quit\n");
    if(m_game)
      m_game->release();
    if(m_app)
      delete m_app;
    m_game = null;
}

JNIEXPORT void JNICALL Java_com_example_Test_TestLib_idle(JNIEnv * env, jobject obj)
{
    //LOGI("kgmTest idle\n");
    if(m_game)
    {
      m_game->onIdle();

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
          m_game->onEvent(&evt);
          break;
      case 1:
          evt.event = evtKeyUp;
          evt.key = keyTranslate(key);
          m_game->onEvent(&evt);
          break;
      default:
          break;
      }
    }
}

JNIEXPORT void JNICALL Java_com_example_Test_TestLib_onTouch(JNIEnv * env, jobject obj, jint a, jint x, jint y)
{
    //int x = 0, y = 0;
    LOGI("kgmTest onTouch %i %i %i\n", a, x, y);
    if(m_game)
    {
      kgmEvent::Event evt;

      switch(a)
      {
      case 0:
          evt.event = evtMsMove;
          evt.msx = x;
          evt.msy = y;
          m_game->onEvent(&evt);
          break;
      case 1:
          evt.event = evtMsLeftDown;
          evt.msx = x;
          evt.msy = y;
          m_game->onEvent(&evt);
          break;
      case 2:
          evt.event = evtMsLeftUp;
          evt.msx = x;
          evt.msy = y;
          m_game->onEvent(&evt);
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
