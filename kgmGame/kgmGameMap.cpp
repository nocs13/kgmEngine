#include "kgmGameMap.h"

KGMOBJECT_IMPLEMENT(kgmGameMap, kgmObject);

kgmGameMap::kgmGameMap(kgmIGame* g, OpenType ot)
{
  m_game = g;
  m_type = ot;

  m_xml = null;
  m_mem = null;
}

bool kgmGameMap::open(kgmXml& xml)
{
  if(m_mem)
    return false;

  m_xml = &xml;

  if(m_xml != null && m_xml->m_node == null)
  {
    m_xml->m_node = new kgmXml::Node(null);
    m_xml->m_node->m_name = "kgm";
  }
}

bool kgmGameMap::open(kgmMemory<u8>& mem)
{
  if(m_xml)
    return false;

  m_mem = &mem;
}


bool kgmGameMap::addMesh(Node n)
{
  if(m_type == OpenRead || !n.obj)
    return false;

  if(m_xml && m_xml->m_node->m_name == "kgm")
  {
    kgmXml::Node* node = new kgmXml::Node(m_xml->m_node);

    node->m_name = "kgmMesh";
    node->m_attributes.add(new kgmXml::Attribute("name", n.nam));
    node->m_attributes.add(new kgmXml::Attribute("link", n.lnk));

    kgmXml::Node* snode = new kgmXml::Node(node);
    snode->m_name = "kgmMaterial";
    snode->m_attributes.add(new kgmXml::Attribute("value", n.mtl));

    snode = new kgmXml::Node(node);
    snode->m_name = "kgmPosition";
    snode->m_attributes.add(new kgmXml::Attribute("value", kgmConvert::toString(n.pos.x) + kgmString(" ") +
                                                           kgmConvert::toString(n.pos.y) + kgmString(" ") +
                                                           kgmConvert::toString(n.pos.z) + kgmString(" ")));

    snode = new kgmXml::Node(node);
    snode->m_name = "kgmRotation";
    snode->m_attributes.add(new kgmXml::Attribute("value", kgmConvert::toString(n.rot.x) + kgmString(" ") +
                                                           kgmConvert::toString(n.rot.y) + kgmString(" ") +
                                                           kgmConvert::toString(n.rot.z) + kgmString(" ")));
    if(n.col != 0)
    {
      snode = new kgmXml::Node(node);
      snode->m_name = "Collision";
      snode->m_attributes.add(new kgmXml::Attribute("value", kgmConvert::toString(n.col)));
    }

    if(n.lck)
    {
      snode = new kgmXml::Node(node);
      snode->m_name = "Locked";
      snode->m_attributes.add(new kgmXml::Attribute("value", "true"));
    }
  }
}

bool kgmGameMap::addLight(Node n)
{
  if(m_type == OpenRead || !n.obj)
    return false;

  if(m_xml && m_xml->m_node->m_name == "kgm")
  {
    kgmXml::Node* node = new kgmXml::Node(m_xml->m_node);

    node->m_name = "kgmLight";
    node->m_attributes.add(new kgmXml::Attribute("name", n.nam));

    kgmXml::Node* snode = new kgmXml::Node(node);
    snode->m_name = "Position";
    snode->m_attributes.add(new kgmXml::Attribute("value", kgmConvert::toString(n.pos.x) + kgmString(" ") +
                                                           kgmConvert::toString(n.pos.y) + kgmString(" ") +
                                                           kgmConvert::toString(n.pos.z) + kgmString(" ")));

    snode = new kgmXml::Node(node);
    snode->m_name = "Rotation";
    snode->m_attributes.add(new kgmXml::Attribute("value", kgmConvert::toString(n.rot.x) + kgmString(" ") +
                                                           kgmConvert::toString(n.rot.y) + kgmString(" ") +
                                                           kgmConvert::toString(n.rot.z) + kgmString(" ")));

    if(n.lck)
    {
      snode = new kgmXml::Node(node);
      snode->m_name = "Locked";
      snode->m_attributes.add(new kgmXml::Attribute("value", "true"));
    }
  }
}

bool kgmGameMap::addMaterial(Node n)
{
  if(m_type == OpenRead || !n.obj)
    return false;

  if(m_xml && m_xml->m_node->m_name == "kgm")
  {
    kgmXml::Node* node = new kgmXml::Node(m_xml->m_node);

    node->m_name = "kgmMaterial";
    node->m_attributes.add(new kgmXml::Attribute("name", ((kgmMaterial*)n.obj)->m_id));
  }
}

bool kgmGameMap::addActor(Node n)
{
  if(m_type == OpenRead || !n.obj)
    return false;

}

