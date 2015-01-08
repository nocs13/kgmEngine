#include "kgmLnCamera.h"
#include "../kgmIGame.h"
#include "../kgmILogic.h"
#include "../kgmActor.h"
#include "../../kgmGraphics/kgmIGraphics.h"

KGMOBJECT_IMPLEMENT(kgmLnCamera, kgmSensor);

kgmLnCamera::kgmLnCamera(kgmIGame* g)
:kgmSensor(g)
{
  m_camera = null;
  m_linked = null;
}

kgmLnCamera::~kgmLnCamera()
{
  if(m_linked)
    m_linked->release();
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

bool kgmLnCamera::linkto(kgmString name)
{
  if(!game() || !game()->getLogic())
    return false;

  kgmActor *a = (kgmActor*)game()->getLogic()->getObjectById(name);

  if(!a)
    return false;

  if(m_linked)
    m_linked->release();

  a->increment();

  m_linked = a;
  m_lname = name;

  return true;
}
