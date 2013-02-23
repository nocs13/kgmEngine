#pragma once

#include <time.h>
#include <stdarg.h>
#include <stdio.h>
#include "kgmString.h"

class kgmLog;
class kgmLogError;
class kgmLogDebug;
class kgmLogWarning;

class kgmLog{
public:
    static kgmLog log;

private:
    FILE* out;

public:
    kgmLog(){
        out = fopen("kgmLog.log", "a");
    }

    kgmLog(const char* path){
        out = fopen(path, "a");
    }

    ~kgmLog(){
        fclose(out);
    }

    kgmLog& operator << (kgmString& s){
        fprintf(out, (char*)s);
        return *this;
    }

    kgmLog& operator << (kgmString s){
        fprintf(out, (char*)s);
        return *this;
    }
    
    kgmLog& operator << (const char* s){
        fprintf(out, s);
        return *this;
    }
    
    
    kgmLog& operator << (char* s){
        fprintf(out, s);
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
#ifdef DEBUG
    kgm_log(s);
    fprintf(stderr, "%s\n", s);
#endif
}

#define KGM_DBG_MSG(x) fprintf(stderr, "%s", x)

