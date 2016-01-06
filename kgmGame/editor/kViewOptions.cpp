#include "kViewOptions.h"
#include "../../kgmBase/kgmLog.h"
#include "../../kgmBase/kgmConvert.h"
#include "../../kgmGame/kgmGameApp.h"
#include "../../kgmGame/kgmGameBase.h"
#include "kFileDialog.h"
#include "kViewObjects.h"

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
    g->setText("Pos");
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
    g->setText("Rot");
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
    g->setText("RotX");
    g = new kgmGuiScroll(tgeneral, 51, y_coord, 153, 20);
    g->show();
    ((kgmGuiScroll*)g)->setOrientation(kgmGuiScroll::ORIENT_HORIZONTAL);
    ((kgmGuiScroll*)g)->setRange(360);
    ((kgmGuiScroll*)g)->setPosition(RADTODEG(node->rot.x));
    ((kgmGuiScroll*)g)->setChangeEventCallback(kgmGuiScroll::ChangeEventCallback(this, (kgmGuiScroll::ChangeEventCallback::Function)&kViewOptions::onRotationX));
    y_coord += 23;
    g = new kgmGuiLabel(tgeneral, 0, y_coord, 50, 20);
    g->setText("RotY");
    g = new kgmGuiScroll(tgeneral, 51, y_coord, 153, 20);
    g->show();
    ((kgmGuiScroll*)g)->setOrientation(kgmGuiScroll::ORIENT_HORIZONTAL);
    ((kgmGuiScroll*)g)->setRange(360);
    ((kgmGuiScroll*)g)->setPosition(RADTODEG(node->rot.y));
    ((kgmGuiScroll*)g)->setChangeEventCallback(kgmGuiScroll::ChangeEventCallback(this, (kgmGuiScroll::ChangeEventCallback::Function)&kViewOptions::onRotationY));
    y_coord += 23;
    g = new kgmGuiLabel(tgeneral, 0, y_coord, 50, 20);
    g->setText("RotZ");
    g = new kgmGuiScroll(tgeneral, 51, y_coord, 153, 20);
    g->show();
    ((kgmGuiScroll*)g)->setOrientation(kgmGuiScroll::ORIENT_HORIZONTAL);
    ((kgmGuiScroll*)g)->setRange(360);
    ((kgmGuiScroll*)g)->setPosition(RADTODEG(node->rot.z));
    ((kgmGuiScroll*)g)->setChangeEventCallback(kgmGuiScroll::ChangeEventCallback(this, (kgmGuiScroll::ChangeEventCallback::Function)&kViewOptions::onRotationZ));

    y_coord += 23;
    kgmGuiCheck* lock = new kgmGuiCheck(tgeneral, 0, y_coord, 204, 20);
    lock->setText("Locked");
    lock->setCheck(node->lock);
    lock->setClickCallback(kgmGuiCheck::ClickEventCallback(this, (kgmGuiCheck::ClickEventCallback::Function)&kViewOptions::onSelectLock));
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

