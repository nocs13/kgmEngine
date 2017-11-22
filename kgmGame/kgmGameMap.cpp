#include "kgmGameMap.h"

#include "../kgmBase/kgmResource.h"

#include "kgmIGame.h"
#include "kgmILogic.h"
#include "kgmGameResources.h"

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

  return true;
}

bool kgmGameMap::open(kgmMemory<u8>& mem)
{
  if(m_xml)
    return false;

  m_mem = &mem;

  return true;
}

bool kgmGameMap::save(kgmString path)
{
  kgmXml     xml;

  if(m_type != OpenWrite)
    return false;

  open(xml);

  kgmList<kgmUnit*> units;
  kgmList<kgmActor*> actors;
  kgmList<kgmEffect*> effects;
  kgmList<kgmSensor*> sensors;
  kgmList<kgmTrigger*> triggers;

  kgmList<kgmUnit*> nodes;

  m_game->getLogic()->getObjects(nodes);

  for(kgmList<kgmUnit*>::iterator i = nodes.begin(); i != nodes.end(); ++i)
  {
    if((*i)->isClass("kgmUnit"))
      units.add((kgmUnit*) (*i));
    else if((*i)->isClass("kgmActor"))
      actors.add((kgmActor*) (*i));
    else if((*i)->isClass("kgmEffect"))
      effects.add((kgmEffect*) (*i));
    else if((*i)->isClass("kgmSensor"))
      sensors.add((kgmSensor*) (*i));
    else if((*i)->isClass("kgmTrigger"))
      triggers.add((kgmTrigger*) (*i));
  }

  for(kgmList<kgmUnit*>::iterator i = units.begin(); i != units.end(); ++i) {
    switch((*i)->getType())
    {
    case kgmUnit::Light:
      addLight(*i);
      break;
    case kgmUnit::Camera:
      addCamera(*i);
      break;
    case kgmUnit::Visual:
      addVisual(*i);
      break;
    default:
      addUnit(*i);
    }
  }

  for(kgmList<kgmActor*>::iterator i = actors.begin(); i != actors.end(); ++i)
    addActor(*i);

  for(kgmList<kgmEffect*>::iterator i = effects.begin(); i != effects.end(); ++i)
    addEffect(*i);

  for(kgmList<kgmSensor*>::iterator i = sensors.begin(); i != sensors.end(); ++i)
    addSensor(*i);

  for(kgmList<kgmTrigger*>::iterator i = triggers.begin(); i != triggers.end(); ++i)
    addTrigger(*i);

  triggers.clear();
  sensors.clear();
  effects.clear();
  actors.clear();
  units.clear();

  FILE* f = fopen(path.data(), "w");

  if(!f) {
    return false;
  }

  kgmString str;

  if(xml.toString(str) > 1) {
    fprintf(f, "%s", str.data());
  }

  fclose(f);

  return true;
}

bool kgmGameMap::addLight(kgmUnit* n)
{
  if(m_type == OpenRead || !n)
    return false;

  if(m_xml && m_xml->m_node->m_name == "kgm")
  {
    kgmXml::Node* node = new kgmXml::Node(m_xml->m_node);

    node->m_name = "Light";
    node->m_attributes.add(new kgmXml::Attribute("name", n->getName()));

    addPosition(*node, n->position());
    addRotation(*node, n->rotation());

    if(n->lock())
      addLocked(*node, n->lock());
  }

  return true;
}

bool kgmGameMap::addCamera(kgmUnit* n)
{
  if(m_type == OpenRead || !n)
    return false;

  if(m_xml && m_xml->m_node->m_name == "kgm")
  {
    kgmXml::Node* node = new kgmXml::Node(m_xml->m_node);

    vec3 pos, dir;

    pos = n->camera()->mPos;
    dir = n->camera()->mDir;

    node->m_name = "Camera";
    node->m_attributes.add(new kgmXml::Attribute("name", n->getName()));
    node->m_attributes.add(new kgmXml::Attribute("fov", kgmConvert::toString(n->camera()->mFov)));

    addPosition(*node, pos);
    addRotation(*node, dir);

    if(n->body())
      addCollision(*node, n);

    if(n->lock())
      addLocked(*node, n->lock());
  }

  return true;
}

