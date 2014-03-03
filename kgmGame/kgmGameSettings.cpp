#include "kgmGameSettings.h"
#include "../kgmBase/kgmFile.h"
#include "../kgmBase/kgmMemory.h"

static short g_gsid = 0x0101;
kgmString kgmGameSettings::m_name = "settings.cfg";

kgmGameSettings::kgmGameSettings(){
  m_xml = null;
  m_gsid = 0xf0f0f0f0;
  m_mouse_camera = true;

  load();
}

kgmGameSettings::kgmGameSettings(const char* path)
{
  m_xml = null;
  m_gsid = 0xf0f0f0f0;
  m_mouse_camera = true;

  if(path && strlen(path) < 128)
    m_name = path;
  else
    m_name = "settings.cfg";

  load();
}

kgmGameSettings::~kgmGameSettings()
{
  if(m_xml)
    delete m_xml;
}

void kgmGameSettings::load()
{
  kgmFile file;

  if(!file.open(m_name, kgmFile::Read))
    return;

  kgmMemory<u8> m;

  m.alloc(file.length());
  file.read(m, m.length());

  if(m_xml)
    delete m_xml;

  m_xml = new kgmXml(m);
}

void kgmGameSettings::save()
{
}

kgmString kgmGameSettings::get(char* key)
{
  kgmString val;

  if(m_xml && m_xml->m_node)
  {
    kgmXml::Node* node = m_xml->m_node->node(key);

    if(node)
    {
      node->attribute("value", val);
    }
  }

  return val;
}

void kgmGameSettings::set(char* key, char* val)
{

}

