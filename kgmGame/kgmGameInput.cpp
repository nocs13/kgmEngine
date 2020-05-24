#include "kgmGameInput.h"
#include "../kgmBase/kgmEvent.h"

kgmGameInput::kgmGameInput(kgmIGame* g, u8* keys, s32 len)
{
  m_game = g;

  m_keys = keys;

  m_length = len;
}

kgmGameInput::~kgmGameInput()
{
}

s32 kgmGameInput::keyState(u8 key)
{
  if (!m_keys || key >= m_length)
    return -1;

  if (key < 0 || key > KEY_END)
    return -1;

  return m_keys[key];
}

kgmIInput::Point kgmGameInput::pointState(s32 id)
{
  Point pt = {0};

  if (id < 0 || id >= 2)
    return pt;

  pt.x = m_points[id][0];
  pt.y = m_points[id][1];
  pt.z = m_points[id][2];

  return pt;
}

void kgmGameInput::setX(s32 id, s32 x)
{
  if (id < 0 || id >= 2)
    return;

  m_points[id][0] = x;
}

void kgmGameInput::setY(s32 id, s32 y)
{
  if (id < 0 || id >= 2)
    return;

  m_points[id][1] = y;
}

void kgmGameInput::setZ(s32 id, s32 z)
{
  if (id < 0 || id >= 2)
    return;

  m_points[id][2] = z;
}