kViewOptionsForMaterial::kViewOptionsForMaterial(kNode* n, int x, int y, int w, int h)
:kViewOptions(n, x, y, w, h)
{
  kgmGui* tmaterial = tab->addTab("Material");
  y_coord = 1;

  y_coord += 23;
  kgmMaterial* mtl = n->getMaterial();

  if(!mtl)
    return;

  kgmGui* g = new kgmGuiLabel(tmaterial, 0, y_coord, 50, 20);
  g->setText("TexColor");
  g = guiTextTexColor = new kgmGuiText(tmaterial, 51, y_coord, 70, 20);

  if(mtl->getTexColor())
    g->setText(mtl->getTexColor()->m_id);

  kgmGuiButton* btn = new kgmGuiButton(tmaterial, 125, y_coord, 50, 20);
  btn->setText("select");
  slotSelectColor.connect(this, &kViewOptionsForMaterial::onSelectTexColor, &btn->sigClick);

  y_coord += 23;
  g = new kgmGuiLabel(tmaterial, 0, y_coord, 50, 20);
  g->setText("TexNormal");
  g = guiTextTexNormal = new kgmGuiText(tmaterial, 51, y_coord, 70, 20);

  if(mtl->getTexNormal())
    g->setText(mtl->getTexNormal()->m_id);

  btn = new kgmGuiButton(tmaterial, 125, y_coord, 50, 20);
  btn->setText("select");
  slotSelectNormal.connect(this, &kViewOptionsForMaterial::onSelectTexNormal, &btn->sigClick);

  y_coord += 23;
  g = new kgmGuiLabel(tmaterial, 0, y_coord, 50, 20);
  g->setText("TexSpecular");
  g = guiTextTexSpecular = new kgmGuiText(tmaterial, 51, y_coord, 70, 20);

  if(mtl->getTexSpecular())
    g->setText(mtl->getTexSpecular()->m_id);

  btn = new kgmGuiButton(tmaterial, 125, y_coord, 50, 20);
  btn->setText("select");
  slotSelectSpecular.connect(this, &kViewOptionsForMaterial::onSelectTexSpecular, &btn->sigClick);

  y_coord += 23;
  g = new kgmGuiLabel(tmaterial, 0, y_coord, 50, 20);
  g->setText("Shininess");
  g = new kgmGuiScroll(tmaterial, 51, y_coord, 140, 20);
  g->show();
  ((kgmGuiScroll*)g)->setOrientation(kgmGuiScroll::ORIENT_HORIZONTAL);
  ((kgmGuiScroll*)g)->setRange(100);
  ((kgmGuiScroll*)g)->setPosition(mtl->m_shininess - 1);
  ((kgmGuiScroll*)g)->setChangeEventCallback(kgmGuiScroll::ChangeEventCallback(this, (kgmGuiScroll::ChangeEventCallback::Function)&kViewOptionsForMaterial::onShininess));

  y_coord += 23;
  g = new kgmGuiLabel(tmaterial, 0, y_coord, 50, 20);
  g->setText("Transparency");
  g = new kgmGuiScroll(tmaterial, 51, y_coord, 140, 20);
  g->show();
  ((kgmGuiScroll*)g)->setOrientation(kgmGuiScroll::ORIENT_HORIZONTAL);
  ((kgmGuiScroll*)g)->setRange(100);
  ((kgmGuiScroll*)g)->setPosition(mtl->m_transparency);
  ((kgmGuiScroll*)g)->setChangeEventCallback(kgmGuiScroll::ChangeEventCallback(this, (kgmGuiScroll::ChangeEventCallback::Function)&kViewOptionsForMaterial::onTransparency));
  g = new kgmGuiLabel(tmaterial, 0, y_coord, 50, 20);
  g->setText("Shader");
  g = guiTextShader = new kgmGuiText(tmaterial, 51, y_coord, 70, 20);

  if(mtl->getShader())
    g->setText(mtl->getShader()->m_id);

  btn = new kgmGuiButton(tmaterial, 125, y_coord, 50, 20);
  btn->setText("select");
  slotSelectShader.connect(this, &kViewOptionsForMaterial::onSelectShader, &btn->sigClick);
}

void kViewOptionsForMaterial::onSelectFailed(kFileDialog* fd)
{
  fd->erase();
}

void kViewOptionsForMaterial::onSelectSuccess(kFileDialog* fd)
{
  switch (mode)
  {
    case Mode_Shader:
      this->node->getMaterial()->setShader(kgmIGame::getGame()->getResources()->getShader(fd->getFile()));
      guiTextShader->setText(fd->getFile());
      break;
    case Mode_Color:
      this->node->getMaterial()->setTexColor(kgmIGame::getGame()->getResources()->getTexture(fd->getFile()));
      guiTextTexColor->setText(fd->getFile());
      break;
    case Mode_Normal:
      this->node->getMaterial()->setTexNormal(kgmIGame::getGame()->getResources()->getTexture(fd->getFile()));
      guiTextTexNormal->setText(fd->getFile());
      break;
    case Mode_Specular:
      this->node->getMaterial()->setTexSpecular(kgmIGame::getGame()->getResources()->getTexture(fd->getFile()));
      guiTextTexSpecular->setText(fd->getFile());
      break;
    default:
      break;
  }

  fd->erase();
}

void kViewOptionsForMaterial::onSelectShader(int)
{
  mode = Mode_Shader;

  kFileDialog* fd = new kFileDialog();
  fd->m_rect.x = 300;
  fd->showHidden(false);
  fd->show();
  fd->setFilter("glsl");
  fd->setFailCallback(kFileDialog::ClickEventCallback(this, (kFileDialog::ClickEventCallback::Function)&kViewOptionsForMaterial::onSelectFailed));
  fd->forOpen(((kgmGameBase*)kgmGameApp::gameApplication()->game())->getSettings()->get("Path"), kFileDialog::ClickEventCallback(this, (kFileDialog::ClickEventCallback::Function)&kViewOptionsForMaterial::onSelectSuccess));
  ((kgmGameBase*)kgmGameApp::gameApplication()->game())->guiAdd(fd);
}

