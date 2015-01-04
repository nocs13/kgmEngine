#include "kViewOptions.h"
#include "../../kgmBase/kgmLog.h"
#include "../../kgmBase/kgmConvert.h"
#include "../../kgmGame/kgmGameApp.h"
#include "../../kgmGame/kgmGameBase.h"
#include "kFileDialog.h"

using namespace kgmGameEditor;

KGMOBJECT_IMPLEMENT(kViewOptions, kgmGuiFrame);

kViewOptions::kViewOptions(kNode* n, int x, int y, int w, int h)
:callClose(null, null),
kgmGuiFrame("Options", x, y, w, h)
{
  tab = null;
  node = n;
  y_coord = 0;

  if(n)
  {
    kgmGui* g;

    tab = new kgmGuiTab(getClient(), 0, 0, getClient()->getRect().width(), getClient()->getRect().height());

    kgmGui* tgeneral = tab->addTab("General");

    g = new kgmGuiLabel(tgeneral, 0, y_coord, 50, 20);
    g->setText(n->nam);
    y_coord += 22;
    g = new kgmGuiLabel(tgeneral, 0, y_coord, 50, 20);
    g->setText("Position");
    g = new kgmGuiText(tgeneral, 51, y_coord, 50, 20);
    g->setSid("position_x");
    g->setText(kgmConvert::toString(n->pos.x));
    ((kgmGuiText*)g)->setChangeEventCallback(kgmGuiText::ChangeEventCallback(this, (kgmGuiText::ChangeEventCallback::Function)&kViewOptions::onPositionX));
    ((kgmGuiText*)g)->setEditable(true);
    ((kgmGuiText*)g)->setNumeric(true);
    g = new kgmGuiText(tgeneral, 102, y_coord, 50, 20);
    g->setSid("position_y");
    g->setText(kgmConvert::toString(n->pos.y));
    ((kgmGuiText*)g)->setChangeEventCallback(kgmGuiText::ChangeEventCallback(this, (kgmGuiText::ChangeEventCallback::Function)&kViewOptions::onPositionY));
    ((kgmGuiText*)g)->setEditable(true);
    ((kgmGuiText*)g)->setNumeric(true);
    g = new kgmGuiText(tgeneral, 154, y_coord, 50, 20);
    g->setSid("position_z");
    g->setText(kgmConvert::toString(n->pos.z));
    ((kgmGuiText*)g)->setChangeEventCallback(kgmGuiText::ChangeEventCallback(this, (kgmGuiText::ChangeEventCallback::Function)&kViewOptions::onPositionZ));
    ((kgmGuiText*)g)->setEditable(true);
    ((kgmGuiText*)g)->setNumeric(true);

    y_coord += 23;
    g = new kgmGuiLabel(tgeneral, 0, y_coord, 50, 20);
    g->setText("Rotation");
    g = new kgmGuiText(tgeneral, 51, y_coord, 50, 20);
    g->setSid("rotation_x");
    g->setText(kgmConvert::toString(n->rot.x));
    ((kgmGuiText*)g)->setEditable(true);
    ((kgmGuiText*)g)->setNumeric(true);
    g = new kgmGuiText(tgeneral, 102, y_coord, 50, 20);
    g->setSid("rotation_y");
    g->setText(kgmConvert::toString(n->rot.y));
    ((kgmGuiText*)g)->setEditable(true);
    ((kgmGuiText*)g)->setNumeric(true);
    g = new kgmGuiText(tgeneral, 154, y_coord, 50, 20);
    g->setSid("rotation_z");
    g->setText(kgmConvert::toString(n->rot.z));
    ((kgmGuiText*)g)->setEditable(true);
    ((kgmGuiText*)g)->setNumeric(true);

    y_coord += 23;
    g = new kgmGuiLabel(tgeneral, 0, y_coord, 50, 20);
    g->setText("Rotation X");
    g = new kgmGuiScroll(tgeneral, 51, y_coord, 140, 20);
    g->show();
    ((kgmGuiScroll*)g)->setOrientation(kgmGuiScroll::ORIENT_HORIZONTAL);
    ((kgmGuiScroll*)g)->setRange(360);
    ((kgmGuiScroll*)g)->setPosition(RADTODEG(node->rot.x));
    ((kgmGuiScroll*)g)->setChangeEventCallback(kgmGuiScroll::ChangeEventCallback(this, (kgmGuiScroll::ChangeEventCallback::Function)&kViewOptions::onRotationX));
    y_coord += 23;
    g = new kgmGuiLabel(tgeneral, 0, y_coord, 50, 20);
    g->setText("Rotation Y");
    g = new kgmGuiScroll(tgeneral, 51, y_coord, 140, 20);
    g->show();
    ((kgmGuiScroll*)g)->setOrientation(kgmGuiScroll::ORIENT_HORIZONTAL);
    ((kgmGuiScroll*)g)->setRange(360);
    ((kgmGuiScroll*)g)->setPosition(RADTODEG(node->rot.y));
    ((kgmGuiScroll*)g)->setChangeEventCallback(kgmGuiScroll::ChangeEventCallback(this, (kgmGuiScroll::ChangeEventCallback::Function)&kViewOptions::onRotationY));
    y_coord += 23;
    g = new kgmGuiLabel(tgeneral, 0, y_coord, 50, 20);
    g->setText("Rotation Z");
    g = new kgmGuiScroll(tgeneral, 51, y_coord, 140, 20);
    g->show();
    ((kgmGuiScroll*)g)->setOrientation(kgmGuiScroll::ORIENT_HORIZONTAL);
    ((kgmGuiScroll*)g)->setRange(360);
    ((kgmGuiScroll*)g)->setPosition(RADTODEG(node->rot.z));
    ((kgmGuiScroll*)g)->setChangeEventCallback(kgmGuiScroll::ChangeEventCallback(this, (kgmGuiScroll::ChangeEventCallback::Function)&kViewOptions::onRotationZ));

    y_coord += 23;
    kgmGuiCheck* lock = new kgmGuiCheck(tgeneral, 1, y_coord, 150, 20);
    lock->setText("Locked");
    lock->setCheck(node->lock);
    lock->setClickCallback(kgmGuiCheck::ClickEventCallback(this, (kgmGuiCheck::ClickEventCallback::Function)&kViewOptions::onSelectLock));

    kgmGui* gcollision = tab->addTab("Collide");
    y_coord = 1;

    kgmGuiCheck* chk = new kgmGuiCheck(gcollision, 1, y_coord, 150, 20);
    chk->setText("collide");
    chk->setCheck(node->col);
    kgm_log() <<  "collision ";
    kgm_log() << (s32)node->col;
    kgm_log() <<  " "; 
    kgm_log() << node->shp.data();
    kgm_log() << "\n";
    chk->setClickCallback(kgmGuiCheck::ClickEventCallback(this, (kgmGuiCheck::ClickEventCallback::Function)&kViewOptionsForMesh::onSelectCollision));
    y_coord += 23;

    kgmGuiCheck* chk_grp = new kgmGuiCheck(gcollision, 1, y_coord, 150, 20);
    chk_grp->setText("Shape Box");
    chk_grp->setClickCallback(kgmGuiCheck::ClickEventCallback(this, (kgmGuiCheck::ClickEventCallback::Function)&kViewOptionsForMesh::onSelectShapeBox));
    if(node->shp == "box") chk_grp->setCheck(true);
    y_coord += 23;

    chk = new kgmGuiCheck(gcollision, 1, y_coord, 150, 20);
    chk->setText("Shape Sphere");
    chk->setClickCallback(kgmGuiCheck::ClickEventCallback(this, (kgmGuiCheck::ClickEventCallback::Function)&kViewOptionsForMesh::onSelectShapeSphere));
    chk_grp->addGroup(chk);
    if(node->shp == "sphere") chk->setCheck(true);
    y_coord += 23;

    if(node->typ == kNode::MESH)
    {
      chk = new kgmGuiCheck(gcollision, 1, y_coord, 150, 20);
      chk->setText("Shape Mesh");
      chk->setClickCallback(kgmGuiCheck::ClickEventCallback(this, (kgmGuiCheck::ClickEventCallback::Function)&kViewOptionsForMesh::onSelectShapeMesh));
      chk_grp->addGroup(chk);
      if(node->shp == "mesh") chk->setCheck(true);
      y_coord += 23;

      chk = new kgmGuiCheck(gcollision, 1, y_coord, 150, 20);
      chk->setText("Shape Plane");
      chk->setClickCallback(kgmGuiCheck::ClickEventCallback(this, (kgmGuiCheck::ClickEventCallback::Function)&kViewOptionsForMesh::onSelectShapePlane));
      chk_grp->addGroup(chk);
      if(node->shp == "plane") chk->setCheck(true);
      y_coord += 23;
    }
    else if(node->typ == kNode::OBSTACLE)
    {
      chk = new kgmGuiCheck(gcollision, 1, y_coord, 150, 20);
      chk->setText("Shape Convex");
      chk->setClickCallback(kgmGuiCheck::ClickEventCallback(this, (kgmGuiCheck::ClickEventCallback::Function)&kViewOptionsForMesh::onSelectShapeConvex));
      chk_grp->addGroup(chk);
      if(node->shp == "convex") chk->setCheck(true);
      y_coord += 23;
    }

    g = new kgmGuiLabel(gcollision, 0, y_coord, 50, 20);
    g->setText("Bound");
    g = new kgmGuiText(gcollision, 51, y_coord, 50, 20);
    g->setSid("Length x");
    g->setText(kgmConvert::toString(n->bnd.max.x - n->bnd.min.x));
    ((kgmGuiText*)g)->setChangeEventCallback(kgmGuiText::ChangeEventCallback(this, (kgmGuiText::ChangeEventCallback::Function)&kViewOptions::onBoundX));
    ((kgmGuiText*)g)->setEditable(true);
    ((kgmGuiText*)g)->setNumeric(true);
    g = new kgmGuiText(gcollision, 102, y_coord, 50, 20);
    g->setSid("Width y");
    g->setText(kgmConvert::toString(n->bnd.max.y - n->bnd.min.y));
    ((kgmGuiText*)g)->setChangeEventCallback(kgmGuiText::ChangeEventCallback(this, (kgmGuiText::ChangeEventCallback::Function)&kViewOptions::onBoundY));
    ((kgmGuiText*)g)->setEditable(true);
    ((kgmGuiText*)g)->setNumeric(true);
    g = new kgmGuiText(gcollision, 154, y_coord, 50, 20);
    g->setSid("Height z");
    g->setText(kgmConvert::toString(n->bnd.max.z - n->bnd.min.z));
    ((kgmGuiText*)g)->setChangeEventCallback(kgmGuiText::ChangeEventCallback(this, (kgmGuiText::ChangeEventCallback::Function)&kViewOptions::onBoundZ));
    ((kgmGuiText*)g)->setEditable(true);
    ((kgmGuiText*)g)->setNumeric(true);
    y_coord += 23;
  }
}

