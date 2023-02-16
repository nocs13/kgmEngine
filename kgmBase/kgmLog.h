#pragma once
#include <time.h>
#include <stdarg.h>
#include <stdio.h>
#include "kgmString.h"
#include "kgmConvert.h"
#include "../kgmSystem/kgmTime.h"

class kgmLog;
class kgmLogError;
class kgmLogDebug;
class kgmLogWarning;

class kgmLog
{
public:
    static kgmLog LOG;

    kgmString f_path;

private:
    kgmTime time;

public:
    kgmLog()
    {
    }

    kgmLog(const char* path)
    {
      f_path = path;
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
    static const char* kgm_log_label();

private:
    static void log_(kgmString s);

};

inline kgmLog& kgm_log()
{
  printf("kgmEngine [%s]: ", kgmLog::kgm_log_label());

  return kgmLog::LOG;
}

inline void kgm_log_print(const char* s)
{
#ifdef DEBUG
  fprintf(stderr, "%s\n", s);
#endif
}

#define KGM_DBG_MSG(x) fprintf(stderr, "%s", x)
#define KGM_LOG() (kgm_log() << __FILE__ << ":" << __LINE__ << ":")

//#define sc_log(ctx, format, args...)   sc_do_log(ctx, SC_LOG_DEBUG_NORMAL, __FILE__, __LINE__, __FUNCTION__, format , ## args)

