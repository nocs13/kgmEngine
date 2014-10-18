#include "kEditor.h"

#include "../../kgmBase/kgmLog.h"
#include "../../kgmBase/kgmConvert.h"
#include "../../kgmSystem/kgmSystem.h"
#include "../kgmGameBase.h"
#include "../kgmGameMap.h"

using namespace kgmGameEditor;

enum MENUEVENT
{
  ME_QUIT,
  ME_MAP_OPEN,
  ME_MAP_SAVE,
  ME_EDIT_REMOVE,
  ME_EDIT_DUPLICATE,
  ME_EDIT_OPTIONS,
  ME_ADD_MESH,
  ME_ADD_LIGHT,
  ME_ADD_ACTOR,
  ME_ADD_SENSOR,
  ME_ADD_OBJECT,
  ME_ADD_TRIGGER,
  ME_RUN_RUN,
  ME_VIEW_OBJECTS,
  ME_OPTIONS_DATABASE,
  ME_HELP_ABOUT
};

kEditor::kEditor(kgmGameBase* g)
{
  game = g;

  game->setMsAbsolute(true);

  ms_click[0] = ms_click[1] = ms_click[2] = false;
  ctrl_left = false;

  cam_pos = vec3(0, 0, 0);
  cam_rot = 0.0f;

  selected = null;

  oquered = 0;

  mtlLines = new kgmMaterial();
  mtlLines->m_shader = kgmMaterial::ShaderNone;

  if(game->m_render)
  {
    game->m_render->setEditor(true);

    menu = new kMenu(null, this);
    kgmGuiMenu::Item* item = menu->add("Map");
    item->add(ME_MAP_OPEN, "Open", kgmGuiMenu::Item::ClickEventCallback(this, (kgmGuiMenu::Item::ClickEventCallback::Function)&kEditor::onMapOpen));
    item->add(ME_MAP_SAVE, "Save", kgmGuiMenu::Item::ClickEventCallback(this, (kgmGuiMenu::Item::ClickEventCallback::Function)&kEditor::onMapSave));
    item->add(ME_QUIT, "Quit", kgmGuiMenu::Item::ClickEventCallback(this, (kgmGuiMenu::Item::ClickEventCallback::Function)&kEditor::onQuit));
    item = menu->add("Edit");
    item->add(ME_EDIT_REMOVE, "Remove", kgmGuiMenu::Item::ClickEventCallback(this, (kgmGuiMenu::Item::ClickEventCallback::Function)&kEditor::onEditRemove));
    //item->add(ME_EDIT_DUPLICATE, "Duplicate", kgmGuiMenu::Item::ClickEventCallback(this, (kgmGuiMenu::Item::ClickEventCallback::Function)&kEditor::onEditDuplicate));
    item->add(ME_EDIT_OPTIONS, "Options", kgmGuiMenu::Item::ClickEventCallback(this, (kgmGuiMenu::Item::ClickEventCallback::Function)&kEditor::onEditOptions));
    item = menu->add("Add");
    item->add(ME_ADD_MESH, "Mesh", kgmGuiMenu::Item::ClickEventCallback(this, (kgmGuiMenu::Item::ClickEventCallback::Function)&kEditor::onAddMesh));
    item->add(ME_ADD_LIGHT, "Light", kgmGuiMenu::Item::ClickEventCallback(this, (kgmGuiMenu::Item::ClickEventCallback::Function)&kEditor::onAddLight));
    item->add(ME_ADD_ACTOR, "Actor", kgmGuiMenu::Item::ClickEventCallback(this, (kgmGuiMenu::Item::ClickEventCallback::Function)&kEditor::onAddActor));
    item->add(ME_ADD_SENSOR, "Sensor", kgmGuiMenu::Item::ClickEventCallback(this, (kgmGuiMenu::Item::ClickEventCallback::Function)&kEditor::onAddSensor));
    item->add(ME_ADD_OBJECT, "Object", kgmGuiMenu::Item::ClickEventCallback(this, (kgmGuiMenu::Item::ClickEventCallback::Function)&kEditor::onAddObject));
    item->add(ME_ADD_TRIGGER, "Trigger", kgmGuiMenu::Item::ClickEventCallback(this, (kgmGuiMenu::Item::ClickEventCallback::Function)&kEditor::onAddTrigger));
    item = menu->add("Run");
    item->add(ME_RUN_RUN, "Run", kgmGuiMenu::Item::ClickEventCallback(this, (kgmGuiMenu::Item::ClickEventCallback::Function)&kEditor::onRunRun));
    item = menu->add("View");
    item->add(ME_VIEW_OBJECTS, "Objects", kgmGuiMenu::Item::ClickEventCallback(this, (kgmGuiMenu::Item::ClickEventCallback::Function)&kEditor::onViewObjects));
    game->m_render->add(menu);

    gridline = new kGridline();
    game->m_render->add(gridline, mtlLines);

    pivot = new kPivot();
    pivot->setPos(vec3(0, 0, 0));
    game->m_render->add(pivot, mtlLines);
    game->m_render->set(pivot, pivot->getTransform());

    game->m_render->setBgColor(0xffbbaa99);

    logView = new kgmVisual();
    logView->m_typerender = kgmVisual::RenderText;
    kgmText* text = new kgmText();
    logView->set(text);
    text->release();
    //logView->m_text->m_text = "mouse point: ";
    logView->m_text->m_rect.y += 100;
    game->m_render->add(logView);
  }
}

