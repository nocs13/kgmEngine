#pragma once
#include "kgmIGame.h"
#include "../kgmSystem/kgmApp.h"

class kgmGameApp: public kgmApp
{
  kgmIGame*  m_game;
public:
  virtual ~kgmGameApp(){}

  kgmIGame* game(){
    return m_game;
  }

  static kgmGameApp* gameApplication(){
    return (kgmGameApp*)kgmApp::application();
  }
};

#ifdef ANDROID
  //initiate exit from program
  extern void       kgm_android_exit();
  //create game exempliar
  extern kgmIGame*  kgm_android_init_game();
#endif
