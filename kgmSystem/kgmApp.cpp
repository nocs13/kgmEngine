#include "kgmApp.h"
#include "kgmWindow.h"
#include "../kgmBase/kgmLog.h"
#include "../kgmBase/kgmMemory.h"

#include <signal.h>

#ifdef WIN32
#include <windows.h>
#endif

#ifdef LINUX
#include <unistd.h>
#endif

static void kgm_signal_handler(int s);
static void kgm_sigterm_handler(int s);
extern void ClearDbgMemory();

//kgmApp

//Application object, unique only  
kgmApp* kgmApp::m_app = 0;

kgmApp::kgmApp()
{
  m_app = this;
}

kgmApp::~kgmApp()
{
  m_app = 0;
}

s32 kgmApp::main(s32 argc, s8 **argv)
{
  return 0;
}

void kgmApp::abort()
{

}

void kgm_abort()
{
#ifdef WIN32
#else
  kill(getpid(), SIGTERM);
#endif
}

void kgm_sigterm_handler(int s)
{
  if (kgmApp::application())
    kgmApp::application()->abort();
}

//FOR WINDOWS
#if defined WIN32
HINSTANCE hInst = 0;
LONG kgmDumper(struct _EXCEPTION_POINTERS* pe)
{
  MessageBox(0, "Exception", "abort!", 0);
  return 0;
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
  typedef char* char_ptr;

  int rValue = 0;
  hInst = a;

  int  argc = 0;
  char *args = 0;
  char **argv = 0;

  if(pStr != NULL)
  {
    argv = new char_ptr[sizeof(char_ptr) * 256];

    args = new char[strlen(GetCommandLine()) + 1];

    memset(args, 0, strlen(GetCommandLine()) + 1);

    strcpy(args, GetCommandLine());

    char* pch = strtok(args, " ");

    while(pch != NULL)
    {
      argc++;

      if(argc > 255)
        break;

      argv[argc - 1] = pch;

      pch = strtok(NULL, " ");
    }
  }

  signal(SIGINT,   kgm_signal_handler);
  signal(SIGILL,   kgm_signal_handler);
  signal(SIGTERM,  kgm_signal_handler);
  signal(SIGSEGV,  kgm_signal_handler);
  signal(SIGABRT,  kgm_signal_handler);
  signal(SIGBREAK, kgm_signal_handler);

  kgm_log() << "LOL: " << pStr << "\n";

  if(kgmApp::application())
    kgmApp::application()->main(argc, argv);

  if(args)
    delete [] args;

  if(argv)
    delete [] argv;

  //kgmClearAllocatedMemory();
  return rValue;
}
#endif
#endif

//FOR LINUX
#ifdef LINUX

int main(int argc, char** argv){
  int rValue = 0;

  signal(SIGINT,   kgm_signal_handler);
  signal(SIGILL,   kgm_signal_handler);
  signal(SIGSEGV,  kgm_signal_handler);
  signal(SIGABRT,  kgm_signal_handler);
  signal(SIGTERM,  kgm_sigterm_handler);

  if(kgmApp::application())
    rValue = kgmApp::application()->main((s32)argc, (s8**)argv);

  // kgmObject::releaseObjects();
  ClearDbgMemory();

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

void kgm_signal_handler(int s)
{
  printf("Caught signal %d\n",s);
  exit(1);
}
/////////

