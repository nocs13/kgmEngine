#include "kgmGameMap.h"

#include "../kgmBase/kgmResource.h"
#include "../kgmBase/kgmConvert.h"

#include "kgmIGame.h"
#include "kgmILogic.h"
#include "kgmGameBase.h"
#include "kgmGameResources.h"
#include "kgmGameTerrain.h"

#include "../kgmGraphics/kgmMesh.h"
#include "../kgmGraphics/kgmLight.h"
#include "../kgmGraphics/kgmTerrain.h"

kgmGameMap::kgmGameMap(kgmGameBase* g, OpenType ot)
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

bool kgmGameMap::open(kgmArray<u8>& mem)
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

  //m_game->getLogic()->getObjects(nodes);

  for(kgmList<kgmUnit*>::iterator i = nodes.begin(); !i.end(); ++i)
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

  for(kgmList<kgmUnit*>::iterator i = units.begin(); !i.end(); ++i) {
    kgmIGraphics::INode* n = (*i)->getNode();

    if (!n)
      continue;


    switch(n->getNodeType())
    {
    case kgmIGraphics::NodeLight:
      addLight(*i);
      break;
    case kgmIGraphics::NodeCamera:
      addCamera(*i);
      break;
    case kgmIGraphics::NodeMesh:
      addMesh(*i);
      break;
    default:
      addUnit(*i);
    }
  }

  for(kgmList<kgmActor*>::iterator i = actors.begin(); !i.end(); ++i)
    addActor(*i);

  for(kgmList<kgmEffect*>::iterator i = effects.begin(); !i.end(); ++i)
    addEffect(*i);

  for(kgmList<kgmSensor*>::iterator i = sensors.begin(); !i.end(); ++i)
    addSensor(*i);

  for(kgmList<kgmTrigger*>::iterator i = triggers.begin(); !i.end(); ++i)
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

    kgmCamera* camera = (kgmCamera*)n->getNode()->getNodeObject();

    pos = camera->mPos;
    dir = camera->mDir;

    node->m_name = "Camera";
    node->m_attributes.add(new kgmXml::Attribute("name", n->getName()));
    node->m_attributes.add(new kgmXml::Attribute("fov", kgmConvert::toString(camera->mFov)));

    addPosition(*node, pos);
    addRotation(*node, dir);

    //if(n->body())
    //  addCollision(*node, n);

    if(n->lock())
      addLocked(*node, n->lock());
  }

  return true;
}

