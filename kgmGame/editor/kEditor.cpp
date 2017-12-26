#include "kEditor.h"

#include "../../kgmBase/kgmLog.h"
#include "../../kgmBase/kgmConvert.h"
#include "../../kgmSystem/kgmSystem.h"
#include "../../kgmSystem/kgmThread.h"
#include "../kgmGameBase.h"
#include "../kgmGameMap.h"

#include "../../kgmGraphics/kgmShape.h"

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
  ME_ADD_BOX,
  ME_ADD_PLANE,
  ME_ADD_SPHERE,
  ME_ADD_CYLINDER,
  ME_ADD_MESH,
  ME_ADD_UNIT,
  ME_ADD_LIGHT,
  ME_ADD_ACTOR,
  ME_ADD_EFFECT,
  ME_ADD_SENSOR,
  ME_ADD_TRIGGER,
  ME_RUN_PLAY,
  ME_RUN_STOP,
  ME_VIEW_OBJECTS,
  ME_VIEW_PERSPECTIVE,
  ME_VIEW_FRONT,
  ME_VIEW_BACK,
  ME_VIEW_TOP,
  ME_OPTIONS_DATABASE,
  ME_HELP_ABOUT
};

kEditor::kEditor(kgmGameBase* g)
{
  game = g;

  graphics = (kgmGameGraphics*) g->getRender();

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

  pv_delta = 0.0f;

  mode_play = false;

  if(graphics)
  {
    graphics->setEditor(true);

    menu = new kgmGuiMenu(null);
    menu->setSid("editor_main_menu");
    slotMenu.connect(this, (Slot<kEditor, u32>::FN) &kEditor::onMenu, &menu->sigChoose);

    kgmGuiMenu::Item* item = menu->add("Map");
    item->add(ME_MAP_NEW, "New");
    item->add(ME_MAP_OPEN, "Open");
    item->add(ME_MAP_SAVE, "Save");
    item->add(ME_QUIT, "Quit");
    item = menu->add("Edit");
    item->add(ME_EDIT_CLONE, "Clone");
    item->add(ME_EDIT_REMOVE, "Remove");
    item->add(ME_EDIT_OPTIONS, "Options");
    item = menu->add("Add");
    item->add(ME_ADD_BOX, "Box");
    item->add(ME_ADD_PLANE, "Plane");
    item->add(ME_ADD_SPHERE, "Sphere");
    item->add(ME_ADD_CYLINDER, "Cylinder");
    item->add(ME_ADD_MESH, "Mesh");
    item->add(ME_ADD_UNIT, "Unit");
    item->add(ME_ADD_LIGHT, "Light");
    item->add(ME_ADD_ACTOR, "Actor");
    item->add(ME_ADD_EFFECT, "Effect");
    item->add(ME_ADD_SENSOR, "Sensor");
    item->add(ME_ADD_TRIGGER, "Trigger");
    item = menu->add("Run");
    item->add(ME_RUN_PLAY, "Play");
    item->add(ME_RUN_STOP, "Stop");
    item = menu->add("View");
    item->add(ME_VIEW_OBJECTS, "Objects");
    item->add(ME_VIEW_PERSPECTIVE, "Perspective");
    item->add(ME_VIEW_FRONT, "Front");
    item->add(ME_VIEW_BACK, "Left");
    item->add(ME_VIEW_TOP, "Top");
    game->guiAdd(menu);

    mtlLines = new kgmMaterial();
    mtlLines->setShader(null);
    mtlLines->shade(false);
    mtlLines->m_color = kgmMaterial::Color((u32)100, (u32)100, (u32)100, (u32)255);

    mtlPivot = new kgmMaterial();
    mtlPivot->setShader(null);
    mtlPivot->shade(false);
    mtlLines->m_color = kgmMaterial::Color((u32)200, (u32)150, (u32)100, (u32)255);

    gridline = new kGridline(100);
    gridline->rebound();

    graphics->set(gridline);

    pivot = new kPivot();
    pivot->rebound();

    graphics->set(pivot);

    textData = new kgmText();
    textData->rect(uRect(600, 200, 300, 100));
    text     = new kgmVisual();
    text->set(textData);
    game->getRender()->add(text);

    graphics->setBgColor(0xffbbaa99);
  }

  m_isVisual = true;
  m_thVisual = kgmThread::thread_create(kEditor::doVisUpdate, this);
}

