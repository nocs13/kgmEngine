#pragma once

#include <iostream>
#include <fstream>
#include <time.h>
#include <stdarg.h>
#include "kgmString.h"

using namespace std;

class kgmLog{
public:
    static kgmLog log;

private:
    ofstream out;

public:
    kgmLog(){
      out.open("kgmLog.log", ios_base::app);
    }

    kgmLog(const char* path){
      out.open(path, ios_base::app);
    }

    ~kgmLog(){
      out.close();
    }

    kgmLog& operator << (kgmString& s){
      return *this << (char*)s.data();
    }

    kgmLog& operator << (kgmString s){
      return *this << (char*)s.data();
    }
    
    kgmLog& operator << (const char* s){
      return *this << (char*)s;
    }
    
    
    kgmLog& operator << (char* s){
      if(s && out.is_open()){
        out << s;
      }
      return *this;
    }
};

inline kgmLog& kgm_log()
{
  //  kgmLog log;
  //  va_list vl;
  //  va_start(vl, a);
  return kgmLog::log;
}

inline void kgm_log(const char* s)
{
  kgmLog log;
  time_t rawtime;
  struct tm * timeinfo;
  
  time ( &rawtime );
  timeinfo = localtime ( &rawtime );
  if(s)
    log << asctime (timeinfo) << "\t" << s << "\n";
}

inline const char* kgm_log_label(){
  time_t rawtime;
  struct tm * timeinfo;
  
  time ( &rawtime );
  timeinfo = localtime ( &rawtime );
  return asctime (timeinfo);
}

inline void kgm_log_print(const char* s){
  kgm_log(s);
#ifdef DEBUG
  fprintf(stderr, "%s\n", s);
#endif
}

#define KGM_DBG_MSG(x) fprintf(stderr, "%s", x)