kEditor::~kEditor()
{
  gridline->release();
  pivot->release();
  menu->release();

  logView->remove();
  logView->release();

  mtlLines->release();

  kgmGameObject::g_typ_objects.clear();
}

void kEditor::clear()
{
  game->m_render->clear();
  game->getPhysics()->clear();

  for(int i = 0; i < nodes.size(); i++)
  {
    nodes[i]->release();
  }

  nodes.clear();

  oquered = 0;
}

void kEditor::select(kgmString name)
{
  for(kgmList<kNode*>::iterator i = nodes.begin(); i != nodes.end(); ++i)
  {
    if((*i)->nam == name)
    {
      selected = (*i);

      if(pivot)
      {
        pivot->setPos((*i)->pos);
        game->m_render->set(pivot, pivot->getTransform());
      }
    }
  }
}

void kEditor::select(int x, int y)
{
  iRect vp = game->m_render->viewport();

  float unit_x = (2.0f * ((float)(x - vp.x) / (vp.w - vp.x))) - 1.0f,
        unit_y = (2.0f * ((float)(y - vp.y) / (vp.h - vp.y))) - 1.0f;

  kgmCamera cam = game->m_render->camera();
  kgmRay3d<float> ray;

  {
    vec3 ms, mr, md;
    vec3 view = cam.mDir;
    view.normalize();
    vec3 h = view.cross(cam.mUp);
    h.normalize();
    vec3 v = h.cross(view);
    v.normalize();
    float dist = 0.1;
    float vL = tan(cam.mFov) * dist;
    float hL = vL * ((float)vp.width() / (float)vp.height());
    v = v * vL;
    h = h * hL;

    ms = cam.mPos + view * dist + h * unit_x - v * unit_y;
    md = ms - cam.mPos;
    md.normalize();

    ray = kgmRay3d<float>(cam.mPos, md);
  }

  for(kgmList<kNode*>::iterator i = nodes.begin(); i != nodes.end(); ++i)
  {
    vec3  c, n;
    plane3 pln;

    n = vec3(1, 0, 0);
    pln = plane3(n, (*i)->pos);

    if((*i)->lock)
      continue;

    if(pln.intersect(ray, c) && ((*i)->pos.distance(c) < 1.0))
    {
      selected = (*i);
      select((*i)->nam);
      pivot->setPos(selected->pos);
      game->m_render->set(pivot, pivot->getTransform());

      break;
    }

    n = vec3(0, 1, 0);
    pln = plane3(n, (*i)->pos);

    if(pln.intersect(ray, c) && ((*i)->pos.distance(c) < 1.0))
    {
      selected = (*i);
      select((*i)->nam);
      pivot->setPos(selected->pos);
      game->m_render->set(pivot, pivot->getTransform());

      break;
    }

    n = vec3(0, 0, 1);
    pln = plane3(n, (*i)->pos);

    if(pln.intersect(ray, c) && ((*i)->pos.distance(c) < 1.0))
    {
      selected = (*i);
      select((*i)->nam);
      pivot->setPos(selected->pos);
      game->m_render->set(pivot, pivot->getTransform());

      break;
    }
  }

  if(pivot->peekAxis(ray) != kPivot::AXIS_NONE)
  {
    int k = 0;
  }
}

