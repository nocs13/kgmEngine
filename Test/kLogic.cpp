#include "kLogic.h"

kLogic::kLogic(kgmIGame* g)
{
  m_game = g;
}

void kLogic::update(u32 milliseconds)
{

    kgmGameLogic::update(milliseconds);
}

void kLogic::input(int btn, int val)
{
    if(m_player && btn == grot_x)
    {
        vec3 v = m_player->m_body->m_rotation;
        v.x += (val * 0.2);
        m_player->setRotation(v);
    }

}
