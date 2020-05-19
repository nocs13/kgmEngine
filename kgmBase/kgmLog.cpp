#include "kgmLog.h"
#include "kgmString.h"
#include "../kgmSystem/kgmThread.h"

#ifdef ANDROID

#include <jni.h>
#include <android/log.h>

#define  LOG_TAG    "kgmEngine"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

#endif

kgmLog kgmLog::LOG;

void kgmLog::log(kgmString s)
{
#ifdef ANDROID
    LOGI("kgmEngine: %s\n", (char*)s);
#else
    printf("kgmEngine: %s\n", (char*) s);
#endif
}

void kgmLog::log_(kgmString s)
{
#ifdef ANDROID
    LOGI("%s", (char*) s);
#else
    printf("%s", (s8*) s);
    fflush(stdout);
#endif
}

const char* kgmLog::kgm_log_label()
{
  time_t rawtime;
  struct tm * ti;

  ::time ( &rawtime );
  ti = localtime ( &rawtime );

  timeval tv;
  gettimeofday(&tv, 0);

  static char text[128];

  kgmTime time;
  time.update();

  memset(text, 0, sizeof(text));
  snprintf(text, sizeof(text) - 1, "%p %2d:%2d:%2d %d", (void*) kgmThread::id(),
           ti->tm_hour, ti->tm_min, ti->tm_sec, time.getUSecond());

  return (const char*) text;
}