kgmRay3d<float> kEditor::getPointRay(int x, int y)
{
  iRect vp = game->m_render->viewport();

  float unit_x = (2.0f * ((float)(x - vp.x) / (vp.w - vp.x))) - 1.0f,
  unit_y = (2.0f * ((float)(y - vp.y) / (vp.h - vp.y))) - 1.0f;

  kgmCamera cam = game->m_render->camera();
  kgmRay3d<float> ray;

  vec3 ms, mr, md;
  vec3 view = cam.mDir;
  view.normalize();
  vec3 h = view.cross(cam.mUp);
  h.normalize();
  vec3 v = h.cross(view);
  v.normalize();
  float dist = 0.1;
  float vL = tan(cam.mFov) * dist;
  float hL = vL * ((float)vp.width() / (float)vp.height());
  v = v * vL;
  h = h * hL;

  ms = cam.mPos + view * dist + h * unit_x - v * unit_y;
  md = ms - cam.mPos;
  md.normalize();

  return kgmRay3d<float>(cam.mPos, md);
}

bool kEditor::mapOpen(kgmString s)
{
  kgmFile file;

  if(!kgmSystem::isFile(s) || kgmSystem::isDirectory(s))
    return false;

  if(!file.open(s, kgmFile::Read))
    return false;

  kgmMemory<u8> mem;

  mem.alloc(file.length());
  file.read(mem.data(), mem.length());
  file.close();

  kgmXml xml;

  xml.open(mem);
  mem.clear();

  selected = null;

  clear();

  game->m_render->add(gridline, null);
  game->m_render->add(pivot, mtlLines);

  pivot->setPos(vec3(0, 0, 0));
  game->m_render->set(pivot, pivot->getTransform());


  kNode* node = null;

  oquered = 0;

  kgmString ntype;

  kgmGameMap map(game, kgmGameMap::OpenRead);

  map.open(xml);

  kgmGameMap::Node mnode = map.next();

  while(mnode.obj)
  {
    if(mnode.typ == kgmGameMap::NodeMsh)
    {
      oquered++;
      node = new kNode((kgmMesh*)mnode.obj);

      node->nam = mnode.nam;
      node->lnk = mnode.lnk;
      node->bnd = mnode.bnd;
      node->lock = mnode.lck;
      game->m_render->add(node->msh, null);
      nodes.add(node);

      node->setPosition(mnode.pos);
      node->setRotation(mnode.rot);
    }
    else if(mnode.typ == kgmGameMap::NodeLgt)
    {
      oquered++;
      node = new kNode((kgmLight*)mnode.obj);

      node->nam = mnode.nam;
      node->bnd = mnode.bnd;
      node->lock = mnode.lck;
      node->icn = new kgmGraphics::Icon(game->getResources()->getTexture("light_ico.tga"));
      node->geo = new kArrow();

      game->m_render->add(node->lgt);
      game->m_render->add(node->icn);
      game->m_render->add(node->geo, mtlLines);

      nodes.add(node);
      node->setPosition(mnode.pos);
      node->setRotation(mnode.rot);
    }
    else if(mnode.typ == kgmGameMap::NodeCam)
    {
      game->getRender()->camera().mPos = ((kgmCamera*)mnode.obj)->mPos;
      game->getRender()->camera().mDir = ((kgmCamera*)mnode.obj)->mDir;
      game->getRender()->camera().mFov = ((kgmCamera*)mnode.obj)->mFov;

      mnode.obj->release();

      game->getRender()->camera().update();
    }
    else if(mnode.typ == kgmGameMap::NodeAct)
    {
      oquered++;
      node = new kNode((kgmActor*)mnode.obj);

      node->bnd = mnode.bnd;
      node->nam = mnode.nam;
      node->lock = mnode.lck;
      node->icn = new kgmGraphics::Icon(game->getResources()->getTexture("actor_ico.tga"));
      node->geo = new kArrow();

      game->m_render->add(node->icn);
      game->m_render->add(node->geo, mtlLines);

      nodes.add(node);
      node->setPosition(mnode.pos);
      node->setRotation(mnode.rot);
    }
    else if(mnode.typ == kgmGameMap::NodeSns)
    {
      oquered++;
      node = new kNode((kgmSensor*)mnode.obj);

      node->bnd = mnode.bnd;
      node->nam = mnode.nam;
      node->lock = mnode.lck;
      node->icn = new kgmGraphics::Icon(game->getResources()->getTexture("sensor_ico.tga"));
      node->geo = new kArrow();

      game->m_render->add(node->icn);
      game->m_render->add(node->geo, mtlLines);

      nodes.add(node);
      node->setPosition(mnode.pos);
      node->setRotation(mnode.rot);
    }
    else if(mnode.typ == kgmGameMap::NodeTrg)
    {
      oquered++;
      node = new kNode((kgmTrigger*)mnode.obj);

      node->nam = mnode.nam;
      node->bnd = mnode.bnd;
      node->lock = mnode.lck;

      node->icn = new kgmGraphics::Icon(game->getResources()->getTexture("trigger_ico.tga"));
      node->geo = new kArrow();

      game->m_render->add(node->icn);
      game->m_render->add(node->geo, mtlLines);

      nodes.add(node);
      node->setPosition(mnode.pos);
      node->setRotation(mnode.rot);
    }
    else if(mnode.typ == kgmGameMap::NodeObj)
    {
      oquered++;
      node = new kNode((kgmGameObject*)mnode.obj);

      node->bnd = mnode.bnd;
      node->nam = mnode.nam;
      node->lock = mnode.lck;
      node->icn = new kgmGraphics::Icon(game->getResources()->getTexture("object_ico.tga"));
      node->geo = new kArrow();


      game->m_render->add(node->icn);
      game->m_render->add(node->geo, mtlLines);

      nodes.add(node);
      node->setPosition(mnode.pos);
      node->setRotation(mnode.rot);
    }

    mnode = map.next();
  }

  xml.close();

  selected = null;

  return true;
}

