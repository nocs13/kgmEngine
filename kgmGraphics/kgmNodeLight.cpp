#include "kgmNodeLight.h"

kgmNodeLight::kgmNodeLight()
{
  m_light = new kgmLight();
}

kgmNodeLight::~kgmNodeLight()
{
  delete m_light;
}