kEditor::~kEditor()
{
  clear();

  m_isVisual = false;
  kgmThread::thread_join(m_thVisual);

  game->getRender()->remove(text);

  delete pivot;
  delete gridline;
  delete textData;

  delete mtlLines;
  delete mtlPivot;

  delete text;

  //kgmUnit::g_typ_objects.clear();
}

void kEditor::clear()
{
  if(mode_play)
    onRunStop();

  oquered = 0;

  pv_delta = 0.0;

  selected = null;
  dragging = null;
}

void kEditor::select(kgmString name)
{
  kgmUnit* node = game->gUnit(name);

  if(node)
  {
    selected = node;

    textData->m_text =  "Selected: ";
    textData->m_text += name;

    if(pivot && selected)
    {
      pivot->setPos(node->position());
    }
  }
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
  kgmUnit* peeked = null;

  kgmList<kgmUnit*> nodes;

  kgmIGame::Iterator* i = game->gObjects();

  while(kgmUnit* un = i->next())
  {
    vec3  c, n_x, n_y, n_z;
    plane3 pln_x, pln_y, pln_z;

    n_x = vec3(1, 0, 0);
    n_y = vec3(0, 1, 0);
    n_z = vec3(0, 0, 1);

    pln_x = plane3(n_x, un->position());
    pln_y = plane3(n_y, un->position());
    pln_z = plane3(n_z, un->position());


    if(pln_x.intersect(ray, c) && (un->position().distance(c) < 1.0))
    {
      if(distance < 0.0)
      {
        peeked = un;
        distance = cam.mPos.distance(un->position());
      }
      else
      {
        if (distance > cam.mPos.distance(un->position()))
        {
          peeked   = un;
          distance = cam.mPos.distance(un->position());
        }
      }

      pv_delta = un->position().distance(c);
    }
    else if(pln_y.intersect(ray, c) && (un->position().distance(c) < 1.0))
    {
      if(distance < 0.0)
      {
        peeked = un;
        distance = cam.mPos.distance(un->position());
      }
      else
      {
        if (distance > cam.mPos.distance(un->position()))
        {
          peeked   = un;
          distance = cam.mPos.distance(un->position());
        }
      }

      pv_delta = un->position().distance(c);
    }
    else if(pln_z.intersect(ray, c) && (un->position().distance(c) < 1.0))
    {
      if(distance < 0.0)
      {
        peeked = un;
        distance = cam.mPos.distance(un->position());
      }
      else
      {
        if (distance > cam.mPos.distance(un->position()))
        {
          peeked   = un;
          distance = cam.mPos.distance(un->position());
        }
      }

      pv_delta = un->position().distance(c);
    }
  }

  delete i;

  if(peeked)
  {
    select(peeked->getName());

    dragging = peeked;

    if(pivot)
      pivot->peekAxis(ray);
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

bool kEditor::fdMapOpen(kgmGuiFileDialog* fd)
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

  pivot->setPos(vec3(0, 0, 0));

  oquered = 0;

  kgmGameMap map(game, kgmGameMap::OpenRead);

  map.open(xml);

  kgmUnit* mnode = map.next();

  while(mnode != null)
  {
    oquered++;
    game->getLogic()->add(mnode);

    mnode = map.next();
  }

  xml.close();

  selected = null;

  return true;
}

bool kEditor::fdMapSave(kgmGuiFileDialog* fd)
{
  kgmString s = fd->getPath();

  return mapSave(s);
}

bool kEditor::mapSave(kgmString s)
{
  kgmXml     xml;
  kgmGameMap map(game, kgmGameMap::OpenWrite);

  map.open(xml);

  return map.save(s);
}

bool kEditor::fdAddMesh(kgmGuiFileDialog* fd)
{
  return addMesh(fd->getFile());
}

bool kEditor::addMesh(kgmString name)
{
  kgmMesh* mesh = game->getResources()->getMesh(name);

  if(!mesh)
    return false;

  kgmUnit* visual = new kgmUnit(game, new kgmVisual);

  if(!visual)
    return false;

  if(mesh->getMtlId().length() > 0)
    visual->visual()->set(game->getResources()->getMaterial(mesh->getMtlId()));

  visual->visual()->set(mesh);

  visual->setName(kgmString("Mesh_") + kgmConvert::toString((s32)(++oquered)));

  selected = visual;

  game->gAppend(visual);

  return true;
}

bool kEditor::addUnit(kgmString type)
{
  if(type.length() < 1)
    return false;

  kgmUnit* unit = new kgmUnit(game);

  unit->setName(kgmString("Unit_") + kgmConvert::toString((s32)(++oquered)));

  game->gAppend(unit);

  selected = unit;

  return true;
}

bool kEditor::addActor(kgmGuiFileDialog* fdd)
{
  if(!fdd || fdd->getFile().empty())
    return false;

  kgmActor* actor = new kgmActor(game);

  actor->setName(kgmString("Actor_") + kgmConvert::toString((s32)(++oquered)));

  game->gAppend(actor);

  selected = actor;

  return true;
}

bool kEditor::addEffect(kgmString type)
{
  if(type.length() < 1)
    return false;

  kgmEffect* effect = new kgmEffect(game);

  effect->setName(kgmString("Effect_") + kgmConvert::toString((s32)(++oquered)));

  game->gAppend(effect);

  selected = effect;

  return true;
}

bool kEditor::addSensor(kgmString type)
{
  if(type.length() < 1)
    return false;

  kgmSensor* sensor = new kgmSensor(game);

  sensor->setName(kgmString("Sensor_") + kgmConvert::toString((s32)(++oquered)));

  game->gAppend(sensor);

  selected = sensor;

  return true;
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

  pv_delta = 0.0f;
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
    paxes = pivot->getAxis();
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

        vec3 pos = selected->position();

        if(cam.isOrthogonal())
        {
          switch(view_mode)
          {
          case ViewFront:
            pos.y += x * cam_scale;
            pos.z += y * cam_scale;
            break;
          case ViewLeft:
            pos.x += x * cam_scale;
            pos.z += y * cam_scale;
            break;
          case ViewTop:
            pos.x += x * cam_scale;
            pos.y += y * cam_scale;
            break;
          }
        }
        else
        {
          pos.x += 0.01 * x;
          pos.y += 0.01 * y;
        }

        selected->position(pos);

        mtx4 m;
        m.identity();
        m.translate(pos);

        if(selected->isClass("kgmGameVisual"))
        {
          selected->visual()->set(m);
        }
        else if(selected->isClass("kgmGameLight"))
        {
          selected->light()->position = selected->position();
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
  else if(dragging && pivot && paxes != kPivot::AXIS_NONE && ms_click[0] && !dragging->lock())
  {
    kgmRay3d<float> ray = getPointRay(x, y);

    kgmCamera& cam = game->getRender()->camera();

    vec3 pt = ray.s + ray.d * cam.mPos.distance(pivot->pos);
    vec3 pr, tm, nd;
    line lax;
    float prdist;

    switch(paxes)
    {
    case kPivot::AXIS_X:
      tm = pivot->pos + vec3(1, 0, 0);
      lax = line(pivot->pos, tm);
      nd.set(1, 0, 0);
      break;
    case kPivot::AXIS_Y:
      tm = pivot->pos + vec3(0, 1, 0);
      lax = line(pivot->pos, tm);
      nd.set(0, 1, 0);
      break;
    case kPivot::AXIS_Z:
      tm = pivot->pos + vec3(0, 0, 1);
      lax = line(pivot->pos, tm);
      nd.set(0, 0, 1);
      break;
    }

    pr = lax.projection(pt);
    prdist = pivot->pos.distance(pr);
    //prdist *= 2;

    vec3 dir = pr - pivot->pos;
    dir.normalize();

    //selected->setPosition(selected->pos + dir * prdist);
    vec3 pos = pr - nd * pv_delta;
    dragging->position(pos);

    mtx4 m;
    m.identity();
    m.translate(pos);
    pivot->setPos(pos);
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
}

void kEditor::onMenu(u32 id)
{
  switch (id) {
  case ME_QUIT:
    onQuit();
    break;
  case ME_MAP_NEW:
    onMapNew();
    break;
  case ME_MAP_OPEN:
    onMapOpen();
    break;
  case ME_MAP_SAVE:
    //onMapSave();
    break;
  case ME_EDIT_CLONE:
    onEditClone();
    break;
  case ME_EDIT_REMOVE:
    onEditRemove();
    break;
  case ME_EDIT_OPTIONS:
    onEditOptions();
    break;
  case ME_ADD_BOX:
    onAddBox();
    break;
  case ME_ADD_PLANE:
    onAddPlane();
    break;
  case ME_ADD_SPHERE:
    onAddSphere();
    break;
  case ME_ADD_CYLINDER:
    onAddCylinder();
    break;
  case ME_ADD_MESH:
    onAddMesh();
    break;
  case ME_ADD_UNIT:
    onAddUnit();
    break;
  case ME_ADD_LIGHT:
    onAddLight();
    break;
  case ME_ADD_ACTOR:
    onAddActor();
    break;
  case ME_ADD_EFFECT:
    onAddEffect();
    break;
  case ME_ADD_SENSOR:
    onAddSensor();
    break;
  case ME_ADD_TRIGGER:
    onAddTrigger();
    break;
  case ME_RUN_PLAY:
    onRunPlay();
    break;
  case ME_RUN_STOP:
    onRunStop();
    break;
  case ME_VIEW_OBJECTS:
      onViewObjects();
    break;
  case ME_VIEW_PERSPECTIVE:
      onViewPerspective();
    break;
  case ME_VIEW_FRONT:
      onViewFront();
    break;
  case ME_VIEW_BACK:
      onViewLeft();
    break;
  case ME_VIEW_TOP:
      onViewTop();
    break;
  case ME_OPTIONS_DATABASE:
    break;
  case ME_HELP_ABOUT:
    break;
  default:
    break;
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
  kgmGuiFileDialog *fdd = kgmGuiFileDialog::getDialog();

  if(!fdd)
    return;

  fdd->showHidden(false);
  game->guiAdd(fdd);

  fdd->setFilter(".map");
  fdd->changeLocation(false);
  fdd->forOpen(game->getSettings()->get((char*)"Path"));
  slotMapOpen.connect(this, (Slot<kEditor, kgmGuiFileDialog*>::FN) &kEditor::fdMapOpen, &fdd->sigSelect);
}

void kEditor::onMapSave()
{
  kgmGuiFileDialog *fdd = kgmGuiFileDialog::getDialog();

  if(!fdd)
    return;

  fdd->showHidden(false);
  game->guiAdd(fdd);

  fdd->setFilter(".map");
  fdd->changeLocation(false);
  fdd->forSave(game->getSettings()->get((char*)"Path"));
  slotMapSave.connect(this, (Slot<kEditor, kgmGuiFileDialog*>::FN) &kEditor::fdMapSave, &fdd->sigSelect);
}

void kEditor::onEditClone()
{
  if(!selected)
    return;

  /*kNode* node = (kNode*) selected->clone();

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

  add(node);*/
}

void kEditor::onEditRemove()
{
  if(!selected)
    return;

  remove(selected);

  selected = null;
}


void kEditor::onEditOptions()
{
  if(!selected)
    return;

  kViewOptions* vop = null;

  switch(selected->type())
  {
  case kgmUnit::Light:
    vop = kViewOptionsForLight::getDialog(selected, 50, 50, 250, 300);
    break;
  case kgmUnit::Visual:
    vop = kViewOptionsForVisual::getDialog(selected, 50, 50, 260, 300);
    break;
  case kgmUnit::Unit:
    vop = kViewOptionsForUnit::getDialog(selected, 50, 50, 300, 300);
    break;
  case kgmUnit::Actor:
    vop = kViewOptionsForActor::getDialog(selected, 50, 50, 300, 300);
    break;
  case kgmUnit::Effect:
    vop = kViewOptionsForEffect::getDialog(selected, 50, 50, 300, 300);
    break;
  case kgmUnit::Sensor:
    vop = kViewOptionsForSensor::getDialog(selected, 50, 50, 300, 300);
    break;
  case kgmUnit::Trigger:
    vop = kViewOptionsForTrigger::getDialog(selected, 50, 50, 300, 300);
    break;
  }

  if(vop)
  {
    game->guiAdd(vop);
    vop->show();
  }
}

void kEditor::onAddBox()
{
  kgmUnit* unit = new kgmUnit(game, new kgmVisual());

  unit->visual()->set(new kgmShape(1.f, 1.f, 1.f));

  selected = unit;

  selected->setName(kgmString("Unit_") + kgmConvert::toString((s32)(++oquered)));

  add(selected);
}

void kEditor::onAddPlane()
{
  kgmUnit* unit = new kgmUnit(game, new kgmVisual());

  unit->visual()->set(new kgmShape(1.f, 1.f));

  selected = unit;

  selected->setName(kgmString("Unit_plane_") + kgmConvert::toString((s32)(++oquered)));

  add(selected);
}

void kEditor::onAddSphere()
{
}

void kEditor::onAddCylinder()
{
}

void kEditor::onAddMesh()
{
  kgmGuiFileDialog *fdd = kgmGuiFileDialog::getDialog();

  if(!fdd)
    return;

  slotOpenFile.reset();
  slotOpenFile.connect(this, (Slot<kEditor, kgmGuiFileDialog*>::FN) &kEditor::fdAddMesh, &fdd->sigSelect);

  fdd->showHidden(false);
  game->guiAdd(fdd);

  fdd->setFilter(".msh");
  fdd->changeLocation(false);
  fdd->forOpen(game->getSettings()->get((char*)"Data"));
}

void kEditor::onAddUnit()
{
  /*kViewObjects* vs = new kViewObjects(this, 1, 50, 200, 300);

  vs->setSelectCallback(kViewObjects::SelectCallback(this, (kViewObjects::SelectCallback::Function)&kEditor::addUnit));

  for(int i = 0; i < kgmUnit::g_list_units.length(); i++)
  {
    kgmString s = kgmUnit::g_list_units[i];
    vs->addItem(s);
  }

  game->guiAdd(vs);*/

  kgmUnit* unit = new kgmUnit(game);
  selected = unit;

  selected->setName(kgmString("Unit_") + kgmConvert::toString((s32)(++oquered)));

  add(selected);
}

void kEditor::onAddLight()
{
  kgmUnit* light = new kgmUnit(game, new kgmLight);
  selected = light;

  light->setName(kgmString("Light_") + kgmConvert::toString((s32)(++oquered)));

  add(selected);
}

void kEditor::onAddActor()
{
  kgmGuiFileDialog *fdd = kgmGuiFileDialog::getDialog();

  if(!fdd)
    return;

  fdd->showHidden(false);

  fdd->setFilter(".act");
  fdd->changeLocation(false);
  fdd->forOpen(game->getSettings()->get((char*)"Path"));

  game->guiAdd(fdd);
}

void kEditor::onAddEffect()
{
  kViewObjects* vo = kViewObjects::getDialog();

  if(!vo)
    return;

  Slot<kEditor, kgmString> slotSelect;
  slotSelect.connect(this, (Slot<kEditor, kgmString>::FN) &kEditor::addEffect, &vo->sigSelect);

  game->guiAdd(vo);
}

void kEditor::onAddSensor()
{
  kViewObjects* vo = kViewObjects::getDialog();

  if(!vo)
    return;

  Slot<kEditor, kgmString> slotSelect;
  slotSelect.connect(this, (Slot<kEditor, kgmString>::FN) &kEditor::addSensor, &vo->sigSelect);
  //vs->setSelectCallback(kViewObjects::SelectCallback(this, (kViewObjects::SelectCallback::Function)&kEditor::addSensor));

  game->guiAdd(vo);
}

void kEditor::onAddTrigger()
{
  kgmTrigger* tr = new kgmTrigger();

  selected = tr;
  selected->setName(kgmString("Trigger_") + kgmConvert::toString((s32)(++oquered)));

  add(selected);
}

void kEditor::onRunPlay()
{
  if(mode_play)
    return;

  mode_play = true;
}

void kEditor::onRunStop()
{
  if(!mode_play)
    return;

  mode_play = false;
}

void kEditor::onViewObjects()
{
  kViewObjects* vo = kViewObjects::getDialog();

  if(!vo)
    return;

  Slot<kEditor, kgmString> slotSelect;
  slotSelect.connect(this, (Slot<kEditor, kgmString>::FN) &kEditor::onSelectObject, &vo->sigSelect);

  kgmIGame::Iterator* i = game->gObjects();

  while(kgmUnit* un = i->next())
      vo->addItem(un->getName());

  delete i;

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
  kgmString loc = game->getSettings()->get((char*)"Path");

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

void kEditor::add(kgmUnit* node)
{
  if(!node)
    return;

  game->gAppend(node);

  select(node->getName());
}

void kEditor::remove(kgmUnit* node)
{
  if(!node)
    return;

  node->remove();
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