void kViewOptions::onCloseOptions()
{
  if(callClose.hasObject() && callClose.hasFunction())
    callClose();

  erase();
}

void kViewOptions::onPositionX(kgmString s)
{
  node->pos.x = kgmConvert::toDouble(s);
  node->setPosition(node->pos);
}

void kViewOptions::onPositionY(kgmString s)
{
  node->pos.y = kgmConvert::toDouble(s);
  node->setPosition(node->pos);
}

void kViewOptions::onPositionZ(kgmString s)
{
  node->pos.z = kgmConvert::toDouble(s);
  node->setPosition(node->pos);
}

void kViewOptions::onRotationX(u32 s)
{
  node->rot.x = DEGTORAD(s);
  node->setRotation(node->rot);
}

void kViewOptions::onRotationY(u32 s)
{
  node->rot.y = DEGTORAD(s);
  node->setRotation(node->rot);
}

void kViewOptions::onRotationZ(u32 s)
{
  node->rot.z = DEGTORAD(s);
  node->setRotation(node->rot);
}

void kViewOptions::onSelectLock(bool s)
{
  node->lock = s;
}

void kViewOptions::onSelectCollision(bool s)
{
  node->col = s;
}

void kViewOptions::onSelectShapeBox(bool s)
{
  if(s)
    node->shp = "box";
}