bool kEditor::mapSave(kgmString s)
{
  kgmXml     xml;
  kgmGameMap map(game, kgmGameMap::OpenWrite);

  map.open(xml);

  kgmList<kNode*> meshes;
  kgmList<kNode*> lights;
  kgmList<kNode*> actors;
  kgmList<kNode*> sensors;
  kgmList<kNode*> objects;
  kgmList<kNode*> triggers;
  kgmList<kNode*> materials;

  for(kgmList<kNode*>::iterator i = nodes.begin(); i != nodes.end(); ++i)
  {
    switch ((*i)->typ)
    {
    case kNode::MESH:
      meshes.add(*i);
      continue;
    case kNode::LIGHT:
      lights.add(*i);
      continue;
    case kNode::ACTOR:
      actors.add(*i);
      continue;
    case kNode::SENSOR:
      sensors.add(*i);
      continue;
    case kNode::OBJECT:
      objects.add(*i);
      continue;
    case kNode::TRIGGER:
      triggers.add(*i);
      continue;
    default:
      continue;
    }
  }

  kgmCamera& mcam = game->getRender()->camera();

  kgmGameMap::Node node;

  node.obj = &mcam;
  node.nam = "main_camera";

  map.addCamera(node);

  //fprintf(f, " <kgmCamera name='main_camera' active='true'>\n");
  //fprintf(f, "  <Position value='%f %f %f'/>\n", mcam.mPos.x, mcam.mPos.y, mcam.mPos.z);
  //fprintf(f, "  <Direction value='%f %f %f'/>\n", mcam.mDir.x, mcam.mDir.y, mcam.mDir.z);
  //fprintf(f, " </kgmCamera>\n");

  for(kgmList<kNode*>::iterator i = lights.begin(); i != lights.end(); ++i)
  {
    kgmGameMap::Node node;

    node.obj = (*i)->lgt;
    node.pos = (*i)->pos;
    node.rot = (*i)->rot;
    node.nam = (*i)->nam;
    node.lck = (*i)->lock;

    map.addLight(node);
  }

  for(kgmList<kNode*>::iterator i = meshes.begin(); i != meshes.end(); ++i)
  {
    kgmMaterial* mtl = game->m_render->getMeshMaterial((*i)->msh);

    kgmGameMap::Node node;

    node.obj = (*i)->msh;
    node.pos = (*i)->pos;
    node.rot = (*i)->rot;
    node.nam = (*i)->nam;
    node.lnk = (*i)->lnk;
    node.mtl = (*i)->mat;
    node.lck = (*i)->lock;

    map.addMesh(node);
  }

  for(kgmList<kNode*>::iterator i = actors.begin(); i != actors.end(); ++i)
  {
    kgmGameMap::Node node;

    node.obj = (*i)->act;
    node.pos = (*i)->pos;
    node.rot = (*i)->rot;
    node.nam = (*i)->nam;
    node.lck = (*i)->lock;

    map.addActor(node);
  }

  for(kgmList<kNode*>::iterator i = sensors.begin(); i != sensors.end(); ++i)
  {
    kgmGameMap::Node node;

    node.obj = (*i)->sns;
    node.pos = (*i)->pos;
    node.rot = (*i)->rot;
    node.nam = (*i)->nam;
    node.lck = (*i)->lock;

    map.addSensor(node);
  }

  for(kgmList<kNode*>::iterator i = objects.begin(); i != objects.end(); ++i)
  {
    kgmGameMap::Node node;

    node.obj = (*i)->obj;
    node.pos = (*i)->pos;
    node.rot = (*i)->rot;
    node.nam = (*i)->nam;
    node.lck = (*i)->lock;

    map.addGameObject(node);
  }

  for(kgmList<kNode*>::iterator i = triggers.begin(); i != triggers.end(); ++i)
  {
    kgmGameMap::Node node;

    node.obj = (*i)->trg;
    node.pos = (*i)->pos;
    node.rot = (*i)->rot;
    node.nam = (*i)->nam;
    node.lck = (*i)->lock;

    map.addTrigger(node);
  }

  materials.clear();
  actors.clear();
  lights.clear();
  meshes.clear();

  FILE* f = fopen(s.data(), "w");

  if(!f)
    return false;

  kgmString str;

  if(xml.toString(str) > 1)
  {
    fprintf(f, "%s", str.data());
  }

  fclose(f);

  return true;
}

