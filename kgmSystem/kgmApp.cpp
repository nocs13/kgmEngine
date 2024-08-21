#include "kgmApp.h"
#include "kgmWindow.h"
#include "../kgmBase/kgmLog.h"
#include "../kgmBase/kgmArray.h"

#include <signal.h>

#ifdef WIN32

#define SYMOPT_LOAD_LINES 0x10
#define SYMOPT_DEFERRED_LOADS 0x4

typedef struct _SYMBOL_INFO {
  ULONG   SizeOfStruct;
  ULONG   TypeIndex;
  ULONG64 Reserved[2];
  ULONG   Index;
  ULONG   Size;
  ULONG64 ModBase;
  ULONG   Flags;
  ULONG64 Value;
  ULONG64 Address;
  ULONG   Register;
  ULONG   Scope;
  ULONG   Tag;
  ULONG   NameLen;
  ULONG   MaxNameLen;
  CHAR    Name[1];
} SYMBOL_INFO, *PSYMBOL_INFO;

typedef USHORT __stdcall (*FN_RtlCaptureStackBackTrace)(ULONG, ULONG, PVOID*, PULONG);

typedef BOOL __stdcall (*FN_SymInitialize)(HANDLE, PCSTR, BOOL);
typedef DWORD __stdcall (*FN_SymSetOptions)(DWORD);
typedef DWORD __stdcall (*FN_SymGetOptions)();
typedef BOOL __stdcall (*FN_SymFromAddr)(HANDLE, DWORD64, PDWORD64, PSYMBOL_INFO);

FN_RtlCaptureStackBackTrace RtlCaptureStackBackTrace = NULL; 

FN_SymGetOptions SymGetOptions = NULL;
FN_SymSetOptions SymSetOptions = NULL;
FN_SymInitialize SymInitialize = NULL;
FN_SymFromAddr   SymFromAddr = NULL;
#else
#include <execinfo.h>
#include <unistd.h>
#include <sys/types.h>

static char stack_body[64*1024];
static stack_t sigseg_stack;
static struct sigaction sigseg_handler;

#endif

static void kgm_register_signals();
static void kgm_sigterm_handler(int s);

extern void kgm_memory_init();
extern void kgm_memory_cleanup();

static void* exit_target = nullptr;

static bool aborting = false;

//kgmApp

//Application object, unique only
kgmApp* kgmApp::m_app = 0;

kgmString kgmApp::m_execPath;

kgmApp::kgmApp()
{
  m_app = this;
  
  #ifdef WIN32
  m_hDbg = LoadLibrary("dbghelp.dll");
  
  if (m_hDbg == NULL)
  {
	fprintf(stderr, "Error: Unable load dbghelp.dll. [%x]", GetLastError());
  }
  
  SymGetOptions = (FN_SymGetOptions) GetProcAddress(m_hDbg, "SymGetOptions");
  SymSetOptions = (FN_SymSetOptions) GetProcAddress(m_hDbg, "SymSetOptions");
  SymInitialize = (FN_SymInitialize) GetProcAddress(m_hDbg, "SymInitialize");
  SymFromAddr   = (FN_SymFromAddr)   GetProcAddress(m_hDbg, "SymFromAddr");
  
  RtlCaptureStackBackTrace = (FN_RtlCaptureStackBackTrace) GetProcAddress(NULL, "RtlCaptureStackBackTrace");
  #endif


  kgm_register_signals();

  kgm_memory_init();
}

kgmApp::~kgmApp()
{
  m_app = null;

  #ifdef WIN32
  if (m_hDbg != NULL)
  {
	FreeLibrary(m_hDbg);
  }
  #endif

  kgmObject::kgm_objects_cleanup();
  kgm_memory_cleanup();
}

s32 kgmApp::exec(s32 argc, s8 **argv)
{
  if (argc > 0)
  {
    kgmApp::m_execPath = argv[0];
  }

  return 0;
}

void kgmApp::exit(s32 e)
{
  if (aborting)
    return;

  if (e != 0)
    kgm_log() << "ABORT process!!!\n";

  aborting = true;

  if (m_app)
    m_app->abort();

  exit(e);
}

void kgmApp::Abort()
{
#ifdef WIN32
  TerminateProcess((HANDLE) GetCurrentProcess(), -1);
#else
  kill(getpid(), SIGTERM);
#endif
}

#ifdef WIN32
void kgm_print_backstack( void )
{
  unsigned int  i;
  void         *stack[ 100 ];
  unsigned short frames;
  SYMBOL_INFO  *symbol;
  HANDLE        process;

  process = GetCurrentProcess();

  SymInitialize( process, NULL, TRUE );

  frames               = RtlCaptureStackBackTrace( 0, 100, stack, NULL );
  symbol               = ( SYMBOL_INFO * )calloc( sizeof( SYMBOL_INFO ) + 256 * sizeof( char ), 1 );
  symbol->MaxNameLen   = 255;
  symbol->SizeOfStruct = sizeof( SYMBOL_INFO );

  for( i = 0; i < frames; i++ )
  {
    SymFromAddr( process, ( DWORD64 )( stack[ i ] ), 0, symbol );

    fprintf( stderr, "%i: %s - 0x%0X\n", frames - i - 1, symbol->Name, symbol->Address );
  }

  free( symbol );
}

