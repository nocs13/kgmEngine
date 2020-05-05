#include "kgmGameRetention.h"
#include "../kgmSystem/kgmSystem.h"
#include "../kgmBase/kgmFile.h"
#include "../kgmBase/kgmXml.h"

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

  kgmFile f;

  if (!f.open(phome))
    return false;

  kgmMemory<s8> m;

  m.alloc(f.length());

  f.read(m.data(), f.length());

  f.close();

  void* dm = null;
  s32 len = 0;

  len = m_encryptor.encrypt(m.data(), m.length(), &dm);

  if (!dm || !len)
    return false;

  kgmXml x(dm, len);

  kgmXml::Node* root = x.m_node;

  if (root)
  {
    auto i = root->m_nodes.begin();

    while(!i.end())
    {
      if ((*i)->hasattr("key") && (*i)->hasattr("value"))
      {
        kgmString k, v;

        (*i)->attribute("key", k);
        (*i)->attribute("value", v);

        m_data.set(k, v);
      }
    }
  }

  return true;
}

bool kgmGameRetention::keep()
{
  if (m_data.length() < 1)
    return false;

  kgmString x = "<?xml version='1.0'?>\n";

  x += "<kgm>";

  auto i = m_data.begin();

  while(!i.end())
  {

    x += " <Item key='";
    //x +=

  }

  x += "</kgm>";

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

