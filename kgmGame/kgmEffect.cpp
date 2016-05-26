#include "kgmEffect.h"

kgmEffect::kgmEffect(kgmIGame *g)
:kgmUnit(g)
{
  m_body = new kgmBody();

  m_body->m_collision = false;
}

void kgmEffect::init()
{
}

void kgmEffect::exit()
{
}
