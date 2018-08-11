#include "kEditor.h"
#include "kViewOptions.h"

#include "../../kgmBase/kgmConvert.h"
#include "../../kgmSystem/kgmSystem.h"

enum MENUEVENT
{
  ME_FILE_QUIT,
  ME_FILE_OPEN,
  ME_FILE_SAVE,
  ME_EDIT_REMOVE,
  ME_EDIT_DUPLICATE,
  ME_EDIT_OPTIONS,
  ME_MAP_OPEN,
  ME_MAP_SAVE,
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

kEditor::kEditor()
{
  setMsAbsolute(true);

  ms_click[0] = ms_click[1] = ms_click[2] = false;

  cam_pos = vec3(0, 0, 0);
  cam_rot = 0.0f;

  selected = null;

  oquered = 0;

  if(m_render)
  {
    m_render->setEditor(true);

    menu = new kMenu(null, this);
    kgmGuiMenu::Item* item = menu->add("File");
    item->add(ME_FILE_OPEN, "Open");
    item->add(ME_FILE_SAVE, "Save");
    item->add(ME_FILE_QUIT, "Quit");
    item = menu->add("Edit");
    item->add(ME_EDIT_REMOVE, "Remove");
    item->add(ME_EDIT_DUPLICATE, "Duplicate");
    item->add(ME_EDIT_OPTIONS, "Options");
    item = menu->add("Map");
    item->add(ME_MAP_OPEN, "Open");
    item->add(ME_MAP_SAVE, "Save");
    item = menu->add("Add");
    item->add(ME_ADD_MESH, "Mesh");
    item->add(ME_ADD_LIGHT, "Light");
    item->add(ME_ADD_ACTOR, "Actor");
    item->add(ME_ADD_SENSOR, "Sensor");
    item->add(ME_ADD_TRIGGER, "Trigger");
    item->add(ME_ADD_MATERIAL, "Material");
    item = menu->add("Run");
    item->add(ME_RUN_RUN, "Run");
    item = menu->add("View");
    item->add(ME_VIEW_OBJECTS, "Objects");
    item = menu->add("Options");
    item->add(ME_OPTIONS_DATABASE, "Database");
    item = menu->add("Help");
    item->add(ME_HELP_ABOUT, "About");
    m_render->add(menu);

    gridline = new kGridline();
    m_render->add(gridline, null);

    pivot = new kPivot();
    m_render->add(pivot, null);

    fdd = new kFDD(this);
    fdd->showHidden(false);
    fdd->hide();
    m_render->add(fdd);

    vo = new kViewObjects(this, 1, 50, 100, 300);
    vo->hide();
    m_render->add(vo);

    m_render->setBgColor(0xffbbaa99);
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
  m_render->clear();
  m_physics->clear();

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
  iRect vp = m_render->viewport();
  float unit_x = (2.0f * ((float)(x - vp.x) / (vp.w - vp.x))) - 1.0f,
        unit_y = (2.0f * ((float)(y - vp.y) / (vp.h - vp.y))) - 1.0f;

  kgmCamera cam = m_render->camera();
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

    //if(pl.intersect(ray, c))
    {
      plane3 pln(vec3(0,0,1), vec3(0,0,0));
      ms = vec3(0,0,0);
      pln.intersect(ray, ms);

      mtx4 m;
      m.identity();
      m.translate(ms);
      //m_render->set(pivot, m);
    }
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

      return *i;
    }
  }
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
  m_render->add(gridline, null);

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
        m_render->add(node->mtl);
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
        node->icn = new kgmGraphics::Icon(getResources()->getTexture("light_ico.tga"), 1, 1, vec3(0, 0, 0));

        vo->getGuiList()->addItem(node->nam);
        vo->getGuiList()->setSel(vo->getGuiList()->m_items.length() - 1);

        m_render->add(node->lgt);
        m_render->add(node->icn);