void kViewOptionsForMaterial::onSelectTexColor(int)
{
  mode = Mode_Color;

  kFileDialog* fd = new kFileDialog();
  fd->m_rect.x = 300;
  fd->showHidden(false);
  fd->show();
  fd->setFilter("tga");
  fd->setFailCallback(kFileDialog::ClickEventCallback(this, (kFileDialog::ClickEventCallback::Function)&kViewOptionsForMaterial::onSelectFailed));
  fd->forOpen(((kgmGameBase*)kgmGameApp::gameApplication()->game())->getSettings()->get("Path"), kFileDialog::ClickEventCallback(this, (kFileDialog::ClickEventCallback::Function)&kViewOptionsForMaterial::onSelectSuccess));
  ((kgmGameBase*)kgmGameApp::gameApplication()->game())->guiAdd(fd);
}

void kViewOptionsForMaterial::onSelectTexNormal(int)
{
  mode = Mode_Normal;

  kFileDialog* fd = new kFileDialog();
  fd->m_rect.x = 300;
  fd->showHidden(false);
  fd->show();
  fd->setFilter("tga");
  fd->setFailCallback(kFileDialog::ClickEventCallback(this, (kFileDialog::ClickEventCallback::Function)&kViewOptionsForMaterial::onSelectFailed));
  fd->forOpen(((kgmGameBase*)kgmGameApp::gameApplication()->game())->getSettings()->get("Path"), kFileDialog::ClickEventCallback(this, (kFileDialog::ClickEventCallback::Function)&kViewOptionsForMaterial::onSelectSuccess));
  ((kgmGameBase*)kgmGameApp::gameApplication()->game())->guiAdd(fd);
}

void kViewOptionsForMaterial::onSelectTexSpecular(int)
{
  mode = Mode_Specular;

  kFileDialog* fd = new kFileDialog();
  fd->m_rect.x = 300;
  fd->showHidden(false);
  fd->show();
  fd->setFilter("tga");
  fd->setFailCallback(kFileDialog::ClickEventCallback(this, (kFileDialog::ClickEventCallback::Function)&kViewOptionsForMaterial::onSelectFailed));
  fd->forOpen(((kgmGameBase*)kgmGameApp::gameApplication()->game())->getSettings()->get("Path"), kFileDialog::ClickEventCallback(this, (kFileDialog::ClickEventCallback::Function)&kViewOptionsForMaterial::onSelectSuccess));
  ((kgmGameBase*)kgmGameApp::gameApplication()->game())->guiAdd(fd);
}

void kViewOptionsForMaterial::onShininess(u32 s)
{
  kgmMaterial* mtl = node->getMaterial();

  if(!mtl)
    return;

  mtl->m_shininess = 1.0f + (float)s;
}

void kViewOptionsForMaterial::onTransparency(u32 s)
{
  kgmMaterial* mtl = node->getMaterial();

  if(!mtl)
    return;

  mtl->m_transparency = (float)s;
}

