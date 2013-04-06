#include "../kgmSystem/kgmApp.h"
#include "../kgmGame/kgmGameBase.h"
//#include "../kgmGame/kgmGameLogic.h"

#include "../kgmBase/kgmXml.h"

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
  printf("\n id=%s", (char*)s);
  printf("\n data=%s", (char*)d);
  
  for(int i = 0; i < n->attributes(); i++){
    kgmString s, d;
    n->attribute(i, s, d);
    printf("\n   attr: %s=%s", (char*)s, (char*)d);
  }
  
  for(int i = 0; i < n->nodes(); i++)
   print_xml(n->node(i));
 }
 
  void printf_xml(char* path){
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
   printf("hello\n");
 } 
};

//main object
kApp theApp;
//////////////

//FOR ANDROID
#ifdef ANDROID

#include <jni.h>
#include <android/log.h>

#define  LOG_TAG    "libkgmEngine"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

kGame*	m_game = null;

extern "C"
{
  JNIEXPORT void  JNICALL Java_com_example_Test_Test_init(JNIEnv * env, jobject obj,  jint width, jint height);
  JNIEXPORT void  JNICALL Java_com_example_Test_Test_quit(JNIEnv * env, jobject obj);
  JNIEXPORT void  JNICALL Java_com_example_Test_Test_idle(JNIEnv * env, jobject obj);
  JNIEXPORT void  JNICALL Java_com_example_Test_Test_msMove(JNIEnv * env, jobject obj,  jint x, jint y);
  JNIEXPORT jstring  JNICALL Java_com_example_Test_Test_stringFromJNI(JNIEnv * env, jobject obj);
};

JNIEXPORT void JNICALL Java_com_example_Test_Test_init(JNIEnv * env, jobject obj,  jint width, jint height)
{
    m_game = new kGame();
}

JNIEXPORT void JNICALL Java_com_example_Test_Test_quit(JNIEnv * env, jobject obj)
{
    if(m_game)
      m_game->release();
    m_game = null;
}

JNIEXPORT void JNICALL Java_com_example_Test_Test_idle(JNIEnv * env, jobject obj)
{
    if(m_game)
      m_game->onIdle();
}

JNIEXPORT void JNICALL Java_com_example_Test_Test_msMove(JNIEnv * env, jobject obj,  jint x, jint y)
{
    if(m_game)
      m_game->onMsMove(0, x, y);
}

JNIEXPORT jstring  JNICALL Java_com_example_Test_Test_stringFromJNI(JNIEnv * env, jobject obj){
    return (env)->NewStringUTF("Hello from TEST JNI !");
}


#endif