bool kgmGameMap::addVisual(kgmUnit* n)
{
  if(m_type == OpenRead || !n)
    return false;

  if(m_xml && m_xml->m_node->m_name == "kgm")
  {
    kgmXml::Node* node = new kgmXml::Node(m_xml->m_node);
    //kgmXml::Node* snode = null;

    node->m_name = "Visual";
    node->m_attributes.add(new kgmXml::Attribute("name", n->getName()));

    kgmVisual* vis = n->visual();

    switch(vis->type())
    {
    case kgmVisual::TypeMesh:
      node->m_attributes.add(new kgmXml::Attribute("type", "mesh"));
      node->m_attributes.add(new kgmXml::Attribute("id", vis->getMesh()->m_id));
      break;
    case kgmVisual::TypeParticles:
      node->m_attributes.add(new kgmXml::Attribute("type", "particles"));
      node->m_attributes.add(new kgmXml::Attribute("count", kgmConvert::toString((s32)vis->getParticles()->count())));
      node->m_attributes.add(new kgmXml::Attribute("loop", kgmConvert::toString((s32)vis->getParticles()->loop())));
      node->m_attributes.add(new kgmXml::Attribute("fade", kgmConvert::toString((s32)vis->getParticles()->fade())));
      node->m_attributes.add(new kgmXml::Attribute("angle", kgmConvert::toString((s32)vis->getParticles()->angle())));
      node->m_attributes.add(new kgmXml::Attribute("speed", kgmConvert::toString((s32)vis->getParticles()->speed())));
      node->m_attributes.add(new kgmXml::Attribute("divspeed", kgmConvert::toString((s32)vis->getParticles()->divspeed())));
      node->m_attributes.add(new kgmXml::Attribute("life", kgmConvert::toString((s32)vis->getParticles()->life())));
      node->m_attributes.add(new kgmXml::Attribute("divlife", kgmConvert::toString((s32)vis->getParticles()->divlife())));
      node->m_attributes.add(new kgmXml::Attribute("size", kgmConvert::toString((s32)vis->getParticles()->size())));
      node->m_attributes.add(new kgmXml::Attribute("esize", kgmConvert::toString((s32)vis->getParticles()->esize())));
      break;
    }

    kgmMaterial* mtl = vis->getMaterial();

    if(mtl)
    {
      node->m_attributes.add(new kgmXml::Attribute("material", mtl->name()));
    }

    addPosition(*node, n->position());
    addRotation(*node, n->rotation());

    if(n->body())
      addCollision(*node, n);

    if(n->lock())
      addLocked(*node, n->lock());
  }

  return true;
}

bool kgmGameMap::addActor(kgmActor* n)
{
  if(m_type == OpenRead || !n)
    return false;

  if(m_xml && m_xml->m_node->m_name == "kgm")
  {
    kgmXml::Node* node = new kgmXml::Node(m_xml->m_node);

    node->m_name = "Actor";
    node->m_attributes.add(new kgmXml::Attribute("name", n->getName()));
    node->m_attributes.add(new kgmXml::Attribute("type", "*"));

    if(n->m_gameplayer)
    {
      node->m_attributes.add(new kgmXml::Attribute("player", "on"));
    }

    addPosition(*node, n->position());
    addRotation(*node, n->rotation());

    if(n->body())
      addCollision(*node, true);

    if(n->lock())
      addLocked(*node, n->lock());

    addParameters(*node, n->m_variables);
  }

  return true;
}

bool kgmGameMap::addEffect(kgmEffect* n)
{
  if(m_type == OpenRead || !n)
    return false;

  if(m_xml && m_xml->m_node->m_name == "kgm")
  {
    kgmXml::Node* node = new kgmXml::Node(m_xml->m_node);

    node->m_name = "Effect";
    node->m_attributes.add(new kgmXml::Attribute("name", n->getName()));
    node->m_attributes.add(new kgmXml::Attribute("type", "*"));

    addPosition(*node, n->position());
    addRotation(*node, n->rotation());

    if(n->body())
      addCollision(*node, true);

    if(n->lock())
      addLocked(*node, true);

    addParameters(*node, n->m_variables);
  }

  return true;
}

bool kgmGameMap::addSensor(kgmSensor* n)
{
  if(m_type == OpenRead || !n)
    return false;

  if(m_xml && m_xml->m_node->m_name == "kgm")
  {
    kgmXml::Node* node = new kgmXml::Node(m_xml->m_node);

    node->m_name = "Sensor";
    node->m_attributes.add(new kgmXml::Attribute("name", n->getName()));
    node->m_attributes.add(new kgmXml::Attribute("target", n->getTarget()));

    addPosition(*node, n->position());
    addRotation(*node, n->rotation());

    if(n->body())
      addCollision(*node, true);

    if(n->lock())
      addLocked(*node, n->lock());

    addParameters(*node, n->m_variables);
  }

  return true;
}

