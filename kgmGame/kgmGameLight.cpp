#include "kgmGameLight.h"

kgmGameLight::kgmGameLight(kgmIGame* g)
    :kgmGameNode(g)
{
  m_light = new kgmLight();

  m_type = LIGHT;
}
