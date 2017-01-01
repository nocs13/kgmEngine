#ifndef KGMGAMECAMERA_H
#define KGMGAMECAMERA_H

#include "kgmGameNode.h"
#include "../kgmGraphics/kgmCamera.h"

class kgmGameCamera : public kgmGameNode
{
  kgmCamera* m_camera;
public:
  kgmGameCamera(kgmIGame*);

  kgmCamera* camera() const
  {
    return m_camera;
  }
};

#endif // KGMGAMECAMERA_H
