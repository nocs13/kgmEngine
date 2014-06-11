#include "kEditor.h"

#include "../../kgmBase/kgmConvert.h"
#include "../../kgmSystem/kgmSystem.h"
#include "../kgmGameBase.h"

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
  ME_ADD_TRIGGER,
  ME_ADD_MATERIAL,
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

  cam_pos = vec3(0, 0, 0);
  cam_rot = 0.0f;

  selected = null;

  oquered = 0;

  if(game->m_render)
  {
    game->m_render->setEditor(true);

    menu = new kMenu(null, this);
    kgmGuiMenu::Item* item = menu->add("Map");
    item->add(ME_MAP_OPEN, "Open", kgmGuiMenu::Item::ClickEventCallback(this, (kgmGuiMenu::Item::ClickEventCallback::Function)&onMapOpen));
    item->add(ME_MAP_SAVE, "Save", kgmGuiMenu::Item::ClickEventCallback(this, (kgmGuiMenu::Item::ClickEventCallback::Function)&onMapSave));
    item->add(ME_QUIT, "Quit", kgmGuiMenu::Item::ClickEventCallback(this, (kgmGuiMenu::Item::ClickEventCallback::Function)&onQuit));
    item = menu->add("Edit");
    item->add(ME_EDIT_REMOVE, "Remove", kgmGuiMenu::Item::ClickEventCallback(this, (kgmGuiMenu::Item::ClickEventCallback::Function)&onEditRemove));
    item->add(ME_EDIT_DUPLICATE, "Duplicate", kgmGuiMenu::Item::ClickEventCallback(this, (kgmGuiMenu::Item::ClickEventCallback::Function)&onEditDuplicate));
    item->add(ME_EDIT_OPTIONS, "Options", kgmGuiMenu::Item::ClickEventCallback(this, (kgmGuiMenu::Item::ClickEventCallback::Function)&onEditOptions));
    item = menu->add("Add");
    item->add(ME_ADD_MESH, "Mesh", kgmGuiMenu::Item::ClickEventCallback(this, (kgmGuiMenu::Item::ClickEventCallback::Function)&onAddMesh));
    item->add(ME_ADD_LIGHT, "Light", kgmGuiMenu::Item::ClickEventCallback(this, (kgmGuiMenu::Item::ClickEventCallback::Function)&onAddLight));
    item->add(ME_ADD_ACTOR, "Actor", kgmGuiMenu::Item::ClickEventCallback(this, (kgmGuiMenu::Item::ClickEventCallback::Function)&onAddActor));
    item->add(ME_ADD_SENSOR, "Sensor", kgmGuiMenu::Item::ClickEventCallback(this, (kgmGuiMenu::Item::ClickEventCallback::Function)&onAddSensor));
    item->add(ME_ADD_TRIGGER, "Trigger", kgmGuiMenu::Item::ClickEventCallback(this, (kgmGuiMenu::Item::ClickEventCallback::Function)&onAddTrigger));
    item->add(ME_ADD_MATERIAL, "Material", kgmGuiMenu::Item::ClickEventCallback(this, (kgmGuiMenu::Item::ClickEventCallback::Function)&onAddMaterial));
    item = menu->add("Run");
    //item->add(ME_RUN_RUN, "Run");
    item->add(ME_RUN_RUN, "Run", kgmGuiMenu::Item::ClickEventCallback(this, (kgmGuiMenu::Item::ClickEventCallback::Function)&onRunRun));
    item = menu->add("View");
    item->add(ME_VIEW_OBJECTS, "Objects", kgmGuiMenu::Item::ClickEventCallback(this, (kgmGuiMenu::Item::ClickEventCallback::Function)&onViewObjects));
    //item = menu->add("Options");
    //item->add(ME_OPTIONS_DATABASE, "Database");
    //item = menu->add("Help");
    //item->add(ME_HELP_ABOUT, "About");
    game->m_render->add(menu);

    gridline = new kGridline();
    game->m_render->add(gridline, null);

    pivot = new kPivot();
    game->m_render->add(pivot, null);

    fdd = new kFDD(this);
    fdd->showHidden(false);
    fdd->hide();
    game->m_render->add(fdd);

    vop = null;

    vo = new kViewObjects(this, 1, 50, 100, 300);
    vo->hide();
    game->m_render->add(vo);

    game->m_render->setBgColor(0xffbbaa99);
  }
}

