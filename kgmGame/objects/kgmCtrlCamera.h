#ifndef KGMCTRLCAMERA_H
#define KGMCTRLCAMERA_H

#include "../../kgmBase/kgmObject.h"

class kgmCamera;
class kgmActor;
class kgmIGame;

class kgmCtrlCamera : public kgmObject
{
  KGM_OBJECT(kgmCtrlCamera);

private:
  kgmIGame*   game;
  kgmCamera*  camera;
  kgmActor*   actor;
public:
  kgmCtrlCamera(kgmIGame* g = null);

  void setCamera(kgmCamera*);
  void setActor(kgmActor*);

  void update();
};

#endif // KGMCTRLCAMERA_H