bool kgmGameMap::addMesh(kgmUnit* n)
{
  if(m_type == OpenRead || !n)
    return false;

  if(m_xml && m_xml->m_node->m_name == "kgm")
  {
    kgmXml::Node* node = new kgmXml::Node(m_xml->m_node);
    //kgmXml::Node* snode = null;

    node->m_name = "Mesh";
    node->m_attributes.add(new kgmXml::Attribute("name", n->getName()));

    kgmVisual* vis = (kgmVisual*) n->getNode()->getNodeObject();

    if (!vis)
      return false;

    kgmString sid;

    sid = kgmConvert::toString(n->getId());

    node->m_attributes.add(new kgmXml::Attribute("type", "mesh"));
    node->m_attributes.add(new kgmXml::Attribute("id", sid));
    /*node->m_attributes.add(new kgmXml::Attribute("type", "particles"));
      node->m_attributes.add(new kgmXml::Attribute("count", kgmConvert::toString((s32)vis->getParticles()->count())));
      node->m_attributes.add(new kgmXml::Attribute("loop", kgmConvert::toString((s32)vis->getParticles()->loop())));
      node->m_attributes.add(new kgmXml::Attribute("fade", kgmConvert::toString((s32)vis->getParticles()->fade())));
      node->m_attributes.add(new kgmXml::Attribute("angle", kgmConvert::toString((s32)vis->getParticles()->angle())));
      node->m_attributes.add(new kgmXml::Attribute("speed", kgmConvert::toString((s32)vis->getParticles()->speed())));
      node->m_attributes.add(new kgmXml::Attribute("divspeed", kgmConvert::toString((s32)vis->getParticles()->divspeed())));
      node->m_attributes.add(new kgmXml::Attribute("life", kgmConvert::toString((s32)vis->getParticles()->life())));
      node->m_attributes.add(new kgmXml::Attribute("divlife", kgmConvert::toString((s32)vis->getParticles()->divlife())));
      node->m_attributes.add(new kgmXml::Attribute("size", kgmConvert::toString((s32)vis->getParticles()->size())));
      node->m_attributes.add(new kgmXml::Attribute("esize", kgmConvert::toString((s32)vis->getParticles()->esize())));*/

    kgmMaterial* mtl = vis->getMaterial();

    if(mtl)
    {
      node->m_attributes.add(new kgmXml::Attribute("material", mtl->id()));
    }

    addPosition(*node, n->position());
    addRotation(*node, n->rotation());

    //if(n->body())
    //  addCollision(*node, n);

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

    //if(n->body())
    //  addCollision(*node, true);

    if(n->lock())
      addLocked(*node, n->lock());
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

    //if(n->body())
    //  addCollision(*node, true);

    if(n->lock())
      addLocked(*node, true);
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

    //if(n->body())
    //  addCollision(*node, true);

    if(n->lock())
      addLocked(*node, n->lock());
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

    //if(n->body())
    //  addCollision(*node, true);

    if(n->lock())
      addLocked(*node, n->lock());
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

    //if(n->body())
    //  addCollision(*node, true);

    if(n->lock())
      addLocked(*node, n->lock());
  }

  return true;
}

