#include "kEditor.h"

kEditor::kEditor()
{
  setMsAbsolute(true);

  ms_click[0] = ms_click[1] = ms_click[2] = false;

  cam_pos = vec3(0, 0, 0);
  cam_rot = 0.0f;

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

    fdd = new kFileDialog();
    fdd->hide();
    m_render->add(fdd);
  }
}

kEditor::~kEditor()
{
  gridline->release();
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
    }
  }
}

void kEditor::onQuit()
{
  close();
}

void kEditor::onMapOpen()
{
  fdd->forRead("/");
}

void kEditor::onMapSave()
{

}

