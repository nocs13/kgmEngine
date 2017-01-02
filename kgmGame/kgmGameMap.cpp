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
}

bool kgmGameMap::open(kgmMemory<u8>& mem)
{
  if(m_xml)
    return false;

  m_mem = &mem;
}

bool kgmGameMap::save(kgmString path)
{
  kgmXml     xml;

  if(m_type != OpenWrite)
    return false;

  open(xml);

  kgmList<kgmGameLight*> lights;
  kgmList<kgmGameVisual*> visuals;
  kgmList<kgmGameCamera*> cameras;

  kgmList<kgmUnit*> units;
  kgmList<kgmActor*> actors;
  kgmList<kgmEffect*> effects;
  kgmList<kgmSensor*> sensors;
  kgmList<kgmTrigger*> triggers;

  kgmList<kgmGameNode*> nodes;

  m_game->getLogic()->getObjects(nodes);

  for(kgmList<kgmGameNode*>::iterator i = nodes.begin(); i != nodes.end(); ++i)
  {
    if((*i)->isClass("kgmUnit"))
      units.add((kgmUnit*) (*i));
    else if((*i)->isClass("kgmGameLight"))
      lights.add((kgmGameLight*) (*i));
    else if((*i)->isClass("kgmGameCamera"))
      cameras.add((kgmGameCamera*) (*i));
    else if((*i)->isClass("kgmActor"))
      actors.add((kgmActor*) (*i));
    else if((*i)->isClass("kgmGameVisual"))
      visuals.add((kgmGameVisual*) (*i));
    else if((*i)->isClass("kgmEffect"))
      effects.add((kgmEffect*) (*i));
    else if((*i)->isClass("kgmSensor"))
      sensors.add((kgmSensor*) (*i));
    else if((*i)->isClass("kgmTrigger"))
      triggers.add((kgmTrigger*) (*i));
  }

  for(kgmList<kgmGameCamera*>::iterator i = cameras.begin(); i != cameras.end(); ++i)
    addCamera(*i);

  for(kgmList<kgmGameLight*>::iterator i = lights.begin(); i != lights.end(); ++i)
    addLight(*i);

  for(kgmList<kgmGameVisual*>::iterator i = visuals.begin(); i != visuals.end(); ++i)
    addVisual(*i);

  for(kgmList<kgmUnit*>::iterator i = units.begin(); i != units.end(); ++i)
    addUnit(*i);

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
  visuals.clear();
  cameras.clear();
  lights.clear();

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

bool kgmGameMap::addLight(kgmGameLight* n)
{
  if(m_type == OpenRead || !n)
    return false;

  if(m_xml && m_xml->m_node->m_name == "kgm")
  {
    kgmXml::Node* node = new kgmXml::Node(m_xml->m_node);

    node->m_name = "kgmGameLight";
    node->m_attributes.add(new kgmXml::Attribute("name", n->getId()));

    addPosition(*node, n->position());
    addRotation(*node, n->rotation());

    if(n->lock())
      addLocked(*node, n->lock());
  }
}

bool kgmGameMap::addCamera(kgmGameCamera* n)
{
  if(m_type == OpenRead || !n)
    return false;

  if(m_xml && m_xml->m_node->m_name == "kgm")
  {
    kgmXml::Node* node = new kgmXml::Node(m_xml->m_node);

    vec3 pos, dir;

    pos = n->camera()->mPos;
    dir = n->camera()->mDir;

    node->m_name = "kgmCamera";
    node->m_attributes.add(new kgmXml::Attribute("name", n->getId()));
    node->m_attributes.add(new kgmXml::Attribute("fov", kgmConvert::toString(n->camera()->mFov)));

    addPosition(*node, pos);
    addRotation(*node, dir);

    if(n->body())
      addCollision(*node, n);

    if(n->lock())
      addLocked(*node, n->lock());
  }
}

bool kgmGameMap::addVisual(kgmGameVisual* n)
{
  if(m_type == OpenRead || !n)
    return false;

  if(m_xml && m_xml->m_node->m_name == "kgm")
  {
    kgmXml::Node* node = new kgmXml::Node(m_xml->m_node);
    kgmXml::Node* snode = null;

    node->m_name = "kgmVisual";
    node->m_attributes.add(new kgmXml::Attribute("name", n->getId()));

    kgmVisual* vis = n->visual();

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

    addPosition(*node, n->position());
    addRotation(*node, n->rotation());

    if(n->body())
      addCollision(*node, n);

    if(n->lock())
      addLocked(*node, n->lock());
  }
}

bool kgmGameMap::addActor(kgmActor* n)
{
  if(m_type == OpenRead || !n)
    return false;

  if(m_xml && m_xml->m_node->m_name == "kgm")
  {
    kgmXml::Node* node = new kgmXml::Node(m_xml->m_node);

    node->m_name = "kgmActor";
    node->m_attributes.add(new kgmXml::Attribute("name", n->getId()));
    //node->m_attributes.add(new kgmXml::Attribute("actor", n.ini));

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
}

bool kgmGameMap::addEffect(kgmEffect* n)
{
  if(m_type == OpenRead || !n)
    return false;

  if(m_xml && m_xml->m_node->m_name == "kgm")
  {
    kgmXml::Node* node = new kgmXml::Node(m_xml->m_node);

    node->m_name = "kgmEffect";
    node->m_attributes.add(new kgmXml::Attribute("name", n->getId()));

    addPosition(*node, n->position());
    addRotation(*node, n->rotation());

    if(n->body())
      addCollision(*node, true);

    if(n->lock())
      addLocked(*node, true);

    addParameters(*node, n->m_variables);
  }
}

bool kgmGameMap::addSensor(kgmSensor* n)
{
  if(m_type == OpenRead || !n)
    return false;

  if(m_xml && m_xml->m_node->m_name == "kgm")
  {
    kgmXml::Node* node = new kgmXml::Node(m_xml->m_node);

    node->m_name = "kgmSensor";
    node->m_attributes.add(new kgmXml::Attribute("name", n->getId()));
    node->m_attributes.add(new kgmXml::Attribute("target", n->getTarget()));

    addPosition(*node, n->position());
    addRotation(*node, n->rotation());

    if(n->body())
      addCollision(*node, true);

    if(n->lock())
      addLocked(*node, n->lock());

    addParameters(*node, n->m_variables);
  }
}

bool kgmGameMap::addTrigger(kgmTrigger* n)
{
  if(m_type == OpenRead || !n)
    return false;

  if(m_xml && m_xml->m_node->m_name == "kgm")
  {
    kgmXml::Node* node = new kgmXml::Node(m_xml->m_node);

    node->m_name = "kgmTrigger";
    node->m_attributes.add(new kgmXml::Attribute("name", n->getId()));
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
}

bool kgmGameMap::addUnit(kgmUnit* n)
{
  if(m_type == OpenRead || !n)
    return false;

  if(m_xml && m_xml->m_node->m_name == "kgm")
  {
    kgmXml::Node* node = new kgmXml::Node(m_xml->m_node);

    node->m_name = "kgmUnit";
    node->m_attributes.add(new kgmXml::Attribute("name", n->getId()));

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
        vnode->m_attributes.add(new kgmXml::Attribute("mesh", n->visual()->getMesh()->getMesh()->id()));
    }

    addParameters(*node, n->m_variables);
  }
}

kgmGameNode* kgmGameMap::next()
{
  kgmGameNode* node = null;

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

      if(id == "kgmCamera")
      {
        ntype = "camera";

        kgmString id, fov;
        m_xml->attribute("name", id);
        m_xml->attribute("fov",  fov);

        node = new kgmGameCamera(m_game);
        node->setId(id);
        ((kgmGameCamera*)node)->camera()->mFov = kgmConvert::toDouble(fov);

        closed = false;
      }
      else if(id == "kgmLight")
      {
        ntype = "light";

        kgmString id;
        m_xml->attribute("name", id);

        node = new kgmGameLight(m_game);

        node->setId(id);

        closed = false;
      }
      else if(id == "kgmVisual")
      {
        ntype = "visual";

        kgmString id;
        m_xml->attribute("name", id);

        node = new kgmGameVisual(m_game);

        node->setId(id);

        closed = false;
      }
      else if(id == "kgmUnit")
      {
        ntype = "gobject";

        kgmString id;
        m_xml->attribute("name", id);

        node = new kgmUnit(m_game);
        node->setId(id);

        closed = false;
      }
      else if(id == "kgmActor")
      {
        ntype = "actor";

        kgmString id, plr, ini;

        m_xml->attribute("name", id);
        m_xml->attribute("actor", ini);
        m_xml->attribute("player", plr);

        node = new kgmActor(m_game);

        //kgmGameTools::initActor(m_game, act, ini);

        node->setId(id);
        //node.ini = ini;

        if(plr == "on")
          ((kgmActor*)node)->m_gameplayer = true;

        closed = false;
      }
      else if(id == "kgmEffect")
      {
        ntype = "effect";

        kgmString id, cls;
        m_xml->attribute("name", id);
        m_xml->attribute("class", cls);

        node = new kgmEffect(m_game);

        node->setId(id);

        closed = false;
      }
      else if(id == "kgmSensor")
      {
        ntype = "sensor";

        kgmString id, cls, trg;
        m_xml->attribute("name", id);
        m_xml->attribute("class", cls);
        m_xml->attribute("target", trg);

        node = new kgmSensor(m_game);

        node->setId(id);

        closed = false;
      }
      else if(id == "kgmTrigger")
      {
        ntype = "trigger";

        kgmString id, chn, trg;
        m_xml->attribute("name", id);
        m_xml->attribute("channels", chn);
        m_xml->attribute("target", trg);

        node = new kgmTrigger(m_game);

        node->setId(id);
        ((kgmTrigger*)node)->setCount(kgmConvert::toInteger(chn));
        ((kgmTrigger*)node)->setTarget(trg);

        closed = false;
      }
      /*else if(id == "kgmObstacle")
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

        m_materials.add(mtl);
      }*/
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

        if(ntype == "visual")
        {
          ((kgmGameVisual*)node)->visual()->set(m_game->getResources()->getMesh(link));
        }
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
          ((kgmGameVisual*)node)->visual()->set(pts);
        }
      }
      /*else if(id == "Shader")
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
      }*/
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
      /*else if(id == "Scale")
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
      }*/
      else if(id == "Visual")
      {
        kgmString value;

        m_xml->attribute("material", value);

        if(node && value.length() > 0)
        {
          //if(!((kgmUnit*)node)->visual())
          //  ((kgmUnit*)node)->visual(new kgmVisual());

          for(kgmList<kgmMaterial*>::iterator i = m_materials.begin(); i != m_materials.end(); ++i)
          {
            //if((*i)->name() == id)
            //  ((kgmUnit*)node.obj)->visual()->set((*i));
          }
        }

        m_xml->attribute("mesh", value);

        if(node && value.length() > 0)
        {
          //if(!((kgmUnit*)node)->visual())
          //  ((kgmUnit*)node)->visual(new kgmVisual());

          ((kgmUnit*)node)->visual()->set(kgmIGame::getGame()->getResources()->getMesh(value));
        }
      }
      else if(id == "Action")
      {
        kgmString value;

        m_xml->attribute("id", value);

        if(node)
          ((kgmUnit*)node)->action(value);
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
