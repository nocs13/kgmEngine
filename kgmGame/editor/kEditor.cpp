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
  ME_MAP_NEW,
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
  ME_ADD_MATERIAL,
  ME_ADD_PARTICLES,
  ME_RUN_PLAY,
  ME_RUN_STOP,
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
  dragging = null;

  oquered = 0;
  view_mode = ViewPerspective;

  gridline = null;
  pivot    = null;

  mtlLines = null;
  mtlPivot = null;

  mode_play = false;

  if(game->getRender())
  {
    game->getRender()->setEditor(true);

    menu = new kgmGuiMenu(null);

    menu->setSid("editor_main_menu");

    kgmGuiMenu::Item* item = menu->add("Map");
    item->add(ME_MAP_NEW, "New", kgmGuiMenu::Item::ClickEventCallback(this, (kgmGuiMenu::Item::ClickEventCallback::Function)&kEditor::onMapNew));
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
    item->add(ME_ADD_MATERIAL, "Material", kgmGuiMenu::Item::ClickEventCallback(this, (kgmGuiMenu::Item::ClickEventCallback::Function)&kEditor::onAddMaterial));
    item->add(ME_ADD_PARTICLES, "Particles", kgmGuiMenu::Item::ClickEventCallback(this, (kgmGuiMenu::Item::ClickEventCallback::Function)&kEditor::onAddParticles));
    item = menu->add("Run");
    item->add(ME_RUN_PLAY, "Play", kgmGuiMenu::Item::ClickEventCallback(this, (kgmGuiMenu::Item::ClickEventCallback::Function)&kEditor::onRunPlay));
    item->add(ME_RUN_STOP, "Stop", kgmGuiMenu::Item::ClickEventCallback(this, (kgmGuiMenu::Item::ClickEventCallback::Function)&kEditor::onRunStop));
    item = menu->add("View");
    item->add(ME_VIEW_OBJECTS, "Objects", kgmGuiMenu::Item::ClickEventCallback(this, (kgmGuiMenu::Item::ClickEventCallback::Function)&kEditor::onViewObjects));
    item->add(ME_VIEW_OBJECTS, "Perspective", kgmGuiMenu::Item::ClickEventCallback(this, (kgmGuiMenu::Item::ClickEventCallback::Function)&kEditor::onViewPerspective));
    item->add(ME_VIEW_OBJECTS, "Front", kgmGuiMenu::Item::ClickEventCallback(this, (kgmGuiMenu::Item::ClickEventCallback::Function)&kEditor::onViewFront));
    item->add(ME_VIEW_OBJECTS, "Left", kgmGuiMenu::Item::ClickEventCallback(this, (kgmGuiMenu::Item::ClickEventCallback::Function)&kEditor::onViewLeft));
    item->add(ME_VIEW_OBJECTS, "Top", kgmGuiMenu::Item::ClickEventCallback(this, (kgmGuiMenu::Item::ClickEventCallback::Function)&kEditor::onViewTop));
    game->guiAdd(menu);

    mtlLines = new kgmMaterial();
    mtlLines->setShader(null);
    mtlLines->shade(false);
    mtlLines->m_color = kgmMaterial::Color((u32)100, (u32)100, (u32)100, (u32)255);

    mtlPivot = new kgmMaterial();
    mtlPivot->setShader(null);
    mtlPivot->shade(false);
    mtlLines->m_color = kgmMaterial::Color((u32)200, (u32)150, (u32)100, (u32)255);

    meshGridline = new kGridline(100);
    meshGridline->rebound();

    gridline = new kgmVisual();
    gridline->set(mtlLines);
    gridline->set(meshGridline);
    gridline->lighting(false);
    game->getRender()->add(gridline);

    meshPivot = new kPivot();

    pivot = new kgmVisual();
    mtx4 mtr;
    mtr.identity();
    pivot->set(mtlPivot);
    pivot->set(meshPivot);
    pivot->set(&mtr);
    pivot->lighting(false);
    game->getRender()->add(pivot);

    textData = new kgmText();
    textData->rect(uRect(600, 200, 300, 100));
    text     = new kgmVisual();
    text->set(textData);
    game->getRender()->add(text);

    game->getRender()->setBgColor(0xffbbaa99);
  }

  m_isVisual = true;
  m_thVisual.exec((int (*)(void*))doVisUpdate, this);
}

