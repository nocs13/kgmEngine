#include "kEditor.h"
#include "../../kgmBase/kgmConvert.h"
#include "../../kgmSystem/kgmSystem.h"

enum FDDMODE
{
  FDDMOD_NONE,
  FDDMODE_MAP,
  FDDMODE_MESH,
  FDDMODE_PATH
};

enum MENUEVENT
{
  ME_FILE_QUIT,
  ME_FILE_OPEN,
  ME_FILE_SAVE,
  ME_EDIT_REMOVE,
  ME_EDIT_DUPLICATE,
  ME_ADD_MESH,
  ME_ADD_LIGHT,
  ME_ADD_ACTOR,
  ME_ADD_OBJECT,
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
    menu = new kMenu(null, this);
    kgmGuiMenu::Item* item = menu->add("File");
    item->add(ME_FILE_OPEN, "Open");
    item->add(ME_FILE_SAVE, "Save");
    item->add(ME_FILE_QUIT, "Quit");
    item = menu->add("Edit");
    item->add(ME_EDIT_REMOVE, "Remove");
    item->add(ME_EDIT_DUPLICATE, "Duplicate");
    item = menu->add("Add");
    item->add(ME_ADD_MESH, "Mesh");
    item->add(ME_ADD_LIGHT, "Light");
    item->add(ME_ADD_ACTOR, "Actor");
    item->add(ME_ADD_OBJECT, "Object");
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

kEditor::Node* kEditor::select(int x, int y)
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
    md = cam.mPos - ms;
    md.normalize();

    vec3 nor(0, 0, 1), pos(0, 0, 0), c;
    kgmPlane3d<float> pl(nor, pos);

    ray = kgmRay3d<float>(cam.mPos, md);

    //if(pl.intersect(ray, c))
    {
      mtx4 m;
      m.identity();
      m.translate(ms);
      //m_render->set(pivot, m);
    }
  }

  for(kgmList<Node*>::iterator i = nodes.begin(); i != nodes.end(); ++i)
  {
    vec3 c;

    if((*i)->bnd.intersect(ray, c))
      return *i;
  }
}

bool kEditor::mapOpen(kgmString s)
{
  return true;
}

bool kEditor::mapSave(kgmString s)
{
  FILE* f = fopen(s.data(), "w");

  if(!f)
    return false;

  fprintf(f, "<?xml version='1.0'?>\n");
  fprintf(f, "<kgm>\n");

  kgmList<Node*> meshes;
  kgmList<Node*> lights;
  kgmList<Node*> actors;
  kgmList<Node*> materials;

  for(kgmList<Node*>::iterator i = nodes.begin(); i != nodes.end(); ++i)
  {
    switch ((*i)->typ)
    {
    case Node::MESH:
      meshes.add(*i);
      continue;
    case Node::LIGHT:
      lights.add(*i);
      continue;
    case Node::ACTOR:
      actors.add(*i);
      continue;
    case Node::MATERIAL:
      materials.add(*i);
      continue;
    default:
      continue;
    }
  }

  for(kgmList<Node*>::iterator i = materials.begin(); i != materials.end(); ++i)
  {
    fprintf(f, " <kgmMaterial name='%s'>\n", (*i)->nam.data());
    fprintf(f, " </kgmMaterial>\n");
  }

  for(kgmList<Node*>::iterator i = lights.begin(); i != lights.end(); ++i)
  {
    fprintf(f, " <kgmLight name='%s'>\n", (*i)->nam.data());
    fprintf(f, " </kgmLight>\n");
  }

  for(kgmList<Node*>::iterator i = meshes.begin(); i != meshes.end(); ++i)
  {
    fprintf(f, " <kgmMesh name='%s' link='%s'>\n", (*i)->nam.data(), (*i)->lnk.data());
    fprintf(f, " </kgmMesh>\n");
  }

  for(kgmList<Node*>::iterator i = actors.begin(); i != actors.end(); ++i)
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

  if(nodes.length() > 0)
  {
    selected = select(x, y);

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

        if(selected->typ == Node::MESH)
        {
          m_render->set(selected->msh, m);
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
    case ME_ADD_MESH:
      onAddMesh();
      break;
    case ME_VIEW_OBJECTS:
      onViewObjects();
      break;
    case ME_OPTIONS_DATABASE:
      onOptionsDatabase();
      break;
    }
  }
  else if(gui == fdd)
  {
    switch(id)
    {
    case 0:
      fddMode = 0;
      break;
    case 1:
      if(fddMode == FDDMODE_MESH)
      {
        kgmFile file;
        kgmString fpath = fdd->getPath();

        if(file.open(fpath, kgmFile::Read))
        {
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
            Node* node = new Node(mesh);
            node->bnd = mesh->bound();
            node->nam = kgmString("Mesh_") + kgmConvert::toString((s32)(++oquered));
            node->lnk = fdd->getFile();
            selected = node;
            nodes.add(node);
            vo->getGuiList()->addItem(node->nam);
            vo->getGuiList()->setSel(vo->getGuiList()->m_items.length() - 1);
          }
        }
      }
      else if(fddMode == FDDMODE_MAP)
      {
        mapOpen(fdd->getPath());
      }
      else if(fddMode == FDDMODE_PATH)
      {

      }
      fddMode = 0;
      break;
    case 2:
      if(fddMode == FDDMODE_MAP)
      {
        mapSave(fdd->getPath());
      }
      fddMode = 0;
      break;
    }
  }
  else if(gui == vo)
  {
    kgmString s = vo->getGuiList()->getItem(id);

    for(kgmList<Node*>::iterator i = nodes.begin(); i != nodes.end(); ++i)
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
  fddMode = FDDMODE_MAP;

  if(fdd->getFolder().length())
  {
    fdd->forOpen("");
  }
  else
  {
    kgmString path;
    kgmSystem::getHomeDirectory(path);
    fdd->forOpen(path);
  }
}

void kEditor::onMapSave()
{
  fddMode = FDDMODE_MAP;

  if(fdd->getFolder().length())
  {
    fdd->forSave("");
  }
  else
  {
    kgmString path;
    kgmSystem::getHomeDirectory(path);
    fdd->forSave(path);
  }
}

void kEditor::onAddMesh()
{
  fddMode = FDDMODE_MESH;

  if(fdd->getFolder().length())
  {
    fdd->forOpen("");
  }
  else
  {
    kgmString path;
    kgmSystem::getHomeDirectory(path);
    fdd->forOpen(path);
  }
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
  fddMode = FDDMODE_PATH;

  if(fdd->getFolder().length())
  {
    fdd->forOpen("");
  }
  else
  {
    kgmString path;
    kgmSystem::getHomeDirectory(path);
    fdd->forOpen(path);
  }
}
