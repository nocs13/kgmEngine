#include "kEditor.h"

#include "../../kgmBase/kgmLog.h"
#include "../../kgmBase/kgmConvert.h"
#include "../../kgmSystem/kgmSystem.h"
#include "../../kgmSystem/kgmThread.h"
#include "../kgmGameBase.h"
#include "../kgmGameMap.h"

using namespace kgmGameEditor;

static float cam_scale = 1.0;

kLine g_line;

enum MENUEVENT
{
  ME_QUIT,
  ME_MAP_OPEN,
  ME_MAP_SAVE,
  ME_EDIT_CLONE,
  ME_EDIT_REMOVE,
  ME_EDIT_OPTIONS,
  ME_ADD_MESH,
  ME_ADD_UNIT,
  ME_ADD_LIGHT,
  ME_ADD_ACTOR,
  ME_ADD_EFFECT,
  ME_ADD_SENSOR,
  ME_ADD_TRIGGER,
  ME_ADD_OBSTACLE,
  ME_RUN_RUN,
  ME_VIEW_OBJECTS,
  ME_VIEW_PERSPECTIVE,
  ME_VIEW_FRONT,
  ME_VIEW_BACK,
  ME_OPTIONS_DATABASE,
  ME_HELP_ABOUT
};

kEditor::kEditor(kgmGameBase* g)
{
  game = g;

  game->setMsAbsolute(true);

  ms_click[0] = ms_click[1] = ms_click[2] = false;

  move_camera = false;

  cam_pos = vec3(0, 0, 0);
  cam_rot = 0.0f;

  selected = null;

  oquered = 0;
  view_mode = ViewPerspective;

  mtlLines = new kgmMaterial();

  mtlPivot = new kgmMaterial();
  mtlPivot->setShader(kgmShader::TypeNone);
  mtlPivot->m_depth = false;

  if(game->m_render)
  {
    game->m_render->setEditor(true);

    menu = new kMenu(null, this);

    menu->setSid("editor_main_menu");

    kgmGuiMenu::Item* item = menu->add("Map");
    item->add(ME_MAP_OPEN, "Open", kgmGuiMenu::Item::ClickEventCallback(this, (kgmGuiMenu::Item::ClickEventCallback::Function)&kEditor::onMapOpen));
    item->add(ME_MAP_SAVE, "Save", kgmGuiMenu::Item::ClickEventCallback(this, (kgmGuiMenu::Item::ClickEventCallback::Function)&kEditor::onMapSave));
    item->add(ME_QUIT, "Quit", kgmGuiMenu::Item::ClickEventCallback(this, (kgmGuiMenu::Item::ClickEventCallback::Function)&kEditor::onQuit));
    item = menu->add("Edit");
    item->add(ME_EDIT_CLONE, "Clone", kgmGuiMenu::Item::ClickEventCallback(this, (kgmGuiMenu::Item::ClickEventCallback::Function)&kEditor::onEditClone));
    item->add(ME_EDIT_REMOVE, "Remove", kgmGuiMenu::Item::ClickEventCallback(this, (kgmGuiMenu::Item::ClickEventCallback::Function)&kEditor::onEditRemove));
    item->add(ME_EDIT_OPTIONS, "Options", kgmGuiMenu::Item::ClickEventCallback(this, (kgmGuiMenu::Item::ClickEventCallback::Function)&kEditor::onEditOptions));
    item = menu->add("Add");
    item->add(ME_ADD_MESH, "Mesh", kgmGuiMenu::Item::ClickEventCallback(this, (kgmGuiMenu::Item::ClickEventCallback::Function)&kEditor::onAddMesh));
    item->add(ME_ADD_UNIT, "Unit", kgmGuiMenu::Item::ClickEventCallback(this, (kgmGuiMenu::Item::ClickEventCallback::Function)&kEditor::onAddUnit));
    item->add(ME_ADD_LIGHT, "Light", kgmGuiMenu::Item::ClickEventCallback(this, (kgmGuiMenu::Item::ClickEventCallback::Function)&kEditor::onAddLight));
    item->add(ME_ADD_ACTOR, "Actor", kgmGuiMenu::Item::ClickEventCallback(this, (kgmGuiMenu::Item::ClickEventCallback::Function)&kEditor::onAddActor));
    item->add(ME_ADD_EFFECT, "Effect", kgmGuiMenu::Item::ClickEventCallback(this, (kgmGuiMenu::Item::ClickEventCallback::Function)&kEditor::onAddEffect));
    item->add(ME_ADD_SENSOR, "Sensor", kgmGuiMenu::Item::ClickEventCallback(this, (kgmGuiMenu::Item::ClickEventCallback::Function)&kEditor::onAddSensor));
    item->add(ME_ADD_TRIGGER, "Trigger", kgmGuiMenu::Item::ClickEventCallback(this, (kgmGuiMenu::Item::ClickEventCallback::Function)&kEditor::onAddTrigger));
    item->add(ME_ADD_OBSTACLE, "Obstacle", kgmGuiMenu::Item::ClickEventCallback(this, (kgmGuiMenu::Item::ClickEventCallback::Function)&kEditor::onAddObstacle));
    item = menu->add("Run");
    item->add(ME_RUN_RUN, "Run", kgmGuiMenu::Item::ClickEventCallback(this, (kgmGuiMenu::Item::ClickEventCallback::Function)&kEditor::onRunRun));
    item = menu->add("View");
    item->add(ME_VIEW_OBJECTS, "Objects", kgmGuiMenu::Item::ClickEventCallback(this, (kgmGuiMenu::Item::ClickEventCallback::Function)&kEditor::onViewObjects));
    item->add(ME_VIEW_OBJECTS, "Perspective", kgmGuiMenu::Item::ClickEventCallback(this, (kgmGuiMenu::Item::ClickEventCallback::Function)&kEditor::onViewPerspective));
    item->add(ME_VIEW_OBJECTS, "Front", kgmGuiMenu::Item::ClickEventCallback(this, (kgmGuiMenu::Item::ClickEventCallback::Function)&kEditor::onViewFront));
    item->add(ME_VIEW_OBJECTS, "Left", kgmGuiMenu::Item::ClickEventCallback(this, (kgmGuiMenu::Item::ClickEventCallback::Function)&kEditor::onViewLeft));
    item->add(ME_VIEW_OBJECTS, "Top", kgmGuiMenu::Item::ClickEventCallback(this, (kgmGuiMenu::Item::ClickEventCallback::Function)&kEditor::onViewTop));
    game->m_render->add(menu);

    //gridline = new kgmVisual();
    //gridline->set(mtlLines);
    //gridline->set((kgmMesh*)(new kGridline()));
    //game->m_render->add(gridline);

    pivot = new kgmVisual();
    mtx4 mtr;
    mtr.identity();
    pivot->set(mtlPivot);
    pivot->set((kgmMesh*)(new kPivot()));
    pivot->set(&mtr);
    game->m_render->add(pivot);

    kgmVisual *vis = new kgmVisual();
    vis->set((kgmMesh*)&g_line);
    vis->set(mtlPivot);
    game->m_render->add(vis);
    vis->release();


    game->m_render->setBgColor(0xffbbaa99);
  }
}

