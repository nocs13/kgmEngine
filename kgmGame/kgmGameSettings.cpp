#include "kgmGameSettings.h"
#include "../kgmBase/kgmFile.h"
#include "../kgmBase/kgmArray.h"

// This class provide shared options for configuration.

kgmString kgmGameSettings::m_name = "settings.cfg";

kgmGameSettings::kgmGameSettings()
{
  m_xml = null;
  m_gsid = 0xf0f0;
  m_mouse_camera = true;

  load();
}

kgmGameSettings::kgmGameSettings(const char* path)
{
  m_xml = null;
  m_gsid = 0xf0f0;
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

  kgmArray<u8> m;

  m.alloc(file.length());
  file.read(m, m.length());
  file.close();

  if(m_xml)
    delete m_xml;

  m_xml = new kgmXml(m);
}

void kgmGameSettings::save()
{
  if(!m_xml || !m_xml->m_node)
    return;

  FILE* f = fopen(m_name.data(), "w");

  fprintf(f, "<?xml version='1.0'?>\n");
  fprintf(f, "<kgm>\n");

  for(int i = 0; i < m_xml->m_node->m_nodes.length(); i++)
  {
    kgmXml::Node* node = m_xml->m_node->m_nodes[i];

    if(node)
    {
      kgmString attr;

      if(node->attribute("value", attr))
      {
        fprintf(f, " <%s value='%s' />\n", node->m_name.data(), attr.data());
      }
    }
  }

  fprintf(f, "</kgm>\n");

  fclose(f);
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
  if(m_xml && m_xml->m_node)
  {
    kgmXml::Node* node = m_xml->m_node->node(key);

    if(node)
    {
      for(int i = 0; i < node->attributes(); i++)
      {
        kgmXml::Attribute* attr = node->m_attributes[i];

        if(attr->m_name == kgmString("value"))
        {
          attr->m_data = val;
        }
      }
    }
  }

}