LONG WINAPI kgm_TopLevelExceptionHandler(PEXCEPTION_POINTERS pExceptionInfo)
{
  fprintf(stderr, "TopLevelExceptionHandler: code %x, offset %llx.\n",
          pExceptionInfo->ExceptionRecord->ExceptionCode,
          pExceptionInfo->ExceptionRecord->ExceptionAddress);

  EXCEPTION_RECORD *rec = pExceptionInfo->ExceptionRecord->ExceptionRecord;

  while(rec)
  {
    fprintf(stderr, "  TopLevelExceptionHandler: code %x, offset %llx.\n",
            rec->ExceptionCode,  rec->ExceptionAddress);

    rec = rec->ExceptionRecord;
  }

  kgm_print_backstack();

  return EXCEPTION_CONTINUE_SEARCH;
}

LONG WINAPI kgm_VectoredExceptionHandler(PEXCEPTION_POINTERS pExceptionInfo)
{
  fprintf(stderr, "VectoredExceptionHandler: code %x, offset %llx.\n",
          pExceptionInfo->ExceptionRecord->ExceptionCode,
          pExceptionInfo->ExceptionRecord->ExceptionAddress);

  EXCEPTION_RECORD *rec = pExceptionInfo->ExceptionRecord->ExceptionRecord;

  while(rec)
  {
    fprintf(stderr, "  VectoredExceptionHandler: code %x, offset %llx.\n",
            rec->ExceptionCode,  rec->ExceptionAddress);

    rec = rec->ExceptionRecord;
  }

  kgm_print_backstack();

  return EXCEPTION_CONTINUE_SEARCH;
}
#else
void kgm_sigterm_handler(int s)
{
  void *array[1024];
  size_t size;

  // get void*'s for all entries on the stack
  size = backtrace(array, 1024);

  // print out all the frames to stderr
  fprintf(stderr, "\n============ Error: signal %d =============\n", s);

  backtrace_symbols_fd(array, size, STDERR_FILENO);

  fprintf(stderr, "\n===========================================\n");

  signal(s, SIG_DFL);
  kill(getpid(), s);
}
#endif

void kgm_register_signals()
{
  #ifdef WIN32

  DWORD options = SymGetOptions();

  options |= SYMOPT_LOAD_LINES | SYMOPT_DEFERRED_LOADS;

  SymSetOptions(options);

  if (SymInitialize(GetCurrentProcess(), NULL, 1) == 0)
  {
    fprintf(stderr, "XXX Cannot initialize access to symbols.\n");
  }

  AddVectoredExceptionHandler(0, kgm_TopLevelExceptionHandler);
  SetUnhandledExceptionFilter(kgm_VectoredExceptionHandler);
  #else

  //signal(SIGINT,   kgm_sigterm_handler);
  //signal(SIGILL,   kgm_sigterm_handler);
  //signal(SIGTERM,  kgm_sigterm_handler);
  //signal(SIGSEGV,  kgm_sigterm_handler);
  //signal(SIGABRT,  kgm_sigterm_handler);
  //signal(SIGBREAK, kgm_sigterm_handler);

  sigseg_stack.ss_sp = stack_body;
  sigseg_stack.ss_flags = SS_ONSTACK;
  sigseg_stack.ss_size = sizeof(stack_body);
  assert(!sigaltstack(&sigseg_stack, nullptr));
  sigseg_handler.sa_flags = SA_ONSTACK;
  sigseg_handler.sa_handler = kgm_sigterm_handler;

  assert(!sigaction(SIGINT, &sigseg_handler, nullptr));
  assert(!sigaction(SIGTERM, &sigseg_handler, nullptr));
  assert(!sigaction(SIGSEGV, &sigseg_handler, nullptr));
  assert(!sigaction(SIGABRT, &sigseg_handler, nullptr));

  #endif
}

/*
//FOR WINDOWS
#if defined WIN32
HINSTANCE hInst = 0;
LONG kgmDumper(struct _EXCEPTION_POINTERS* pe)
{
  MessageBox(0, "Exception", "abort!", 0);
  return 0;
}

#if defined _USRDLL

BOOL __stdcall DllMain(HANDLE a, DWORD b, DWORD c){
  hInst = a;
  switch(b)
  {
  case DLL_PROCESS_DETACH:
  case DLL_THREAD_DETACH:
    break;
  }

KGM_APP_EXIT:
  exit_target = &&KGM_APP_EXIT;

  return TRUE;
}

#else

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
    int cmdLen = strlen(GetCommandLine());

    argv = new char_ptr[sizeof(char_ptr) * 256];

    args = new char[cmdLen + 1];

    memset(args, 0, cmdLen + 1);

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

KGM_APP_EXIT:
  exit_target = &&KGM_APP_EXIT;

  if(args)
    delete [] args;

  if(argv)
    delete [] argv;

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
  //ClearDbgMemory();
KGM_APP_EXIT:
  exit_target = &&KGM_APP_EXIT;

  return rValue;
}

#endif
*/
//FOR ANDROID
#ifdef ANDROID
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

  signal(SIGINT,   kgm_signal_handler);
  signal(SIGILL,   kgm_signal_handler);
  signal(SIGSEGV,  kgm_signal_handler);
  signal(SIGABRT,  kgm_signal_handler);
  signal(SIGTERM,  kgm_sigterm_handler);

  if(kgmApp::application())
    kgmApp::application()->main(0, null);

  return rValue;
}

int main()
{
  return 0;
}
#endif
