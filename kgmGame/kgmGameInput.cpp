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

void kgmGameInput::clear()
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