kEditor::~kEditor()
{
  gridline->release();
  pivot->release();
  menu->release();
  fdd->release();
  vo->release();
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

  vo->getGuiList()->clear();

  oquered = 0;
}

kNode* kEditor::select(int x, int y)
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

    /*if(pl.intersect(ray, c))
    {
      plane3 pln(vec3(0,0,1), vec3(0,0,0));
      ms = vec3(0,0,0);
      pln.intersect(ray, ms);

      mtx4 m;
      m.identity();
      m.translate(ms);
      m_render->set(pivot, m);
    }*/
  }

  if(pivot->peekAxis(ray) != kPivot::AXIS_NONE)
  {
    int k = 0;
  }

  for(kgmList<kNode*>::iterator i = nodes.begin(); i != nodes.end(); ++i)
  {
    vec3 c;

    box3 bnd = (*i)->bnd;

    bnd.min = bnd.min + (*i)->pos;
    bnd.max = bnd.max + (*i)->pos;

    printf("AAA\n");

    if(bnd.intersect(ray, c))
    {
      printf("aaa\n");

      return (*i);
    }
  }

  return null;
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

  kNode* node = null;

  while(kgmXml::XmlState xstate = xml.next())
  {
    kgmString id, value, t;

    if(xstate == kgmXml::XML_ERROR)
    {
      break;
    }
    else if(xstate == kgmXml::XML_FINISH)
    {
      break;
    }
    else if(xstate == kgmXml::XML_TAG_OPEN)
    {
      id = xml.m_tagName;

      if(id == "kgmMaterial")
      {
        kgmString id;
        xml.attribute("name", id);
        node = new kNode(new kgmMaterial());
        node->mtl->setId(id);
        node->nam = id;
        game->m_render->add(node->mtl);
        nodes.add(node);
      }
      else if(id == "kgmCamera")
      {

      }
      else if(id == "kgmLight")
      {
        kgmString id;
        xml.attribute("name", id);
        node = new kNode(new kgmLight());

        node->nam = id;
        node->bnd = box3(-1, -1, -1, 1, 1, 1);
        node->icn = new kgmGraphics::Icon(game->getResources()->getTexture("light_ico.tga"), 1, 1, vec3(0, 0, 0));

        vo->getGuiList()->addItem(node->nam);
        vo->getGuiList()->setSel(vo->getGuiList()->m_items.length() - 1);

        game->m_render->add(node->lgt);
        game->m_render->add(node->icn);

        nodes.add(node);
      }
      else if(id == "kgmMesh")
      {
        kgmString id, ln;
        xml.attribute("name", id);
        xml.attribute("link", ln);
        kgmMesh* mesh = game->getResources()->getMesh(ln);
        kgmMaterial* mtl = game->getResources()->getMaterial(ln);

        if(mesh)
        {
          node = new kNode(mesh);
          node->nam = id;
          node->bnd = mesh->bound();
          game->m_render->add(node->msh, mtl);
          nodes.add(node);

          if(mtl)
            mtl->release();

          vo->getGuiList()->addItem(node->nam);
        }
      }
      else if(id == "kgmActor")
      {
      }
      else if(id == "kgmGameObject")
      {
      }
    }
    else if(xstate == kgmXml::XML_TAG_CLOSE)
    {
      kgmString data;
      id = xml.m_tagName;

      if(id == "Position")
      {
        vec3 v;
        xml.attribute("value", value);
        sscanf(value.data(), "%f %f %f", &v.x, &v.y, &v.z);
        node->setPosition(v);
      }
      else if(id == "Rotation")
      {
        vec3 v;
        xml.attribute("value", value);
        sscanf(value.data(), "%f %f %f", &v.x, &v.y, &v.z);
        node->setRotation(v);
      }
    }
    else if(xstate == kgmXml::XML_TAG_DATA)
    {
    }
  }

  xml.close();

  return true;
}

