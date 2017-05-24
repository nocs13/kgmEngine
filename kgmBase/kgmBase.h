#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "kgmTypes.h"

#ifndef __stdcall
  #define __stdcall
#endif

#ifdef WIN32
  #define KGM_DECL_EXPORT     __declspec(dllexport)
  #define KGM_DECL_IMPORT     __declspec(dllimport)
#elif defined(QT_VISIBILITY_AVAILABLE)
  #define KGM_DECL_EXPORT     __attribute__((visibility("default")))
  #define KGM_DECL_IMPORT     __attribute__((visibility("default")))
  #define KGM_DECL_HIDDEN     __attribute__((visibility("hidden")))
#endif


void kgm_abort();
