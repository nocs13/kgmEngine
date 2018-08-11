#pragma once
#include "kgmGui.h"

class kgmGuiProgress: public kgmGui
{
  KGM_OBJECT(kgmGuiProgress);

private:
  float m_progress;

public:

  kgmGuiProgress();

  kgmGuiProgress(kgmGui *par, int x, int y, u32 w, u32 h);

  virtual ~kgmGuiProgress();


  void  set(float);

  float get();
};