bool kEditor::addMesh(kgmString path)
{
  kgmString dir, name;

  kgmSystem::splitPath(path, dir, name);

  kgmMemory<u8> mem;

  game->getResources()->getFile(name, mem);

  if(mem.length() < 1)
    return false;

  kgmXml xml(mem);
  mem.clear();

  kgmMesh* mesh = kgmGameTools::genMesh(xml);
  xml.close();

  if(mesh)
  {
    game->m_render->add(mesh, null);
    kNode* node = new kNode(mesh);
    node->bnd = mesh->bound();
    node->nam = kgmString("Mesh_") + kgmConvert::toString((s32)(++oquered));
    node->lnk = name;
    selected = node;
    nodes.add(node);

    return true;
  }

  return false;
}

bool kEditor::addActor(kgmString type)
{
  if(type.length() < 1)
    return false;

  if(kgmGameObject::g_typ_objects.hasKey(type))
  {
    kgmGameObject::GenGo fn_new = kgmGameObject::g_typ_objects[type];

    if(fn_new)
    {
      kgmActor* ac = (kgmActor*)fn_new(game);

      if(ac)
      {
        kNode* node = new kNode(ac);
        node->bnd = box3(-1, -1, -1, 1, 1, 1);
        node->nam = kgmString("Actor_") + kgmConvert::toString((s32)(++oquered));
        node->icn = new kgmGraphics::Icon(game->getResources()->getTexture("actor_ico.tga"));
        node->geo = new kArrow();

        selected = node;

        game->m_render->add(node->icn);
        game->m_render->add(node->geo, mtlLines);

        nodes.add(node);

        game->getLogic()->add(ac);
        game->getRender()->add(ac->getVisual());
        game->getPhysics()->add(ac->getBody());

        return true;
      }
    }
  }

  return false;
}

bool kEditor::addSensor(kgmString type)
{
  if(type.length() < 1)
    return false;

  if(kgmGameObject::g_typ_objects.hasKey(type))
  {
    kgmGameObject::GenGo fn_new = kgmGameObject::g_typ_objects[type];

    if(fn_new)
    {
      kgmSensor* sn = (kgmSensor*)fn_new(game);

      if(sn)
      {
        kNode* node = new kNode(sn);
        node->bnd = box3(-1, -1, -1, 1, 1, 1);
        node->nam = kgmString("Sensor_") + kgmConvert::toString((s32)(++oquered));
        node->icn = new kgmGraphics::Icon(game->getResources()->getTexture("sensor_ico.tga"));
        node->geo = new kArrow();

        selected = node;

        game->m_render->add(node->icn);
        game->m_render->add(node->geo, mtlLines);

        nodes.add(node);

        game->getLogic()->add(sn);
        game->getRender()->add(sn->getVisual());
        game->getPhysics()->add(sn->getBody());

        return true;
      }
    }
  }

  return false;
}

bool kEditor::addObject(kgmString t)
{
  kgmString type = t;

  if(type.length() < 1)
    return false;

  if(kgmGameObject::g_typ_objects.hasKey(type))
  {
    kgmGameObject* (*fn_new)(kgmIGame*) = kgmGameObject::g_typ_objects[type];

    if(fn_new)
    {
      kgmGameObject* go = (kgmGameObject*)fn_new(game);

      if(go)
      {
        kNode* node = new kNode((kgmGameObject*)go);
        node->bnd = box3(-1, -1, -1, 1, 1, 1);
        node->nam = kgmString("Object_") + kgmConvert::toString((s32)(++oquered));
        node->icn = new kgmGraphics::Icon(game->getResources()->getTexture("object_ico.tga"));
        node->geo = new kArrow();

        selected = node;

        game->m_render->add(node->icn);
        game->m_render->add(node->geo, mtlLines);

        nodes.add(node);

        game->getLogic()->add(go);
        game->getRender()->add(go->getVisual());
        game->getPhysics()->add(go->getBody());

        return true;
      }
    }
  }

  return false;
}