kEditor::~kEditor()
{
  clear();

  m_isVisual = false;
  m_thVisual.join();

  game->getRender()->remove(text);
  game->getRender()->remove(pivot);
  game->getRender()->remove(gridline);

  delete pivot;
  delete gridline;
  delete textData;

  delete meshGridline;
  delete meshPivot;

  delete mtlLines;
  delete mtlPivot;

  kgmUnit::g_typ_objects.clear();
}

void kEditor::clear()
{
  if(mode_play)
    onRunStop();

  for(u32 i = nodes.length(); i > 0; i--)
  {
    kNode* node = nodes[i - 1];

    remove(node);
  }

  nodes.clear();

  oquered = 0;

  selected = null;
  dragging = null;
}

void kEditor::select(kgmString name)
{
  kgmList<kNode*>::iterator i = nodes.begin();

  do
  {
    if((*i)->nam == name)
    {
      selected = (*i);

      textData->m_text =  "Selected: ";
      textData->m_text += name;

      if(pivot)
      {
        ((kPivot*)pivot->getMesh()->getMesh())->setPos((*i)->getPosition());
        pivot->set(((kPivot*)pivot->getMesh()->getMesh())->getTransform());
      }

      break;
    }
  } while(i.next());
}

void kEditor::select(int x, int y)
{
  kgmCamera& cam = game->getRender()->camera();

  iRect vp = game->getRender()->viewport();

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
  float distance = -1.0f;
  kNode* peeked = null;

  for(kgmList<kNode*>::iterator i = nodes.begin(); i != nodes.end(); ++i)
  {
    vec3  c, n_x, n_y, n_z;
    plane3 pln_x, pln_y, pln_z;

    n_x = vec3(1, 0, 0);
    n_y = vec3(0, 1, 0);
    n_z = vec3(0, 0, 1);

    pln_x = plane3(n_x, (*i)->getPosition());
    pln_y = plane3(n_y, (*i)->getPosition());
    pln_z = plane3(n_z, (*i)->getPosition());

    if (*i == null || (*i)->typ == kNode::MATERIAL)
      continue;

    if(pln_x.intersect(ray, c) && ((*i)->getPosition().distance(c) < 1.0))
    {
      if(distance < 0.0)
      {
        peeked = (*i);
        distance = cam.mPos.distance((*i)->getPosition());
      }
      else
      {
        if (distance > cam.mPos.distance((*i)->getPosition()))
        {
          peeked   = (*i);
          distance = cam.mPos.distance((*i)->getPosition());
        }
      }

    }
    else if(pln_y.intersect(ray, c) && ((*i)->getPosition().distance(c) < 1.0))
    {
      if(distance < 0.0)
      {
        peeked = (*i);
        distance = cam.mPos.distance((*i)->getPosition());
      }
      else
      {
        if (distance > cam.mPos.distance((*i)->getPosition()))
        {
          peeked   = (*i);
          distance = cam.mPos.distance((*i)->getPosition());
        }
      }
    }
    else if(pln_z.intersect(ray, c) && ((*i)->getPosition().distance(c) < 1.0))
    {
      if(distance < 0.0)
      {
        peeked = (*i);
        distance = cam.mPos.distance((*i)->getPosition());
      }
      else
      {
        if (distance > cam.mPos.distance((*i)->getPosition()))
        {
          peeked   = (*i);
          distance = cam.mPos.distance((*i)->getPosition());
        }
      }
    }
  }

  if(peeked)
  {
    select(peeked->nam);

    dragging = peeked;

    if(pivot)
      ((kPivot*)pivot->getMesh()->getMesh())->peekAxis(ray);
  }
  else
  {
    dragging = null;
  }
}

