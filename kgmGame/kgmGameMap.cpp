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

bool kgmGameMap::addLight(Node n)
{
  if(m_type == OpenRead || !n.obj)
    return false;

  if(m_xml && m_xml->m_node->m_name == "kgm")
  {
    kgmXml::Node* node = new kgmXml::Node(m_xml->m_node);

    node->m_name = "kgmLight";
    node->m_attributes.add(new kgmXml::Attribute("name", n.nam));

    addPosition(*node, n.pos);
    addRotation(*node, n.rot);

    if(n.lck)
      addLocked(*node, n.lck);
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
    node->m_attributes.add(new kgmXml::Attribute("fov", kgmConvert::toString(((kgmCamera*)n.obj)->mFov)));

    addPosition(*node, pos);
    addRotation(*node, dir);

    if(n.col)
      addCollision(*node, n);

    if(n.lck)
      addLocked(*node, n.lck);
  }
}

bool kgmGameMap::addVisual(Node n)
{
  if(m_type == OpenRead || !n.obj)
    return false;

  if(m_xml && m_xml->m_node->m_name == "kgm")
  {
    kgmXml::Node* node = new kgmXml::Node(m_xml->m_node);
    kgmXml::Node* snode = null;

    node->m_name = "kgmVisual";
    node->m_attributes.add(new kgmXml::Attribute("name", n.nam));

    kgmVisual* vis = (kgmVisual*)n.obj;

    switch(vis->type())
    {
    case kgmVisual::TypeMesh:
      snode = new kgmXml::Node(node);
      snode->m_name = "Mesh";
      snode->m_attributes.add(new kgmXml::Attribute("id", vis->getMesh()->getMesh()->m_id));
      break;
    case kgmVisual::TypeParticles:
      snode = new kgmXml::Node(node);
      snode->m_name = "Particles";
      snode->m_attributes.add(new kgmXml::Attribute("count", kgmConvert::toString((s32)vis->getParticles()->count())));
      snode->m_attributes.add(new kgmXml::Attribute("loop", kgmConvert::toString((s32)vis->getParticles()->loop())));
      snode->m_attributes.add(new kgmXml::Attribute("fade", kgmConvert::toString((s32)vis->getParticles()->fade())));
      snode->m_attributes.add(new kgmXml::Attribute("angle", kgmConvert::toString((s32)vis->getParticles()->angle())));
      snode->m_attributes.add(new kgmXml::Attribute("speed", kgmConvert::toString((s32)vis->getParticles()->speed())));
      snode->m_attributes.add(new kgmXml::Attribute("divspeed", kgmConvert::toString((s32)vis->getParticles()->divspeed())));
      snode->m_attributes.add(new kgmXml::Attribute("life", kgmConvert::toString((s32)vis->getParticles()->life())));
      snode->m_attributes.add(new kgmXml::Attribute("divlife", kgmConvert::toString((s32)vis->getParticles()->divlife())));
      snode->m_attributes.add(new kgmXml::Attribute("size", kgmConvert::toString((s32)vis->getParticles()->size())));
      snode->m_attributes.add(new kgmXml::Attribute("esize", kgmConvert::toString((s32)vis->getParticles()->esize())));
      break;
    }

    kgmMaterial* mtl = vis->getMaterial();

    if(mtl)
    {
      snode = new kgmXml::Node(node);
      snode->m_name = "Material";
      snode->m_attributes.add(new kgmXml::Attribute("id", mtl->name()));
    }

    addPosition(*node, n.pos);
    addRotation(*node, n.rot);

    if(n.lck)
      addLocked(*node, n.lck);
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
    node->m_attributes.add(new kgmXml::Attribute("actor", n.ini));

    if(((kgmActor*)n.obj)->m_gameplayer)
    {
      node->m_attributes.add(new kgmXml::Attribute("player", "on"));
    }

    addPosition(*node, n.pos);
    addRotation(*node, n.rot);

    if(n.col)
      addCollision(*node, n);

    if(n.lck)
      addLocked(*node, n.lck);

    addParameters(*node, ((kgmUnit*)n.obj)->m_variables);
  }
}