kViewOptionsForVisual::kViewOptionsForVisual(kNode* n, int x, int y, int w, int h)
:kViewOptionsForMaterial(n, x, y, w, h)
{
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

kViewOptionsForObject::kViewOptionsForObject(kNode* n, int x, int y, int w, int h)
:kViewOptions(n, x, y, w, h)
{
  kgmGui* gcollision = tab->addTab("Collide");
  y_coord = 1;

  kgmGuiCheck* chk = new kgmGuiCheck(gcollision, 1, y_coord, 150, 20);
  chk->setText("collide");
  chk->setCheck(node->col);
  chk->setClickCallback(kgmGuiCheck::ClickEventCallback(this, (kgmGuiCheck::ClickEventCallback::Function)&kViewOptionsForObject::onSelectCollision));
  y_coord += 23;

  kgmGuiCheck* chk_grp = new kgmGuiCheck(gcollision, 1, y_coord, 150, 20);
  chk_grp->setText("Shape Box");
  chk_grp->setClickCallback(kgmGuiCheck::ClickEventCallback(this, (kgmGuiCheck::ClickEventCallback::Function)&kViewOptionsForObject::onSelectShapeBox));

  if(node->shp == "box") chk_grp->setCheck(true);
  y_coord += 23;

  chk = new kgmGuiCheck(gcollision, 1, y_coord, 150, 20);
  chk->setText("Shape Sphere");
  chk->setClickCallback(kgmGuiCheck::ClickEventCallback(this, (kgmGuiCheck::ClickEventCallback::Function)&kViewOptionsForObject::onSelectShapeSphere));
  chk_grp->addGroup(chk);

  if(node->shp == "sphere") chk->setCheck(true);
  y_coord += 23;

  chk = new kgmGuiCheck(gcollision, 1, y_coord, 150, 20);
  chk->setText("Shape Mesh");
  chk->setClickCallback(kgmGuiCheck::ClickEventCallback(this, (kgmGuiCheck::ClickEventCallback::Function)&kViewOptionsForObject::onSelectShapeMesh));
  chk_grp->addGroup(chk);
  if(node->shp == "mesh") chk->setCheck(true);
  y_coord += 23;

  chk = new kgmGuiCheck(gcollision, 1, y_coord, 150, 20);
  chk->setText("Shape Plane");
  chk->setClickCallback(kgmGuiCheck::ClickEventCallback(this, (kgmGuiCheck::ClickEventCallback::Function)&kViewOptionsForObject::onSelectShapePlane));
  chk_grp->addGroup(chk);

  if(node->shp == "plane") chk->setCheck(true);
  y_coord += 23;

  chk = new kgmGuiCheck(gcollision, 1, y_coord, 150, 20);
  chk->setText("Shape Convex");
  chk->setClickCallback(kgmGuiCheck::ClickEventCallback(this, (kgmGuiCheck::ClickEventCallback::Function)&kViewOptionsForObject::onSelectShapeConvex));
  chk_grp->addGroup(chk);

  if(node->shp == "convex") chk->setCheck(true);
  y_coord += 23;

  kgmGui* g = new kgmGuiLabel(gcollision, 0, y_coord, 50, 20);
  g->setText("Bound");

  g = new kgmGuiText(gcollision, 51, y_coord, 50, 20);
  g->setSid("Length x");
  g->setText(kgmConvert::toString(n->bnd.max.x - n->bnd.min.x));
  ((kgmGuiText*)g)->setChangeEventCallback(kgmGuiText::ChangeEventCallback(this, (kgmGuiText::ChangeEventCallback::Function)&kViewOptionsForObject::onBoundX));
  ((kgmGuiText*)g)->setEditable(true);
  ((kgmGuiText*)g)->setNumeric(true);

  g = new kgmGuiText(gcollision, 102, y_coord, 50, 20);
  g->setSid("Width y");
  g->setText(kgmConvert::toString(n->bnd.max.y - n->bnd.min.y));
  ((kgmGuiText*)g)->setChangeEventCallback(kgmGuiText::ChangeEventCallback(this, (kgmGuiText::ChangeEventCallback::Function)&kViewOptionsForObject::onBoundY));
  ((kgmGuiText*)g)->setEditable(true);
  ((kgmGuiText*)g)->setNumeric(true);

  g = new kgmGuiText(gcollision, 154, y_coord, 50, 20);
  g->setSid("Height z");
  g->setText(kgmConvert::toString(n->bnd.max.z - n->bnd.min.z));
  ((kgmGuiText*)g)->setChangeEventCallback(kgmGuiText::ChangeEventCallback(this, (kgmGuiText::ChangeEventCallback::Function)&kViewOptionsForObject::onBoundZ));
  ((kgmGuiText*)g)->setEditable(true);
  ((kgmGuiText*)g)->setNumeric(true);
  y_coord += 23;

  kgmGui* tobject = tab->addTab("Object");
  y_coord = 1;

  kgmGuiCheck* enable = new kgmGuiCheck(tobject, 1, y_coord, 150, 20);
  enable->setText("Enabled");
  enable->setCheck(node->unt->valid());
  enable->setClickCallback(kgmGuiCheck::ClickEventCallback(this, (kgmGuiCheck::ClickEventCallback::Function)&kViewOptionsForObject::onSelectEnable));

  y_coord += 23;

  if(node->unt->m_variables.length() > 0)
    y_coord += 23;

  for(int i = 0; i < node->unt->m_variables.length(); i++)
  {
    kgmVariable& var = node->unt->m_variables[i];

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

void kViewOptionsForObject::onSelectCollision(bool s)
{
  node->col = s;
}

void kViewOptionsForObject::onSelectShapeBox(bool s)
{
  if(s)
    node->shp = "box";
}

void kViewOptionsForObject::onSelectShapeMesh(bool s)
{
  if(s)
    node->shp = "mesh";
}

void kViewOptionsForObject::onSelectShapePlane(bool s)
{
  if(s)
    node->shp = "plane";
}

void kViewOptionsForObject::onSelectShapeSphere(bool s)
{
  if(s)
    node->shp = "sphere";
}

void kViewOptionsForObject::onSelectShapeConvex(bool s)
{
  if(s)
    node->shp = "convex";
}

void kViewOptionsForObject::onBoundX(kgmString s)
{
  double v = kgmConvert::toDouble(s);

  node->bnd.min.x = -0.5 * v;
  node->bnd.max.x =  0.5 * v;
}

void kViewOptionsForObject::onBoundY(kgmString s)
{
  double v = kgmConvert::toDouble(s);

  node->bnd.min.y = -0.5 * v;
  node->bnd.max.y =  0.5 * v;
}

void kViewOptionsForObject::onBoundZ(kgmString s)
{
  double v = kgmConvert::toDouble(s);

  node->bnd.min.z = -0.5 * v;
  node->bnd.max.z =  0.5 * v;
}

void kViewOptionsForObject::onSelectEnable(bool state)
{
  if(state)
    node->unt->enable();
  else
    node->unt->disable();
}

void kViewOptionsForObject::updateVariable(kgmString id, kgmString data)
{
  for(int i = 0; i < node->unt->m_variables.length(); i++)
  {
    if(node->unt->m_variables[i].getName() == id)
    {
      switch(node->unt->m_variables[i].getType())
      {
      case kgmVariable::TString:
        node->unt->m_variables[i].setString(data);
        break;
      case kgmVariable::TInteger:
        node->unt->m_variables[i].setInteger(kgmConvert::toInteger(data));
        break;
      case kgmVariable::TFloat:
        node->unt->m_variables[i].setFloat(kgmConvert::toDouble(data));
        break;
      case kgmVariable::TBoolean:
        node->unt->m_variables[i].setBoolean(kgmConvert::toBoolean(data));
        break;
      }

      node->unt->eupdate();
    }
  }
}

kViewOptionsForSensor::kViewOptionsForSensor(kNode* n, int x, int y, int w, int h)
:kViewOptionsForObject(n, x, y, w, h)
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

kViewOptionsForUnit::kViewOptionsForUnit(kNode* n, int x, int y, int w, int h)
:kViewOptionsForObject(n, x, y, w, h)
{
  kgmGui* tunit = tab->addTab("Unit");

  y_coord = 1;
}

kViewOptionsForActor::kViewOptionsForActor(kNode* n, int x, int y, int w, int h)
:kViewOptionsForObject(n, x, y, w, h)
{
  kgmGui* tactor = tab->addTab("Actor");
  y_coord = 1;

  kgmGui* g = new kgmGuiLabel(tactor, 0, y_coord, 50, 20);
  g->setText("State");
  g = guiState = new kgmGuiText(tactor, 51, y_coord, 70, 20);

  kgmString state = node->act->getState();

  if(state.length())
    g->setText(state);

  kgmGuiButton* btn = new kgmGuiButton(tactor, 125, y_coord, 50, 20);
  btn->setText("select");
  btn->setClickCallback(kgmGuiButton::ClickEventCallback(this, (kgmGuiButton::ClickEventCallback::Function)&kViewOptionsForActor::showStates));

  y_coord += 23;

  kgmGuiCheck* enable = new kgmGuiCheck(tactor, 1, y_coord, 150, 20);
  enable->setText("Player");
  enable->setCheck(node->unt == kgmIGame::getGame()->getLogic()->getPlayer());
  enable->setClickCallback(kgmGuiCheck::ClickEventCallback(this, (kgmGuiCheck::ClickEventCallback::Function)&kViewOptionsForActor::onPlayer));

  y_coord += 23;
}

void kViewOptionsForActor::showStates()
{
  if(vo)
    return;
  vo = new kViewObjects();

  vo->setSelectCallback(kViewObjects::SelectCallback(this, (kViewObjects::SelectCallback::Function)&kViewOptionsForActor::onState));

  for(u32 i = 0; i < node->act->getStatesCount(); i++)
    vo->addItem(node->act->getStateName(i));

  ((kgmGameBase*)kgmGameApp::gameApplication()->game())->guiAdd(vo);
}

void kViewOptionsForActor::onState(kgmString state)
{
  node->act->setState(state);

  guiState->setText(state);

  vo->erase();
  vo = null;
}

void kViewOptionsForActor::onPlayer(bool state)
{
  if(state)
    kgmIGame::getGame()->getLogic()->setPlayer(node->act);
  else
    kgmIGame::getGame()->getLogic()->setPlayer(null);
}

kViewOptionsForEffect::kViewOptionsForEffect(kNode* n, int x, int y, int w, int h)
:kViewOptionsForObject(n, x, y, w, h)
{
  kgmGui* teffect = tab->addTab("Effect");
  y_coord = 1;
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
  kgmGui* tobstacle = tab->addTab("Obstacle");
  y_coord = 3;
  fd = null;
  kgmGui* g = new kgmGuiLabel(tobstacle, 0, y_coord, 50, 20);
  g->setText("Polygons");
  g = guiCnvText = new kgmGuiText(tobstacle, 51, y_coord, 70, 20);

  kgmGuiButton* btn = new kgmGuiButton(tobstacle, 125, y_coord, 50, 20);
  btn->setText("select");
  btn->setClickCallback(kgmGuiButton::ClickEventCallback(this, (kgmGuiButton::ClickEventCallback::Function)&kViewOptionsForObstacle::onSelectPolygons));
  y_coord += 23;

  btn = new kgmGuiButton(tobstacle, 0, y_coord, 50, 20);
  btn->setText("Rect");
  btn->setClickCallback(kgmGuiButton::ClickEventCallback(this, (kgmGuiButton::ClickEventCallback::Function)&kViewOptionsForObstacle::onRect));

  btn = new kgmGuiButton(tobstacle, 51, y_coord, 50, 20);
  btn->setText("Box");
  btn->setClickCallback(kgmGuiButton::ClickEventCallback(this, (kgmGuiButton::ClickEventCallback::Function)&kViewOptionsForObstacle::onBox));
  y_coord += 23;

  g = new kgmGuiLabel(tobstacle, 0, y_coord, 50, 20);
  g->setText("Scale");
  g = new kgmGuiText(tobstacle, 51, y_coord, 50, 20);
  g->setSid("scale");
  g->setText(kgmConvert::toString(n->obs->getScale()));
  ((kgmGuiText*)g)->setChangeEventCallback(kgmGuiText::ChangeEventCallback(this, (kgmGuiText::ChangeEventCallback::Function)&kViewOptionsForObstacle::onScale));
  ((kgmGuiText*)g)->setEditable(true);
  ((kgmGuiText*)g)->setNumeric(true);
  y_coord += 23;
}

void kViewOptionsForObstacle::onSelectPolygons()
{
  if(fd)
    return;

  fd = new kFileDialog();
  fd->m_rect.x = 300;
  fd->showHidden(false);
  fd->show();
  fd->setFilter("plg");
  fd->setFailCallback(kFileDialog::ClickEventCallback(this, (kFileDialog::ClickEventCallback::Function)&kViewOptionsForVisual::onSelectFailed));
  fd->forOpen(((kgmGameBase*)kgmGameApp::gameApplication()->game())->getSettings()->get("Path"), kFileDialog::ClickEventCallback(this, (kFileDialog::ClickEventCallback::Function)&kViewOptionsForObstacle::onSelectedPolygons));
  ((kgmGameBase*)kgmGameApp::gameApplication()->game())->guiAdd(fd);
}

void kViewOptionsForObstacle::onSelectedPolygons()
{
  node->setConvex(fd->getFile());
  guiCnvText->setText(fd->getFile());

  fd->erase();
  fd = null;
}

void kViewOptionsForObstacle::onScale(kgmString s)
{
  node->obs->set((f32) kgmConvert::toDouble(s));
}

void kViewOptionsForObstacle::onRect()
{
  node->obs->fromRect(vec2(-1, -1), vec2(1, 1));
}

void kViewOptionsForObstacle::onBox()
{
  node->obs->fromBox(vec3(-1, -1, -1), vec3(1, 1, 1));
}