void kEditor::initPhysics()
{
  for(kgmList<kNode*>::iterator i = nodes.begin(); i != nodes.end(); ++i)
  {
    kNode* node = (*i);

    if(node->typ == kNode::MESH)
    {

    }
    else if(node->typ == kNode::ACTOR)
    {

    }
  }
}

void kEditor::initLogic()
{

}

void kEditor::onEvent(kgmEvent::Event *e)
{
  kgmEvent::onEvent(e);

//  if(game)
//    game->

  if(menu->visible() && game->m_msAbs)
    menu->onEvent(e);
}

void kEditor::onKeyUp(int k)
{
  if(k == KEY_ESCAPE && game->m_state != kgmIGame::State_Edit)
  {
    if(game->getPhysics())
    {
      game->getPhysics()->clear();
    }

    if(game->getLogic())
    {
      game->getLogic()->clear();
    }

    for(kgmList<kNode*>::iterator i = nodes.begin(); i != nodes.end(); ++i)
    {
      switch ((*i)->typ)
      {
      case kNode::ACTOR:
      case kNode::OBJECT:
      case kNode::SENSOR:
      case kNode::TRIGGER:
        (*i)->obj->resetToVariables();
        break;
      default:
        break;
      }
    }

    menu->show();
    game->m_state = kgmIGame::State_Edit;

    game->getRender()->camera().mPos = cam_pos_bk;
    game->getRender()->camera().mDir = cam_dir_bk;
  }

  if(k == KEY_LCTRL)
  {
    ctrl_left = false;
    game->setMsAbsolute(true);
  }
}

void kEditor::onKeyDown(int k)
{
  if(k == KEY_LCTRL)
  {
    ctrl_left = true;
    game->setMsAbsolute(false);
  }
}

void kEditor::onMsLeftUp(int k, int x, int y)
{
  //game->setMsAbsolute(true);
  ms_click[0] = false;
}

void kEditor::onMsLeftDown(int k, int x, int y)
{
  ms_click[0] = true;
  //game->setMsAbsolute(false);

  if(game->getKeyState(KEY_Z))
    return;

  if(nodes.length() > 0)
  {
    if(selected)
    {
      mtx4 m;
      m.identity();
      m.translate(selected->pos);
      game->m_render->set(pivot, m);
    }
  }

  select(x, y);

  if(selected && pivot->axis != kPivot::AXIS_NONE)
    game->setMsAbsolute(true);
}

void kEditor::onMsRightUp(int k, int x, int y)
{
  game->setMsAbsolute(true);

  ms_click[1] = false;
}

void kEditor::onMsRightDown(int k, int x, int y)
{
  ms_click[1] = true;
}

void kEditor::onMsMove(int k, int x, int y)
{
  if(selected && pivot->axis != kPivot::AXIS_NONE && ms_click[0] && !selected->lock)
  {
    kgmRay3d<float> ray = getPointRay(x, y);

    kgmCamera cam = game->m_render->camera();

    vec3 pt = ray.s + ray.d * cam.mPos.distance(pivot->pos);
    vec3 pr, tm;
    line lax;
    plane pln;
    float prdist;

    switch(pivot->axis)
    {
    case kPivot::AXIS_X:
      tm = pivot->pos + vec3(1, 0, 0);
      lax = line(pivot->pos, tm);
      break;
    case kPivot::AXIS_Y:
      tm = pivot->pos + vec3(0, 1, 0);
      lax = line(pivot->pos, tm);
      break;
    case kPivot::AXIS_Z:
      tm = pivot->pos + vec3(0, 0, 1);
      lax = line(pivot->pos, tm);
      break;
    }

    pr = lax.projection(pt);
    prdist = pivot->pos.distance(pr);
    prdist *= 2;

    vec3 dir = pr - pivot->pos;
    dir.normalize();

    selected->setPosition(selected->pos + dir * prdist);

    mtx4 m;
    m.identity();
    m.translate(selected->pos);
    pivot->pos = selected->pos;
    game->m_render->set(pivot, m);

    return;
  }
  else if(game->m_render && !game->m_msAbs)
  {
    if(ms_click[0] && ctrl_left)
    {
      kgmCamera& cam = game->m_render->camera();

      cam_rot += 0.001 * x;

      if(cam_rot > 2 * PI)
        cam_rot = 0;

      if(cam_rot < -2 * PI)
        cam_rot = 0;

      cam.mDir = vec3(cos(cam_rot), sin(cam_rot), 0.0);
      cam.mDir.normalize();
      cam.mPos = cam.mPos + cam.mDir * 0.1 * y;
      cam.update();
    }
    else if(ms_click[1])
    {
      if(game->getKeyState(KEY_Z) && selected)
      {
        selected->pos.x += 0.01 * x;
        selected->pos.y += 0.01 * y;

        mtx4 m;
        m.identity();
        m.translate(selected->pos);

        if(selected->typ == kNode::MESH)
        {
          game->m_render->set(selected->msh, m);
        }
        else if(selected->typ == kNode::LIGHT)
        {
          selected->lgt->position = selected->pos;
          selected->icn->setPosition(selected->pos);
        }
      }
      else
      {
        kgmCamera& cam = game->m_render->camera();

        cam.mPos.z += 0.01 * -y;
        cam.update();

        kgm_log() << "Y: " << y << "\n";
      }
    }
  }
}

