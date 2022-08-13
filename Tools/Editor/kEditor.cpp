#include "kEditor.h"

#include "../../kgmBase/kgmLog.h"
#include "../../kgmBase/kgmConvert.h"
#include "../../kgmSystem/kgmSystem.h"
#include "../../kgmSystem/kgmThread.h"
#include "../../kgmGame/kgmGameBase.h"
#include "../../kgmGame/kgmGameMap.h"

#include "../../kgmGraphics/kgmShape.h"

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
  ME_ADD_PARTICLES,
  ME_ADD_MATERIAL,
  ME_RUN_PLAY,
  ME_RUN_STOP,
  ME_VIEW_OBJECTS,
  ME_VIEW_MATERIALS,
  ME_VIEW_PERSPECTIVE,
  ME_VIEW_FRONT,
  ME_VIEW_BACK,
  ME_VIEW_TOP,
  ME_OPTIONS_DATABASE,
  ME_HELP_ABOUT
};

kEditor::kEditor()
{
  setMsAbsolute(true);

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

  m_isVisual = true;
  m_thVisual = kgmThread::thread_create(kEditor::doVisUpdate, this);
}

kEditor::~kEditor()
{
  clear();

  m_isVisual = false;
  kgmThread::thread_join(m_thVisual);

  pivot->release();
  gridline->release();

  mtlLines->release();
  mtlPivot->release();
}

void kEditor::clear()
{
  if(mode_play)
    onRunStop();

  gUnload();

  for(kgmList<kgmObject*>::iterator i = m_objects.begin(); !i.end(); i.next())
    (*i)->release();

  m_objects.clear();

  oquered = 0;

  pv_delta = 0.0;

  selected = null;
  dragging = null;
}

void kEditor::init()
{
  if(!m_graphics)
    return;

    m_graphics->showLights(true);

    menu = new kgmGuiMenu(null);
    menu->setSid("main_menu");
    slotMenu.connect(this, (Slot<kEditor, u32>::FN) &kEditor::onMenu, &menu->sigChoose);

    kgmGuiMenu::Item* item = menu->add("Map");
    //item->add(ME_MAP_NEW, "New");
    item->add(ME_MAP_OPEN, "Open");
    //item->add(ME_MAP_SAVE, "Save");
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
    item->add(ME_ADD_PARTICLES, "Particles");
    item->add(ME_ADD_MATERIAL, "Material");
    item = menu->add("Run");
    item->add(ME_RUN_PLAY, "Play");
    item->add(ME_RUN_STOP, "Stop");
    item = menu->add("View");
    item->add(ME_VIEW_OBJECTS, "Objects");
    item->add(ME_VIEW_MATERIALS, "Materials");
    item->add(ME_VIEW_PERSPECTIVE, "Perspective");
    item->add(ME_VIEW_FRONT, "Front");
    item->add(ME_VIEW_BACK, "Left");
    item->add(ME_VIEW_TOP, "Top");

    guiAdd(menu);
    menu->show();

    mtlLines = new kgmMaterial();
    mtlLines->m_color = kgmMaterial::Color((u32)100, (u32)100, (u32)100, (u32)255);

    mtlPivot = new kgmMaterial();
    mtlLines->m_color = kgmMaterial::Color((u32)200, (u32)150, (u32)100, (u32)255);

    gridline = new kGridline(100);
    gridline->rebuild();

    auto pvt = new kPivot();
    pvt->rebuild();

    pivot = new kgmUnit(this);
    pivot->setNode(new kgmGNode(pivot, pvt, kgmIGraphics::NodeMesh));
    pivot->setName("pivot");

    gAppend(pivot);
    pivot->increment();

    m_graphics->setBgColor(0xff222222);

    kgmUnit* u = new kgmUnit(this);

    u->setNode(new kgmGNode(u, gridline, kgmIGraphics::NodeMesh));

    u->getNode()->setNodeShading(kgmIGraphics::ShadingNone);

    gAppend(u);

    //u = new kgmUnit(this);
    //u->setNode(new kgmGNode(u, pivot, kgmIGraphics::NodeMesh));
    //u->getNode()->setNodeShading(kgmIGraphics::ShadingNone);
}

void kEditor::select(kgmString name)
{
  kgmUnit* node = gUnit(name);

  if(node)
  {
    selected = node;

    if(pivot && selected)
    {
      vec3 v = node->position();
      pivot->position(v);
    }
  }
}

