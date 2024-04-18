#pragma once
#include <time.h>
#include <stdarg.h>
#include <stdio.h>
#include "kgmString.h"
#include "kgmConvert.h"
#include "../kgmSystem/kgmTime.h"

class kgmLog;

class kgmLog
{
public:
  enum Level
  {
    DL_None,
    DL_Error,
    DL_Warning,
    DL_Info,
    DL_Debug,
    DL_Verbose
  };
  
  static kgmLog LOG;
  static kgmLog EMPTY;
private:
  kgmTime time;
  
  Level level;
  
public:
  kgmLog()
  {
    level = DL_Error;
  }
  
  kgmLog(Level l)
  {
    level = l;
  }
  
  ~kgmLog()
  {
  }

  kgmLog& operator << (kgmString& s)
  {
    log(s);
    
    return *this;
  }

  kgmLog& operator << (kgmString s)
  {
    log(s);

    return *this;
  }

  kgmLog& operator << (const char* s)
  {
    log(s);

    return *this;
  }


  kgmLog& operator << (char* s)
  {
    log(s);

    return *this;
  }

  kgmLog& operator << (s32 s)
  {
    log(kgmConvert::toString(s));

    return *this;
  }

  kgmLog& operator << (u32 s)
  {
    log(kgmConvert::toString(s));

    return *this;
  }

  kgmLog& operator << (f64 s)
  {
    log(kgmConvert::toString(s));

    return *this;
  }

  kgmLog& operator << (void* p)
  {
    log(kgmConvert::toString(p));

    return *this;
  }

  void setLevel(Level level)
  {
    this->level = level;
  }
  
  Level getLevel()
  {
    return this->level;
  }
  
  void log(kgmString s);
  
  static const char* kgm_log_label();
};

inline kgmLog& kgm_log(kgmLog::Level level = kgmLog::DL_Error)
{
  if ((kgmLog::LOG.getLevel() == kgmLog::DL_None) || ( level > kgmLog::LOG.getLevel()))
  {
    return kgmLog::EMPTY;
  }

  fprintf(stderr, "kgmEngine [%s]: ", kgmLog::kgm_log_label());

  return kgmLog::LOG;
}

inline void kgm_set_loglevel(kgmLog::Level level)
{
  kgmLog::LOG.setLevel(level);
}

#define KGM_DBG_MSG(x) fprintf(stderr, "%s", x)
#define KGM_LOG() (kgm_log() << __FILE__ << ":" << __LINE__ << ":")

//#define sc_log(ctx, format, args...)   sc_do_log(ctx, SC_LOG_DEBUG_NORMAL, __FILE__, __LINE__, __FUNCTION__, format , ## args)