bool kgmGameMap::addEffect(Node n)
{
  if(m_type == OpenRead || !n.obj)
    return false;

  if(m_xml && m_xml->m_node->m_name == "kgm")
  {
    kgmXml::Node* node = new kgmXml::Node(m_xml->m_node);

    node->m_name = "kgmEffect";
    node->m_attributes.add(new kgmXml::Attribute("name", n.nam));
    node->m_attributes.add(new kgmXml::Attribute("class", ((kgmUnit*)n.obj)->runtime().nClass));

    addPosition(*node, n.pos);
    addRotation(*node, n.rot);

    if(n.col)
      addCollision(*node, n);

    if(n.lck)
      addLocked(*node, n.lck);

    addParameters(*node, ((kgmUnit*)n.obj)->m_variables);
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

    addPosition(*node, n.pos);
    addRotation(*node, n.rot);

    if(n.col)
      addCollision(*node, n);

    if(n.lck)
      addLocked(*node, n.lck);

    addParameters(*node, ((kgmUnit*)n.obj)->m_variables);
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

    addPosition(*node, n.pos);
    addRotation(*node, n.rot);

    if(n.col)
      addCollision(*node, n);

    if(n.lck)
      addLocked(*node, n.lck);

    addParameters(*node, ((kgmUnit*)n.obj)->m_variables);
  }
}

bool kgmGameMap::addUnit(Node n)
{
  if(m_type == OpenRead || !n.obj)
    return false;

  if(m_xml && m_xml->m_node->m_name == "kgm")
  {
    kgmXml::Node* node = new kgmXml::Node(m_xml->m_node);

    node->m_name = "kgmUnit";
    node->m_attributes.add(new kgmXml::Attribute("name", n.nam));
    node->m_attributes.add(new kgmXml::Attribute("class", ((kgmUnit*)n.obj)->runtime().nClass));

    addPosition(*node, n.pos);
    addRotation(*node, n.rot);

    if(n.col)
      addCollision(*node, n);

    if(n.lck)
      addLocked(*node, n.lck);

    if(((kgmUnit*)n.obj)->visual())
    {
      kgmUnit* unt = (kgmUnit*)n.obj;

      kgmXml::Node* vnode = new kgmXml::Node(node);
      vnode->m_name = "Visual";

      if(unt->visual()->getMaterial())
        vnode->m_attributes.add(new kgmXml::Attribute("material", unt->visual()->getMaterial()->name()));

      if(unt->visual()->getMesh())
        vnode->m_attributes.add(new kgmXml::Attribute("mesh", unt->visual()->getMesh()->getMesh()->id()));
    }

    addParameters(*node, ((kgmUnit*)n.obj)->m_variables);
  }
}

bool kgmGameMap::addObstacle(Node n)
{
  if(m_type == OpenRead || !n.obj)
    return false;

  if(!m_xml || m_xml->m_node->m_name != "kgm")
    return false;

  kgmXml::Node* node = new kgmXml::Node(m_xml->m_node);

  node->m_name = "kgmObstacle";
  node->m_attributes.add(new kgmXml::Attribute("name", n.nam));


  mtx4 tr = ((kgmObstacle*)n.obj)->getTransfom();
  vec3 pos(0,0,0);
  pos = tr * pos;
  quat qt;
  tr.quaternion(qt);
  float scale = ((kgmObstacle*)n.obj)->getScale();

  addPosition(*node, pos);
  addQuaternion(*node, qt);

  kgmXml::Node* snode = new kgmXml::Node(node);
  snode->m_name = "Scale";
  snode->m_attributes.add(new kgmXml::Attribute("value", kgmConvert::toString(scale)));

  if(n.lck)
    addLocked(*node, n.lck);

  for(u32 i = 0; i < ((kgmObstacle*)n.obj)->length(); i++)
  {
    triangle3 tr = ((kgmObstacle*)n.obj)->getTriangle(i);

    addTriangle(*node, tr);
  }

  return true;
}