bool kEditor::mapSave(kgmString s)
{
  FILE* f = fopen(s.data(), "w");

  if(!f)
    return false;

  fprintf(f, "<?xml version='1.0'?>\n");
  fprintf(f, "<kgm>\n");

  kgmList<kNode*> meshes;
  kgmList<kNode*> lights;
  kgmList<kNode*> actors;
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
    case kNode::MATERIAL:
      materials.add(*i);
      continue;
    default:
      continue;
    }
  }

  for(kgmList<kNode*>::iterator i = materials.begin(); i != materials.end(); ++i)
  {
    fprintf(f, " <kgmMaterial name='%s'>\n", (*i)->nam.data());
    fprintf(f, " </kgmMaterial>\n");
  }

  for(kgmList<kNode*>::iterator i = lights.begin(); i != lights.end(); ++i)
  {
    kgmLight* l = (*i)->lgt;

    fprintf(f, " <kgmLight name='%s'>\n", (*i)->nam.data());
    fprintf(f, "  <Position value='%f %f %f'/>\n", l->position.x, l->position.y, l->position.z);
    fprintf(f, " </kgmLight>\n");
  }

  for(kgmList<kNode*>::iterator i = meshes.begin(); i != meshes.end(); ++i)
  {
    kgmMaterial* mtl = game->m_render->getMeshMaterial((*i)->msh);

    fprintf(f, " <kgmMesh name='%s' link='%s'>\n", (*i)->nam.data(), (*i)->lnk.data());

    if(mtl)
      fprintf(f, "  <Material value='%s'/>\n", mtl->m_id.data());
    else
      fprintf(f, "  <Material value=''/>\n");

    fprintf(f, "  <Position value='%f %f %f'/>\n", (*i)->pos.x, (*i)->pos.y, (*i)->pos.z);
    fprintf(f, "  <Rotation value='%f %f %f'/>\n", (*i)->rot.x, (*i)->rot.y, (*i)->rot.z);
    fprintf(f, " </kgmMesh>\n");
  }

  for(kgmList<kNode*>::iterator i = actors.begin(); i != actors.end(); ++i)
  {
    fprintf(f, " <kgmActor name='%s'>\n", (*i)->nam.data());
    fprintf(f, " </kgmActor>\n");
  }

  materials.clear();
  actors.clear();
  lights.clear();
  meshes.clear();

  fprintf(f, "</kgm>");
  fclose(f);

  return true;
}

bool kEditor::addMesh(kgmString fpath)
{
  kgmFile file;

  if(!file.open(fpath, kgmFile::Read))
    return false;

  kgmMemory<u8> mem;

  mem.alloc(file.length());
  file.read(mem, file.length());
  file.close();

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
    node->lnk = fdd->getFile();
    selected = node;
    nodes.add(node);
    vo->getGuiList()->addItem(node->nam);
    vo->getGuiList()->setSel(vo->getGuiList()->m_items.length() - 1);

    return true;
  }

  return false;
}

bool kEditor::addActor(kgmString path)
{
  return false;
}

bool kEditor::addMaterial(kgmString id)
{
  kgmMaterial* mtl = game->getResources()->getMaterial(id.data());

  if(!mtl)
    return false;

  if(!vo->hasItem(id))
  {
    kNode* node = new kNode(mtl);
    node->nam = id;

    game->m_render->add(mtl);
    vo->getGuiList()->addItem(id);
  }

  return false;
}

void kEditor::onEvent(kgmEvent::Event *e)
{
  kgmEvent::onEvent(e);

  if(menu->visible() && game->m_msAbs)
    menu->onEvent(e);

  if(fdd->visible())
    fdd->onEvent(e);

  if(vo->visible())
    vo->onEvent(e);

  if(vop && vop->visible())
    vop->onEvent(e);
}

void kEditor::onKeyUp(int k)
{
}

void kEditor::onKeyDown(int k)
{
}

void kEditor::onMsLeftUp(int k, int x, int y)
{
  game->setMsAbsolute(true);
  ms_click[0] = false;
}

void kEditor::onMsLeftDown(int k, int x, int y)
{
  ms_click[0] = true;
  game->setMsAbsolute(false);

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

  if(fdd->visible())
    game->setMsAbsolute(true);
  else if(selected && pivot->axis != kPivot::AXIS_NONE)
    game->setMsAbsolute(true);
}

void kEditor::onMsRightUp(int k, int x, int y)
{
  game->setMsAbsolute(true);

  ms_click[1] = false;
}

void kEditor::onMsRightDown(int k, int x, int y)
{
  if(!fdd->visible())
    game->setMsAbsolute(false);

  ms_click[1] = true;
}