kEditor::~kEditor()
{
  gridline->release();
  pivot->release();
  menu->release();

  mtlLines->release();
  mtlPivot->release();

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
        ((kPivot*)pivot->getMesh()->getMesh())->setPos((*i)->pos);
        pivot->set(((kPivot*)pivot->getMesh()->getMesh())->getTransform());
      }
    }
  }
}

void kEditor::select(int x, int y)
{
  kgmCamera cam = game->m_render->camera();

  iRect vp = game->m_render->viewport();

  float unit_x = (2.0f * ((float)(x - vp.x) / (vp.w - vp.x))) - 1.0f,
        unit_y = (2.0f * ((float)(y - vp.y) / (vp.h - vp.y))) - 1.0f;

  if(cam.isOrthogonal())
  {
    unit_x *= cam_scale;
    unit_y *= cam_scale;
  }

  kgmRay3d<float> ray;

  vec3 ms, mr, md;
  vec3 view = cam.mDir;
  view.normalize();
  vec3 h = view.cross(cam.mUp);
  h.normalize();
  vec3 v = h.cross(view);
  v.normalize();
  float dist = 1.0;
  float vL = tan(cam.mFov) * dist;
  float hL = vL * ((float)vp.width() / (float)vp.height());
  v = v * vL;
  h = h * hL;

  if(cam.isOrthogonal())
  {
    v.normalize();
    h.normalize();
  }

  ms = cam.mPos + view * dist + h * unit_x - v * unit_y;
  md = ms - cam.mPos;
  md.normalize();

  g_line.set(cam.mPos, ms);

  if(cam.isOrthogonal())
  {
    ray = kgmRay3d<float>(ms, cam.mDir);
  }
  else
  {
    ray = kgmRay3d<float>(cam.mPos, md);
  }

  //kgm_log() << "Ray s: " << ray.s.x << " " << ray.s.y << " " << ray.s.z << "\n";
  //kgm_log() << "Ray d: " << ray.d.x << " " << ray.d.y << " " << ray.d.z << "\n\n";

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

      if(pivot)
      {
        ((kPivot*)pivot->getMesh()->getMesh())->setPos(selected->pos);
        pivot->set(((kPivot*)pivot->getMesh()->getMesh())->getTransform());
      }

      break;
    }

    n = vec3(0, 1, 0);
    pln = plane3(n, (*i)->pos);

    if(pln.intersect(ray, c) && ((*i)->pos.distance(c) < 1.0))
    {
      selected = (*i);
      select((*i)->nam);

      if(pivot)
      {
        ((kPivot*)pivot->getMesh()->getMesh())->setPos(selected->pos);
        pivot->set(((kPivot*)pivot->getMesh()->getMesh())->getTransform());
      }

      break;
    }

    n = vec3(0, 0, 1);
    pln = plane3(n, (*i)->pos);

    if(pln.intersect(ray, c) && ((*i)->pos.distance(c) < 1.0))
    {
      selected = (*i);
      select((*i)->nam);

      if(pivot)
      {
        ((kPivot*)pivot->getMesh()->getMesh())->setPos(selected->pos);
        pivot->set(((kPivot*)pivot->getMesh()->getMesh())->getTransform());
      }

      break;
    }
  }

  if(((kPivot*)pivot->getMesh()->getMesh())->peekAxis(ray) != kPivot::AXIS_NONE)
  {
    int k = 0;
  }
}

