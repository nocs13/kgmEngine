#pragma once
#ifdef WIN32
#include <windows.h>
#else
#include <time.h>
#include <sys/time.h>
#endif

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

    gettimeofday(&tv, 0);

    char *string = ctime((time_t *) &tv.tv_sec);

    text.alloc(30);

    sprintf(text.data(), "%.8s %07.3f", string + 11,
                (double) tv.tv_usec / 1000);


    return text;
  }
};