kgmRay3d<float> kEditor::getPointRay(int x, int y)
{
  kgmCamera& cam = game->getRender()->camera();

  iRect vp = game->getRender()->viewport();

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

bool kEditor::fdMapOpen(kFileDialog* fd)
{
  kgmString s = fd->getPath();

  fd->erase();

  return mapOpen(s);
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

  game->getRender()->add(gridline);
  game->getRender()->add(pivot);

  ((kPivot*)pivot->getMesh()->getMesh())->setPos(vec3(0, 0, 0));
  pivot->set(((kPivot*)pivot->getMesh()->getMesh())->getTransform());

  kNode* node = null;

  oquered = 0;

  kgmGameMap map(game, kgmGameMap::OpenRead);

  map.open(xml);

  kgmGameMap::Node mnode = map.next();

  while(mnode.typ != kgmGameMap::NodeNone)
  {
    if(mnode.typ == kgmGameMap::NodeVis)
    {
      oquered++;
      node = new kNode((kgmVisual*)mnode.obj);

      node->nam = mnode.nam;
      node->lnk = mnode.lnk;
      node->bnd = mnode.bnd;
      node->col = mnode.col;
      node->shp = mnode.shp;
      node->bnd = mnode.bnd;
      node->lock = mnode.lck;

      if(mnode.mtl.length() > 0)
      {
        kgmMaterial* mtl = null;

        for(int i = 0; i < nodes.length(); i++)
          if(nodes[i]->typ == kNode::MATERIAL && nodes[i]->nam == mnode.mtl)
          {
            mtl = nodes[i]->mtl;

            break;
          }

        if(mtl)
          node->vis->set(mtl);
      }

      add(node);

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

      add(node);
      node->setPosition(mnode.pos);
      node->setRotation(mnode.rot);
    }
    else if(mnode.typ == kgmGameMap::NodeCam)
    {
      game->getRender()->camera().mPos = mnode.pos;
      game->getRender()->camera().mDir = mnode.rot;
      game->getRender()->camera().mFov = mnode.fov;

      game->getRender()->camera().update();
    }
    if(mnode.typ == kgmGameMap::NodeMtl)
    {
      oquered++;
      node = new kNode((kgmMaterial*)mnode.obj);

      node->nam  = mnode.nam;
      node->lock = true;

      add(node);
    }
    else if(mnode.typ == kgmGameMap::NodeUnt)
    {
      oquered++;
      node = new kNode((kgmUnit*)mnode.obj);

      node->nam = mnode.nam;
      node->bnd = mnode.bnd;
      node->col = mnode.col;
      node->shp = mnode.shp;
      node->bnd = mnode.bnd;
      node->lock = mnode.lck;

      add(node);
      node->setPosition(mnode.pos);
      node->setRotation(mnode.rot);
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

      add(node);
      node->setPosition(mnode.pos);
      node->setRotation(mnode.rot);
    }
    else if(mnode.typ == kgmGameMap::NodeEff)
    {
      oquered++;
      node = new kNode((kgmEffect*)mnode.obj);

      node->nam = mnode.nam;
      node->bnd = mnode.bnd;
      node->col = mnode.col;
      node->shp = mnode.shp;
      node->bnd = mnode.bnd;
      node->lock = mnode.lck;

      add(node);
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


      add(node);
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

      add(node);
      node->setPosition(mnode.pos);
      node->setRotation(mnode.rot);
    }
    else if(mnode.typ == kgmGameMap::NodeObs)
    {
      oquered++;
      node = new kNode((kgmObstacle*)mnode.obj);

      node->nam = mnode.nam;
      node->bnd = mnode.bnd;
      node->col = mnode.col;
      node->shp = mnode.shp;
      node->bnd = mnode.bnd;
      node->lock = mnode.lck;

      add(node);
      node->setPosition(mnode.pos);
      node->setRotation(mnode.rot);
    }
    else if(mnode.typ == kgmGameMap::NodeMtl)
    {
      oquered++;
      node = new kNode((kgmMaterial*)mnode.obj);

      node->nam  = mnode.nam;
      node->lock = true;

      node->setPosition(vec3(0, 0, -1000));

      add(node);
    }

    mnode = map.next();
  }

  xml.close();

  selected = null;

  return true;
}

bool kEditor::fdMapSave(kFileDialog* fd)
{
  kgmString s = fd->getPath();

  return mapSave(s);
}

bool kEditor::mapSave(kgmString s)
{
  kgmXml     xml;
  kgmGameMap map(game, kgmGameMap::OpenWrite);

  map.open(xml);

  kgmList<kNode*> lights;
  kgmList<kNode*> visuals;

  kgmList<kNode*> units;
  kgmList<kNode*> actors;
  kgmList<kNode*> effects;
  kgmList<kNode*> sensors;
  kgmList<kNode*> triggers;
  kgmList<kNode*> obstacles;
  kgmList<kNode*> materials;

  for(kgmList<kNode*>::iterator i = nodes.begin(); i != nodes.end(); ++i)
  {
    switch ((*i)->typ)
    {
    case kNode::UNIT:
      units.add(*i);
      continue;
    case kNode::LIGHT:
      lights.add(*i);
      continue;
    case kNode::ACTOR:
      actors.add(*i);
      continue;
    case kNode::VISUAL:
      visuals.add(*i);
      continue;
    case kNode::EFFECT:
      effects.add(*i);
      continue;
    case kNode::SENSOR:
      sensors.add(*i);
      continue;
    case kNode::TRIGGER:
      triggers.add(*i);
      continue;
    case kNode::OBSTACLE:
      obstacles.add(*i);
      continue;
    case kNode::MATERIAL:
      materials.add(*i);
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

  for(kgmList<kNode*>::iterator i = materials.begin(); i != materials.end(); ++i)
  {
    kgmGameMap::Node node;

    node.obj = (*i)->mtl;
    node.nam = (*i)->nam;

    map.addMaterial(node);
  }

  for(kgmList<kNode*>::iterator i = lights.begin(); i != lights.end(); ++i)
  {
    kgmGameMap::Node node;

    node.obj = (*i)->lgt;
    node.nam = (*i)->nam;
    node.pos = (*i)->pos;
    node.rot = (*i)->rot;
    node.col = (*i)->col;
    node.shp = (*i)->shp;
    node.lck = (*i)->lock;

    map.addLight(node);
  }

  for(kgmList<kNode*>::iterator i = visuals.begin(); i != visuals.end(); ++i)
  {
    kgmGameMap::Node node;

    memset(&node, 0, sizeof(node));

    node.obj = (*i)->vis;
    node.nam = (*i)->nam;
    node.pos = (*i)->pos;
    node.rot = (*i)->rot;
    node.lnk = (*i)->lnk;
    node.col = (*i)->col;
    node.shp = (*i)->shp;
    node.lck = (*i)->lock;

    map.addVisual(node);
  }

  for(kgmList<kNode*>::iterator i = units.begin(); i != units.end(); ++i)
  {
    kgmGameMap::Node node;

    node.obj = (*i)->unt;
    node.nam = (*i)->nam;
    node.pos = (*i)->pos;
    node.rot = (*i)->rot;
    node.col = (*i)->col;
    node.shp = (*i)->shp;
    node.bnd = (*i)->bnd;
    node.lck = (*i)->lock;

    map.addUnit(node);
  }

  for(kgmList<kNode*>::iterator i = actors.begin(); i != actors.end(); ++i)
  {
    kgmGameMap::Node node;

    node.obj = (*i)->act;
    node.nam = (*i)->nam;
    node.pos = (*i)->pos;
    node.rot = (*i)->rot;
    node.bnd = (*i)->bnd;
    node.ini = (*i)->ini;
    node.lck = (*i)->lock;

    map.addActor(node);
  }

  for(kgmList<kNode*>::iterator i = effects.begin(); i != effects.end(); ++i)
  {
    kgmGameMap::Node node;

    node.obj = (*i)->eff;
    node.nam = (*i)->nam;
    node.pos = (*i)->pos;
    node.rot = (*i)->rot;
    node.col = (*i)->col;
    node.shp = (*i)->shp;
    node.bnd = (*i)->bnd;
    node.lck = (*i)->lock;

    map.addEffect(node);
  }

  for(kgmList<kNode*>::iterator i = sensors.begin(); i != sensors.end(); ++i)
  {
    kgmGameMap::Node node;

    node.obj = (*i)->sns;
    node.nam = (*i)->nam;
    node.pos = (*i)->pos;
    node.rot = (*i)->rot;
    node.col = (*i)->col;
    node.shp = (*i)->shp;
    node.bnd = (*i)->bnd;
    node.lck = (*i)->lock;

    map.addSensor(node);
  }

  for(kgmList<kNode*>::iterator i = triggers.begin(); i != triggers.end(); ++i)
  {
    kgmGameMap::Node node;

    node.obj = (*i)->trg;
    node.nam = (*i)->nam;
    node.pos = (*i)->pos;
    node.rot = (*i)->rot;
    node.col = (*i)->col;
    node.shp = (*i)->shp;
    node.bnd = (*i)->bnd;
    node.lck = (*i)->lock;

    map.addTrigger(node);
  }

  for(kgmList<kNode*>::iterator i = obstacles.begin(); i != obstacles.end(); ++i)
  {
    kgmGameMap::Node node;

    node.obj = (*i)->obs;
    node.nam = (*i)->nam;
    node.pos = (*i)->pos;
    node.rot = (*i)->rot;
    node.col = (*i)->col;
    node.shp = (*i)->shp;
    node.bnd = (*i)->bnd;
    node.lck = (*i)->lock;

    map.addObstacle(node);
  }

  obstacles.clear();
  triggers.clear();
  sensors.clear();
  effects.clear();
  actors.clear();
  units.clear();
  visuals.clear();
  lights.clear();

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

bool kEditor::fdAddMesh(kFileDialog* fd)
{
  return addMesh(fd->getFile());
}

bool kEditor::addMesh(kgmString name)
{
  kgmMesh* mesh = game->getResources()->getMesh(name);

  if(!mesh)
    return false;

  kgmVisual* visual = new kgmVisual();

  visual->set(mesh);

  selected = new kNode(visual);
  selected->nam = kgmString("Mesh_") + kgmConvert::toString((s32)(++oquered));
  selected->lnk = name;

  add(selected);

  return true;
}

bool kEditor::addUnit(kgmString type)
{
  if(type.length() < 1)
    return false;

  if(kgmUnit::g_typ_objects.hasKey(type))
  {
    kgmUnit::Generate fn_new = kgmUnit::g_typ_objects[type];

    if(fn_new)
    {
      kgmUnit* un = (kgmUnit*)fn_new(game);

      if(un)
      {
        selected = new kNode(un);
        selected->bnd = box3(-1, -1, -1, 1, 1, 1);
        selected->nam = kgmString("Unit_") + kgmConvert::toString((s32)(++oquered));

        add(selected);

        return true;
      }
    }
  }

  return false;
}

bool kEditor::addActor(kFileDialog* fdd)
{
  if(!fdd || fdd->getFile().empty())
    return false;

  kgmActor* ac = new kgmActor(game);

  if(ac)
  {
    kgmGameTools::initActor(game, ac, fdd->getFile());

    selected = new kNode(ac);
    selected->bnd = box3(-1, -1, -1, 1, 1, 1);
    selected->ini = fdd->getFile();
    selected->nam = kgmString(ac->getClass()) + kgmString("_") +
                    kgmConvert::toString((s32)(++oquered));

    add(selected);

    return true;
  }

  return false;
}

bool kEditor::addEffect(kgmString type)
{
  if(type.length() < 1)
    return false;

  if(kgmUnit::g_typ_objects.hasKey(type))
  {
    kgmUnit::Generate fn_new = kgmUnit::g_typ_objects[type];

    if(fn_new)
    {
      kgmEffect* eff = (kgmEffect*)fn_new(game);

      if(eff)
      {
        selected = new kNode(eff);
        selected->bnd = box3(-1, -1, -1, 1, 1, 1);
        selected->nam = kgmString("Effect_") + kgmConvert::toString((s32)(++oquered));

        add(selected);

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

  if(kgmUnit::g_typ_objects.hasKey(type))
  {
    kgmUnit::Generate fn_new = kgmUnit::g_typ_objects[type];

    if(fn_new)
    {
      kgmSensor* sn = (kgmSensor*)fn_new(game);

      if(sn)
      {
        selected = new kNode(sn);
        selected->bnd = box3(-1, -1, -1, 1, 1, 1);
        selected->nam = kgmString("Sensor_") + kgmConvert::toString((s32)(++oquered));

        add(selected);

        return true;
      }
    }
  }

  return false;
}

void kEditor::initPhysics()
{
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
    ctick = kgmTime::getTicks();
  }
}

void kEditor::onEvent(kgmEvent::Event *e)
{
  kgmEvent::onEvent(e);
}

void kEditor::onKeyUp(int k)
{
  if(k == KEY_LCTRL)
  {
    move_camera = false;
    game->setMsAbsolute(true);
    menu->show();
    menu->freeze(false);
  }
}

void kEditor::onKeyDown(int k)
{
  if(k == KEY_LCTRL)
  {
    move_camera = true;
    game->setMsAbsolute(false);
    menu->hide();
    menu->freeze(true);
  }
}

void kEditor::onMsLeftUp(int k, int x, int y)
{
  ms_click[0] = false;

  dragging = null;
}

void kEditor::onMsLeftDown(int k, int x, int y)
{
  ms_click[0] = true;

  if(move_camera)
    return;

  select(x, y);
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
  u32 paxes = kPivot::AXIS_NONE;

  if(game->getRender())
  {
    kgmVisual::Mesh* mesh = pivot->getMesh();

    kPivot *pvt = (kPivot *) mesh->getMesh();

    paxes = pvt->getAxis();
  }

  if(game->getRender() && move_camera)
  {
    if(ms_click[0])
    {
      kgmCamera& cam = game->getRender()->camera();

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
        kgmCamera& cam = game->getRender()->camera();

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

        if(selected->typ == kNode::VISUAL)
        {
          selected->vis->set(m);
        }
        else if(selected->typ == kNode::LIGHT)
        {
          selected->lgt->position = selected->pos;
          selected->icn->setPosition(selected->pos);
        }
      }
      else
      {
        kgmCamera& cam = game->getRender()->camera();

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
  else if(dragging && pivot && paxes != kPivot::AXIS_NONE && ms_click[0] && !dragging->lock)
  {
    kgmRay3d<float> ray = getPointRay(x, y);

    kgmCamera& cam = game->getRender()->camera();

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
    dragging->setPosition(pr);

    mtx4 m;
    m.identity();
    m.translate(dragging->pos);
    ((kPivot*)pivot->getMesh()->getMesh())->setPos(dragging->pos);
    pivot->set(m);
  }
}

void kEditor::onMsWheel(int k, int x, int y, int z)
{
}

void kEditor::onAction(kgmEvent *gui, int id)
{
}

void kEditor::update()
{
  for(kgmList<kNode*>::iterator i = nodes.begin(); i != nodes.end(); ++i)
  {
    (*i)->update();

    if(mode_play)
    {
      if ((*i)->typ == kNode::VISUAL)
      {
        (*i)->vis->update();
      }
    }
  }
}

void kEditor::onQuit()
{
  game->close();
}

void kEditor::onMapNew()
{
  clear();
}

void kEditor::onMapOpen()
{
  kFileDialog *fdd = new kFileDialog();
  fdd->showHidden(false);
  game->guiAdd(fdd);

  fdd->setFilter(".map");
  fdd->changeLocation(false);
  fdd->forOpen(game->getSettings()->get("Path"), kFileDialog::ClickEventCallback(this, (kFileDialog::ClickEventCallback::Function)&kEditor::fdMapOpen));
}

void kEditor::onMapSave()
{
  kFileDialog *fdd = new kFileDialog();
  fdd->showHidden(false);
  game->guiAdd(fdd);

  fdd->setFilter(".map");
  fdd->changeLocation(false);
  fdd->forSave(game->getSettings()->get("Path"), kFileDialog::ClickEventCallback(this, (kFileDialog::ClickEventCallback::Function)&kEditor::fdMapSave));
}

void kEditor::onEditClone()
{
  if(!selected)
    return;

  kNode* node = (kNode*) selected->clone();

  if(!node)
  {
    return;
  }
  else if (!node->obj)
  {
   delete node;

    return;
  }


  switch(selected->typ)
  {
  case kNode::LIGHT:
  {
    node->nam = kgmString("Light_") + kgmConvert::toString((s32)(++oquered));

    break;
  }
  case kNode::VISUAL:
  {
    node->nam = kgmString("Mesh_") + kgmConvert::toString((s32)(++oquered));

    break;
  }
  case kNode::ACTOR:
  {
    node->nam = kgmString("Actor_") + kgmConvert::toString((s32)(++oquered));

    break;
  }
  case kNode::EFFECT:
  {
    node->nam = kgmString("Effect_") + kgmConvert::toString((s32)(++oquered));

    break;
  }
  case kNode::SENSOR:
  {
    node->nam = kgmString("Sensor_") + kgmConvert::toString((s32)(++oquered));

    break;
  }
  case kNode::TRIGGER:
  {
    node->nam = kgmString("Trigger_") + kgmConvert::toString((s32)(++oquered));

    break;
  }
  case kNode::OBSTACLE:
  {
    node->nam = kgmString("Obstacle_") + kgmConvert::toString((s32)(++oquered));

    break;
  }
  default:

    delete node;

    return;

    break;
  }

  add(node);
}

void kEditor::onEditRemove()
{
  if(!selected)
    return;

  for(int i = nodes.length(); i > 0; i--)
  {
    kNode* n = nodes[i - 1];

    if(n == selected)
    {
      nodes.erase(i - 1);

      break;
    }
  }

  remove(selected);

  selected = null;
}


void kEditor::onEditOptions()
{
  if(!selected)
    return;

  kViewOptions* vop = null;

  switch(selected->typ)
  {
  case kNode::VISUAL:
    vop = new kViewOptionsForVisual(selected, 50, 50, 260, 300);
    break;
  case kNode::UNIT:
    vop = new kViewOptionsForUnit(selected, 50, 50, 300, 300);
    break;
  case kNode::EFFECT:
    vop = new kViewOptionsForEffect(selected, 50, 50, 300, 300);
    break;
  case kNode::ACTOR:
    vop = new kViewOptionsForActor(selected, 50, 50, 300, 300);
    break;
  case kNode::LIGHT:
    vop = new kViewOptionsForLight(selected, 50, 50, 250, 300);
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
  case kNode::MATERIAL:
    vop = new kViewOptionsForMaterial(selected, 50, 50, 250, 350);
    break;
  }

  if(vop)
  {
    game->guiAdd(vop);
    vop->show();
  }
}

void kEditor::onAddMesh()
{
  kFileDialog *fdd = new kFileDialog();
  fdd->showHidden(false);
  game->guiAdd(fdd);

  fdd->setFilter(".msh");
  fdd->changeLocation(false);
  fdd->forOpen(game->getSettings()->get("Path"), kFileDialog::ClickEventCallback(this, (kFileDialog::ClickEventCallback::Function)&kEditor::fdAddMesh));
}

void kEditor::onAddUnit()
{
  kViewObjects* vs = new kViewObjects(this, 1, 50, 200, 300);

  vs->setSelectCallback(kViewObjects::SelectCallback(this, (kViewObjects::SelectCallback::Function)&kEditor::addUnit));

  for(int i = 0; i < kgmUnit::g_list_units.length(); i++)
  {
    kgmString s = kgmUnit::g_list_units[i];
    vs->addItem(s);
  }

  game->guiAdd(vs);
}

void kEditor::onAddLight()
{
  kgmLight* light = new kgmLight();

  light->m_id = kgmString("Light_") + kgmConvert::toString((s32)(++oquered));

  selected = new kNode(light);

  selected->nam = light->m_id;

  add(selected);
}

void kEditor::onAddActor()
{
  kFileDialog *fdd = new kFileDialog();
  fdd->showHidden(false);

  fdd->setFilter(".act");
  fdd->changeLocation(false);
  fdd->forOpen(game->getSettings()->get("Path"), kFileDialog::ClickEventCallback(this, (kFileDialog::ClickEventCallback::Function)&kEditor::addActor));

  game->guiAdd(fdd);
}

void kEditor::onAddEffect()
{
  kViewObjects* vs = new kViewObjects(this, 1, 50, 200, 300);

  vs->setSelectCallback(kViewObjects::SelectCallback(this, (kViewObjects::SelectCallback::Function)&kEditor::addEffect));

  for(int i = 0; i < kgmUnit::g_list_effects.length(); i++)
  {
    kgmString s = kgmUnit::g_list_effects[i];
    vs->addItem(s);
  }

  game->guiAdd(vs);
}

void kEditor::onAddSensor()
{
  kViewObjects* vs = new kViewObjects(this, 1, 50, 200, 300);
  vs->setSelectCallback(kViewObjects::SelectCallback(this, (kViewObjects::SelectCallback::Function)&kEditor::addSensor));

  for(int i = 0; i < kgmUnit::g_list_sensors.length(); i++)
  {
    kgmString s = kgmUnit::g_list_sensors[i];
    vs->addItem(s);
  }

  game->guiAdd(vs);
}

void kEditor::onAddTrigger()
{
  kgmTrigger* tr = new kgmTrigger();

  selected = new kNode(tr);
  selected->nam = kgmString("Trigger_") + kgmConvert::toString((s32)(++oquered));
  tr->setId(selected->nam);

  add(selected);
}

void kEditor::onAddObstacle()
{
  selected = new kNode(new kgmObstacle());

  selected->nam = kgmString("Obstacle_") + kgmConvert::toString((s32)(++oquered));

  add(selected);
}

void kEditor::onAddMaterial()
{
  kNode* node = new kNode(new kgmMaterial());

  node->nam = kgmString("Material_") + kgmConvert::toString((s32)(++oquered));
  node->mtl->name(node->nam);
  node->setPosition(vec3(0, 0, -1000));

  node->lock = true;

  add(node);
}

void kEditor::onAddParticles()
{
  kNode* node = new kNode(new kgmVisual());
  kgmParticles* particles = new kgmParticles();
  particles->camera(&game->getRender()->camera());

  node->vis->set(particles);
  particles->build();

  node->nam = kgmString("Particles_") + kgmConvert::toString((s32)(++oquered));
  node->setPosition(vec3(0, 0, 0));

  add(node);
}

void kEditor::onRunPlay()
{
  if(mode_play)
    return;

  for(kgmList<kNode*>::iterator i = nodes.begin(); i != nodes.end(); ++i)
  {
    switch((*i)->typ)
    {
    case kNode::UNIT:
    case kNode::ACTOR:
    case kNode::EFFECT:
    case kNode::SENSOR:
    case kNode::TRIGGER:
      game->getPhysics()->add((*i)->unt->getBody());
      game->getLogic()->add((*i)->unt);
      break;
    case kNode::OBSTACLE:
      game->getPhysics()->add((*i)->obs);
      break;
    }
  }

  mode_play = true;
}

void kEditor::onRunStop()
{
  if(!mode_play)
    return;

  for(kgmList<kNode*>::iterator i = nodes.begin(); i != nodes.end(); ++i)
  {
    switch((*i)->typ)
    {
    case kNode::UNIT:
    case kNode::ACTOR:
    case kNode::EFFECT:
    case kNode::SENSOR:
    case kNode::TRIGGER:
      game->getPhysics()->remove((*i)->unt->getBody());
      game->getLogic()->remove((*i)->unt);
      break;
    case kNode::OBSTACLE:
      game->getPhysics()->remove((*i)->obs);
      break;
    }
  }

  mode_play = false;
}

void kEditor::onViewObjects()
{
  kViewObjects* vo = new kViewObjects(this, 1, 50, 200, 300);
  vo->setSelectCallback(kViewObjects::SelectCallback(this, (kViewObjects::SelectCallback::Function)&kEditor::onSelectObject));

  for(int i = 0; i < nodes.length(); i++)
    vo->addItem(nodes[i]->nam);

  game->guiAdd(vo);
}

void kEditor::onViewPerspective()
{
  view_mode = ViewPerspective;
  kgmCamera& cam = game->getRender()->camera();

  cam.mDir = vec3( 0.5, 0.5, -0.3).normal();
  cam.mUp  = vec3(0, 0, 1);
  cam.setOrthogonal(false);
  cam.update();

  cam_scale = 1.0;
}

void kEditor::onViewFront()
{
  view_mode = ViewFront;
  kgmCamera& cam = game->getRender()->camera();

  cam.mDir = vec3(-1, 0, 0);
  cam.mUp  = vec3(0, 0, 1);
  cam.setOrthogonal(true);
  cam.update();
}

void kEditor::onViewLeft()
{
  view_mode = ViewLeft;
  kgmCamera& cam = game->getRender()->camera();

  cam.mDir = vec3(0, -1, 0);
  cam.mUp  = vec3(0, 0, 1);
  cam.setOrthogonal(true);
  cam.update();
}

void kEditor::onViewTop()
{
  view_mode = ViewTop;
  kgmCamera& cam = game->getRender()->camera();

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

void kEditor::add(kNode* node)
{
  if(!node)
    return;

  game->getRender()->add(node->icn);

  switch (node->typ)
  {
  case kNode::LIGHT:
    game->getRender()->add(node->lgt);
    break;
  case kNode::VISUAL:
    game->getRender()->add(node->vis);
    break;
  case kNode::UNIT:
  case kNode::ACTOR:
  case kNode::EFFECT:
  case kNode::SENSOR:
  case kNode::TRIGGER:
    game->getRender()->add(node->unt->getVisual());
    break;
  case kNode::OBSTACLE:
      game->getRender()->add(node->obs);
    break;
  default:
    break;
  }

  nodes.add(node);

  select(node->nam);
}

void kEditor::remove(kNode* node)
{
  if(!node)
    return;

  game->getRender()->remove(node->icn);

  switch (node->typ)
  {
  case kNode::LIGHT:
    game->getRender()->remove(node->lgt);
    break;
  case kNode::VISUAL:
    game->getRender()->remove(node->vis);
    break;
  case kNode::UNIT:
  case kNode::ACTOR:
  case kNode::EFFECT:
  case kNode::SENSOR:
  case kNode::TRIGGER:
    game->getPhysics()->remove(node->unt->getBody());
    game->getRender()->remove(node->unt->getVisual());
    game->getLogic()->remove(node->unt);
    break;
  case kNode::OBSTACLE:
      game->getPhysics()->remove(node->obs);
      game->getRender()->remove(node->obs);
    break;
  default:
    break;
  }

  delete node;
}

int kEditor::doVisUpdate(void *v)
{
  kEditor* e = (kEditor*)v;

  while(e->m_isVisual)
  {
    e->update();

    kgmThread::sleep(0);
  }

  return 1;
}
