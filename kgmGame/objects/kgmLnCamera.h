#ifndef KGMLNCAMERA_H
#define KGMLNCAMERA_H

#include "../kgmSensor.h"

class kgmLnCamera : public kgmSensor
{
  KGM_GO_OBJECT(kgmLnCamera);

  kgmCamera *m_camera;

public:
  kgmLnCamera(kgmIGame *game = null);

  void init();
  void update(u32 ms);
};

#endif // KGMLNCAMERA_H
