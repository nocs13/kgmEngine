#include "kgmApp.h"
#include "kgmWindow.h"
#include "../kgmBase/kgmMemory.h"


#ifdef WIN32
 #include <windows.h>
#endif

#ifdef LINUX
 #include <unistd.h>
#endif


//kgmApp

//Application object, unique only  
kgmApp* kgmApp::m_app = 0;

kgmApp::kgmApp(){
 m_app = this; 
}

kgmApp::~kgmApp(){
 m_app = 0;
}

void kgmApp::main(){
}

//FOR WINDOWS
#if defined WIN32
HINSTANCE hInst = 0;
LONG kgmDumper(struct _EXCEPTION_POINTERS* pe)
{
 MessageBox(0, "Exception", "abort!", 0);
 return NULL;
}

#if defined _USRDLL
//DLL MAIN Function
BOOL __stdcall DllMain(HANDLE a, DWORD b, DWORD c){
 hInst = a;
 switch(b)
 {
 case DLL_PROCESS_DETACH:
	 kgmMemoryClear();
	 break;
 case DLL_THREAD_DETACH:
	 kgmMemoryClear();
	 break;
 }
 return TRUE;
}
#else
//WinMain  Function
int __stdcall WinMain(HINSTANCE a, HINSTANCE b, LPSTR pStr, int s)
{
 int rValue = 0;
 hInst = a;

 int argc = 0;
 char **argv = 0;

 ::SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)kgmDumper);

 if(kgmApp::application())
   kgmApp::application()->main();

 //kgmClearAllocatedMemory();
 return rValue;
}
#endif
#endif

//FOR LINUX
#ifdef LINUX

int main(int argc, char** argv){
 int rValue = 0;

 if(kgmApp::application())
   kgmApp::application()->main();

// kgmObject::releaseObjects();

 return rValue;
}

#endif

//FOR ANDROID
#ifdef ANDROID
/*
#include <jni.h>
#include <android/log.h>

#define  LOG_TAG    "libkgmEngine"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

extern "C"
{
    JNIEXPORT int JNICALL Java_com_android_kgmEngine_kgmEngine_main(JNIEnv * env, jobject obj,  jint width, jint height);
};

JNIEXPORT int JNICALL Java_com_android_kgmEngine_kgmEngine_main(JNIEnv * env, jobject obj,  jint width, jint height)
{
  int rValue = 0;

  if(kgmApp::application())
    kgmApp::application()->main();

  kgmObject::releaseObjects();

  return rValue;
}

int main()
{
  return 0;
}*/
#endif
/////////