bool kgmGameMap::addMaterial(Node n)
{
  if(m_type == OpenRead || !n.obj)
    return false;

  if(!m_xml || m_xml->m_node->m_name != "kgm")
    return false;

  kgmMaterial* mtl = (kgmMaterial*)n.obj;

  kgmXml::Node* node = new kgmXml::Node(m_xml->m_node);

  node->m_name = "kgmMaterial";
  node->m_attributes.add(new kgmXml::Attribute("name", n.nam));

  kgmXml::Node* snode = new kgmXml::Node(node);
  snode->m_name = "Shininess";
  snode->m_attributes.add(new kgmXml::Attribute("value", kgmConvert::toString(mtl->shininess())));

  snode = new kgmXml::Node(node);
  snode->m_name = "Transparency";
  snode->m_attributes.add(new kgmXml::Attribute("value", kgmConvert::toString(mtl->transparency())));

  if(mtl->alpha())
  {
    snode = new kgmXml::Node(node);
    snode->m_name = "Alpha";
    snode->m_attributes.add(new kgmXml::Attribute("value", "true"));
  }

  if(!mtl->depth())
  {
    snode = new kgmXml::Node(node);
    snode->m_name = "Depth";
    snode->m_attributes.add(new kgmXml::Attribute("value", "false"));
  }

  if(!mtl->shade())
  {
    snode = new kgmXml::Node(node);
    snode->m_name = "Shade";
    snode->m_attributes.add(new kgmXml::Attribute("value", "false"));
  }

  if(!mtl->cull())
  {
    snode = new kgmXml::Node(node);
    snode->m_name = "Cull";
    snode->m_attributes.add(new kgmXml::Attribute("value", "false"));
  }

  if(mtl->blend())
  {
    snode = new kgmXml::Node(node);
    snode->m_name = "Blending";
    snode->m_attributes.add(new kgmXml::Attribute("source",
                            kgmMaterial::blendToString(mtl->srcblend())));
    snode->m_attributes.add(new kgmXml::Attribute("destination",
                            kgmMaterial::blendToString(mtl->dstblend())));
  }

  if(mtl->getShader())
  {
    snode = new kgmXml::Node(node);
    snode->m_name = "Shader";
    snode->m_attributes.add(new kgmXml::Attribute("id", mtl->getShader()->m_id));
  }

  if(mtl->hasTexColor())
  {
    snode = new kgmXml::Node(node);
    snode->m_name = "Texture";
    snode->m_attributes.add(new kgmXml::Attribute("type", "color"));
    snode->m_attributes.add(new kgmXml::Attribute("id", mtl->getTexColor()->m_id));
  }

  if(mtl->hasTexNormal())
  {
    snode = new kgmXml::Node(node);
    snode->m_name = "Texture";
    snode->m_attributes.add(new kgmXml::Attribute("type", "normal"));
    snode->m_attributes.add(new kgmXml::Attribute("id", mtl->getTexNormal()->m_id));
  }

  if(mtl->hasTexSpecular())
  {
    snode = new kgmXml::Node(node);
    snode->m_name = "Texture";
    snode->m_attributes.add(new kgmXml::Attribute("type", "specular"));
    snode->m_attributes.add(new kgmXml::Attribute("id", mtl->getTexSpecular()->m_id));
  }

  return true;
}

