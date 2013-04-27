#include "kgmGameLogic.h"

kgmGameLogic::kgmGameLogic()
{
}

void kgmGameLogic::clear()
{
  m_actors.clear();
  m_ais.clear();
}

void kgmGameLogic::update(u32 milliseconds)
{
  for(kgmList<Sensor*>::iterator i = m_sensors.begin(); i != m_sensors.end(); ++i)
  {
    (*i)->sense();
  }
}