bool kgmGameMap::addTrigger(kgmTrigger* n)
{
  if(m_type == OpenRead || !n)
    return false;

  if(m_xml && m_xml->m_node->m_name == "kgm")
  {
    kgmXml::Node* node = new kgmXml::Node(m_xml->m_node);

    node->m_name = "Trigger";
    node->m_attributes.add(new kgmXml::Attribute("name", n->getName()));
    node->m_attributes.add(new kgmXml::Attribute("channels", kgmConvert::toString((s32) n->getCount())));
    node->m_attributes.add(new kgmXml::Attribute("target", n->getTarget()));

    addPosition(*node, n->position());
    addRotation(*node, n->rotation());

    if(n->body())
      addCollision(*node, true);

    if(n->lock())
      addLocked(*node, n->lock());

    addParameters(*node, n->m_variables);
  }

  return true;
}

bool kgmGameMap::addUnit(kgmUnit* n)
{
  if(m_type == OpenRead || !n)
    return false;

  if(m_xml && m_xml->m_node->m_name == "kgm")
  {
    kgmXml::Node* node = new kgmXml::Node(m_xml->m_node);

    node->m_name = "Unit";
    node->m_attributes.add(new kgmXml::Attribute("name", n->getName()));

    addPosition(*node, n->position());
    addRotation(*node, n->rotation());

    if(n->body())
      addCollision(*node, true);

    if(n->lock())
      addLocked(*node, n->lock());

    if(n->visual())
    {
      kgmXml::Node* vnode = new kgmXml::Node(node);
      vnode->m_name = "Visual";

      if(n->visual()->getMaterial())
        vnode->m_attributes.add(new kgmXml::Attribute("material", n->visual()->getMaterial()->name()));

      if(n->visual()->getMesh())
        vnode->m_attributes.add(new kgmXml::Attribute("mesh", n->visual()->getMesh()->id()));
    }

    addParameters(*node, n->m_variables);
  }

  return true;
}