void kEditor::onMsWheel(int k, int x, int y, int z)
{
}

void kEditor::onAction(kgmEvent *gui, int id)
{
  if(gui == menu)
  {
  }
}

void kEditor::onQuit()
{
  game->close();
}

void kEditor::onMapOpen()
{
  kFileDialog *fdd = new kFileDialog();
  fdd->showHidden(false);
  game->guiAdd(fdd);

  fdd->setFilter(".map");
  fdd->changeLocation(false);
  fdd->forOpen(game->getSettings()->get("Path"), kFileDialog::ClickEventCallback(this, (kFileDialog::ClickEventCallback::Function)&kEditor::mapOpen));
}

void kEditor::onMapSave()
{
  kFileDialog *fdd = new kFileDialog();
  fdd->showHidden(false);
  game->guiAdd(fdd);

  fdd->setFilter(".map");
  fdd->changeLocation(false);
  fdd->forSave(game->getSettings()->get("Path"), kFileDialog::ClickEventCallback(this, (kFileDialog::ClickEventCallback::Function)&kEditor::mapSave));
}

void kEditor::onEditRemove()
{
  if(!selected)
    return;

  switch(selected->typ)
  {
  case kNode::MESH:
    game->getRender()->remove(selected->msh);
    break;
  case kNode::LIGHT:
    game->getRender()->remove(selected->lgt);
    game->getRender()->remove(selected->geo);
    game->getRender()->remove(selected->icn);
    break;
  }

  nodes.erase(selected);

  selected->release();

  selected = null;
}

void kEditor::onEditDuplicate()
{
  if(!selected)
    return;

  kNode* node = null;

  switch(selected->typ)
  {
  case kNode::MESH:
    node = new kNode(selected->msh);
    node->bnd = selected->msh->bound();
    node->nam = kgmString("Mesh_") + kgmConvert::toString((s32)(++oquered));
    node->lnk = selected->lnk;

    game->m_render->add(node->msh, null);

    break;
  case kNode::LIGHT:
    node = new kNode((kgmLight*)selected->lgt->clone());
    node->bnd = box3(-1, -1, -1, 1, 1, 1);
    node->nam = kgmString("Light_") + kgmConvert::toString((s32)(++oquered));
    node->icn = new kgmGraphics::Icon(game->getResources()->getTexture("light_ico.tga"));
    node->geo = new kArrow();

    game->m_render->add(node->lgt);
    game->m_render->add(node->icn);
    game->m_render->add(node->geo, mtlLines);

    break;
  default:
    break;
  }

  if(node)
  {
    node->setPosition(selected->pos + vec3(0.1, 0, 0));
    node->setRotation(selected->rot);
    nodes.add(node);

    selected = node;
  }

}

void kEditor::onEditOptions()
{
  if(!selected)
    return;

  kViewOptions* vop = null;

  switch(selected->typ)
  {
  case kNode::MESH:
    vop = new kViewOptionsForMesh(selected, 50, 50, 250, 300);
    break;
  case kNode::LIGHT:
    vop = new kViewOptionsForLight(selected, 50, 50, 250, 300);
    break;
  case kNode::ACTOR:
    vop = new kViewOptionsForActor(selected, 50, 50, 250, 300);
    break;
  case kNode::SENSOR:
    vop = new kViewOptionsForSensor(selected, 50, 50, 250, 300);
    break;
  case kNode::OBJECT:
    vop = new kViewOptionsForObject(selected, 50, 50, 250, 300);
    break;
  case kNode::TRIGGER:
    vop = new kViewOptionsForTrigger(selected, 50, 50, 250, 300);
    break;
  }

  if(vop)
  {
    game->guiAdd(vop);
    vop->show();
    vop->release();
  }
}