void kEditor::onMsMove(int k, int x, int y)
{
  if(selected && pivot->axis != kPivot::AXIS_NONE && ms_click[0])
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
      //pln = plane(pivot->pos, vec3(0, 0, 1));
      //pln.intersect(ray, pr);
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
    //selected->setPosition(selected->pos + pr);

    mtx4 m;
    m.identity();
    m.translate(selected->pos);
    pivot->pos = selected->pos;
    game->m_render->set(pivot, m);


    return;
  }

  if(game->m_render && !game->m_msAbs)
  {
    if(ms_click[0])
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
  else if(gui == vo)
  {
    kgmString s = vo->getGuiList()->getItem(id);

    for(kgmList<kNode*>::iterator i = nodes.begin(); i != nodes.end(); ++i)
    {
      if((*i)->nam == s)
      {
        selected = *i;

        break;
      }
    }
  }
}

void kEditor::onQuit()
{
  game->close();
}

void kEditor::onMapOpen()
{
  fdd->setFilter(".map");
  fdd->changeLocation(false);
  fdd->forOpen(game->getSettings()->get("Path"), (kFileDialog::ClickEventCallback)&mapOpen, this);
}

void kEditor::onMapSave()
{
  fdd->setFilter(".map");
  fdd->changeLocation(false);
  fdd->forSave(game->getSettings()->get("Path"), (kFileDialog::ClickEventCallback)&mapSave, this);
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
    break;
  case kNode::MATERIAL:
    game->getRender()->remove(selected->mtl);
    break;
  }

  vo->getGuiList()->remItem(vo->getGuiList()->getSel());
}

void kEditor::onEditDuplicate()
{

}

void kEditor::onEditOptions()
{
  if(!selected)
    return;

  switch(selected->typ)
  {
  case kNode::MESH:
    vop = new kViewOptionsForMesh(selected, 50, 50, 200, 300);
    break;
  case kNode::LIGHT:
    vop = new kViewOptionsForLight(selected, 50, 50, 200, 300);
    break;
  case kNode::ACTOR:
    vop = new kViewOptionsForActor(selected, 50, 50, 200, 300);
    break;
  case kNode::MATERIAL:
    vop = new kViewOptionsForMaterial(selected, 50, 50, 200, 300);
    break;
  }

  if(vop)
  {
    vop->setCloseCallback(kViewOptions::CloseCallback(this, (kViewOptions::CloseCallback::Function)&onCloseVop));
    game->m_render->add(vop);
    vop->show();
  }
}

void kEditor::onAddMesh()
{
  fdd->setFilter(".msh");
  fdd->changeLocation(false);
  fdd->forOpen(game->getSettings()->get("Path"), (kFileDialog::ClickEventCallback)&addMesh, this);
}

void kEditor::onAddLight()
{
  kgmLight* l = new kgmLight();

  kNode* node = new kNode(l);
  node->bnd = box3(-1, -1, -1, 1, 1, 1);
  node->nam = kgmString("Light_") + kgmConvert::toString((s32)(++oquered));
  node->icn = new kgmGraphics::Icon(game->getResources()->getTexture("light_ico.tga"), 1, 1, vec3(0, 0, 0));

  selected = node;
  nodes.add(node);
  vo->getGuiList()->addItem(node->nam);
  vo->getGuiList()->setSel(vo->getGuiList()->m_items.length() - 1);

  game->m_render->add(l);
  game->m_render->add(node->icn);
}

void kEditor::onAddActor()
{
  fdd->setFilter(".act");
  fdd->changeLocation(false);
  fdd->forOpen(game->getSettings()->get("Path"), (kFileDialog::ClickEventCallback)&addActor, this);
}

void kEditor::onAddSensor()
{

}

void kEditor::onAddTrigger()
{

}

void kEditor::onAddMaterial()
{
  fdd->setFilter(".mtl");
  fdd->changeLocation(false);
  fdd->forOpen(game->getSettings()->get("Path"), (kFileDialog::ClickEventCallback)&addMaterial, this);
}

void kEditor::onRunRun()
{

}

void kEditor::onViewObjects()
{
  if(vo->visible())
    vo->hide();
  else
    vo->show();
}

void kEditor::onOptionsDatabase()
{
  kgmString loc = game->getSettings()->get("Path");

  fdd->changeLocation(true);

  if(!loc.length())
  {
    kgmString cwd;
    kgmSystem::getCurrentDirectory(cwd);
    fdd->forOpen(cwd);
  }
  else
  {
    fdd->forOpen(loc);
  }
}

void kEditor::onCloseVop()
{
  vop = null;
}
