#include "kgmUnit.h"
#include "kgmIGame.h"

kgmUnit::kgmUnit(kgmIGame* g)
{
  m_game = g;

  m_valid   = true;
  m_remove  = false;
  m_culled  = false;
  m_visible = true;

  m_birth   = kgmTime::getTicks();
  m_living  = -1;

  m_name  = "None";
  m_class = "None";
}

kgmUnit::~kgmUnit()
{
  if(m_remove)
    remove();
}

void kgmUnit::clear()
{
  if (m_node)
    m_node->release();

  if (m_body)
    m_body->release();

  m_node = null;
}

void kgmUnit::remove()
{
  clear();

  m_remove  = true;
  m_valid   = false;
  m_visible = false;
}

u32 kgmUnit::timeout()
{
  return (kgmTime::getTicks() - m_birth);
}

void kgmUnit::update(u32 ms)
{
  if(removed())
    return;

  if((getLiving() != (u32) -1) && (timeout() > getLiving())) {
    remove();

    return;
  }

  if (m_parent) {
    vec3 v = m_parent->position();

    vec3 f = v + position();

    position(f);
  }
}

void kgmUnit::start()
{
}

void kgmUnit::setNode(kgmGNode* n)
{
  if (!n)
    return;

  if (m_node)
    ((kgmObject*)m_node)->release();

  m_node = n;
}

void kgmUnit::setBody(kgmPBody* b)
{
  if (!b)
    return;

  if (m_body)
    m_body->release();

  m_body = b;
}
