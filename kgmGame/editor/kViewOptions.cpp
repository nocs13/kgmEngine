#include "kViewOptions.h"
#include "../../kgmBase/kgmConvert.h"
#include "../../kgmGame/kgmGameApp.h"
#include "../../kgmGame/kgmGameBase.h"
#include "kFileDialog.h"

using namespace kgmGameEditor;

kViewOptions::kViewOptions(kNode* n, int x, int y, int w, int h)
:callClose(null, null),
kgmGui(null, x, y, w, h)
{
  //tab = new kgmGuiTab(this, 0, 0, w, h);
  tab = null;
  node = n;
  y_coord = 0;

  btn_close = new kgmGuiButton(this, w - 21, y_coord + 1, 20, 20);
  btn_close->setText("X");
  btn_close->setClickCallback(kgmGuiButton::ClickEventCallback(this, (kgmGuiButton::ClickEventCallback::Function)&kViewOptions::onCloseOptions));

  if(n)
  {
    kgmGui* g;

    g = new kgmGuiLabel(this, 0, y_coord, 50, 20);
    g->setText(n->nam);
    y_coord += 22;
    g = new kgmGuiLabel(this, 0, y_coord, 50, 20);
    g->setText("Position");
    g = new kgmGuiText(this, 51, y_coord, 50, 20);
    g->setSid("position_x");
    g->setText(kgmConvert::toString(n->pos.x));
    ((kgmGuiText*)g)->setChangeEventCallback(kgmGuiText::ChangeEventCallback(this, (kgmGuiText::ChangeEventCallback::Function)&kViewOptions::onPositionX));
    ((kgmGuiText*)g)->setEditable(true);
    ((kgmGuiText*)g)->setNumeric(true);
    g = new kgmGuiText(this, 102, y_coord, 50, 20);
    g->setSid("position_y");
    g->setText(kgmConvert::toString(n->pos.y));
    ((kgmGuiText*)g)->setChangeEventCallback(kgmGuiText::ChangeEventCallback(this, (kgmGuiText::ChangeEventCallback::Function)&kViewOptions::onPositionY));
    ((kgmGuiText*)g)->setEditable(true);
    ((kgmGuiText*)g)->setNumeric(true);
    g = new kgmGuiText(this, 154, y_coord, 50, 20);
    g->setSid("position_z");
    g->setText(kgmConvert::toString(n->pos.z));
    ((kgmGuiText*)g)->setChangeEventCallback(kgmGuiText::ChangeEventCallback(this, (kgmGuiText::ChangeEventCallback::Function)&kViewOptions::onPositionZ));
    ((kgmGuiText*)g)->setEditable(true);
    ((kgmGuiText*)g)->setNumeric(true);

    y_coord += 23;
    g = new kgmGuiLabel(this, 0, y_coord, 50, 20);
    g->setText("Rotation");
    g = new kgmGuiText(this, 51, y_coord, 50, 20);
    g->setSid("rotation_x");
    g->setText(kgmConvert::toString(n->rot.x));
    ((kgmGuiText*)g)->setEditable(true);
    ((kgmGuiText*)g)->setNumeric(true);
    g = new kgmGuiText(this, 102, y_coord, 50, 20);
    g->setSid("rotation_y");
    g->setText(kgmConvert::toString(n->rot.y));
    ((kgmGuiText*)g)->setEditable(true);
    ((kgmGuiText*)g)->setNumeric(true);
    g = new kgmGuiText(this, 154, y_coord, 50, 20);
    g->setSid("rotation_z");
    g->setText(kgmConvert::toString(n->rot.z));
    ((kgmGuiText*)g)->setEditable(true);
    ((kgmGuiText*)g)->setNumeric(true);

    y_coord += 23;
    g = new kgmGuiLabel(this, 0, y_coord, 50, 20);
    g->setText("Rotation X");
    g = new kgmGuiScroll(this, 51, y_coord, 140, 20);
    g->show();
    ((kgmGuiScroll*)g)->setOrientation(kgmGuiScroll::ORIENT_HORIZONTAL);
    ((kgmGuiScroll*)g)->setRange(360);
    ((kgmGuiScroll*)g)->setPosition(RADTODEG(node->rot.x));
    ((kgmGuiScroll*)g)->setChangeEventCallback(kgmGuiScroll::ChangeEventCallback(this, (kgmGuiScroll::ChangeEventCallback::Function)&kViewOptions::onRotationX));
    y_coord += 23;
    g = new kgmGuiLabel(this, 0, y_coord, 50, 20);
    g->setText("Rotation Y");
    g = new kgmGuiScroll(this, 51, y_coord, 140, 20);
    g->show();
    ((kgmGuiScroll*)g)->setOrientation(kgmGuiScroll::ORIENT_HORIZONTAL);
    ((kgmGuiScroll*)g)->setRange(360);
    ((kgmGuiScroll*)g)->setPosition(RADTODEG(node->rot.y));
    ((kgmGuiScroll*)g)->setChangeEventCallback(kgmGuiScroll::ChangeEventCallback(this, (kgmGuiScroll::ChangeEventCallback::Function)&kViewOptions::onRotationY));
    y_coord += 23;
    g = new kgmGuiLabel(this, 0, y_coord, 50, 20);
    g->setText("Rotation Z");
    g = new kgmGuiScroll(this, 51, y_coord, 140, 20);
    g->show();
    ((kgmGuiScroll*)g)->setOrientation(kgmGuiScroll::ORIENT_HORIZONTAL);
    ((kgmGuiScroll*)g)->setRange(360);
    ((kgmGuiScroll*)g)->setPosition(RADTODEG(node->rot.z));
    ((kgmGuiScroll*)g)->setChangeEventCallback(kgmGuiScroll::ChangeEventCallback(this, (kgmGuiScroll::ChangeEventCallback::Function)&kViewOptions::onRotationZ));

    y_coord += 23;
    kgmGuiCheck* lock = new kgmGuiCheck(this, 1, y_coord, 150, 20);
    lock->setText("Locked");
    lock->setCheck(node->lock);
    lock->setClickCallback(kgmGuiCheck::ClickEventCallback(this, (kgmGuiCheck::ClickEventCallback::Function)&kViewOptions::onSelectLock));
  }
}

