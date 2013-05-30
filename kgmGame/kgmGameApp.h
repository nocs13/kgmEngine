#pragma once
#include "kgmIGame.h"

#ifdef ANDROID
  //initiate exit from program
  extern void       kgm_android_exit();
  //create game exempliar
  extern kgmIGame*  kgm_android_init_game();
#endif
