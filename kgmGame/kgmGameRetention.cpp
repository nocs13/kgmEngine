#include "kgmGameRetention.h"
#include "../kgmSystem/kgmSystem.h"
#include "../kgmBase/kgmFile.h"

kgmGameRetention::kgmGameRetention()
{

}

void kgmGameRetention::name(kgmString n)
{
  m_name = n;
}

void kgmGameRetention::encryptor(s32 x, s32 y, s32 e, s32 d)
{
  m_encryptor.init(x, y, e, d);
}

bool kgmGameRetention::load()
{
  kgmString phome;

  kgmSystem::getHomeDirectory(phome);

  phome = phome + kgmSystem::getPathDelim() + m_name;

  return true;
}

bool kgmGameRetention::keep()
{
  return true;
}

void kgmGameRetention::set(kgmString key, kgmString val)
{
  m_data.set(key, val);
}

kgmString kgmGameRetention::get(kgmString key)
{
  kgmString r;

  auto i = m_data.get(key);

  if (i.end())
    return r;


  return i.data();
}