kgmGameMap::Node kgmGameMap::next()
{
  Node node;

  bool closed = true;

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

        kgmString id, fov;
        m_xml->attribute("name", id);
        m_xml->attribute("fov",  fov);

        node.nam = id;
        node.fov = kgmConvert::toDouble(fov);

        node.typ = NodeCam;
        closed = false;
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
        closed = false;
      }
      else if(id == "kgmVisual")
      {
        ntype = "visual";

        kgmString id, ln;
        m_xml->attribute("name", id);

        kgmVisual* vis = new kgmVisual();

        node.nam = id;
        node.obj = vis;

        node.typ = NodeVis;
        closed = false;
      }
      else if(id == "kgmUnit")
      {
        ntype = "gobject";

        kgmString id, cls;
        m_xml->attribute("name", id);
        m_xml->attribute("class", cls);

        if(kgmUnit::g_typ_objects.hasKey(cls))
        {
          kgmUnit::Generate fn_new = kgmUnit::g_typ_objects[cls];

          if(fn_new)
          {
            kgmUnit* go = (kgmUnit*)fn_new(m_game);

            if(go)
            {
              node.obj = go;
              node.bnd = box3(-1, -1, -1, 1, 1, 1);
              node.nam = id;
            }
          }
        }
        else
        {
          node.obj = new kgmUnit(kgmIGame::getGame());
          node.bnd = box3(-1, -1, -1, 1, 1, 1);
          node.nam = id;
        }

        node.typ = NodeUnt;
        closed = false;
      }
      else if(id == "kgmActor")
      {
        ntype = "actor";

        kgmString id, plr, ini;

        m_xml->attribute("name", id);
        m_xml->attribute("actor", ini);
        m_xml->attribute("player", plr);

        kgmActor* act = new kgmActor(m_game);

        kgmGameTools::initActor(m_game, act, ini);

        node.obj = act;
        node.bnd = box3(-1, -1, -1, 1, 1, 1);
        node.nam = id;
        node.ini = ini;

        if(plr == "on")
          act->m_gameplayer = true;

        node.typ = NodeAct;
        closed = false;
      }
      else if(id == "kgmEffect")
      {
        ntype = "effect";

        kgmString id, cls;
        m_xml->attribute("name", id);
        m_xml->attribute("class", cls);

        if(kgmUnit::g_typ_objects.hasKey(cls))
        {
          kgmUnit::Generate fn_new = kgmUnit::g_typ_objects[cls];

          if(fn_new)
          {
            kgmEffect* go = (kgmEffect*)fn_new(m_game);

            if(go)
            {
              node.obj = go;
              node.bnd = box3(-1, -1, -1, 1, 1, 1);
              node.nam = id;
            }
          }
        }

        node.typ = NodeUnt;
        closed = false;
      }
      else if(id == "kgmSensor")
      {
        ntype = "sensor";

        kgmString id, cls, trg;
        m_xml->attribute("name", id);
        m_xml->attribute("class", cls);
        m_xml->attribute("target", trg);

        kgmSensor* sns = null;

        if(kgmUnit::g_typ_objects.hasKey(cls))
        {
          kgmUnit::Generate fn_new = kgmUnit::g_typ_objects[cls];

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
        closed = false;
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
        closed = false;
      }
      else if(id == "kgmObstacle")
      {
        ntype = "obstacle";

        kgmString id;
        m_xml->attribute("name", id);

        kgmObstacle* obs= new kgmObstacle();

        node.obj = obs;
        node.nam = id;

        node.typ = NodeObs;
        closed = false;
      }
      else if(id == "kgmMaterial")
      {
        ntype = "material";

        kgmString id;
        m_xml->attribute("name", id);

        kgmMaterial* mtl= new kgmMaterial();
        mtl->name(id);

        node.obj = mtl;
        node.nam = id;

        node.typ = NodeMtl;
        closed = false;
      }
    }
    else if(xstate == kgmXml::XML_TAG_CLOSE)
    {
      id = m_xml->m_tagName;

      if (closed)
        continue;

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

        node.rot = v;
      }
      else if(id == "Quaternion")
      {
        quat q;
        m_xml->attribute("value", value);
        sscanf(value.data(), "%f %f %f %f", &q.x, &q.y, &q.z, &q.w);

        q.angles(node.rot);
      }
      else if(id == "Mesh")
      {
        kgmString link;

        m_xml->attribute("id", link);

        if(node.typ == NodeVis && ((kgmVisual*)node.obj)->type() == kgmVisual::TypeNone)
        {
          ((kgmVisual*)node.obj)->set(m_game->getResources()->getMesh(link));
        }
      }
      else if(id == "Particles")
      {
        kgmString value;

        if(node.typ == NodeVis && ((kgmVisual*)node.obj)->type() == kgmVisual::TypeNone)
        {
          kgmParticles* pts = new kgmParticles();
          m_xml->attribute("count", value); pts->count(kgmConvert::toInteger(value));
          m_xml->attribute("fade", value); pts->fade(kgmConvert::toInteger(value));
          m_xml->attribute("loop", value); pts->loop(kgmConvert::toInteger(value));
          m_xml->attribute("angle", value); pts->angle(kgmConvert::toDouble(value));
          m_xml->attribute("speed", value); pts->speed(kgmConvert::toDouble(value));
          m_xml->attribute("divspeed", value); pts->divspeed(kgmConvert::toDouble(value));
          m_xml->attribute("life", value); pts->life(kgmConvert::toDouble(value));
          m_xml->attribute("divlife", value); pts->divlife(kgmConvert::toDouble(value));
          m_xml->attribute("size", value); pts->size(kgmConvert::toDouble(value));
          m_xml->attribute("esize", value); pts->esize(kgmConvert::toDouble(value));
          pts->build();
          ((kgmVisual*)node.obj)->set(pts);
        }
      }
      else if(id == "Shader")
      {
        kgmString id;

        m_xml->attribute("id", id);

        ((kgmMaterial*)node.obj)->setShader(kgmIGame::getGame()->getResources()->getShader(id));
      }
      else if(id == "Texture")
      {
        kgmString type, id;

        m_xml->attribute("id", id);
        m_xml->attribute("type", type);

        if(type == "color")
          ((kgmMaterial*)node.obj)->setTexColor(kgmIGame::getGame()->getResources()->getTexture(id));

        if(type == "normal")
          ((kgmMaterial*)node.obj)->setTexNormal(kgmIGame::getGame()->getResources()->getTexture(id));

        if(type == "specular")
          ((kgmMaterial*)node.obj)->setTexSpecular(kgmIGame::getGame()->getResources()->getTexture(id));
      }
      else if(id == "Shininess")
      {
        kgmString value;

        m_xml->attribute("value", value);

        ((kgmMaterial*)node.obj)->shininess(kgmConvert::toDouble(value));
      }
      else if(id == "Transparency")
      {
        kgmString value;

        m_xml->attribute("value", value);

        ((kgmMaterial*)node.obj)->transparency(kgmConvert::toDouble(value));
      }
      else if(id == "Cull")
      {
        kgmString value;

        m_xml->attribute("value", value);

        bool s = (value == "false") ? (false) : (true);

        ((kgmMaterial*)node.obj)->cull(s);
      }
      else if(id == "Shade")
      {
        kgmString value;

        m_xml->attribute("value", value);

        bool s = (value == "false") ? (false) : (true);

        ((kgmMaterial*)node.obj)->shade(s);
      }
      else if(id == "Depth")
      {
        kgmString value;

        m_xml->attribute("value", value);

        bool s = (value == "false") ? (false) : (true);

        ((kgmMaterial*)node.obj)->depth(s);
      }
      else if(id == "Alpha")
      {
        kgmString value;

        m_xml->attribute("value", value);

        bool s = (value == "false") ? (false) : (true);

        ((kgmMaterial*)node.obj)->alpha(s);
      }
      else if(id == "Blending")
      {
        kgmString src, dst;

        m_xml->attribute("source", src);
        m_xml->attribute("destination", dst);

        ((kgmMaterial*)node.obj)->blend(true);
        ((kgmMaterial*)node.obj)->srcblend(kgmMaterial::stringToBlend(src));
        ((kgmMaterial*)node.obj)->dstblend(kgmMaterial::stringToBlend(dst));
      }
      else if(id == "Material")
      {
        kgmString value;

        m_xml->attribute("id", value);

        node.mtl = value;
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
          ((kgmUnit*)node.obj)->setParameter(name, value);
      }
      else if(id == "Scale")
      {
        value.clear();
        m_xml->attribute("value", value);

        if(node.typ == NodeObs)
          ((kgmObstacle*)node.obj)->set(kgmConvert::toDouble(value));
      }
      else if(id == "Triangle")
      {
        kgmString ta, tb, tc;
        vec3 va, vb, vc;

        m_xml->attribute("a", ta);
        m_xml->attribute("b", tb);
        m_xml->attribute("c", tc);

        sscanf(ta.data(), "%f %f %f", &va.x, &va.y, &va.z);
        sscanf(tb.data(), "%f %f %f", &vb.x, &vb.y, &vb.z);
        sscanf(tc.data(), "%f %f %f", &vc.x, &vc.y, &vc.z);

        if(node.obj)
          ((kgmObstacle*)node.obj)->add(va, vb, vc);
      }
      else if(id == "Visual")
      {
        kgmString value;

        m_xml->attribute("material", value);

        if(node.obj && value.length() > 0)
        {

        }

        m_xml->attribute("mesh", value);

        if(node.obj && value.length() > 0)
        {
          if(!((kgmUnit*)node.obj)->visual())
            ((kgmUnit*)node.obj)->visual(new kgmVisual());

          ((kgmUnit*)node.obj)->visual()->set(kgmIGame::getGame()->getResources()->getMesh(value));
        }
      }
      else if((id == "kgmVisual") || (id == "kgmLight") || (id == "kgmEffect")  ||
              (id == "kgmActor") || (id == "kgmSensor") || (id == "kgmTrigger") ||
              (id == "kgmUnit") || (id == "kgmCamera") || (id == "kgmObstacle") ||
              (id == "kgmMaterial"))
      {
        closed = true;

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

void kgmGameMap::addName(kgmXml::Node& xnode, kgmString& name)
{
  kgmXml::Node* snode = new kgmXml::Node(&xnode);
  snode->m_name = "Name";
  snode->m_attributes.add(new kgmXml::Attribute("value", name));
}

void kgmGameMap::addLocked(kgmXml::Node& xnode, bool lock)
{
  if(!lock)
    return;

  kgmXml::Node* snode = new kgmXml::Node(&xnode);
  snode->m_name = "Locked";
  snode->m_attributes.add(new kgmXml::Attribute("value", "true"));
}

void kgmGameMap::addPosition(kgmXml::Node& xnode, vec3& pos)
{
  kgmXml::Node* snode = new kgmXml::Node(&xnode);
  snode->m_name = "Position";
  snode->m_attributes.add(new kgmXml::Attribute("value", kgmConvert::toString(pos.x) + kgmString(" ") +
                                                         kgmConvert::toString(pos.y) + kgmString(" ") +
                                                         kgmConvert::toString(pos.z)));
}

void kgmGameMap::addRotation(kgmXml::Node& xnode, vec3& rot)
{
  kgmXml::Node* snode = new kgmXml::Node(&xnode);
  snode->m_name = "Rotation";
  snode->m_attributes.add(new kgmXml::Attribute("value", kgmConvert::toString(rot.x) + kgmString(" ") +
                                                         kgmConvert::toString(rot.y) + kgmString(" ") +
                                                         kgmConvert::toString(rot.z)));

}

void kgmGameMap::addQuaternion(kgmXml::Node& xnode, quat& qt)
{
  kgmXml::Node* snode = new kgmXml::Node(&xnode);
  snode->m_name = "Quaternion";
  snode->m_attributes.add(new kgmXml::Attribute("value", kgmConvert::toString(qt.x) + kgmString(" ") +
                                                         kgmConvert::toString(qt.y) + kgmString(" ") +
                                                         kgmConvert::toString(qt.z) + kgmString(" ") +
                                                         kgmConvert::toString(qt.w)));
}

void kgmGameMap::addTriangle(kgmXml::Node &xnode, triangle3 &tr)
{
  kgmXml::Node* snode = new kgmXml::Node(&xnode);
  snode->m_name = "Triangle";
  snode->m_attributes.add(new kgmXml::Attribute("a", kgmConvert::toString(tr.pt[0].x) + kgmString(" ") +
                                                     kgmConvert::toString(tr.pt[0].y) + kgmString(" ") +
                                                     kgmConvert::toString(tr.pt[0].z)));
  snode->m_attributes.add(new kgmXml::Attribute("b", kgmConvert::toString(tr.pt[1].x) + kgmString(" ") +
                                                     kgmConvert::toString(tr.pt[1].y) + kgmString(" ") +
                                                     kgmConvert::toString(tr.pt[1].z)));
  snode->m_attributes.add(new kgmXml::Attribute("c", kgmConvert::toString(tr.pt[2].x) + kgmString(" ") +
                                                     kgmConvert::toString(tr.pt[2].y) + kgmString(" ") +
                                                     kgmConvert::toString(tr.pt[2].z)));
}

void kgmGameMap::addParameter(kgmXml::Node& xnode, kgmVariable& param)
{
  kgmXml::Node* snode = new kgmXml::Node(&xnode);
  snode->m_name = "Parameter";
  snode->m_attributes.add(new kgmXml::Attribute("name", param.getName()));
  snode->m_attributes.add(new kgmXml::Attribute("value", param.toString()));
}

void kgmGameMap::addParameters(kgmXml::Node& xnode, kgmList<kgmVariable>& params)
{
  for(int i = 0; i < params.length(); i++)
  {
    kgmVariable& param = params[i];
    addParameter(xnode, param);
  }
}
