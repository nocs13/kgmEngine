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
 
  void print_xml(char* path){
   kgmString s(path, strlen(path));
   kgmFile f;
   f.open(s, kgmFile::Read);
   kgmMemory<char> m;
   m.alloc(f.length());
   f.read(m.data(), f.length());
   f.close();
   kgmXml xml(m);
   print_xml(xml.m_node);
  }

 void main(){
  //printf_xml("map001.kgm");
  m_game = new kGame();
  m_game->loop();

  delete m_game;
  //kgmOGLWindow* w = new kgmOGLWindow(0, "", 0, 0, 100, 100, 16, false); 
  //w->loop();
  //delete w;
   kgmLog::log("hello\n");
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
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

#define  LOG_TAG    "kgmEngine"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

kGame*	m_game = null;
AAssetManager* g_assetManager = NULL;

extern "C"
{
  JNIEXPORT void  JNICALL Java_com_example_Test_TestLib_init(JNIEnv * env, jobject obj,  jint width, jint height, jobject am);
  JNIEXPORT void  JNICALL Java_com_example_Test_TestLib_quit(JNIEnv * env, jobject obj);
  JNIEXPORT void  JNICALL Java_com_example_Test_TestLib_idle(JNIEnv * env, jobject obj);
  JNIEXPORT void  JNICALL Java_com_example_Test_TestLib_msMove(JNIEnv * env, jobject obj,  jint x, jint y);
  JNIEXPORT jstring  JNICALL Java_com_example_Test_TestLib_stringFromJNI(JNIEnv * env, jobject obj);
};

JNIEXPORT void JNICALL Java_com_example_Test_TestLib_init(JNIEnv * env, jobject obj,  jint width, jint height, jobject am)
{
    LOGI("kgmTest init\n");
    AAssetManager* mgr = AAssetManager_fromJava(env, am);
    assert(NULL != mgr);
    g_assetManager = mgr;
    env->NewGlobalRef(am);
    LOGI("kgmTest init asset manager\n");


    kgmString spath;
    m_game = new kGame();
    m_game->onResize(width, height);
    LOGI("kgmTest inited\n");
}

JNIEXPORT void JNICALL Java_com_example_Test_TestLib_quit(JNIEnv * env, jobject obj)
{
    LOGI("kgmTest quit\n");
    if(m_game)
      m_game->release();
    m_game = null;
}

JNIEXPORT void JNICALL Java_com_example_Test_TestLib_idle(JNIEnv * env, jobject obj)
{
    LOGI("kgmTest idle\n");
    if(m_game)
      m_game->onIdle();
}

JNIEXPORT void JNICALL Java_com_example_Test_TestLib_msMove(JNIEnv * env, jobject obj,  jint x, jint y)
{
    if(m_game)
      m_game->onMsMove(0, x, y);
}

JNIEXPORT jstring  JNICALL Java_com_example_Test_TestLib_stringFromJNI(JNIEnv * env, jobject obj){
    return (env)->NewStringUTF("Hello from TEST JNI !");
}

AAssetManager* kgm_getAssetManager()
{
    return g_assetManager;
}

#endif
