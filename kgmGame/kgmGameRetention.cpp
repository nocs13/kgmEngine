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
  kgmString path;

  path = getPath();

  kgmFile f;

  if (!f.open(path))
    return false;

  kgmMemory<s8> m;

  m.alloc(f.length());

  f.read(m.data(), f.length());

  f.close();

  void* dm = null;
  s32 len = 0;

  len = m_encryptor.decrypt(m.data(), m.length(), &dm);

  if (!dm || !len)
    return false;

  kgmString xs((const s8*) dm, len);

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

      if (!i.next())
        break;
    }
  }

  free(dm);

  dm = null;

  return true;
}

bool kgmGameRetention::keep()
{
  kgmString x = "<?xml version='1.0'?>\n";

  x += "<kgm>";

  auto i = m_data.begin();

  while(!i.isEnd())
  {
    x += " <Item key='";
    x += i.key();
    x += "' value='";
    x += i.data();
    x += "' />\n";

    ++i;
  }

  x += "</kgm>";

  void* buf = null;

  s32 len = m_encryptor.encrypt(x.data(), x.length(), &buf);

  if (!buf)
    return false;

  kgmString path = getPath();

  kgmFile f;

  if (!f.open(path, kgmFile::Create | kgmFile::Write))
  {
    free(buf);

    return false;
  }

  f.write(buf, len);

  free(buf);

  f.close();

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

  if (i.isEnd())
    return r;


  return i.data();
}

kgmString kgmGameRetention::getPath()
{
  kgmString path;

  kgmSystem::getHomeDirectory(path);

  path = path + kgmSystem::getPathDelim() + m_name;

  return path;
}
