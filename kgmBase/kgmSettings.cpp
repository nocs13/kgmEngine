#include "kgmSettings.h"
#include "kgmString.h"
#include "kgmFile.h"

#define DEFAULT_NAME "settings.cfg";

kgmSettings::kgmSettings()
{
  m_xml = null;
  m_gsid = 0xf0f0;
  m_mouse_camera = true;

  m_name = DEFAULT_NAME;

  load();
}

kgmSettings::kgmSettings(const char* path)
{
  m_xml = null;
  m_gsid = 0xf0f0;
  m_mouse_camera = true;

  if(path && strlen(path) < 128)
    m_name = path;
  else
    m_name = DEFAULT_NAME;

  load();
}

kgmSettings::~kgmSettings()
{
  if(m_xml)
    kgmObject::Release(m_xml);
}

void kgmSettings::load()
{
  kgmFile file;

  if(!file.open(m_name, kgmFile::Read))
    return;

  kgmArray<u8> m;

  m.alloc(file.length());
  file.read(m, m.length());
  file.close();

  if(m_xml)
    m_xml->release();

  m_xml = new kgmXml(m);
}

void kgmSettings::save()
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

kgmString kgmSettings::get(char* key)
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

void kgmSettings::set(char* key, char* val)
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