kgmUnit* kgmGameMap::next()
{
  kgmUnit*   node = null;
  kgmObject* data = null;


  bool closed   = true;

  s32       vint;
  f32       vfloat;
  vec3      vfloat3;
  vec4      vfloat4;
  kgmString vtext;

  kgmString ntype  = "";
  kgmString m_data = "";

  u32 index = 0;

  if(m_type == OpenWrite)
    return node;

  while(kgmXml::XmlState xstate = m_xml->next())
  {

    kgmString id, value;

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

        kgmCamera* c = new kgmCamera;

        m_game->m_objects.add(c);

        data = c;

        node = new kgmUnit(m_game);

        node->setNode(new kgmGNode(node, c, kgmIGraphics::NodeCamera));

        node->setName(id);

        closed = false;
      }
      else if (id == "Material")
      {
        ntype = "material";

        kgmString id;
        m_xml->attribute("name", id);

        kgmMaterial* m = new kgmMaterial;

        m_game->m_objects.add(m);

        m->setId(id);

        data = m;

        closed = false;
      }
      else if (id == "Mesh")
      {
        ntype = "mesh";

        kgmString id;
        m_xml->attribute("name", id);

        kgmMesh* m = null;

        if (m_xml->hasattr("link"))
        {
          kgmString v;

          m_xml->attribute("link", v);

          m = m_game->getResources()->getMesh(v);
        }
        else
        {
          m = new kgmMesh();

          //m_game->m_objects.add(m);

          data = m;
        }

        node = new kgmUnit(m_game);
        node->setNode(new kgmGNode(node, m, kgmIGraphics::NodeMesh));

        node->setName(id);

        if(m_xml->hasattr("material"))
        {
          kgmString v;

          m_xml->attribute("material", v);

          node->getNode()->setNodeMaterial(getGameMaterial(v));
          node->getNode()->setNodeShader(kgmIGraphics::ShaderPhong);
        }

        closed = false;
      }
      else if(id == "Light")
      {
        ntype = "light";

        kgmString id;
        m_xml->attribute("name", id);

        kgmLight* l = new kgmLight;

        m_game->m_objects.add(l);

        data = l;

        node = new kgmUnit(m_game);
        node->setNode(new kgmGNode(node, l, kgmIGraphics::NodeLight));

        node->setName(id);

        closed = false;
      }
      else if(id == "Terrain")
      {
        ntype = "terrain";

        kgmString id;
        m_xml->attribute("name", id);

        vec3 v;
        m_xml->attribute("dimensions", value);
        sscanf(value.data(), "%f %f %f", &v.x, &v.y, &v.z);

        kgmTerrain* t = new kgmTerrain();

        t->width(v.x);
        t->length(v.y);
        t->height(v.z);

        m_game->m_objects.add(t);

        data = t;

        node = new kgmUnit(m_game);
        node->setNode(new kgmGNode(node, t, kgmIGraphics::NodeTerrain));

        node->setName(id);

        closed = false;

        index = 0;
      }
      else if(id == "Unit")
      {
        ntype = "unit";

        kgmString id;
        m_xml->attribute("name", id);

        node = new kgmUnit(m_game);
        node->setName(id);

        data = null;

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

        data = null;

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

        data = null;

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

        data = null;

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

        data = null;

        closed = false;
      }
      else if(id == "Obstacle")
      {
        ntype = "obstacle";

        kgmString id;
        m_xml->attribute("name", id);

        kgmObstacle* o = new kgmObstacle;

        m_game->m_objects.add(o);

        data = o;

        node = new kgmUnit(m_game);
        node->setName(id);

        closed = false;
      }
      else if(id == "Particles")
      {
        ntype = "particles";

        kgmString id, mtl;
        m_xml->attribute("name", id);
        m_xml->attribute("material", mtl);

        kgmParticles* p = new kgmParticles;

        m_game->m_objects.add(p);

        data = p;

        node = new kgmUnit(m_game);
        node->setName(id);

        kgmGNode* n = new kgmGNode(node, p, kgmIGraphics::NodeParticles);

        node->setNode(n);

        if(m_xml->hasattr("material"))
        {
          kgmString v;

          m_xml->attribute("material", v);

          node->getNode()->setNodeMaterial(getGameMaterial(v));
        }

        closed = false;
      }
      else if(id == "Vertices")
      {
        kgmMesh* m = (kgmMesh*) data;
        xmlAttr(m_xml, "length", vint);
        m->vAlloc(vint, kgmMesh::FVF_P_N_T);
        m_data = "vertices";
      }
      else if(id == "Faces")
      {
        kgmMesh* m = (kgmMesh*) (data);
        xmlAttr(m_xml, "length", vint);
        m->fAlloc(vint, kgmMesh::FFF_16);
        m_data = "faces";
      }
      else if(id == "Polygon")
      {
        //xml.attribute("vertices", value);
        //sscanf(value.data(), "%i", &vts);
        m_data = "polygon";
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

        //node->rotation(v);
        if (ntype == "light")
          ((kgmLight*)data)->direction(v);
      }
      else if(id == "Quaternion")
      {
        quat q;
        m_xml->attribute("value", value);
        sscanf(value.data(), "%f %f %f %f", &q.x, &q.y, &q.z, &q.w);

        //node->quaternion(q);
      }
      else if(id == "Scale")
      {
        vec3 v;
        m_xml->attribute("value", value);
        sscanf(value.data(), "%f %f %f", &v.x, &v.y, &v.z);

        node->scale(v);
      }
      else if(id == "Color")
      {
        xmlAttr(m_xml, "value", vfloat3);

        if(ntype == "material")
          ((kgmMaterial*)data)->m_color = kgmMaterial::Color(vfloat3.x, vfloat3.y, vfloat3.z, 1.0);
        else if(ntype == "light")
          ((kgmLight*)data)->color(vfloat3);
      }
      else if(id == "Specular")
      {
        xmlAttr(m_xml, "value", vfloat3);
      }
      else if(id == "Distortion")
      {
        xmlAttr(m_xml, "value", vfloat);
        ((kgmMaterial*)data)->distortion(vfloat);
      }
      else if(id == "Shininess")
      {
        xmlAttr(m_xml, "value", vfloat);
        ((kgmMaterial*)data)->shininess(vfloat);
      }
      else if(id == "Alpha")
      {
        xmlAttr(m_xml, "value", vfloat);
        ((kgmMaterial*)data)->transparency(1.0f - vfloat);
      }
      else if(id == "Fresnel")
      {
        f32 fm = 0.0, ft = 0.0;

        xmlAttr(m_xml, "mirror", vfloat);
        fm = vfloat;
        xmlAttr(m_xml, "transparency", vfloat);
        ft = vfloat;

        ((kgmMaterial*)data)->fresnel(fm + ft);
      }
      else if(id == "Blend")
      {
        xmlAttr(m_xml, "value", vtext);

        ((kgmMaterial*)data)->blend(kgmMaterial::stringToBlend(vtext));
      }
      else if(id == "Cull")
      {
        xmlAttr(m_xml, "value", vint);
        ((kgmMaterial*)data)->cull((vint != 0));
      }
      else if(id == "Depth")
      {
        xmlAttr(m_xml, "value", vint);
        ((kgmMaterial*)data)->depth((vint != 0));
      }
      else if(id == "map_color")
      {
        xmlAttr(m_xml, "value", vtext);
        ((kgmMaterial*)data)->setTexColor(m_game->getResources()->getTexture(vtext));
      }
      else if(id == "map_normal")
      {
        xmlAttr(m_xml, "value", vtext);
        ((kgmMaterial*)data)->setTexNormal(m_game->getResources()->getTexture(vtext));
      }
      else if(id == "map_specular")
      {
        xmlAttr(m_xml, "value", vtext);
        ((kgmMaterial*)data)->setTexSpecular(m_game->getResources()->getTexture(vtext));
      }
      else if(id == "map_environment")
      {
        xmlAttr(m_xml, "type", vtext);

        if (vtext == "Animated")
        {
          ((kgmMaterial*)data)->envType(kgmMaterial::EnvironmentTypeAnimate);
        }
        else if (vtext == "Static")
        {
          ((kgmMaterial*)data)->envType(kgmMaterial::EnvironmentTypeStatic);
        }
        else if (vtext == "Image")
        {
          ((kgmMaterial*)data)->envType(kgmMaterial::EnvironmentTypeImage);

          xmlAttr(m_xml, "image", vtext);
          ((kgmMaterial*)data)->setTexEnvironment(m_game->getResources()->getTexture(vtext));
        }
        else
        {
          ((kgmMaterial*)data)->envType(kgmMaterial::EnvironmentTypeNone);
        }

        xmlAttr(m_xml, "mapping", vtext);

        if (vtext == "Cube") {
          ((kgmMaterial*)data)->envMapping(kgmMaterial::EnvironmentMappingCube);
        } else if (vtext == "Plane") {
          ((kgmMaterial*)data)->envMapping(kgmMaterial::EnvironmentMappingPlane);
        }

        xmlAttr(m_xml, "intensity", vfloat);
        ((kgmMaterial*)data)->envIntensity(vfloat);
      }
      else if(id == "Shadow")
      {
        xmlAttr(m_xml, "shadeless", vtext);

        if (vtext == "True" ||  vtext == "true")
          ((kgmMaterial*)data)->shading(false);

        xmlAttr(m_xml, "cast", vtext);

        if (vtext == "True" ||  vtext == "true")
          ((kgmMaterial*)data)->shade_cast(true);

        xmlAttr(m_xml, "receive", vtext);

        if (vtext == "True" ||  vtext == "true")
          ((kgmMaterial*)data)->shade_receive(true);
      }
      else if (id == "Type")
      {
        if (ntype == "light")
        {
          xmlAttr(m_xml, "value", vtext);

          if (vtext == "Spot")
            ((kgmLight*)data)->type(kgmLight::TypeSpot);
          else if (vtext == "Sun")
            ((kgmLight*)data)->type(kgmLight::TypeSun);
          else
            ((kgmLight*)data)->type(kgmLight::TypePoint);
        }
      }
      else if (id == "Intensity")
      {
        if (ntype == "light")
        {
          xmlAttr(m_xml, "value", vfloat);

          ((kgmLight*)data)->intensity(vfloat);
        }
      }
      else if (id == "Spot")
      {
        if (ntype == "light")
        {
          xmlAttr(m_xml, "value", vfloat);

          ((kgmLight*)data)->angle(vfloat);
        }
      }
      else if (id == "HeightMap")
      {
        xmlAttr(m_xml, "value", vtext);

        kgmPicture* map = m_game->getResources()->getPicture(vtext);

        if (map)
          ((kgmTerrain*)data)->heightmap(map);

      }
      else if (id == "NormalMap")
      {
        xmlAttr(m_xml, "value", vtext);

        if (ntype == "terrain")
        {
          kgmPicture* map = m_game->getResources()->getPicture(vtext);

          if (map)
            ((kgmTerrain*)data)->normalmap(map);
        }
      }
      else if (id == "UVScale")
      {
        kgmString u, v;

        xmlAttr(m_xml, "u", u);
        xmlAttr(m_xml, "v", v);

        ((kgmTerrain*)data)->uvScale(kgmConvert::toDouble(u), kgmConvert::toDouble(v));
      }
      else if (id == "BlendMap")
      {
        xmlAttr(m_xml, "value", vtext);

        kgmTexture* map = m_game->getResources()->getTexture(vtext);

        if (map)
          ((kgmTerrain*)data)->texBlend(map);
      }
      else if (id == "ColorMap")
      {
        xmlAttr(m_xml, "value", vtext);

        kgmTexture* map = m_game->getResources()->getTexture(vtext);

        if (map) {
          ((kgmTerrain*)data)->texColor(index, map);

          index++;
        }
      }
      else if(id == "Shader")
      {
        xmlAttr(m_xml, "value", vtext);
        ((kgmMaterial*)data)->type(kgmMaterial::stringToType(vtext));
      }
      else if(id == "TexMove")
      {
        xmlAttr(m_xml, "value", vfloat3);
        ((kgmMaterial*)data)->move(vfloat3);
      }
      else if(id == "Mesh")
      {
        kgmString link;

        m_xml->attribute("id", link);

        if (m_xml->hasattr("link"))
        {
          kgmString v;

          m_xml->attribute("link", v);

          kgmMesh* mesh =  m_game->getResources()->getMesh(v);

          if (mesh && node) {
            kgmGNode* n = new kgmGNode(node, mesh, kgmIGraphics::NodeMesh);

            node->setNode(n);
          }
        }
        else if(ntype == "mesh")
        {
          kgmMesh* m = (kgmMesh*) node->getNode()->getNodeObject();

          if (m)
            m->rebuild();

          closed = true;

          break;
        }
      }
      else if(id == "PrData")
      {
        kgmParticles* p = (kgmParticles* ) node->getNode()->getNodeObject();

        if (m_xml->hasattr("count")) {
          xmlAttr(m_xml, "count", vint);
          p->count(vint);
        }

        if (m_xml->hasattr("speed")) {
          xmlAttr(m_xml, "count", vfloat);
          p->speed(vfloat);
        }

        if (m_xml->hasattr("noise")) {
          xmlAttr(m_xml, "noise", vfloat);
          p->noise(vfloat);
        }

        if (m_xml->hasattr("life")) {
          xmlAttr(m_xml, "life", vfloat);
          p->life(vfloat);
        }

        if (m_xml->hasattr("dlife")) {
          xmlAttr(m_xml, "dlife", vfloat);
          p->divlife(vfloat);
        }

        if (m_xml->hasattr("size")) {
          xmlAttr(m_xml, "size", vfloat);
          p->size(vfloat);
        }

        if (m_xml->hasattr("dsize")) {
          xmlAttr(m_xml, "dsize", vfloat);
          p->esize(vfloat);
        }

        if (m_xml->hasattr("mass")) {
          xmlAttr(m_xml, "mass", vfloat);
          p->mass(vfloat);
        }

        if (m_xml->hasattr("gravity")) {
          xmlAttr(m_xml, "gravity", vint);
          p->gravity((vint != 0));
        }

        if (m_xml->hasattr("direction")) {
          xmlAttr(m_xml, "direction", vfloat3);
          p->direction(vfloat3);
        }

        if (m_xml->hasattr("volume")) {
          xmlAttr(m_xml, "volume", vfloat3);
          p->volume(vfloat3);
        }
      }
      else if(id == "Material")
      {
        kgmString link;

        if (m_xml->hasattr("name"))
        {
          m_xml->attribute("name", link);

          node->getNode()->setNodeMaterial(getGameMaterial(link));
        }
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

        //node->setParameter(name, value);
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

        //if(node.obj)
          //((kgmObstacle*)node.obj)->add(va, vb, vc);
      }
      else if(id == "Action")
      {
        kgmString value;

        m_xml->attribute("id", value);

        //if(node)
        //  ((kgmUnit*)node)->action(value);
      }
      else if((id == "Particles") || (id == "Light")  || (id == "Effect") ||
              (id == "Actor")  || (id == "Sensor") || (id == "Trigger")   ||
              (id == "Unit")   || (id == "Camera") || (id == "Obstacle")  ||
              (id == "Material") || (id == "Mesh"))
      {
        closed = true;

        break;
      }
    }
    else if(xstate == kgmXml::XML_TAG_DATA)
    {
      if(m_data == "vertices")
      {
        kgmMesh* m = (kgmMesh*) data;

        s32 n = 0;
        char* pdata = m_xml->m_tagData.data();
        kgmMesh::Vertex_P_N_T* v = (kgmMesh::Vertex_P_N_T*)m->vertices();

        for (u32 i = 0; i < m->vcount(); i++) {
          sscanf(pdata, "%f %f %f %f %f %f %f %f%n",
                 &v[i].pos.x, &v[i].pos.y, &v[i].pos.z,
                 &v[i].nor.x, &v[i].nor.y, &v[i].nor.z,
                 &v[i].uv.x, &v[i].uv.y, &n);
          (pdata) += (u32)n;
        }

        m_data = "";
      }
      else if(m_data == "faces")
      {
        kgmMesh* m = (kgmMesh*) data;

        s32 n = 0;
        char* pdata = m_xml->m_tagData.data();
        kgmMesh::Face_16* f = (kgmMesh::Face_16*) m->faces();

        for (u32 i = 0; i < m->fcount(); i++) {
          u32 fs[4];
          sscanf(pdata, "%i %i %i %n", &fs[0], &fs[1], &fs[2], &n);
          f[i].f[0] = fs[0];
          f[i].f[1] = fs[1];
          f[i].f[2] = fs[2];
          (pdata) += (u32)n;
        }

        m_data = "";
      }
    }
  }

  return node;
}

kgmMesh* kgmGameMap::getGameMesh(kgmString id)
{
  for(kgmList<kgmObject*>::iterator i = m_game->m_objects.begin(); !i.end(); ++i)
  {
    kgmObject* o = (*i);

    if (!strcmp("kgmMesh", o->vClass()))
    {
      kgmMesh* m = (kgmMesh*)o;

      if (m->id() == id)
        return m;
    }
  }

  return null;
}

kgmMaterial* kgmGameMap::getGameMaterial(kgmString id)
{
  for(kgmList<kgmObject*>::iterator i = m_game->m_objects.begin(); !i.end(); ++i)
  {
    kgmObject* o = (*i);

    if (!strcmp("kgmMaterial", o->vClass()))
    {
      kgmMaterial* m = (kgmMaterial*)o;

      if (m->id() == id)
        return m;
    }
  }

  return null;
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
