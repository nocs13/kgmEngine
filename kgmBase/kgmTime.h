#pragma once
#ifdef WIN32
#include <windows.h>
#endif

#ifdef LINUX
#include <time.h>
#include <sys/time.h>
#endif

typedef unsigned int uint;

class kgmTime
{
public:

static uint getTicks(){
#ifdef WIN32
 return (uint)GetTickCount();
#endif

#ifdef LINUX
 timeval tv;
 gettimeofday(&tv, 0);
 return (uint)(tv.tv_sec * 1000) + (tv.tv_usec / 1000);
#endif
}

static uint getTime(){
#ifdef WIN32
 return (uint)GetTickCount();
#endif

#ifdef LINUX
 timeval tv;
 gettimeofday(&tv, 0);
 return (uint)(tv.tv_sec * 1000) + (tv.tv_usec / 1000);
#endif
}

};

