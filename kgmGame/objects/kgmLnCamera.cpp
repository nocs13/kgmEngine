#include "kgmLnCamera.h"
#include "../kgmIGame.h"
#include "../../kgmGraphics/kgmIGraphics.h"

KGMOBJECT_IMPLEMENT(kgmLnCamera, kgmSensor);

kgmLnCamera::kgmLnCamera(kgmIGame* g)
:kgmSensor(g)
{
  m_camera = null;
}

void kgmLnCamera::init()
{
  if(game())
    m_camera = &game()->getGraphics()->camera();
}

void kgmLnCamera::update(u32 ms)
{
  if(m_camera == null)
    return;
}