void kViewOptions::onSelectShapeMesh(bool s)
{
  if(s)
    node->shp = "mesh";
}

void kViewOptions::onSelectShapePlane(bool s)
{
  if(s)
    node->shp = "plane";
}

void kViewOptions::onSelectShapeSphere(bool s)
{
  if(s)
    node->shp = "sphere";
}

void kViewOptions::onSelectShapeConvex(bool s)
{
  if(s)
    node->shp = "convex";
}

void kViewOptions::onBoundX(kgmString s)
{
  double v = kgmConvert::toDouble(s);

  node->bnd.min.x = -0.5 * v;
  node->bnd.max.x =  0.5 * v;
}

void kViewOptions::onBoundY(kgmString s)
{
  double v = kgmConvert::toDouble(s);

  node->bnd.min.y = -0.5 * v;
  node->bnd.max.y =  0.5 * v;
}

void kViewOptions::onBoundZ(kgmString s)
{
  double v = kgmConvert::toDouble(s);

  node->bnd.min.z = -0.5 * v;
  node->bnd.max.z =  0.5 * v;
}

kViewOptionsForMesh::kViewOptionsForMesh(kNode* n, int x, int y, int w, int h)
:kViewOptions(n, x, y, w, h)
{
  kgmGui* tmesh = tab->addTab("Mesh");
  y_coord = 1;
  fd = null;
  y_coord += 23;
  kgmGui* g = new kgmGuiLabel(tmesh, 0, y_coord, 50, 20);
  g->setText("Material");
  g = guiMtlText = new kgmGuiText(tmesh, 51, y_coord, 70, 20);

  if(node->mat.length())
    g->setText(node->mat);

  kgmGuiButton* btn = new kgmGuiButton(tmesh, 125, y_coord, 50, 20);
  btn->setText("select");
  btn->setClickCallback(kgmGuiButton::ClickEventCallback(this, (kgmGuiButton::ClickEventCallback::Function)&kViewOptionsForMesh::onSelectMaterial));

  y_coord += 23;
  g = new kgmGuiLabel(tmesh, 0, y_coord, 50, 20);
  g->setText("Shader");
  g = guiShdText = new kgmGuiText(tmesh, 51, y_coord, 70, 20);

  if(node->shd.length())
    g->setText(node->shd);

  btn = new kgmGuiButton(tmesh, 125, y_coord, 50, 20);
  btn->setText("select");
  btn->setClickCallback(kgmGuiButton::ClickEventCallback(this, (kgmGuiButton::ClickEventCallback::Function)&kViewOptionsForMesh::onSelectShader));
}

