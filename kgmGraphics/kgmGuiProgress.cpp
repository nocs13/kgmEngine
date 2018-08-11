#include "kgmGuiProgress.h"

kgmGuiProgress::kgmGuiProgress()
{

}

kgmGuiProgress::kgmGuiProgress(kgmGui *par, int x, int y, u32 w, u32 h)
{

}

kgmGuiProgress::~kgmGuiProgress()
{

}

inline void kgmGuiProgress::set(float a)
{
  if(a > 1.0f)
  {
    return;
  }

  m_progress = a;
}

inline float kgmGuiProgress::get()
{
  return m_progress;
}