void kEditor::onAddMesh()
{
  kFileDialog *fdd = new kFileDialog();
  fdd->showHidden(false);
  game->guiAdd(fdd);

  fdd->setFilter(".msh");
  fdd->changeLocation(false);
  fdd->forOpen(game->getSettings()->get("Path"), kFileDialog::ClickEventCallback(this, (kFileDialog::ClickEventCallback::Function)&kEditor::addMesh));
}

void kEditor::onAddLight()
{
  kgmLight* l = new kgmLight();

  kNode* node = new kNode(l);
  node->bnd = box3(-1, -1, -1, 1, 1, 1);
  node->nam = kgmString("Light_") + kgmConvert::toString((s32)(++oquered));
  node->icn = new kgmGraphics::Icon(game->getResources()->getTexture("light_ico.tga"));
  node->geo = new kArrow();

  selected = node;
  nodes.add(node);

  game->m_render->add(l);
  game->m_render->add(node->icn);
  game->m_render->add(node->geo, mtlLines);

  l->release();
}

void kEditor::onAddActor()
{
  kViewObjects* vs = new kViewObjects(this, 1, 50, 200, 300);
  vs->setSelectCallback(kViewObjects::SelectCallback(this, (kViewObjects::SelectCallback::Function)&kEditor::addActor));

  for(int i = 0; i < kgmGameObject::g_list_actors.length(); i++)
  {
    kgmString s = kgmGameObject::g_list_actors[i];
    vs->addItem(s);
  }

  game->guiAdd(vs);
  vs->release();
}

void kEditor::onAddSensor()
{
  kViewObjects* vs = new kViewObjects(this, 1, 50, 200, 300);
  vs->setSelectCallback(kViewObjects::SelectCallback(this, (kViewObjects::SelectCallback::Function)&kEditor::addSensor));

  for(int i = 0; i < kgmGameObject::g_list_sensors.length(); i++)
  {
    kgmString s = kgmGameObject::g_list_sensors[i];
    vs->addItem(s);
  }

  game->guiAdd(vs);
  vs->release();
}

void kEditor::onAddObject()
{
  kViewObjects* vs = new kViewObjects(this, 1, 50, 200, 300);
  vs->setSelectCallback(kViewObjects::SelectCallback(this, (kViewObjects::SelectCallback::Function)&kEditor::addObject));

  for(int i = 0; i < kgmGameObject::g_list_objects.length(); i++)
  {
    kgmString s = kgmGameObject::g_list_objects[i];
    vs->addItem(s);
  }

  game->guiAdd(vs);
  vs->release();
}

void kEditor::onAddTrigger()
{
  kgmTrigger* tr = new kgmTrigger();

  kNode* node = new kNode(tr);
  node->bnd = box3(-1, -1, -1, 1, 1, 1);
  node->nam = kgmString("Trigger_") + kgmConvert::toString((s32)(++oquered));
  node->icn = new kgmGraphics::Icon(game->getResources()->getTexture("light_ico.tga"));
  node->geo = new kArrow();

  selected = node;
  nodes.add(node);

  game->m_render->add(node->icn);
  game->m_render->add(node->geo, mtlLines);

  game->getLogic()->add(tr);
}

void kEditor::onRunRun()
{
  menu->hide();

  cam_pos_bk = game->getRender()->camera().mPos;
  cam_dir_bk = game->getRender()->camera().mDir;

  if(game->getPhysics())
  {
    game->getPhysics()->build();
  }

  if(game->getLogic())
  {
    game->getLogic()->prepare();
  }

  game->m_state = kgmIGame::State_Play;
}

void kEditor::onViewObjects()
{
  kViewObjects* vo = new kViewObjects(this, 1, 50, 200, 300);
  vo->setSelectCallback(kViewObjects::SelectCallback(this, (kViewObjects::SelectCallback::Function)&kEditor::onSelectObject));

  for(int i = 0; i < nodes.length(); i++)
    vo->addItem(nodes[i]->nam);

  game->guiAdd(vo);
  vo->release();
}

void kEditor::onOptionsDatabase()
{
  kgmString loc = game->getSettings()->get("Path");

  if(!loc.length())
  {
    kgmString cwd;
    kgmSystem::getCurrentDirectory(cwd);
  }
  else
  {
  }
}

void kEditor::onSelectObject(kgmString id)
{
  select(id);
}