kgmUnit* kgmGameMap::next()
{
  kgmUnit* node = null;

  bool closed = true;

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

      if(id == "Camera")
      {
        ntype = "camera";

        kgmString id, fov;
        m_xml->attribute("name", id);
        m_xml->attribute("fov",  fov);

        node = new kgmUnit(m_game, new kgmCamera);
        node->setName(id);
        node->camera()->mFov = kgmConvert::toDouble(fov);

        closed = false;
      }
      else if (id == "Mesh")
      {

      }
      else if(id == "Light")
      {
        ntype = "light";

        kgmString id;
        m_xml->attribute("name", id);

        node = new kgmUnit(m_game, new kgmLight);

        node->setName(id);

        closed = false;
      }
      else if(id == "Visual")
      {
        ntype = "visual";

        kgmString id;
        m_xml->attribute("name", id);

        node = new kgmUnit(m_game, new kgmVisual);

        node->setName(id);

        closed = false;
      }
      else if(id == "Unit")
      {
        ntype = "unit";

        kgmString id;
        m_xml->attribute("name", id);

        node = new kgmUnit(m_game);
        node->setName(id);

        closed = false;
      }
      else if(id == "Actor")
      {
        ntype = "actor";

        kgmString id, plr, ini;

        m_xml->attribute("name", id);
        m_xml->attribute("actor", ini);
        m_xml->attribute("player", plr);

        node = new kgmActor(m_game);

        //kgmGameTools::initActor(m_game, act, ini);

        node->setName(id);
        //node.ini = ini;

        if(plr == "on")
          ((kgmActor*)node)->m_gameplayer = true;

        closed = false;
      }
      else if(id == "Effect")
      {
        ntype = "effect";

        kgmString id, cls;
        m_xml->attribute("name", id);
        m_xml->attribute("class", cls);

        node = new kgmEffect(m_game);

        node->setName(id);

        closed = false;
      }
      else if(id == "Sensor")
      {
        ntype = "sensor";

        kgmString id, cls, trg;
        m_xml->attribute("name", id);
        m_xml->attribute("class", cls);
        m_xml->attribute("target", trg);

        node = new kgmSensor(m_game);

        node->setName(id);

        closed = false;
      }
      else if(id == "Trigger")
      {
        ntype = "trigger";

        kgmString id, chn, trg;
        m_xml->attribute("name", id);
        m_xml->attribute("channels", chn);
        m_xml->attribute("target", trg);

        node = new kgmTrigger(m_game);

        node->setName(id);
        ((kgmTrigger*)node)->setCount(kgmConvert::toInteger(chn));
        ((kgmTrigger*)node)->setTarget(trg);

        closed = false;
      }
      else if(id == "Obstacle")
      {
        ntype = "obstacle";

        kgmString id;
        m_xml->attribute("name", id);

        node = new kgmUnit(m_game, new kgmObstacle());
        node->setName(id);

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

        node->position(v);
      }
      else if(id == "Rotation")
      {
        vec3 v;
        m_xml->attribute("value", value);
        sscanf(value.data(), "%f %f %f", &v.x, &v.y, &v.z);

        node->rotation(v);
      }
      else if(id == "Direction")
      {
        vec3 v;
        m_xml->attribute("value", value);
        sscanf(value.data(), "%f %f %f", &v.x, &v.y, &v.z);

        node->rotation(v);
      }
      else if(id == "Quaternion")
      {
        quat q;
        m_xml->attribute("value", value);
        sscanf(value.data(), "%f %f %f %f", &q.x, &q.y, &q.z, &q.w);

        node->quaternion(q);
      }
      else if(id == "Mesh")
      {
        kgmString link;

        m_xml->attribute("id", link);

        if(node && node->visual())
          node->visual()->set(m_game->getResources()->getMesh(link));
      }
      else if(id == "Particles")
      {
        kgmString value;

        if(ntype == "visual")
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
          node->visual()->set(pts);
        }
      }
      else if(id == "Material")
      {
        kgmString link;

        m_xml->attribute("name", link);

        if(node && node->visual())
          node->visual()->set(m_game->getResources()->getMaterial(link));
      }
      else if(id == "Locked")
      {
        value.clear();
        m_xml->attribute("value", value);

        if(value == "true")
          node->lock(true);
      }
      else if(id == "Parameter")
      {
        kgmString name, type, value;

        m_xml->attribute("name",  name);
        m_xml->attribute("type",  type);
        m_xml->attribute("value", value);

        if(ntype == "gobject")
          ((kgmUnit*)node)->setParameter(name, value);
      }
      /*
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
      */
      else if(id == "Visual")
      {
        kgmString value;

        m_xml->attribute("material", value);

        if (node && value.length() > 0 && node->visual())
          node->visual()->set(kgmIGame::getGame()->getResources()->getMaterial(value));

        m_xml->attribute("mesh", value);

        if(node && value.length() > 0 && node->visual())
          node->visual()->set(kgmIGame::getGame()->getResources()->getMesh(value));
      }
      else if(id == "Action")
      {
        kgmString value;

        m_xml->attribute("id", value);

        if(node)
          ((kgmUnit*)node)->action(value);
      }
      else if((id == "Visual") || (id == "Light")  || (id == "Effect")  ||
              (id == "Actor")  || (id == "Sensor") || (id == "Trigger") ||
              (id == "Unit")   || (id == "Camera") || (id == "Obstacle") ||
              (id == "Material") || (id == "Mesh"))
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

void kgmGameMap::addCollision(kgmXml::Node& xnode, bool col)
{
  if(!col)
    return;

  kgmXml::Node* snode = new kgmXml::Node(&xnode);
  snode->m_name = "Collision";
  snode->m_attributes.add(new kgmXml::Attribute("value", "true"));
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

void kgmGameMap::addPosition(kgmXml::Node& xnode, vec3 pos)
{
  kgmXml::Node* snode = new kgmXml::Node(&xnode);
  snode->m_name = "Position";
  snode->m_attributes.add(new kgmXml::Attribute("value", kgmConvert::toString(pos.x) + kgmString(" ") +
                                                         kgmConvert::toString(pos.y) + kgmString(" ") +
                                                         kgmConvert::toString(pos.z)));
}

void kgmGameMap::addRotation(kgmXml::Node& xnode, vec3 rot)
{
  kgmXml::Node* snode = new kgmXml::Node(&xnode);
  snode->m_name = "Rotation";
  snode->m_attributes.add(new kgmXml::Attribute("value", kgmConvert::toString(rot.x) + kgmString(" ") +
                                                         kgmConvert::toString(rot.y) + kgmString(" ") +
                                                         kgmConvert::toString(rot.z)));

}

void kgmGameMap::addQuaternion(kgmXml::Node& xnode, quat qt)
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
