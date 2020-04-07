#pragma once
#include <time.h>
#include <stdarg.h>
#include <stdio.h>
#include "kgmTime.h"
#include "kgmString.h"
#include "kgmConvert.h"

class kgmLog;
class kgmLogError;
class kgmLogDebug;
class kgmLogWarning;

class kgmLog
{
public:
    static kgmLog LOG;

private:
    kgmTime time;

public:
    kgmLog()
    {
    }

    kgmLog(const char* path)
    {
    }

    ~kgmLog()
    {
    }

    kgmLog& operator << (kgmString& s)
    {
      log_(s);
 
      return *this;
    }

    kgmLog& operator << (kgmString s)
    {
      log_(s);
    
      return *this;
    }
    
    kgmLog& operator << (const char* s)
    {
      log_(s);
      
      return *this;
    }
    
    
    kgmLog& operator << (char* s)
    {
      log_(s);

      return *this;
    }

    kgmLog& operator << (s32 s)
    {
      log_(kgmConvert::toString(s));

      return *this;
    }

    kgmLog& operator << (u32 s)
    {
      log_(kgmConvert::toString(s));

      return *this;
    }

    kgmLog& operator << (f64 s)
    {
      log_(kgmConvert::toString(s));
      
      return *this;
    }

    kgmLog& operator << (void* p)
    {
      log_(kgmConvert::toString(p));

      return *this;
    }

    static void log(kgmString s);

private:
    static void log_(kgmString s);
};

inline kgmLog& kgm_log()
{
    //  kgmLog log;
    //  va_list vl;
    //  va_start(vl, a);
    return kgmLog::LOG;
}

inline const char* kgm_log_label()
{
  time_t rawtime;
  struct tm * ti;

  time ( &rawtime );
  ti = localtime ( &rawtime );

  timeval tv;
  gettimeofday(&tv, 0);

  static char text[128];

  kgmTime time;
  time.update();

  memset(text, 0, sizeof(text));
  sprintf(text, "%2d:%2d:%2d %d", ti->tm_hour, ti->tm_min, ti->tm_sec, time.getUSecond());
  
  //return asctime (timeinfo);
  return (const char*) text;
}

inline void kgm_log_print(const char* s)
{
#ifdef DEBUG
  fprintf(stderr, "%s\n", s);
#endif
}

#define KGM_DBG_MSG(x) fprintf(stderr, "%s", x)
#define KGM_LOG() (kgm_log() << __FILE__ << ":" << __LINE__ << ":")

