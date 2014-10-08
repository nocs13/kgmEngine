#pragma once
#ifdef WIN32
#include <windows.h>
#include <time.h>
#else
#include <time.h>
#include <sys/time.h>
#endif

#include <stdio.h>
#include "kgmString.h"

typedef unsigned int u32;

class kgmTime
{
public:

  static u32 getTicks()
  {
#ifdef WIN32
    return (u32)GetTickCount();
#else
    timeval tv;
    gettimeofday(&tv, 0);

    return (u32)(tv.tv_sec * 1000) + (tv.tv_usec / 1000);
#endif
  }

  static u32 getTime()
  {
#ifdef WIN32
    return (u32)GetTickCount();
#else
    timeval tv;
    gettimeofday(&tv, 0);

    return (u32)(tv.tv_sec * 1000) + (tv.tv_usec / 1000);
#endif
  }

  static kgmString getTimeText()
  {
    kgmString text;

    timeval tv;

#ifdef WIN32
    static const uint64_t EPOCH = ((uint64_t) 116444736000000000ULL);

    SYSTEMTIME  system_time;
    FILETIME    file_time;
    uint64_t    time;

    GetSystemTime( &system_time );
    SystemTimeToFileTime( &system_time, &file_time );
    time =  ((uint64_t)file_time.dwLowDateTime )      ;
    time += ((uint64_t)file_time.dwHighDateTime) << 32;

    tv.tv_sec  = (long) ((time - EPOCH) / 10000000L);
    tv.tv_usec = (long) (system_time.wMilliseconds * 1000);
#else
    gettimeofday(&tv, 0);
#endif

    char *string = ctime((time_t *) &tv.tv_sec);

    text.alloc(30);

    sprintf(text.data(), "%.8s %07.3f", string + 11,
                (double) tv.tv_usec / 1000);

    return text;
  }
};