void kViewOptionsForMesh::onSelectMaterial()
{
  if(fd)
    return;

  fd = new kFileDialog();
  fd->m_rect.x = 300;
  fd->showHidden(false);
  fd->show();
  fd->setFilter("mtl");
  fd->setFailCallback(kgmGuiButton::ClickEventCallback(this, (kgmGuiButton::ClickEventCallback::Function)&kViewOptionsForMesh::onSelectFailed));
  fd->forOpen(((kgmGameBase*)kgmGameApp::gameApplication()->game())->getSettings()->get("Path"), kFileDialog::ClickEventCallback(this, (kFileDialog::ClickEventCallback::Function)&kViewOptionsForMesh::onSelectedMaterial));

  ((kgmGameBase*)kgmGameApp::gameApplication()->game())->guiAdd(fd);
}

void kViewOptionsForMesh::onSelectedMaterial()
{
  node->setMaterial(fd->getFile());
  guiMtlText->setText(fd->getFile());

  fd->erase();
  fd->release();
  fd = null;
}

void kViewOptionsForMesh::onSelectShader()
{
  if(fd || node->mat.length() < 1)
    return;

  fd = new kFileDialog();
  fd->m_rect.x = 300;
  fd->showHidden(false);
  fd->show();
  fd->setFilter("glsl");
  fd->setFailCallback(kgmGuiButton::ClickEventCallback(this, (kgmGuiButton::ClickEventCallback::Function)&kViewOptionsForMesh::onSelectFailed));
  fd->forOpen(((kgmGameBase*)kgmGameApp::gameApplication()->game())->getSettings()->get("Path"), kFileDialog::ClickEventCallback(this, (kFileDialog::ClickEventCallback::Function)&kViewOptionsForMesh::onSelectedShader));

  ((kgmGameBase*)kgmGameApp::gameApplication()->game())->guiAdd(fd);
}