kgmRay3d<float> kEditor::getPointRay(int x, int y)
{
  kgmCamera cam = game->m_render->camera();

  iRect vp = game->m_render->viewport();

  float unit_x = (2.0f * ((float)(x - vp.x) / (vp.w - vp.x))) - 1.0f,
        unit_y = (2.0f * ((float)(y - vp.y) / (vp.h - vp.y))) - 1.0f;

  if(cam.isOrthogonal())
  {
    unit_x *= cam_scale;
    unit_y *= cam_scale;
  }

  kgmRay3d<float> ray;

  vec3 ms, mr, md;
  vec3 view = cam.mDir;
  view.normalize();
  vec3 h = view.cross(cam.mUp);
  h.normalize();
  vec3 v = h.cross(view);
  v.normalize();
  float dist = 1.0;
  float vL = tan(cam.mFov) * dist;
  float hL = vL * ((float)vp.width() / (float)vp.height());
  v = v * vL;
  h = h * hL;

  if(cam.isOrthogonal())
  {
    v.normalize();
    h.normalize();
  }

  ms = cam.mPos + view * dist + h * unit_x - v * unit_y;
  md = ms - cam.mPos;
  md.normalize();

  if(cam.isOrthogonal())
  {
    ray = kgmRay3d<float>(ms, cam.mDir);
  }
  else
  {
    ray = kgmRay3d<float>(cam.mPos, md);
  }

  return ray;
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

  game->m_render->add(gridline);
  game->m_render->add(pivot);

  ((kPivot*)pivot->getMesh()->getMesh())->setPos(vec3(0, 0, 0));
  pivot->set(((kPivot*)pivot->getMesh()->getMesh())->getTransform());

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
      node->col = mnode.col;
      node->shp = mnode.shp;
      node->bnd = mnode.bnd;
      node->mat = mnode.mtl;
      node->shd = mnode.shd;
      node->lock = mnode.lck;

      kgmMaterial *mtl = null;

      mtl = game->getResources()->getMaterial(mnode.mtl);

      if(mtl)
        mtl->setShader(kgmShader::toType(mnode.shd));

      node->msh->set(mtl);
      game->m_render->add(node->msh);
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
      node->col = mnode.col;
      node->shp = mnode.shp;
      node->bnd = mnode.bnd;
      node->lock = mnode.lck;
      node->icn = new kgmGraphics::Icon(game->getResources()->getTexture("light_ico.tga"));
      node->geo = new kgmVisual();

      node->geo->set((kgmMesh*)(new kArrow()));
      node->geo->set(mtlLines);

      game->m_render->add(node->lgt);
      game->m_render->add(node->icn);
      game->m_render->add(node->geo);

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
      node->col = mnode.col;
      node->shp = mnode.shp;
      node->bnd = mnode.bnd;
      node->ini = mnode.ini;
      node->lock = mnode.lck;
      node->icn = new kgmGraphics::Icon(game->getResources()->getTexture("actor_ico.tga"));
      node->geo = new kgmVisual();

      node->geo->set((kgmMesh*)(new kArrow()));
      node->geo->set(mtlLines);

      if(node->act)
      {
        if(node->act->getVisual())
          game->getGraphics()->add(node->act->getVisual());
      }

      game->m_render->add(node->icn);
      game->m_render->add(node->geo);

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
      node->col = mnode.col;
      node->shp = mnode.shp;
      node->bnd = mnode.bnd;
      node->lock = mnode.lck;
      node->icn = new kgmGraphics::Icon(game->getResources()->getTexture("sensor_ico.tga"));
      node->geo = new kgmVisual();

      node->geo->set((kgmMesh*)(new kArrow()));
      node->geo->set(mtlLines);

      game->m_render->add(node->icn);
      game->m_render->add(node->geo);

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
      node->col = mnode.col;
      node->shp = mnode.shp;
      node->bnd = mnode.bnd;
      node->lock = mnode.lck;

      node->icn = new kgmGraphics::Icon(game->getResources()->getTexture("trigger_ico.tga"));
      node->geo = new kgmVisual();

      node->geo->set((kgmMesh*)(new kArrow()));
      node->geo->set(mtlLines);

      game->m_render->add(node->icn);
      game->m_render->add(node->geo);

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
    node.col = (*i)->col;
    node.shp = (*i)->shp;
    node.bnd = (*i)->bnd;
    node.lck = (*i)->lock;

    map.addLight(node);
  }

  for(kgmList<kNode*>::iterator i = meshes.begin(); i != meshes.end(); ++i)
  {
    kgmMaterial* mtl = (*i)->msh->getMaterial();

    kgmGameMap::Node node;

    memset(&node, 0, sizeof(node));

    node.obj = (*i)->msh;
    node.pos = (*i)->pos;
    node.rot = (*i)->rot;
    node.nam = (*i)->nam;
    node.lnk = (*i)->lnk;
    node.mtl = (*i)->mat;
    node.shd = (*i)->shd;
    node.col = (*i)->col;
    node.shp = (*i)->shp;
    node.bnd = (*i)->bnd;
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
    node.bnd = (*i)->bnd;
    node.ini = (*i)->ini;
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
    node.col = (*i)->col;
    node.shp = (*i)->shp;
    node.bnd = (*i)->bnd;
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
    node.col = (*i)->col;
    node.shp = (*i)->shp;
    node.bnd = (*i)->bnd;
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
    node.col = (*i)->col;
    node.shp = (*i)->shp;
    node.bnd = (*i)->bnd;
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
    kNode* node = new kNode(mesh);
    node->bnd = mesh->bound();
    node->nam = kgmString("Mesh_") + kgmConvert::toString((s32)(++oquered));
    node->lnk = name;
    selected = node;
    nodes.add(node);
    game->m_render->add(node->msh);

    return true;
  }

  return false;
}

