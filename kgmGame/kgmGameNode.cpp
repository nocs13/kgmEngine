#include "kgmGameNode.h"

kgmGameNode::kgmGameNode(kgmIGame* g)
{
  m_game = g;

  m_valid   = true;
  m_remove  = false;
  m_culled  = false;
  m_visible = true;

  m_position   = vec3(0, 0, 0);
  m_rotation   = vec3(0, 0, 0);
  m_quaternion = quat(0, 0, 0, 1);

  m_birth   = kgmTime::getTicks();
  m_living  = -1;
}

kgmGameNode::~kgmGameNode()
{
  if(m_remove)
    remove();
}

void kgmGameNode::remove()
{
  clear();

  m_remove  = true;
  m_valid   = false;
  m_visible = false;
}

u32 kgmGameNode::timeout()
{
  return (kgmTime::getTicks() - m_birth);
}