/*void kViewOptions::onAction(kgmGui *from, u32 arg)
{
  kgmString txt;

  if(from)
  {
    txt = from->getText();
  }

  if(from == btn_close)
  {
    if(callClose.hasObject() && callClose.hasFunction())
      callClose(callClose.getObject());

    erase();
    release();
  }

  if(from->getSid() == "position_x")
  {
    node->pos.x = kgmConvert::toDouble(txt);
    node->setPosition(node->pos);
  }
  else if(from->getSid() == "position_y")
  {
    node->pos.y = kgmConvert::toDouble(txt);
    node->setPosition(node->pos);
  }
  else if(from->getSid() == "position_z")
  {
    node->pos.z = kgmConvert::toDouble(txt);
    node->setPosition(node->pos);
  }
}*/

void kViewOptions::onCloseOptions()
{
  if(callClose.hasObject() && callClose.hasFunction())
    callClose(callClose.getObject());

  erase();
  release();
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

kViewOptionsForMesh::kViewOptionsForMesh(kNode* n, int x, int y, int w, int h)
:kViewOptions(n, x, y, w, h)
{
  fd = null;
  y_coord += 23;
  kgmGui* g = new kgmGuiLabel(this, 0, y_coord, 50, 20);
  g->setText("Material");
  g = guiMtlText = new kgmGuiText(this, 51, y_coord, 70, 20);

  if(node->mat.length())
    g->setText(node->mat);

  kgmGuiButton* btn = new kgmGuiButton(this, 125, y_coord, 50, 20);
  btn->setText("select");
  btn->setClickCallback(kgmGuiButton::ClickEventCallback(this, (kgmGuiButton::ClickEventCallback::Function)&kViewOptionsForMesh::onSelectMaterial));

  y_coord += 23;
  kgmGuiCheck* chk = new kgmGuiCheck(this, 1, y_coord, 150, 20);
  chk->setText("collide");
  chk->setCheck(node->col);
  chk->setClickCallback(kgmGuiCheck::ClickEventCallback(this, (kgmGuiCheck::ClickEventCallback::Function)&kViewOptionsForMesh::onSelectCollision));
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

void kViewOptionsForMesh::onSelectFailed()
{
  fd->erase();
  fd->release();
  fd = null;
}

void kViewOptionsForMesh::onSelectCollision(bool s)
{
  node->col = s;
}

kViewOptionsForLight::kViewOptionsForLight(kNode* n, int x, int y, int w, int h)
:kViewOptions(n, x, y, w, h)
{
  y_coord += 23;
  kgmGui* g = new kgmGuiLabel(this, 0, y_coord, 50, 20);
  g->setText("Intensity");

  g = new kgmGuiText(this, 51, y_coord, 70, 20);

  g->setText(kgmConvert::toString(node->lgt->intensity));
  ((kgmGuiText*)g)->setEditable(true);
  ((kgmGuiText*)g)->setChangeEventCallback(kgmGuiText::ChangeEventCallback(this, (kgmGuiText::ChangeEventCallback::Function)&kViewOptionsForLight::setIntencity));

  y_coord += 23;
}

void kViewOptionsForLight::setIntencity(kgmString s)
{
  f32 in = kgmConvert::toDouble(s);

  node->lgt->intensity = in;
}

kViewOptionsForActor::kViewOptionsForActor(kNode* n, int x, int y, int w, int h)
:kViewOptions(n, x, y, w, h)
{
}

kViewOptionsForSensor::kViewOptionsForSensor(kNode* n, int x, int y, int w, int h)
:kViewOptions(n, x, y, w, h)
{
  y_coord += 23;

  kgmGui* g = new kgmGuiLabel(this, 0, y_coord, 50, 20);
  g->setText("Target");

  g = new kgmGuiText(this, 51, y_coord, 70, 20);

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
  if(node->obj->m_variables.length() > 0)
    y_coord += 23;

  for(int i = 0; i < node->obj->m_variables.length(); i++)
  {
    kgmVariable& var = node->obj->m_variables[i];

    kgmGui* g = new kgmGuiLabel(this, 0, y_coord, 50, 20);
    g->setText(var.getName());

    g = new kGuiText(this, 51, y_coord, 70, 20);
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
  y_coord += 23;

  kgmGui* g = new kgmGuiLabel(this, 0, y_coord, 50, 20);
  g->setText("Channels");

  g = new kgmGuiText(this, 51, y_coord, 70, 20);

  g->setText(kgmConvert::toString((s32)node->trg->getCount()));
  ((kgmGuiText*)g)->setEditable(true);
  ((kgmGuiText*)g)->setChangeEventCallback(kgmGuiText::ChangeEventCallback(this, (kgmGuiText::ChangeEventCallback::Function)&kViewOptionsForTrigger::setChanels));

  y_coord += 23;

  g = new kgmGuiLabel(this, 0, y_coord, 50, 20);
  g->setText("Target");

  g = new kgmGuiText(this, 51, y_coord, 70, 20);

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
