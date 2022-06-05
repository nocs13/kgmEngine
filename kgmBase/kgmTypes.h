#pragma once

#include <stdint.h>

#ifndef null
  #ifdef nullptr
    #define null nullptr
  #else
    #define null 0L
  #endif
#endif

#ifndef nil
  #define nil 0L
#endif

#ifndef NULL
  #define NULL 0L
#endif

#ifdef __x86_64__
# define __WORDSIZE__     64
#elif defined(__i386__)
# define __WORDSIZE__     32
#endif

typedef uint8_t    u8;
typedef uint16_t   u16;
typedef uint32_t   u32;
typedef uint64_t   u64;

//typedef int8_t     s8;
typedef char     s8;
typedef int16_t    s16;
typedef int32_t    s32;
typedef int64_t    s64;

typedef float       f32;
typedef double      f64;
typedef long double f80;