void kEditor::select(int x, int y)
{
  kgmCamera& cam = getRender()->camera();

  iRect vp = getRender()->viewport();

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

  kgmIGame::Iterator* i = gUnits();

  while(kgmUnit* un = i->next())
  {
    vec3 upos = un->position();
    vec3  c, n_x, n_y, n_z;
    plane3 pln_x, pln_y, pln_z;

    n_x = vec3(1, 0, 0);
    n_y = vec3(0, 1, 0);
    n_z = vec3(0, 0, 1);

    pln_x = plane3(n_x, upos);
    pln_y = plane3(n_y, upos);
    pln_z = plane3(n_z, upos);


    if(pln_x.intersect(ray, c) && (upos.distance(c) < 1.0))
    {
      if(distance < 0.0)
      {
        peeked = un;
        distance = cam.mPos.distance(upos);
      }
      else
      {
        if (distance > cam.mPos.distance(upos))
        {
          peeked   = un;
          distance = cam.mPos.distance(upos);
        }
      }

      pv_delta = un->position().distance(c);
    }
    else if(pln_y.intersect(ray, c) && (upos.distance(c) < 1.0))
    {
      if(distance < 0.0)
      {
        peeked = un;
        distance = cam.mPos.distance(upos);
      }
      else
      {
        if (distance > cam.mPos.distance(upos))
        {
          peeked   = un;
          distance = cam.mPos.distance(upos);
        }
      }

      pv_delta = upos.distance(c);
    }
    else if(pln_z.intersect(ray, c) && (upos.distance(c) < 1.0))
    {
      if(distance < 0.0)
      {
        peeked = un;
        distance = cam.mPos.distance(upos);
      }
      else
      {
        if (distance > cam.mPos.distance(upos))
        {
          peeked   = un;
          distance = cam.mPos.distance(upos);
        }
      }

      pv_delta = upos.distance(c);
    }
  }

  delete i;

  if(peeked)
  {
    select(peeked->getName());

    dragging = peeked;

    vec3 v = peeked->position();
    vec3 r = vec3(0, 0, 0); //peeked->rotation();

    if(pivot)
      ((kPivot*)pivot->getNode()->getNodeObject())->peekAxis(ray, v , r);
  }
  else
  {
    vec3 c(0, 0, 0);
    plane3 pln_z(vec3(0, 0, 1), vec3(0, 0, 0));

    pln_z.intersect(ray, c);

    if (pivot)
      pivot->position(c);

    dragging = null;
  }
}

kgmRay3d<float> kEditor::getPointRay(int x, int y)
{
  kgmCamera& cam = getRender()->camera();

  iRect vp = getRender()->viewport();

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
  kgmString s = fd->getFile();

  fd->erase();

  return mapOpen(s);
}

bool kEditor::mapOpen(kgmString s)
{
  gLoad(s);

  kgmUnit* u = new kgmUnit(this);

  u->setNode(new kgmGNode(u, gridline, kgmIGraphics::NodeMesh));
  u->getNode()->setNodeShading(kgmIGraphics::ShadingNone);
  u->setName("gridline");

  gAppend(u);

  gAppend(pivot);
  pivot->increment();

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
  kgmGameMap map((kgmGameBase*)this, kgmGameMap::OpenWrite);

  map.open(xml);

  return map.save(s);
}

bool kEditor::fdAddMesh(kgmGuiFileDialog* fd)
{
  return addMesh(fd->getFile());
}

bool kEditor::addMesh(kgmString name)
{
  kgmMesh* mesh = getResources()->getMesh(name);

  if(!mesh)
    return false;

  kgmUnit* visual = new kgmUnit(this);

  if(!visual)
    return false;

  visual->setNode(new kgmGNode(visual, mesh, kgmIGraphics::NodeMesh));

  visual->setName(kgmString("Mesh_") + kgmConvert::toString((s32)(++oquered)));

  selected = visual;

  gAppend(visual);

  return true;
}

bool kEditor::addUnit(kgmString type)
{
  if(type.length() < 1)
    return false;

  kgmUnit* unit = new kgmUnit(this);

  unit->setName(kgmString("Unit_") + kgmConvert::toString((s32)(++oquered)));

  gAppend(unit);

  selected = unit;

  return true;
}

bool kEditor::addActor(kgmGuiFileDialog* fdd)
{
  if(!fdd || fdd->getFile().empty())
    return false;

  kgmActor* actor = new kgmActor(this);

  actor->setName(kgmString("Actor_") + kgmConvert::toString((s32)(++oquered)));

  gAppend(actor);

  selected = actor;

  return true;
}