        nodes.add(node);
      }
      else if(id == "kgmMesh")
      {
        kgmString id, ln;
        xml.attribute("name", id);
        xml.attribute("link", ln);
        kgmMesh* mesh = m_resources->getMesh(ln);
        kgmMaterial* mtl = m_resources->getMaterial(ln);

        if(mesh)
        {
          node = new kNode(mesh);
          node->nam = id;
          node->bnd = mesh->bound();
          m_render->add(node->msh, mtl);
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
    kgmMaterial* mtl = m_render->getMeshMaterial((*i)->msh);

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
    m_render->add(mesh, null);
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
  kgmMaterial* mtl = m_resources->getMaterial(id.data());

  if(!mtl)
    return false;

  if(!vo->hasItem(id))
  {
    kNode* node = new kNode(mtl);
    node->nam = id;

    m_render->add(mtl);
    vo->getGuiList()->addItem(id);
  }


  return false;
}

void kEditor::onEvent(kgmEvent::Event *e)
{
  kgmGameBase::onEvent(e);

  if(menu->visible() && m_msAbs)
    menu->onEvent(e);

  if(fdd->visible())
    fdd->onEvent(e);

  if(vo->visible())
    vo->onEvent(e);
}

void kEditor::onKeyUp(int k)
{
  kgmGameBase::onKeyUp(k);
}

void kEditor::onKeyDown(int k)
{
  kgmGameBase::onKeyDown(k);
}

void kEditor::onMsLeftUp(int k, int x, int y)
{
  kgmGameBase::onMsLeftUp(k, x, y);

  setMsAbsolute(true);
  ms_click[0] = false;
}

void kEditor::onMsLeftDown(int k, int x, int y)
{
  kgmGameBase::onMsLeftDown(k, x, y);

  if(!fdd->visible())
    setMsAbsolute(false);

  ms_click[0] = true;

  if(m_keys[KEY_Z])
    return;

  if(nodes.length() > 0)
  {
    //selected = select(x, y);

    if(selected)
    {
      mtx4 m;
      m.identity();
      m.translate(selected->pos);
      m_render->set(pivot, m);
    }
  }
  else
  {
    select(x, y);
  }
}

void kEditor::onMsRightUp(int k, int x, int y)
{
  kgmGameBase::onMsRightUp(k, x, y);

  setMsAbsolute(true);
  ms_click[1] = false;
}

void kEditor::onMsRightDown(int k, int x, int y)
{
  kgmGameBase::onMsRightDown(k, x, y);

  if(!fdd->visible())
    setMsAbsolute(false);

  ms_click[1] = true;
}

void kEditor::onMsMove(int k, int x, int y)
{
  kgmGameBase::onMsMove(k, x, y);

  if(m_render && !m_msAbs)
  {
    if(ms_click[0])
    {
      kgmCamera& cam = m_render->camera();

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
      if(m_keys[KEY_Z] && selected)
      {
        selected->pos.x += 0.01 * x;
        selected->pos.y += 0.01 * y;

        mtx4 m;
        m.identity();
        m.translate(selected->pos);

        if(selected->typ == kNode::MESH)
        {
          m_render->set(selected->msh, m);
        }
        else if(selected->typ == kNode::LIGHT)
        {
          selected->lgt->position = selected->pos;
          selected->icn->setPosition(selected->pos);
        }
      }
      else
      {
        kgmCamera& cam = m_render->camera();

        cam.mPos.z += 0.01 * -y;
        cam.update();
      }
    }
  }
}

void kEditor::onMsWheel(int k, int x, int y, int z)
{
  kgmGameBase::onMsWheel(k, x, y, z);
}

void kEditor::onAction(kgmEvent *gui, int id)
{
  if(gui == menu)
  {
    switch(id)
    {
    case ME_FILE_QUIT:
      onQuit();
      break;
    case ME_FILE_OPEN:
      onMapOpen();
      break;
    case ME_FILE_SAVE:
      onMapSave();
      break;
    case ME_EDIT_OPTIONS:
      onEditOptions();
      break;
    case ME_MAP_OPEN:
      onMapOpen();
      break;
    case ME_MAP_SAVE:
      onMapSave();
      break;
    case ME_ADD_MESH:
      onAddMesh();
      break;
    case ME_ADD_LIGHT:
      onAddLight();
      break;
    case ME_ADD_MATERIAL:
      onAddMaterial();
      break;
    case ME_VIEW_OBJECTS:
      onViewObjects();
      break;
    case ME_OPTIONS_DATABASE:
      onOptionsDatabase();
      break;
    }
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
  close();
}

void kEditor::onMapOpen()
{
  fdd->setFilter(".map");
  fdd->changeLocation(false);
  fdd->forOpen(m_settings->get("Path"), callMapOpen, this);
}

void kEditor::onMapSave()
{
  fdd->setFilter(".map");
  fdd->changeLocation(false);
  fdd->forSave(m_settings->get("Path"), callMapSave, this);
}

void kEditor::onEditOptions()
{
  if(!selected)
    return;

  kViewOptions* vop = null;

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
    m_render->add(vop);
    addListener(vop);
    vop->show();
  }
}

void kEditor::onAddMesh()
{
  fdd->setFilter(".msh");
  fdd->changeLocation(false);
  fdd->forOpen(m_settings->get("Path"), callAddMesh, this);
}

void kEditor::onAddLight()
{
  kgmLight* l = new kgmLight();

  kNode* node = new kNode(l);
  node->bnd = box3(-1, -1, -1, 1, 1, 1);
  node->nam = kgmString("Light_") + kgmConvert::toString((s32)(++oquered));
  node->icn = new kgmGraphics::Icon(getResources()->getTexture("light_ico.tga"), 1, 1, vec3(0, 0, 0));

  selected = node;
  nodes.add(node);
  vo->getGuiList()->addItem(node->nam);
  vo->getGuiList()->setSel(vo->getGuiList()->m_items.length() - 1);

  m_render->add(l);
  m_render->add(node->icn);
}

void kEditor::onAddActor()
{
  fdd->setFilter(".act");
  fdd->changeLocation(false);
  fdd->forOpen(m_settings->get("Path"), callAddActor, this);
}

void kEditor::onAddMaterial()
{
  fdd->setFilter(".mtl");
  fdd->changeLocation(false);
  fdd->forOpen(m_settings->get("Path"), callAddMaterial, this);
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
  kgmString loc = m_settings->get("Path");

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

void kEditor::callMapOpen(void *par)
{
  ((kEditor*)par)->mapOpen(((kEditor*)par)->fdd->getPath());
}

void kEditor::callMapSave(void *par)
{
  ((kEditor*)par)->mapSave(((kEditor*)par)->fdd->getPath());
}

void kEditor::callAddMesh(void *par)
{
  ((kEditor*)par)->addMesh(((kEditor*)par)->fdd->getFile());
}

void kEditor::callAddActor(void *par)
{
  ((kEditor*)par)->addActor(((kEditor*)par)->fdd->getFile());
}

void kEditor::callAddMaterial(void *par)
{
  ((kEditor*)par)->addMaterial(((kEditor*)par)->fdd->getFile());
}
