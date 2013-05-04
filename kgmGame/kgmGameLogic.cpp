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

  for(kgmList<kgmActor*>::iterator i = m_actors.begin(); i != m_actors.end(); ++i)
  {
      AI* ai = m_ais[(*i)->m_class];

      if(ai)
          ai->ai((*i));
  }
}

void kgmGameLogic::onInput(int btn, int state)
{

}