bool kEditor::addEffect(kgmString type)
{
  if(type.length() < 1)
    return false;

  kgmEffect* effect = new kgmEffect(this);

  effect->setName(kgmString("Effect_") + kgmConvert::toString((s32)(++oquered)));

  gAppend(effect);

  selected = effect;

  return true;
}

bool kEditor::addSensor(kgmString type)
{
  if(type.length() < 1)
    return false;

  kgmSensor* sensor = new kgmSensor(this);

  sensor->setName(kgmString("Sensor_") + kgmConvert::toString((s32)(++oquered)));

  gAppend(sensor);

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
  static u32 ctick = 0;
  static u32 cdel  = 50;

  if(kgmTime::getTicks() - ctick > cdel)
  {
    ctick = kgmTime::getTicks();
  }

  kgmGameBase::onIdle();
}

void kEditor::onEvent(kgmEvent::Event *e)
{
  kgmGameBase::onEvent(e);
}

void kEditor::onKeyUp(int k)
{
  if(k == KEY_LCTRL)
  {
    move_camera = false;
    setMsAbsolute(true);
    menu->show();
    menu->freeze(false);
  }
}

void kEditor::onKeyDown(int k)
{
  if(k == KEY_LCTRL)
  {
    move_camera = true;
    setMsAbsolute(false);
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

  if(getRender() && selected)
  {
    if (pivot)
      paxes = ((kPivot*)pivot->getNode()->getNodeObject())->getAxis();
  }

  if(getRender() && move_camera)
  {
    if(ms_click[0])
    {
      kgmCamera& cam = getRender()->camera();

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
      if(getKeyState(KEY_Z) && selected)
      {
        kgmCamera& cam = getRender()->camera();

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
          //selected->visual()->set(m);
        }
        else if(selected->isClass("kgmGameLight"))
        {
          //selected->light()->position = selected->position();
        }
      }
      else
      {
        kgmCamera& cam = getRender()->camera();

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

    kgmCamera& cam = getRender()->camera();

    vec3 pt = ray.s + ray.d * cam.mPos.distance(pivot->position());
    vec3 pr, tm, nd, pv = pivot->position();
    line lax;
    float prdist;

    switch(paxes)
    {
    case kPivot::AXIS_X:
      tm = pivot->position() + vec3(1, 0, 0);
      lax = line(pv, tm);
      nd.set(1, 0, 0);
      break;
    case kPivot::AXIS_Y:
      tm = pivot->position() + vec3(0, 1, 0);
      lax = line(pv, tm);
      nd.set(0, 1, 0);
      break;
    case kPivot::AXIS_Z:
      tm = pivot->position() + vec3(0, 0, 1);
      lax = line(pv, tm);
      nd.set(0, 0, 1);
      break;
    }

    pr = lax.projection(pt);
    prdist = pivot->position().distance(pr);
    //prdist *= 2;

    vec3 dir = pr - pivot->position();
    dir.normalize();

    //selected->setPosition(selected->pos + dir * prdist);
    vec3 pos = pr - nd * pv_delta;
    dragging->position(pos);

    mtx4 m;
    m.identity();
    m.translate(pos);
    pivot->position(pos);
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
    onMapSave();
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
  case ME_ADD_PARTICLES:
    onAddParticles();
    break;
  case ME_ADD_MATERIAL:
    onAddMaterial();
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
  case ME_VIEW_MATERIALS:
      onViewMaterials();
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
  close();
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

  kgmString dir;

  dir = getSettings()->get((char*) "Data");

  s32 i = dir.index(':');

  if (i > -1) {
    kgmString a(dir.data(), i);

    dir = a;
  }

  dir += kgmSystem::getPathDelim();

  dir += "maps";

  fdd->showHidden(false);
  guiAdd(fdd);

  fdd->setFilter(".map");
  fdd->changeLocation(false);
  fdd->forOpen(dir);

  slotMapOpen.reset();
  slotMapOpen.connect(this, (Slot<kEditor, kgmGuiFileDialog*>::FN) &kEditor::fdMapOpen, &fdd->sigSelect);
}

void kEditor::onMapSave()
{
  kgmGuiFileDialog *fdd = kgmGuiFileDialog::getDialog();

  if(!fdd)
    return;

  kgmString dir;

  dir = getSettings()->get((char*) "Data");

  dir += kgmSystem::getPathDelim();

  dir += "maps";

  fdd->showHidden(false);
  guiAdd(fdd);

  fdd->setFilter(".map");
  fdd->changeLocation(false);
  fdd->forSave(dir);

  slotMapOpen.reset();
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

  s32 type = 0;
  if (selected->getNode())
    type = selected->getNode()->getNodeType();
  /*
  switch(selected->type())
  {
  case kgmUnit::Light:
    vop = kViewOptionsForLight::getDialog(selected, 50, 50, 250, 300);
    break;
  case kgmUnit::Mesh:
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
  case kgmUnit::Particles:
    vop = kViewOptionsForParticles::getDialog(selected, 50, 50, 300, 300);
    break;
  }
  */
  if(vop)
  {
    guiAdd(vop);
    vop->show();
  }
}

void kEditor::onAddBox()
{
  kgm_log() << "kEditor::onAddBox";

  kgmShape* s = new kgmShape(1.f, 1.f, 1.f);

  m_objects.add(s);

  kgmUnit* unit = new kgmUnit(this);

  unit->setNode(new kgmGNode(unit, s, kgmIGraphics::NodeNone));

  selected = unit;

  selected->setName(kgmString("Unit_") + kgmConvert::toString((s32)(++oquered)));

  add(selected);
}

void kEditor::onAddPlane()
{
  kgm_log() << "kEditor::onAddPlane";

  kgmShape* s = new kgmShape(1.f, 1.f);

  m_objects.add(s);

  kgmUnit* unit = new kgmUnit(this);

  unit->setNode(new kgmGNode(unit, s, kgmIGraphics::NodeNone));

  selected = unit;

  selected->setName(kgmString("Unit_plane_") + kgmConvert::toString((s32)(++oquered)));

  add(selected);
}

void kEditor::onAddSphere()
{
  kgm_log() << "kEditor::onAddSphere";
}

void kEditor::onAddCylinder()
{
  kgm_log() << "kEditor::onAddCylinder";
}

void kEditor::onAddMesh()
{
  kgmGuiFileDialog *fdd = kgmGuiFileDialog::getDialog();

  if(!fdd)
    return;

  slotOpenFile.reset();
  slotOpenFile.connect(this, (Slot<kEditor, kgmGuiFileDialog*>::FN) &kEditor::fdAddMesh, &fdd->sigSelect);

  fdd->showHidden(false);
  guiAdd(fdd);

  fdd->setFilter(".msh");
  fdd->changeLocation(false);
  fdd->forOpen(getSettings()->get((char*)"Data"));
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

  guiAdd(vs);*/

  kgmUnit* unit = new kgmUnit(this);
  selected = unit;

  selected->setName(kgmString("Unit_") + kgmConvert::toString((s32)(++oquered)));

  add(selected);
}

void kEditor::onAddLight()
{
  kgmUnit* light = new kgmUnit(this);
  light->setNode(new kgmGNode(light, new kgmLight(), kgmIGraphics::NodeLight));
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
  fdd->forOpen(getSettings()->get((char*)"Path"));

  guiAdd(fdd);
}

void kEditor::onAddEffect()
{
  kViewObjects* vo = kViewObjects::getDialog();

  if(!vo)
    return;

  Slot<kEditor, kgmString> slotSelect;
  slotSelect.connect(this, (Slot<kEditor, kgmString>::FN) &kEditor::addEffect, &vo->sigSelect);

  guiAdd(vo);
}

void kEditor::onAddSensor()
{
  kViewObjects* vo = kViewObjects::getDialog();

  if(!vo)
    return;

  Slot<kEditor, kgmString> slotSelect;
  slotSelect.connect(this, (Slot<kEditor, kgmString>::FN) &kEditor::addSensor, &vo->sigSelect);
  //vs->setSelectCallback(kViewObjects::SelectCallback(this, (kViewObjects::SelectCallback::Function)&kEditor::addSensor));

  guiAdd(vo);
}

void kEditor::onAddTrigger()
{
  kgmTrigger* tr = new kgmTrigger();

  selected = tr;
  selected->setName(kgmString("Trigger_") + kgmConvert::toString((s32)(++oquered)));

  add(selected);
}

void kEditor::onAddParticles()
{
  kgmParticles* p = new kgmParticles();

  m_objects.add(p);

  kgmUnit* pr = new kgmUnit(this);

  pr->setNode(new kgmGNode(pr, p, kgmIGraphics::NodeParticles));


  selected = pr;
  selected->setName(kgmString("Particle_") + kgmConvert::toString((s32)(++oquered)));

  add(selected);
}

void kEditor::onAddMaterial()
{
  kgmMaterial* m = new kgmMaterial();

  m_objects.add(m);

  m->setId(kgmString("Material_") + kgmConvert::toString((s32)(++oquered)));

  kViewOptionsForMaterial* vop = kViewOptionsForMaterial::getDialog(m, 50, 50, 250, 350);

  if(vop)
  {
    guiAdd(vop);
    vop->show();
  }
}

void kEditor::onRunPlay()
{
  if(mode_play)
    return;

  mode_play = true;

  gSwitch(kgmIGame::State_Play);
}

void kEditor::onRunStop()
{
  if(!mode_play)
    return;

  mode_play = false;

  gSwitch(kgmIGame::State_Pause);
}

void kEditor::onViewObjects()
{
  kViewObjects* vo = kViewObjects::getDialog();

  if(!vo)
    return;

  Slot<kEditor, kgmString> slotSelect;
  slotSelect.connect(this, (Slot<kEditor, kgmString>::FN) &kEditor::onSelectObject, &vo->sigSelect);

  kgmIGame::Iterator* i = gUnits();

  while(kgmUnit* un = i->next())
      vo->addItem(un->getName());

  delete i;

  guiAdd(vo);
}

void kEditor::onViewMaterials()
{
  kViewObjects* vo = kViewObjects::getDialog();

  if(!vo)
    return;

  slotSelect.reset();
  slotSelect.connect(this, (Slot<kEditor, kgmString>::FN) &kEditor::onSelectMaterial, &vo->sigSelect);

  kgmList<kgmObject*>::iterator i = m_objects.begin();

  while(!i.end())
  {
    if (kgmString((*i)->vClass()) == "kgmMaterial")
    {
      vo->addItem(((kgmMaterial*)(*i))->id());
    }

    ++i;
  }

  guiAdd(vo);
}

void kEditor::onViewPerspective()
{
  view_mode = ViewPerspective;
  kgmCamera& cam = getRender()->camera();

  cam.mPos = vec3(-1, -1, 1);
  cam.mDir = vec3( 0.5, 0.5, -0.3).normal();
  cam.mUp  = vec3(0, 0, 1);
  cam.setOrthogonal(false);
  cam.update();

  cam_scale = 1.0;
}

void kEditor::onViewFront()
{
  view_mode = ViewFront;
  kgmCamera& cam = getRender()->camera();

  cam.mDir = vec3(-1, 0, 0);
  cam.mUp  = vec3(0, 0, 1);
  cam.setOrthogonal(true);
  cam.update();
}

void kEditor::onViewLeft()
{
  view_mode = ViewLeft;
  kgmCamera& cam = getRender()->camera();

  cam.mDir = vec3(0, -1, 0);
  cam.mUp  = vec3(0, 0, 1);
  cam.setOrthogonal(true);
  cam.update();
}

void kEditor::onViewTop()
{
  view_mode = ViewTop;
  kgmCamera& cam = getRender()->camera();

  cam.mDir = vec3(0, 0, -1);
  cam.mUp  = vec3(0, 1,  0);
  cam.setOrthogonal(true);
  cam.update();
}

void kEditor::onOptionsDatabase()
{
  kgmString loc = getSettings()->get((char*)"Path");

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

void kEditor::onSelectMaterial(kgmString id)
{
  kViewOptionsForMaterial* vop = null;

  for(kgmList<kgmObject*>::iterator i = m_objects.begin(); !i.end(); ++i)
  {
    if (kgmString((*i)->vClass()) == "kgmMaterial" && ((kgmMaterial*)(*i))->id() == id)
    {
      vop = kViewOptionsForMaterial::getDialog(((kgmMaterial*)(*i)), 50, 50, 250, 350);
      break;
    }
  }

  if(vop)
  {
    guiAdd(vop);
    vop->show();
  }
}

void kEditor::add(kgmUnit* node)
{
  if(!node)
    return;

  gAppend(node);

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

kgmMaterial* kEditor::getMaterial(kgmString id)
{
  for(kgmList<kgmObject*>::iterator i = m_objects.begin(); !i.end(); ++i)
  {
    kgmString s1 = (*i)->vClass();

    if(kgmString((*i)->vClass()) == "kgmMaterial")
    {
      if (((kgmMaterial*)(*i))->id() == id)
      {
        return (kgmMaterial*)(*i);
      }
    }
  }

  return null;
}

