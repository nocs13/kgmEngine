#include "kEditor.h"
#include "../../kgmSystem/kgmSystem.h"

enum FDDMODE\
{
  FDDMOD_NONE,
  FDDMODE_MAP,
  FDDMODE_MESH
};

kEditor::kEditor()
{
  setMsAbsolute(true);

  ms_click[0] = ms_click[1] = ms_click[2] = false;

  cam_pos = vec3(0, 0, 0);
  cam_rot = 0.0f;

  selected = null;

  if(m_render)
  {
    menu = new kMenu(null, this);
    kgmGuiMenu::Item* item = menu->add("File");
    item->add(1, "Open");
    item->add(2, "Save");
    item->add(0, "Quit");
    item = menu->add("Edit");
    item->add(11, "Delete");
    item->add(12, "Duplicate");
    item = menu->add("Add");
    item->add(41, "Mesh");
    item->add(42, "Light");
    item->add(43, "Material");
    item->add(44, "Object");
    item->add(45, "Actor");
    item = menu->add("Run");
    item->add(21, "Run");
    item = menu->add("View");
    item->add(31, "Meshes");
    item->add(32, "Materials");
    item = menu->add("Help");
    item->add(0xffff, "About");
    m_render->add(menu);

    gridline = new kGridline();
    m_render->add(gridline, null);

    fdd = new kFDD(this);
    fdd->hide();
    m_render->add(fdd);
  }
}

kEditor::~kEditor()
{
  gridline->release();
}

kEditor::Node* kEditor::select(int x, int y)
{
  iRect vp = m_render->viewport();
  float unit_x = (2.0f*((float)(x-vp.x)/(vp.w-vp.x)))-1.0f,
        unit_y = 1.0f-(2.0f*((float)(y-vp.y)/(vp.h - vp.y)));

  kgmCamera cam = m_render->camera();

  vec3 ms;
  ms.x = (2.0f * x) / vp.w - 1.0f;
  ms.y = 1.0f - (2.0f * y) / vp.h;
  ms.z = 1.0f;

  kgmRay3d<float> ray(ms, cam.mDir);

  for(kgmList<Node*>::iterator i = nodes.begin(); i != nodes.end(); i++)
  {
    vec3 c;

    if((*i)->bound().intersect(ray, c))
      return *i;
  }

  //float mx = (float)((x - vp.w * 0.5) * (1.0 / vp.w) * cam.mFov * 0.5);
  //float my = (float)((y - vp.h * 0.5) * (1.0 / vp.w) * cam.mFov * 0.5);
  //float4 dx = cameraRight * mx;
  //float4 dy = cam.mUp * my;

  //float3 dir = normalize(cameraDir + (dx + dy).xyz * 2.0);

}

void kEditor::onEvent(kgmEvent::Event *e)
{
  kgmGameBase::onEvent(e);

  if(menu->visible())
    menu->onEvent(e);

  if(fdd->visible())
    fdd->onEvent(e);
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
    selected = select(x, y);
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
      kgmCamera& cam = m_render->camera();

      cam.mPos.z += 0.01 * -y;
      cam.update();
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
    case 0:
      onQuit();
      break;
    case 1:
      onMapOpen();
      break;
    case 2:
      onMapSave();
      break;
    case 41:
      onAddMesh();
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
            node->set(mesh->bound());
            selected = node;
           nodes.add(node);
          }
        }
      }
      else if(fddMode == FDDMODE_MAP)
      {

      }
      fddMode = 0;
      break;
    case 2:
      fddMode = 0;
      break;
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
