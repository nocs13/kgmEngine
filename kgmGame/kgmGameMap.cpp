#include "kgmGameMap.h"

#include "../kgmBase/kgmResource.h"

#include "kgmIGame.h"
#include "kgmGameResources.h"

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

    if(n.col)
      node->m_attributes.add(new kgmXml::Attribute("collision", "on"));

    kgmXml::Node* snode = new kgmXml::Node(node);
    snode->m_name = "Material";
    snode->m_attributes.add(new kgmXml::Attribute("value", n.mtl));

    snode = new kgmXml::Node(node);
    snode->m_name = "Shader";
    snode->m_attributes.add(new kgmXml::Attribute("value", n.shd));

    snode = new kgmXml::Node(node);
    snode->m_name = "Position";
    snode->m_attributes.add(new kgmXml::Attribute("value", kgmConvert::toString(n.pos.x) + kgmString(" ") +
                                                  kgmConvert::toString(n.pos.y) + kgmString(" ") +
                                                  kgmConvert::toString(n.pos.z)));

    snode = new kgmXml::Node(node);
    snode->m_name = "Rotation";
    snode->m_attributes.add(new kgmXml::Attribute("value", kgmConvert::toString(n.rot.x) + kgmString(" ") +
                                                  kgmConvert::toString(n.rot.y) + kgmString(" ") +
                                                  kgmConvert::toString(n.rot.z)));

    if(n.col)
      addCollision(*node, n);

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
                                                  kgmConvert::toString(n.pos.z)));

    snode = new kgmXml::Node(node);
    snode->m_name = "Rotation";
    snode->m_attributes.add(new kgmXml::Attribute("value", kgmConvert::toString(n.rot.x) + kgmString(" ") +
                                                  kgmConvert::toString(n.rot.y) + kgmString(" ") +
                                                  kgmConvert::toString(n.rot.z)));

    if(n.col)
      addCollision(*node, n);

    if(n.lck)
    {
      snode = new kgmXml::Node(node);
      snode->m_name = "Locked";
      snode->m_attributes.add(new kgmXml::Attribute("value", "true"));
    }
  }
}

