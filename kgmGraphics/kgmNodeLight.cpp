#include "kgmNodeLight.h"

kgmNodeLight::kgmNodeLight()
{
  m_light = new kgmLight();

  m_position = vec3(0, 0, 10);
}

kgmNodeLight::~kgmNodeLight()
{
  delete m_light;
}