void kViewOptionsForMesh::onSelectedShader()
{
  node->setShader(fd->getFile());
  guiShdText->setText(fd->getFile());

  fd->erase();
  fd->release();
  fd = null;
}

void kViewOptionsForMesh::onSelectFailed()
{
  fd->erase();
  fd->release();
  fd = null;
}

kViewOptionsForLight::kViewOptionsForLight(kNode* n, int x, int y, int w, int h)
:kViewOptions(n, x, y, w, h)
{
  kgmGui* tlight = tab->addTab("Light");
  y_coord = 1;
  kgmGui* g = new kgmGuiLabel(tlight, 0, y_coord, 50, 20);
  g->setText("Intensity");

  g = new kgmGuiText(tlight, 51, y_coord, 70, 20);

  g->setText(kgmConvert::toString(node->lgt->intensity));
  ((kgmGuiText*)g)->setEditable(true);
  ((kgmGuiText*)g)->setChangeEventCallback(kgmGuiText::ChangeEventCallback(this, (kgmGuiText::ChangeEventCallback::Function)&kViewOptionsForLight::setIntencity));
  y_coord += 23;

  kgmGuiCheck* check = new kgmGuiCheck(tlight, 1, y_coord, 150, 20);
  check->setText("Shadows");
  check->setCheck(node->lgt->shadows);
  check->setClickCallback(kgmGuiCheck::ClickEventCallback(this, (kgmGuiCheck::ClickEventCallback::Function)&kViewOptionsForLight::setShadows));
  y_coord += 23;
}

void kViewOptionsForLight::setIntencity(kgmString s)
{
  f32 in = kgmConvert::toDouble(s);

  node->lgt->intensity = in;
}

void kViewOptionsForLight::setShadows(bool s)
{
  node->lgt->shadows = s;
}

kViewOptionsForActor::kViewOptionsForActor(kNode* n, int x, int y, int w, int h)
:kViewOptionsForObject(n, x, y, w, h)
{
  kgmGui* tactor = tab->addTab("Actor");
  y_coord = 1;
  fd = null;
  y_coord += 23;
  kgmGui* g = new kgmGuiLabel(tactor, 0, y_coord, 50, 20);
  g->setText("Initial");
  g = guiInit = new kgmGuiText(tactor, 51, y_coord, 70, 20);

  if(node->ini.length())
    g->setText(node->ini);

  kgmGuiButton* btn = new kgmGuiButton(tactor, 125, y_coord, 50, 20);
  btn->setText("select");
  btn->setClickCallback(kgmGuiButton::ClickEventCallback(this, (kgmGuiButton::ClickEventCallback::Function)&kViewOptionsForActor::showInit));

  y_coord += 23;
}

void kViewOptionsForActor::showInit()
{
  if(fd)
    return;

  fd = new kFileDialog();
  fd->m_rect.x = 300;
  fd->showHidden(false);
  fd->show();
  fd->setFilter("act");
  fd->forOpen(((kgmGameBase*)kgmGameApp::gameApplication()->game())->getSettings()->get("Path"), kFileDialog::ClickEventCallback(this, (kFileDialog::ClickEventCallback::Function)&kViewOptionsForActor::onInit));

  ((kgmGameBase*)kgmGameApp::gameApplication()->game())->guiAdd(fd);
}