bool kgmGameMap::addCamera(Node n)
{
  if(m_type == OpenRead || !n.obj)
    return false;

  if(m_xml && m_xml->m_node->m_name == "kgm")
  {
    kgmXml::Node* node = new kgmXml::Node(m_xml->m_node);

    vec3 pos, dir;

    pos = ((kgmCamera*)n.obj)->mPos;
    dir = ((kgmCamera*)n.obj)->mDir;

    node->m_name = "kgmCamera";
    node->m_attributes.add(new kgmXml::Attribute("name", n.nam));
    node->m_attributes.add(new kgmXml::Attribute("position", kgmConvert::toString(pos.x) + kgmString(" ") +
                                                  kgmConvert::toString(pos.y) + kgmString(" ") +
                                                  kgmConvert::toString(pos.z)));
    node->m_attributes.add(new kgmXml::Attribute("direction", kgmConvert::toString(dir.x) + kgmString(" ") +
                                                  kgmConvert::toString(dir.y) + kgmString(" ") +
                                                  kgmConvert::toString(dir.z)));
    node->m_attributes.add(new kgmXml::Attribute("fov", kgmConvert::toString(((kgmCamera*)n.obj)->mFov)));

    if(n.col)
      addCollision(*node, n);

    if(n.lck)
    {
      kgmXml::Node* snode = new kgmXml::Node(node);
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

  if(m_xml && m_xml->m_node->m_name == "kgm")
  {
    kgmXml::Node* node = new kgmXml::Node(m_xml->m_node);

    node->m_name = "kgmActor";
    node->m_attributes.add(new kgmXml::Attribute("name", n.nam));
    node->m_attributes.add(new kgmXml::Attribute("class", ((kgmActor*)n.obj)->runtime().nClass));

    if(n.ini.length() > 0)
      node->m_attributes.add(new kgmXml::Attribute("init", n.ini));


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

    if(n.col)
      addCollision(*node, n);

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

    if(n.col)
      addCollision(*node, n);

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

    if(n.col)
      addCollision(*node, n);

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

    node->m_name = "kgmGameObject";
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

    if(n.col)
      addCollision(*node, n);

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

  memset(&node, 0, sizeof(Node));

  if(m_type == OpenWrite)
    return node;


  while(kgmXml::XmlState xstate = m_xml->next())
  {

    kgmString id, value, t;
    kgmString ntype;

    if(xstate == kgmXml::XML_ERROR)
    {
      return node;
    }
    else if(xstate == kgmXml::XML_FINISH)
    {
      return node;
    }

    if(xstate == kgmXml::XML_TAG_OPEN)
    {
      id = m_xml->m_tagName;

      if(id == "kgmCamera")
      {
        ntype = "camera";

        kgmString id, pos, dir, fov;
        m_xml->attribute("name", id);
        m_xml->attribute("position", pos);
        m_xml->attribute("direction", dir);
        m_xml->attribute("fov", fov);

        node.obj = (kgmObject*)new kgmCamera();

        node.nam = id;
        node.bnd = box3(-1, -1, -1, 1, 1, 1);
        ((kgmCamera*)node.obj)->mFov = kgmConvert::toDouble(fov);
        sscanf(pos.data(), "%f %f %f", &((kgmCamera*)node.obj)->mPos.x,
                                       &((kgmCamera*)node.obj)->mPos.y,
                                       &((kgmCamera*)node.obj)->mPos.z);
        sscanf(dir.data(), "%f %f %f", &((kgmCamera*)node.obj)->mDir.x,
                                       &((kgmCamera*)node.obj)->mDir.y,
                                       &((kgmCamera*)node.obj)->mDir.z);

        node.typ = NodeCam;
      }
      else if(id == "kgmLight")
      {
        ntype = "light";

        kgmString id;
        m_xml->attribute("name", id);
        node.obj = new kgmLight();

        node.nam = id;
        node.bnd = box3(-1, -1, -1, 1, 1, 1);

        node.typ = NodeLgt;
      }
      else if(id == "kgmMesh")
      {
        ntype = "mesh";

        kgmString id, ln;
        m_xml->attribute("name", id);
        m_xml->attribute("link", ln);
        kgmMesh* mesh = m_game->getResources()->getMesh(ln);

        if(mesh)
        {
          node.nam = id;
          node.lnk = ln;
          node.obj = mesh;
          node.bnd = mesh->bound();
        }

        node.typ = NodeMsh;
      }
      else if(id == "kgmActor")
      {
        ntype = "actor";

        kgmString id, cls, ini;

        m_xml->attribute("name",  id);
        m_xml->attribute("class", cls);
        m_xml->attribute("init",  ini);

        kgmActor* act = null;

        if(kgmGameObject::g_typ_objects.hasKey(cls))
        {
          kgmGameObject::GenGo fn_new = kgmGameObject::g_typ_objects[cls];

          if(fn_new)
          {
            act = (kgmActor*)fn_new(m_game);

            if(act)
            {
              if(ini.length() > 0)
                kgmGameTools::initActor(m_game, act, ini);

              node.obj = act;
              node.bnd = box3(-1, -1, -1, 1, 1, 1);
              node.nam = id;
              node.ini = ini;
            }
          }
        }

        node.typ = NodeAct;
      }
      else if(id == "kgmSensor")
      {
        ntype = "sensor";

        kgmString id, cls, trg;
        m_xml->attribute("name", id);
        m_xml->attribute("class", cls);
        m_xml->attribute("target", trg);

        kgmSensor* sns = null;

        if(kgmGameObject::g_typ_objects.hasKey(cls))
        {
          kgmGameObject::GenGo fn_new = kgmGameObject::g_typ_objects[cls];

          if(fn_new)
          {
            kgmSensor* sn = (kgmSensor*)fn_new(m_game);

            if(sn)
            {
              sn->setTarget(trg);

              node.obj = sn;
              node.bnd = box3(-1, -1, -1, 1, 1, 1);
              node.nam = id;
            }
          }
        }

        node.typ = NodeSns;
      }
      else if(id == "kgmTrigger")
      {
        ntype = "trigger";

        kgmString id, chn, trg;
        m_xml->attribute("name", id);
        m_xml->attribute("channels", chn);
        m_xml->attribute("target", trg);

        kgmTrigger* tr= new kgmTrigger(m_game);
        tr->setCount(kgmConvert::toInteger(chn));
        tr->setTarget(trg);

        node.obj = tr;
        node.nam = id;
        node.bnd = box3(-1, -1, -1, 1, 1, 1);

        node.typ = NodeTrg;
      }
      else if(id == "kgmGameObject")
      {
        ntype = "gobject";

        kgmString id, cls;
        m_xml->attribute("name", id);
        m_xml->attribute("class", cls);

        if(kgmGameObject::g_typ_objects.hasKey(cls))
        {
          kgmGameObject::GenGo fn_new = kgmGameObject::g_typ_objects[cls];

          if(fn_new)
          {
            kgmGameObject* go = (kgmGameObject*)fn_new(m_game);

            if(go)
            {
              node.obj = go;
              node.bnd = box3(-1, -1, -1, 1, 1, 1);
              node.nam = id;
            }
          }
        }

        node.typ = NodeObj;
      }
    }
    else if(xstate == kgmXml::XML_TAG_CLOSE)
    {
      id = m_xml->m_tagName;

      if(id == "Position")
      {
        vec3 v;
        m_xml->attribute("value", value);
        sscanf(value.data(), "%f %f %f", &v.x, &v.y, &v.z);

        node.pos = v;
      }
      else if(id == "Rotation")
      {
        vec3 v;
        m_xml->attribute("value", value);
        sscanf(value.data(), "%f %f %f", &v.x, &v.y, &v.z);

        node.rot = v;
      }
      else if(id == "Direction")
      {
        vec3 v;
        m_xml->attribute("value", value);
        sscanf(value.data(), "%f %f %f", &v.x, &v.y, &v.z);

        if(ntype == "camera")
        {
        }
      }
      else if(id == "Material")
      {
        value.clear();
        m_xml->attribute("value", value);

        if(value.length())
          node.mtl = value;
      }
      else if(id == "Shader")
      {
        value.clear();
        m_xml->attribute("value", value);

        if(value.length())
          node.shd = value;
      }
      else if(id == "Collision")
      {
        value.clear();
        m_xml->attribute("value", value);

        if(value == "true")
          node.col = true;

        value.clear();
        m_xml->attribute("shape", value);
        node.shp = value;

        float dim[3] = {0};
        value.clear();
        m_xml->attribute("bound", value);
        sscanf(value.data(), "%f %f %f", &dim[0], &dim[1], &dim[2]);

        node.bnd.min.x = -0.5 * dim[0];
        node.bnd.min.y = -0.5 * dim[1];
        node.bnd.min.z = -0.5 * dim[2];
        node.bnd.max.x =  0.5 * dim[0];
        node.bnd.max.y =  0.5 * dim[1];
        node.bnd.max.z =  0.5 * dim[2];
      }
      else if(id == "Locked")
      {
        value.clear();
        m_xml->attribute("value", value);

        if(value == "true")
          node.lck = true;
      }
      else if(id == "Parameter")
      {
        kgmString name, type, value;

        m_xml->attribute("name",  name);
        m_xml->attribute("type",  type);
        m_xml->attribute("value", value);

        if(node.obj)
          ((kgmGameObject*)node.obj)->setParameter(name, value);
      }
      else if(id == "kgmCamera")
      {
        ntype = "camera";

        kgmString id, pos, dir, fov;
        m_xml->attribute("name", id);
        m_xml->attribute("position", pos);
        m_xml->attribute("direction", dir);
        m_xml->attribute("fov", fov);

        node.obj = (kgmObject*)new kgmCamera();

        node.nam = id;
        node.bnd = box3(-1, -1, -1, 1, 1, 1);
        ((kgmCamera*)node.obj)->mFov = kgmConvert::toDouble(fov);
        sscanf(pos.data(), "%f %f %f", &((kgmCamera*)node.obj)->mPos.x,
                                       &((kgmCamera*)node.obj)->mPos.y,
                                       &((kgmCamera*)node.obj)->mPos.z);
        sscanf(dir.data(), "%f %f %f", &((kgmCamera*)node.obj)->mDir.x,
                                       &((kgmCamera*)node.obj)->mDir.y,
                                       &((kgmCamera*)node.obj)->mDir.z);

        node.typ = NodeCam;

        break;
      }
      else if((id == "kgmMesh") || (id == "kgmLight") || (id == "kgmMaterial") ||
              (id == "kgmActor") || (id == "kgmSensor") || (id == "kgmTrigger") ||
              (id == "kgmGameObject"))
      {
        break;
      }
    }
    else if(xstate == kgmXml::XML_TAG_DATA)
    {
    }
  }

  return node;
}

void kgmGameMap::addCollision(kgmXml::Node& xnode, Node& node)
{
  if(node.col != 0)
  {
    kgmXml::Node* snode = new kgmXml::Node(&xnode);
    snode->m_name = "Collision";
    snode->m_attributes.add(new kgmXml::Attribute("value", kgmConvert::toString(node.col)));

    kgmString shape = node.shp;

    if(shape.length() < 1)
    {
      shape = "box";
    }

    snode->m_attributes.add(new kgmXml::Attribute("shape", shape));

    kgmString bound = kgmString("") + kgmConvert::toString(node.bnd.max.x - node.bnd.min.x) +
      kgmString(" ") + kgmConvert::toString(node.bnd.max.y - node.bnd.min.y) +
      kgmString(" ") + kgmConvert::toString(node.bnd.max.z - node.bnd.min.z);
    
    snode->m_attributes.add(new kgmXml::Attribute("bound", bound));
  }
}
