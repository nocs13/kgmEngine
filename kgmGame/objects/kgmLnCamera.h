#ifndef KGMLNCAMERA_H
#define KGMLNCAMERA_H

#include "../kgmSensor.h"

class kgmActor;

class kgmLnCamera : public kgmSensor
{
  KGM_UNIT(kgmLnCamera);

  kgmCamera *m_camera;
  kgmActor  *m_linked;
  kgmString  m_lname;

  vec3       m_place;
  vec3       m_look;

public:
  kgmLnCamera(kgmIGame *game = null);
  ~kgmLnCamera();

  void init();
  void update(u32 ms);

  bool linkto(kgmString name);
};

#endif // KGMLNCAMERA_H
