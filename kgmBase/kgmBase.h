#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#ifndef nil
  #define nil 0L
#endif

#ifndef null
  #define null 0L
#endif

#ifndef NULL
  #define NULL 0L
#endif

#ifndef __stdcall
  #define __stdcall
#endif

inline void* kgm_alloc(size_t size);
inline void  kgm_free(void*);
inline void  kgm_assign(void** src, void** dst);