void kViewOptionsForActor::onInit()
{
  kgmMemory<u8> mem;
  kgmGameApp::gameApplication()->game()->getResources()->getFile(fd->getFile(), mem);

  kgmXml xml(mem);

  mem.clear();

  kgmGameTools::initActor(kgmGameApp::gameApplication()->game(), node->act, xml);

  node->ini = fd->getFile();

  fd->erase();
  fd->release();
  fd = null;
}

kViewOptionsForUnit::kViewOptionsForUnit(kNode* n, int x, int y, int w, int h)
:kViewOptionsForObject(n, x, y, w, h)
{
  kgmGui* tunit = tab->addTab("Unit");
  y_coord = 1;
}

kViewOptionsForEffect::kViewOptionsForEffect(kNode* n, int x, int y, int w, int h)
:kViewOptionsForObject(n, x, y, w, h)
{
  kgmGui* teffect = tab->addTab("effect");
  y_coord = 1;
}

kViewOptionsForSensor::kViewOptionsForSensor(kNode* n, int x, int y, int w, int h)
:kViewOptions(n, x, y, w, h)
{
  kgmGui* tsensor = tab->addTab("Sensor");
  y_coord = 1;

  kgmGui* g = new kgmGuiLabel(tsensor, 0, y_coord, 50, 20);
  g->setText("Target");

  g = new kgmGuiText(tsensor, 51, y_coord, 70, 20);

  g->setText(node->sns->getTarget());
  ((kgmGuiText*)g)->setEditable(true);
  ((kgmGuiText*)g)->setChangeEventCallback(kgmGuiText::ChangeEventCallback(this, (kgmGuiText::ChangeEventCallback::Function)&kViewOptionsForSensor::setTarget));

  y_coord += 23;
}

void kViewOptionsForSensor::setTarget(kgmString s)
{
  node->sns->setTarget(s);
}

kViewOptionsForObject::kViewOptionsForObject(kNode* n, int x, int y, int w, int h)
:kViewOptions(n, x, y, w, h)
{
  kgmGui* tobject = tab->addTab("Object");
  y_coord = 1;

  if(node->obj->m_variables.length() > 0)
    y_coord += 23;

  for(int i = 0; i < node->obj->m_variables.length(); i++)
  {
    kgmVariable& var = node->obj->m_variables[i];

    kgmGui* g = new kgmGuiLabel(tobject, 0, y_coord, 50, 20);
    g->setText(var.getName());

    g = new kGuiText(tobject, 51, y_coord, 70, 20);
    g->setSid(var.getName());

    ((kGuiText*)g)->setEditable(true);
    ((kGuiText*)g)->setChangeEventCallback(kGuiText::kChangeEventCallback(this, (kGuiText::kChangeEventCallback::Function)&kViewOptionsForObject::updateVariable));

    switch(var.getType())
    {
    case kgmVariable::TString:
      g->setText(var.getString());
      break;
    case kgmVariable::TInteger:
      g->setText(kgmConvert::toString(var.getInteger()));
      break;
    case kgmVariable::TFloat:
      g->setText(kgmConvert::toString(var.getFloat()));
      break;
    case kgmVariable::TBoolean:
      g->setText(kgmConvert::toString(var.getBoolean()));
      break;
    }

    y_coord += 23;

    if(y_coord > m_rect.height())
      m_rect.h = y_coord + 20;
  }
}

void kViewOptionsForObject::updateVariable(kgmString id, kgmString data)
{
  for(int i = 0; i < node->obj->m_variables.length(); i++)
  {
    if(node->obj->m_variables[i].getName() == id)
    {
      switch(node->obj->m_variables[i].getType())
      {
      case kgmVariable::TString:
        node->obj->m_variables[i].setString(data);
        break;
      case kgmVariable::TInteger:
        node->obj->m_variables[i].setInteger(kgmConvert::toInteger(data));
        break;
      case kgmVariable::TFloat:
        node->obj->m_variables[i].setFloat(kgmConvert::toDouble(data));
        break;
      case kgmVariable::TBoolean:
        node->obj->m_variables[i].setBoolean(kgmConvert::toBoolean(data));
        break;
      }

      node->obj->eupdate();
    }
  }
}