bool kEditor::addUnit(kgmString type)
{
  if(type.length() < 1)
    return false;

  if(kgmGameObject::g_typ_objects.hasKey(type))
  {
    kgmGameObject::GenGo fn_new = kgmGameObject::g_typ_objects[type];

    if(fn_new)
    {
      kgmUnit* un = (kgmUnit*)fn_new(game);

      if(un)
      {
        kNode* node = new kNode(un);
        node->bnd = box3(-1, -1, -1, 1, 1, 1);
        node->nam = kgmString("Unit_") + kgmConvert::toString((s32)(++oquered));
        node->icn = new kgmGraphics::Icon(game->getResources()->getTexture("unit_ico.tga"));
        node->geo = new kgmVisual();
        node->geo->set((kgmMesh*)(new kArrow()));
        node->geo->set(mtlLines);

        //For evade remove unit from scene.
        un->timeout(-1);

        selected = node;

        game->m_render->add(node->icn);
        game->m_render->add(node->geo);

        nodes.add(node);

        game->getRender()->add(un->getVisual());

        return true;
      }
    }
  }

  return false;
}

bool kEditor::addActor(kgmString type)
{
  if(type.length() < 1)
    return false;

  kgmActor* ac = null;

  kgmString cpath = "Data";

  cpath += kgmString(kgmSystem::PathDelim);
  cpath += type;
  cpath += kgmString(".act");

#ifdef DEBUG
  kgm_log() << "kEditor: Check actor path is " << (char*)cpath << ".\n";
#endif

  const char *t = null;

  if(type.data())
    t = strrchr((const char*)type.data(), kgmSystem::PathDelimSym);
//  if(kgmSystem::isFile(cpath))

  if(t)
    ac = game->gSpawn(++t);

  if(ac)
  {
    kNode* node = new kNode(ac);

    //For evade remove unit from scene.
    ac->timeout(-1);

    node->bnd = box3(-1, -1, -1, 1, 1, 1);
    node->nam = kgmString("Actor_") + kgmConvert::toString((s32)(++oquered));
    node->icn = new kgmGraphics::Icon(game->getResources()->getTexture("actor_ico.tga"));
    node->geo = new kgmVisual();
    node->geo->set((kgmMesh*)(new kArrow()));
    node->geo->set(mtlLines);
    node->ini = type;

    selected = node;

    game->m_render->add(node->icn);
    game->m_render->add(node->geo);

    nodes.add(node);

    game->getRender()->add(ac->getVisual());
    
    return true;
  }

  return false;
}

