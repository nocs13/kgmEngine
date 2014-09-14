#include "kgmCtrlCamera.h"
#include "../kgmIGame.h"
#include "../kgmActor.h"
#include "../../kgmGraphics/kgmCamera.h"

kgmCtrlCamera::kgmCtrlCamera(kgmIGame* g)
{
}

void kgmCtrlCamera::setActor(kgmActor *a)
{
  actor = a;
}

void kgmCtrlCamera::setCamera(kgmCamera *c)
{
  camera = c;
}