kViewOptionsForTrigger::kViewOptionsForTrigger(kNode* n, int x, int y, int w, int h)
:kViewOptions(n, x, y, w, h)
{
  kgmGui* ttrigger = tab->addTab("Trigger");
  y_coord = 1;

  kgmGui* g = new kgmGuiLabel(ttrigger, 0, y_coord, 50, 20);
  g->setText("Channels");

  g = new kgmGuiText(ttrigger, 51, y_coord, 70, 20);

  g->setText(kgmConvert::toString((s32)node->trg->getCount()));
  ((kgmGuiText*)g)->setEditable(true);
  ((kgmGuiText*)g)->setChangeEventCallback(kgmGuiText::ChangeEventCallback(this, (kgmGuiText::ChangeEventCallback::Function)&kViewOptionsForTrigger::setChanels));

  y_coord += 23;

  g = new kgmGuiLabel(ttrigger, 0, y_coord, 50, 20);
  g->setText("Target");

  g = new kgmGuiText(ttrigger, 51, y_coord, 70, 20);

  g->setText(node->trg->getTarget());
  ((kgmGuiText*)g)->setEditable(true);
  ((kgmGuiText*)g)->setChangeEventCallback(kgmGuiText::ChangeEventCallback(this, (kgmGuiText::ChangeEventCallback::Function)&kViewOptionsForTrigger::setTarget));

  y_coord += 23;
}

void kViewOptionsForTrigger::setChanels(kgmString s)
{
  u32 ch = kgmConvert::toDouble(s);

  node->trg->setCount(ch);
}

void kViewOptionsForTrigger::setTarget(kgmString s)
{
  node->trg->setTarget(s);
}

kViewOptionsForObstacle::kViewOptionsForObstacle(kNode* n, int x, int y, int w, int h)
:kViewOptions(n, x, y, w, h)
{
  kgmGui* tobs = tab->addTab("Obstacle");
  y_coord = 1;
  fd = null;
  y_coord += 23;
  kgmGui* g = new kgmGuiLabel(tobs, 0, y_coord, 50, 20);
  g->setText("Convex");
  g = guiCnvText = new kgmGuiText(tobs, 51, y_coord, 70, 20);

  if(node->lnk.length() > 0)
    g->setText(node->lnk);

  kgmGuiButton* btn = new kgmGuiButton(tobs, 125, y_coord, 50, 20);
  btn->setText("select");
  btn->setClickCallback(kgmGuiButton::ClickEventCallback(this, (kgmGuiButton::ClickEventCallback::Function)&kViewOptionsForObstacle::onSelectConvex));
  y_coord += 23;
}

void kViewOptionsForObstacle::onSelectConvex()
{
  if(fd)
    return;

  fd = new kFileDialog();
  fd->m_rect.x = 300;
  fd->showHidden(false);
  fd->show();
  fd->setFilter("cvx");
  fd->setFailCallback(kgmGuiButton::ClickEventCallback(this, (kgmGuiButton::ClickEventCallback::Function)&kViewOptionsForMesh::onSelectFailed));
  fd->forOpen(((kgmGameBase*)kgmGameApp::gameApplication()->game())->getSettings()->get("Path"), kFileDialog::ClickEventCallback(this, (kFileDialog::ClickEventCallback::Function)&kViewOptionsForObstacle::onSelectedConvex));
  ((kgmGameBase*)kgmGameApp::gameApplication()->game())->guiAdd(fd);
}

void kViewOptionsForObstacle::onSelectedConvex()
{
  node->setConvex(fd->getFile());
  guiCnvText->setText(fd->getFile());

  fd->erase();
  fd->release();
  fd = null;
}
