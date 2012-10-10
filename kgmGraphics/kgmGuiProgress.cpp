#include "kgmGuiProgress.h"

KGMOBJECT_IMPLEMENT(kgmGuiProgress, kgmGui)

kgmGuiProgress::kgmGuiProgress()
{

}

kgmGuiProgress::kgmGuiProgress(kgmGui *par, int x, int y, uint w, uint h)
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





