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
#else
# define __WORDSIZE__     32
#endif

//typedef uint8_t    u8;
typedef unsigned char u8;
typedef uint16_t   u16;
typedef uint32_t   u32;

//typedef int8_t     s8;
typedef char       s8;
typedef int16_t    s16;
typedef int32_t    s32;

typedef int64_t    s64;
typedef uint64_t   u64;

typedef float  f32;
typedef double f64;

/*
typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned int   u32;

typedef unsigned int   uint;

typedef char  s8;
typedef short s16;
typedef int   s32;

typedef float  f32;
typedef double f64;

typedef int64_t s64;
typedef int64_t u64;

typedef unsigned char    uint8;
typedef unsigned short   uint16;
typedef unsigned int     uint32;
typedef char             int8;
typedef short            int16;
typedef int              int32;
typedef float            float32;
typedef double           float64;
typedef int64_t          int64;
typedef int64_t          uint64;
*/





