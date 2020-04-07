#include "kgmLog.h"
#include "kgmString.h"

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
    printf("kgmEngine: %s\n", (char*)s);
#endif
}

void kgmLog::log_(kgmString s)
{
#ifdef ANDROID
    LOGI("%s", (char*) s);
#else
    printf("%s > %s", kgm_log_label(), (char*) s);
    fflush(stdout);
#endif
}
