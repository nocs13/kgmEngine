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
      if((*i)->m_class.length() < 1)
          continue;

      (*i)->update(milliseconds);

      AI* ai = null;

      if(m_ais.get((*i)->m_class, ai) && ai)
          ai->ai((*i));
  }
}

void kgmGameLogic::onInput(int btn, int state)
{

}

