#include "kgmGameLogic.h"
#include "kgmIGame.h"

kgmGameLogic::kgmGameLogic()
{
}

kgmGameLogic::~kgmGameLogic()
{
    clear();
    m_ais.clear();
}

void kgmGameLogic::clear()
{
    m_actors.clear();
}

bool kgmGameLogic::add(kgmString type, AI* ai)
{
    m_ais.add(type, ai);

    return true;
}

bool kgmGameLogic::add(kgmActor *a)
{
    if(a)
    {
        AI* ai = null;

        if(m_ais.get(a->m_class, ai))
        {
            if(a->m_gameplayer)
                m_gameplayer = a;
            m_actors.add(a, ai);
        }
    }

    return false;
}

void kgmGameLogic::update(u32 milliseconds)
{
    for(kgmList<Sensor*>::iterator i = m_sensors.begin(); i != m_sensors.end(); ++i)
    {
        (*i)->sense();
    }

    for(kgmTab<kgmActor*, AI*>::iterator i = m_actors.begin(); i != m_actors.end(); ++i)
    {
        if((i).key()->m_class.length() < 1)
            continue;

        (i).value()->update((i).key(), milliseconds);
    }
}

void kgmGameLogic::input(int btn, int state)
{
    if(m_gameplayer)
    {
        AI* ai = m_actors[m_gameplayer];
        ai->input(m_gameplayer, btn, state);
    }
    else
    {
    }
}