bool kgmGameMap::addSensor(Node n)
{
  if(m_type == OpenRead || !n.obj)
    return false;

  if(m_xml && m_xml->m_node->m_name == "kgm")
  {
    kgmXml::Node* node = new kgmXml::Node(m_xml->m_node);

    node->m_name = "kgmSensor";
    node->m_attributes.add(new kgmXml::Attribute("name", n.nam));
    node->m_attributes.add(new kgmXml::Attribute("class", ((kgmSensor*)n.obj)->runtime().nClass));
    node->m_attributes.add(new kgmXml::Attribute("target", ((kgmSensor*)n.obj)->getTarget()));

    kgmXml::Node* snode = new kgmXml::Node(node);
    snode->m_name = "Position";
    snode->m_attributes.add(new kgmXml::Attribute("value", kgmConvert::toString(n.pos.x) + kgmString(" ") +
                                                           kgmConvert::toString(n.pos.y) + kgmString(" ") +
                                                           kgmConvert::toString(n.pos.z)));

    snode = new kgmXml::Node(node);
    snode->m_name = "Rotation";
    snode->m_attributes.add(new kgmXml::Attribute("value", kgmConvert::toString(n.rot.x) + kgmString(" ") +
                                                           kgmConvert::toString(n.rot.y) + kgmString(" ") +
                                                           kgmConvert::toString(n.rot.z)));

    if(n.lck)
    {
      snode = new kgmXml::Node(node);
      snode->m_name = "Locked";
      snode->m_attributes.add(new kgmXml::Attribute("value", "true"));
    }
  }
}

bool kgmGameMap::addTrigger(Node n)
{
  if(m_xml && m_xml->m_node->m_name == "kgm")
  {
    kgmXml::Node* node = new kgmXml::Node(m_xml->m_node);

    node->m_name = "kgmTrigger";
    node->m_attributes.add(new kgmXml::Attribute("name", n.nam));
    node->m_attributes.add(new kgmXml::Attribute("channels", kgmConvert::toString((s32) ((kgmTrigger*)n.obj)->getCount())));
    node->m_attributes.add(new kgmXml::Attribute("target", ((kgmTrigger*)n.obj)->getTarget()));

    kgmXml::Node* snode = new kgmXml::Node(node);
    snode->m_name = "Position";
    snode->m_attributes.add(new kgmXml::Attribute("value", kgmConvert::toString(n.pos.x) + kgmString(" ") +
                                                           kgmConvert::toString(n.pos.y) + kgmString(" ") +
                                                           kgmConvert::toString(n.pos.z)));

    snode = new kgmXml::Node(node);
    snode->m_name = "Rotation";
    snode->m_attributes.add(new kgmXml::Attribute("value", kgmConvert::toString(n.rot.x) + kgmString(" ") +
                                                           kgmConvert::toString(n.rot.y) + kgmString(" ") +
                                                           kgmConvert::toString(n.rot.z)));

    if(n.lck)
    {
      snode = new kgmXml::Node(node);
      snode->m_name = "Locked";
      snode->m_attributes.add(new kgmXml::Attribute("value", "true"));
    }
  }
}

bool kgmGameMap::addGameObject(Node n)
{
  if(m_type == OpenRead || !n.obj)
    return false;

  if(m_xml && m_xml->m_node->m_name == "kgm")
  {
    kgmXml::Node* node = new kgmXml::Node(m_xml->m_node);

    node->m_name = "kgmSensor";
    node->m_attributes.add(new kgmXml::Attribute("name", n.nam));
    node->m_attributes.add(new kgmXml::Attribute("class", ((kgmGameObject*)n.obj)->runtime().nClass));

    kgmXml::Node* snode = new kgmXml::Node(node);
    snode->m_name = "Position";
    snode->m_attributes.add(new kgmXml::Attribute("value", kgmConvert::toString(n.pos.x) + kgmString(" ") +
                                                           kgmConvert::toString(n.pos.y) + kgmString(" ") +
                                                           kgmConvert::toString(n.pos.z)));

    snode = new kgmXml::Node(node);
    snode->m_name = "Rotation";
    snode->m_attributes.add(new kgmXml::Attribute("value", kgmConvert::toString(n.rot.x) + kgmString(" ") +
                                                           kgmConvert::toString(n.rot.y) + kgmString(" ") +
                                                           kgmConvert::toString(n.rot.z)));

    for(int i = 0; i < ((kgmGameObject*)n.obj)->m_variables.length(); ++i)
    {
      kgmVariable& var = ((kgmGameObject*)n.obj)->m_variables[i];

      snode = new kgmXml::Node(node);
      snode->m_name = "Parameter";
      snode->m_attributes.add(new kgmXml::Attribute(var.getName(), var.toString()));
    }

    if(n.lck)
    {
      snode = new kgmXml::Node(node);
      snode->m_name = "Locked";
      snode->m_attributes.add(new kgmXml::Attribute("value", "true"));
    }
  }
}

kgmGameMap::Node kgmGameMap::next()
{
  Node node;

  node.obj = null;

  if(m_type == OpenWrite)
    return node;

  return node;
}