bool kEditor::addEffect(kgmString type)
{
  if(type.length() < 1)
    return false;

  if(kgmGameObject::g_typ_objects.hasKey(type))
  {
    kgmGameObject::GenGo fn_new = kgmGameObject::g_typ_objects[type];

    if(fn_new)
    {
      kgmEffect* eff = (kgmEffect*)fn_new(game);

      if(eff)
      {
        kNode* node = new kNode(eff);
        node->bnd = box3(-1, -1, -1, 1, 1, 1);
        node->nam = kgmString("Effect_") + kgmConvert::toString((s32)(++oquered));
        node->icn = new kgmGraphics::Icon(game->getResources()->getTexture("effect_ico.tga"));
        node->geo = new kgmVisual();
        node->geo->set((kgmMesh*)(new kArrow()));
        node->geo->set(mtlLines);

        //For evade remove unit from scene.
        eff->timeout(-1);

        selected = node;

        game->m_render->add(node->icn);
        game->m_render->add(node->geo);

        nodes.add(node);

        game->getRender()->add(eff->getVisual());

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
        node->geo = new kgmVisual();
        node->geo->set((kgmMesh*)(new kArrow()));
        node->geo->set(mtlLines);

        selected = node;

        game->m_render->add(node->icn);
        game->m_render->add(node->geo);

        nodes.add(node);

        game->getRender()->add(sn->getVisual());

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

void kEditor::onIdle()
{
  kgmEvent::onIdle();

  static u32 ctick = 0;
  static u32 cdel  = 50;

  if(kgmTime::getTicks() - ctick > cdel)
  {
    for(kgmList<kNode*>::iterator i = nodes.begin(); i != nodes.end(); ++i)
    {
      if((*i)->typ == kNode::TRIGGER || (*i)->typ == kNode::SENSOR ||
         (*i)->typ == kNode::ACTOR || (*i)->typ == kNode::UNIT ||
        (*i)->typ == kNode::EFFECT)
      {
        kgmGameObject *o = (kgmGameObject*)(*i)->obj;

        if(o->valid())
          o->update(cdel);
      }
    }

    ctick = kgmTime::getTicks();
  }
}

void kEditor::onEvent(kgmEvent::Event *e)
{
  kgmEvent::onEvent(e);

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
    move_camera = false;
    game->setMsAbsolute(true);
  }
}

void kEditor::onKeyDown(int k)
{
  if(k == KEY_LCTRL)
  {
    move_camera = true;
    game->setMsAbsolute(false);
  }
}

void kEditor::onMsLeftUp(int k, int x, int y)
{
  ms_click[0] = false;
}

void kEditor::onMsLeftDown(int k, int x, int y)
{
  ms_click[0] = true;

  if(move_camera)
    return;

  if(nodes.length() > 0)
  {
    if(selected)
    {
      mtx4 m;
      m.identity();
      m.translate(selected->pos);

      if(pivot)
        pivot->set(m);
    }
  }

  select(x, y);

  //if(selected && pivot && ((kPivot*)pivot->getMesh())->axis != kPivot::AXIS_NONE)
  //  game->setMsAbsolute(true);
}

void kEditor::onMsRightUp(int k, int x, int y)
{
  ms_click[1] = false;
}

void kEditor::onMsRightDown(int k, int x, int y)
{
  ms_click[1] = true;
}

void kEditor::onMsMove(int k, int x, int y)
{
  u32 paxes = ((kPivot*)pivot->getMesh()->getMesh())->getAxis();

  if(game->m_render && move_camera)
  {
    if(ms_click[0])
    {
      kgmCamera& cam = game->m_render->camera();

      if(view_mode == ViewPerspective)
      {
        cam_rot += 0.001 * x;

        if(cam_rot > 2 * PI)
          cam_rot = 0;

        if(cam_rot < -2 * PI)
          cam_rot = 0;

        cam.mDir = vec3(cos(cam_rot), sin(cam_rot), 0.0);
        cam.mDir.normalize();
        cam.mPos = cam.mPos + cam.mDir * 0.1 * y;
      }
      else if(view_mode == ViewFront)
      {
        cam.mPos.y -= 0.1 * x;
        cam.mPos.z += 0.1 * y;
      }
      else if(view_mode == ViewLeft)
      {
        cam.mPos.x += 0.1 * x;
        cam.mPos.z += 0.1 * y;
      }
      else if(view_mode == ViewTop)
      {
        cam.mPos.y += 0.1 * y;
        cam.mPos.x -= 0.1 * x;
      }

      cam.update();
    }
    else if(ms_click[1])
    {
      if(game->getKeyState(KEY_Z) && selected)
      {
        kgmCamera& cam = game->m_render->camera();

        if(cam.isOrthogonal())
        {
          switch(view_mode)
          {
          case ViewFront:
            selected->pos.y += x * cam_scale;
            selected->pos.z += y * cam_scale;
            break;
          case ViewLeft:
            selected->pos.x += x * cam_scale;
            selected->pos.z += y * cam_scale;
            break;
          case ViewTop:
            selected->pos.x += x * cam_scale;
            selected->pos.y += y * cam_scale;
            break;
          }
        }
        else
        {
          selected->pos.x += 0.01 * x;
          selected->pos.y += 0.01 * y;
        }

        mtx4 m;
        m.identity();
        m.translate(selected->pos);

        if(selected->typ == kNode::MESH)
        {
          selected->msh->set(m);
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

        if(view_mode == ViewPerspective)
        {
          cam.mPos.z += 0.01 * -y;
        }
        else if(view_mode == ViewFront)
        {
          cam_scale += 0.1 * y;

          if(cam_scale < 1.0)
            cam_scale = 1.0;

          cam.scale(cam_scale);
          cam.mPos.x = cam_scale;
        }
        else if(view_mode == ViewLeft)
        {
          cam_scale += 0.1 * y;

          if(cam_scale < 1.0)
            cam_scale = 1.0;

          cam.scale(cam_scale);
          cam.mPos.y = cam_scale;
        }
        else if(view_mode == ViewTop)
        {
          cam_scale += 0.1 * y;

          if(cam_scale < 1.0)
            cam_scale = 1.0;

          cam.scale(cam_scale);
          cam.mPos.z = cam_scale;
        }

        cam.update();
      }
    }
  }
  else if(selected && pivot && paxes != kPivot::AXIS_NONE && ms_click[0] && !selected->lock)
  {
    kgmRay3d<float> ray = getPointRay(x, y);

    kgmCamera cam = game->m_render->camera();

    vec3 pt = ray.s + ray.d * cam.mPos.distance(((kPivot*)pivot->getMesh()->getMesh())->pos);
    vec3 pr, tm;
    line lax;
    plane pln;
    float prdist;

    switch(paxes)
    {
    case kPivot::AXIS_X:
      tm = ((kPivot*)pivot->getMesh()->getMesh())->pos + vec3(1, 0, 0);
      lax = line(((kPivot*)pivot->getMesh()->getMesh())->pos, tm);
      break;
    case kPivot::AXIS_Y:
      tm = ((kPivot*)pivot->getMesh()->getMesh())->pos + vec3(0, 1, 0);
      lax = line(((kPivot*)pivot->getMesh()->getMesh())->pos, tm);
      break;
    case kPivot::AXIS_Z:
      tm = ((kPivot*)pivot->getMesh()->getMesh())->pos + vec3(0, 0, 1);
      lax = line(((kPivot*)pivot->getMesh()->getMesh())->pos, tm);
      break;
    }

    pr = lax.projection(pt);
    prdist = ((kPivot*)pivot->getMesh()->getMesh())->pos.distance(pr);
    prdist *= 2;

    vec3 dir = pr - ((kPivot*)pivot->getMesh()->getMesh())->pos;
    dir.normalize();

    //selected->setPosition(selected->pos + dir * prdist);
    selected->setPosition(pr);

    mtx4 m;
    m.identity();
    m.translate(selected->pos);
    ((kPivot*)pivot->getMesh()->getMesh())->setPos(selected->pos);
    pivot->set(m);
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

void kEditor::onEditClone()
{
  if(!selected)
    return;

  kNode* node = new kNode(*selected);

  switch(selected->typ)
  {
  case kNode::MESH:
    node->nam = kgmString("Mesh_") + kgmConvert::toString((s32)(++oquered));
    game->m_render->add(node->msh);

    break;
  case kNode::LIGHT:
    node->nam = kgmString("Light_") + kgmConvert::toString((s32)(++oquered));
    node->icn = new kgmGraphics::Icon(game->getResources()->getTexture("light_ico.tga"));
    node->geo = new kgmVisual();
    node->geo->set((kgmMesh*)(new kArrow()));

    game->m_render->add(node->lgt);

    break;
  case kNode::SENSOR:
    node->nam = kgmString("Sensor_") + kgmConvert::toString((s32)(++oquered));
    node->icn = new kgmGraphics::Icon(game->getResources()->getTexture("sensor_ico.tga"));
    node->geo = new kgmVisual();
    node->geo->set((kgmMesh*)(new kArrow()));

    break;
  case kNode::TRIGGER:
    node->nam = kgmString("Trigger_") + kgmConvert::toString((s32)(++oquered));
    node->icn = new kgmGraphics::Icon(game->getResources()->getTexture("trigger_ico.tga"));
    node->geo = new kgmVisual();
    node->geo->set((kgmMesh*)(new kArrow()));

    break;
  case kNode::ACTOR:
    node->nam = kgmString("Actor_") + kgmConvert::toString((s32)(++oquered));

    if(((kgmActor*)node->act)->getVisual())
      game->m_render->add(((kgmActor*)node->act)->getVisual());

    break;
  }

  if(node->icn)
    game->m_render->add(node->icn);

  if(node->geo)
  {
    node->geo->set(mtlLines);
    game->m_render->add(node->geo);
  }

  node->setPosition(selected->pos + vec3(0.1, 0, 0));
  node->setRotation(selected->rot);
  nodes.add(node);

  selected = node;
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
  case kNode::UNIT:
    vop = new kViewOptionsForUnit(selected, 50, 50, 300, 300);
    break;
  case kNode::LIGHT:
    vop = new kViewOptionsForLight(selected, 50, 50, 250, 300);
    break;
  case kNode::ACTOR:
    vop = new kViewOptionsForActor(selected, 50, 50, 300, 300);
    break;
  case kNode::EFFECT:
    vop = new kViewOptionsForEffect(selected, 50, 50, 300, 300);
    break;
  case kNode::SENSOR:
    vop = new kViewOptionsForSensor(selected, 50, 50, 300, 300);
    break;
  case kNode::TRIGGER:
    vop = new kViewOptionsForTrigger(selected, 50, 50, 300, 300);
    break;
  case kNode::OBSTACLE:
    vop = new kViewOptionsForObstacle(selected, 50, 50, 250, 300);
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

void kEditor::onAddUnit()
{
  kViewObjects* vs = new kViewObjects(this, 1, 50, 200, 300);

  vs->setSelectCallback(kViewObjects::SelectCallback(this, (kViewObjects::SelectCallback::Function)&kEditor::addUnit));

  for(int i = 0; i < kgmGameObject::g_list_units.length(); i++)
  {
    kgmString s = kgmGameObject::g_list_units[i];
    vs->addItem(s);
  }

  game->guiAdd(vs);
  vs->release();
}

void kEditor::onAddLight()
{
  kgmLight* l = new kgmLight();

  kNode* node = new kNode(l);
  node->bnd = box3(-1, -1, -1, 1, 1, 1);
  node->nam = kgmString("Light_") + kgmConvert::toString((s32)(++oquered));
  node->icn = new kgmGraphics::Icon(game->getResources()->getTexture("light_ico.tga"));
  node->geo = new kgmVisual();
  node->geo->set((kgmMesh*)(new kArrow()));
  node->geo->set(mtlLines);

  selected = node;
  nodes.add(node);

  game->m_render->add(l);
  game->m_render->add(node->icn);
  //game->m_render->add(node->geo);

  //l->release();
}

void kEditor::onAddActor()
{
  kFileDialog *fdd = new kFileDialog();
  fdd->showHidden(false);
  game->guiAdd(fdd);

  fdd->setFilter(".act");
  fdd->changeLocation(false);
  fdd->forOpen(game->getSettings()->get("Path"), kFileDialog::ClickEventCallback(this, (kFileDialog::ClickEventCallback::Function)&kEditor::addActor));
}

void kEditor::onAddEffect()
{
  kViewObjects* vs = new kViewObjects(this, 1, 50, 200, 300);

  vs->setSelectCallback(kViewObjects::SelectCallback(this, (kViewObjects::SelectCallback::Function)&kEditor::addEffect));

  for(int i = 0; i < kgmGameObject::g_list_effects.length(); i++)
  {
    kgmString s = kgmGameObject::g_list_effects[i];
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

void kEditor::onAddTrigger()
{
  kgmTrigger* tr = new kgmTrigger();

  kNode* node = new kNode(tr);
  node->bnd = box3(-1, -1, -1, 1, 1, 1);
  node->nam = kgmString("Trigger_") + kgmConvert::toString((s32)(++oquered));
  node->icn = new kgmGraphics::Icon(game->getResources()->getTexture("light_ico.tga"));
  node->geo = new kgmVisual();
  node->geo->set((kgmMesh*)(new kArrow()));
  node->geo->set(mtlLines);


  selected = node;
  nodes.add(node);

  game->m_render->add(node->icn);
  game->m_render->add(node->geo);

  game->getLogic()->add(tr);
}

void kEditor::onAddObstacle()
{
  kgmObstacle* o = new kgmObstacle();

  kNode* node = new kNode(o);
  node->bnd = box3(-1, -1, -1, 1, 1, 1);
  node->nam = kgmString("Obstacle_") + kgmConvert::toString((s32)(++oquered));
  node->icn = new kgmGraphics::Icon(game->getResources()->getTexture("obstacle_ico.tga"));
  node->geo = new kgmVisual();
  node->geo->set((kgmMesh*)(new kArrow()));
  node->geo->set(mtlLines);

  selected = node;
  nodes.add(node);

  game->m_render->add(node->icn);
  game->m_render->add(node->geo);
}

static int runRun(void *cmd)
{
  if(!cmd)
    return 1;

  int res = system((char*)cmd);

  return res;
}

void kEditor::onRunRun()
{
  kgmString path;

  kgmSystem::getTemporaryDirectory(path);

  if(path.length() < 1)
    path = ".";

  path += kgmSystem::PathDelim;

  path += "~kgmrun.map";

  mapSave(path);

  kgmString cmd, exe;

  if(kgmSystem::getProcessPath(exe) < 1)
    return;

  cmd = exe;
  cmd += " mapfile ";
  cmd += path;

  //int res = system(cmd.data());
  kgmInstThread thread(cmd.data(), runRun);

  thread.join();

  kgmFile::remove_file(path);
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

void kEditor::onViewPerspective()
{
  view_mode = ViewPerspective;
  kgmCamera& cam = game->m_render->camera();

  cam.mDir = vec3( 0.5, 0.5, -0.3).normal();
  cam.mUp  = vec3(0, 0, 1);
  cam.setOrthogonal(false);
  cam.update();

  cam_scale = 1.0;
}

void kEditor::onViewFront()
{
  view_mode = ViewFront;
  kgmCamera& cam = game->m_render->camera();

  cam.mDir = vec3(-1, 0, 0);
  cam.mUp  = vec3(0, 0, 1);
  cam.setOrthogonal(true);
  cam.update();
}

void kEditor::onViewLeft()
{
  view_mode = ViewLeft;
  kgmCamera& cam = game->m_render->camera();

  cam.mDir = vec3(0, -1, 0);
  cam.mUp  = vec3(0, 0, 1);
  cam.setOrthogonal(true);
  cam.update();
}

void kEditor::onViewTop()
{
  view_mode = ViewTop;
  kgmCamera& cam = game->m_render->camera();

  cam.mDir = vec3(0, 0, -1);
  cam.mUp  = vec3(0, 1,  0);
  cam.setOrthogonal(true);
  cam.update();
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
