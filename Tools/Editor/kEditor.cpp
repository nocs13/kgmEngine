#include "kEditor.h"

kEditor::kEditor()
{
  setMsAbsolute(true);

  ms_click[0] = ms_click[0] = ms_click[0] = false;

  cam_pos = vec3(0, 0, 0);
  cam_rot = 0.0f;


  if(m_render)
  {
    menu = new kgmGuiMenu(null);
    kgmGuiMenu::Item* item = menu->add(-1, "File");
    item->add(0, "Quit");
    item = menu->add(-1, "Help");
    item->add(0xffff, "About");
    m_render->add(menu);


    gridline = new kGridline();
    m_render->add(gridline, null);
  }
}

kEditor::~kEditor()
{
  gridline->release();
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

  setMsAbsolute(false);
  ms_click[1] = true;
}

void kEditor::onMsMove(int k, int x, int y)
{
  kgmGameBase::onMsMove(k, x, y);

  if(menu)
    menu->onMsMove(k, x, y);

  if(m_render)
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
