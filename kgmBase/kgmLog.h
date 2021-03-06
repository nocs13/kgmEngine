#pragma once
#include <time.h>
#include <stdarg.h>
#include <stdio.h>
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
  struct tm * timeinfo;

  time ( &rawtime );
  timeinfo = localtime ( &rawtime );
  
  return asctime (timeinfo);
}

inline void kgm_log_print(const char* s)
{
#ifdef DEBUG
  fprintf(stderr, "%s\n", s);
#endif
}

#define KGM_DBG_MSG(x) fprintf(stderr, "%s", x)
#define KGM_LOG() (kgm_log() << __FILE__ << ":" << __LINE__ << ":")

