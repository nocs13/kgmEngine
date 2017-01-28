#include "kViewOptions.h"
#include "../../kgmBase/kgmLog.h"
#include "../../kgmBase/kgmConvert.h"
#include "../../kgmGame/kgmGameApp.h"
#include "../../kgmGame/kgmGameBase.h"
#include "../../kgmGame/kgmGameBase.h"
#include "../../kgmGame/kgmGameLight.h"
#include "kViewObjects.h"
#include "kEditor.h"

using namespace kgmGameEditor;

kViewOptions* kViewOptions::single = null;

kViewOptions::kViewOptions(kgmGameNode* n, int x, int y, int w, int h)
  : kgmGuiFrame("Options", x, y, w, h)
{
  tab = null;
  node = n;
  y_coord = 0;

  if (!n)
    return;

  kgmGui* g;

  tab = new kgmGuiTab(getClient(), 0, 0, getClient()->getRect().width(), getClient()->getRect().height());

  kgmGui* tgeneral = tab->addTab("General");

  g = new kgmGuiLabel(tgeneral, 1, y_coord, 40, 20);
  g->setText("Name:");
  g = new kgmGuiText(tgeneral, 41, y_coord, w - 42, 20);
  g->setSid("node_name");
  g->setText(n->getName());
  Slot<kViewOptions, kgmString> slotName;
  slotName.connect(this, (Slot<kViewOptions, kgmString>::FN) &kViewOptions::onNodeName, &((kgmGuiText*)g)->sigChange);
  ((kgmGuiText*)g)->setEditable(true);
  y_coord += 22;

  g = new kgmGuiLabel(tgeneral, 0, y_coord, 50, 20);
  g->setText("Pos");
  g = new kgmGuiText(tgeneral, 51, y_coord, 50, 20);
  g->setSid("position_x");
  g->setText(kgmConvert::toString(n->position().x));
  Slot<kViewOptions, kgmString> slotPosX;
  slotPosX.connect(this, (Slot<kViewOptions, kgmString>::FN) &kViewOptions::onPositionX, &((kgmGuiText*)g)->sigChange);
  ((kgmGuiText*)g)->setEditable(true);
  ((kgmGuiText*)g)->setNumeric(true);
  g = new kgmGuiText(tgeneral, 102, y_coord, 50, 20);
  g->setSid("position_y");
  g->setText(kgmConvert::toString(n->position().y));
  Slot<kViewOptions, kgmString> slotPosY;
  slotPosY.connect(this, (Slot<kViewOptions, kgmString>::FN) &kViewOptions::onPositionY, &((kgmGuiText*)g)->sigChange);
  ((kgmGuiText*)g)->setEditable(true);
  ((kgmGuiText*)g)->setNumeric(true);
  g = new kgmGuiText(tgeneral, 154, y_coord, 50, 20);
  g->setSid("position_z");
  g->setText(kgmConvert::toString(n->position().z));
  Slot<kViewOptions, kgmString> slotPosZ;
  slotPosZ.connect(this, (Slot<kViewOptions, kgmString>::FN) &kViewOptions::onPositionX, &((kgmGuiText*)g)->sigChange);
  ((kgmGuiText*)g)->setEditable(true);
  ((kgmGuiText*)g)->setNumeric(true);

  y_coord += 23;
  g = new kgmGuiLabel(tgeneral, 0, y_coord, 50, 20);
  g->setText("Rot");
  g = new kgmGuiText(tgeneral, 51, y_coord, 50, 20);
  g->setSid("rotation_x");
  g->setText(kgmConvert::toString((s32)RADTODEG(n->rotation().x)));
  ((kgmGuiText*)g)->setEditable(true);
  ((kgmGuiText*)g)->setNumeric(true);
  slotRotationX.connect(this, (Slot<kViewOptions, kgmString>::FN) &kViewOptions::onRotationX, &((kgmGuiText*)g)->sigChange);
  g = new kgmGuiText(tgeneral, 102, y_coord, 50, 20);
  g->setSid("rotation_y");
  g->setText(kgmConvert::toString((s32)RADTODEG(n->rotation().y)));
  ((kgmGuiText*)g)->setEditable(true);
  ((kgmGuiText*)g)->setNumeric(true);
  slotRotationY.connect(this, (Slot<kViewOptions, kgmString>::FN) &kViewOptions::onRotationY, &((kgmGuiText*)g)->sigChange);
  g = new kgmGuiText(tgeneral, 154, y_coord, 50, 20);
  g->setSid("rotation_z");
  g->setText(kgmConvert::toString((s32)RADTODEG(n->rotation().z)));
  ((kgmGuiText*)g)->setEditable(true);
  ((kgmGuiText*)g)->setNumeric(true);
  slotRotationZ.connect(this, (Slot<kViewOptions, kgmString>::FN) &kViewOptions::onRotationZ, &((kgmGuiText*)g)->sigChange);

  kgmGuiCheck* lock = new kgmGuiCheck(tgeneral, 0, y_coord, 204, 20);
  lock->setText("Locked");
  lock->setCheck(node->lock());
  //lock->setClickCallback(kgmGuiCheck::ClickEventCallback(this, (kgmGuiCheck::ClickEventCallback::Function)&kViewOptions::onSelectLock));
  y_coord += 23;

  /*g = new kgmGuiLabel(tgeneral, 0, y_coord, 50, 20);
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

    y_coord += 23;*/
}

void kViewOptions::onCloseOptions()
{
  sigClose();

  erase();
}

void kViewOptions::onNodeName(kgmString s)
{
  if(s.length() < 1) return;

  node->setName(s);
}

void kViewOptions::onPositionX(kgmString s)
{
  if(s.length() < 1) return;

  vec3 pos = node->position();
  pos.x = kgmConvert::toDouble(s);
  node->position(pos);
}

void kViewOptions::onPositionY(kgmString s)
{
  if(s.length() < 1) return;

  vec3 pos = node->position();
  pos.y = kgmConvert::toDouble(s);
  node->position(pos);
}

void kViewOptions::onPositionZ(kgmString s)
{
  if(s.length() < 1) return;

  vec3 pos = node->position();
  pos.z = kgmConvert::toDouble(s);
  node->position(pos);
}

void kViewOptions::onRotationX(kgmString s)
{
  if(s.length() < 1) return;

  vec3 rot = node->rotation();
  rot.x = DEGTORAD(kgmConvert::toInteger(s));
  node->rotation(rot);
}

void kViewOptions::onRotationY(kgmString s)
{
  if(s.length() < 1) return;

  vec3 rot = node->rotation();
  rot.y = DEGTORAD(kgmConvert::toInteger(s));
  node->rotation(rot);
}

void kViewOptions::onRotationZ(kgmString s)
{
  if(s.length() < 1) return;

  vec3 rot = node->rotation();
  rot.z = DEGTORAD(kgmConvert::toInteger(s));
  node->rotation(rot);
}

void kViewOptions::onSelectLock(bool s)
{
  node->lock(s);
}

kViewOptionsForVisual::kViewOptionsForVisual(kgmGameNode* n, int x, int y, int w, int h)
:kViewOptions(n, x, y, w, h)
{
  kgmGui* tobstacle = tab->addTab("Obstacle");
  y_coord = 3;

  kgmGui* g = new kgmGuiLabel(tobstacle, 0, y_coord, 50, 20);
  g->setText("Polygons");
  g = new kgmGuiText(tobstacle, 51, y_coord, 70, 20);

  kgmGuiButton* btn = new kgmGuiButton(tobstacle, 125, y_coord, 50, 20);
  btn->setText("select");
  //btn->setClickCallback(kgmGuiButton::ClickEventCallback(this, (kgmGuiButton::ClickEventCallback::Function)&kViewOptionsForObstacle::onSelectPolygons));
  y_coord += 23;

  btn = new kgmGuiButton(tobstacle, 0, y_coord, 50, 20);
  btn->setText("Rect");
  //btn->setClickCallback(kgmGuiButton::ClickEventCallback(this, (kgmGuiButton::ClickEventCallback::Function)&kViewOptionsForObstacle::onRect));

  btn = new kgmGuiButton(tobstacle, 51, y_coord, 50, 20);
  btn->setText("Box");
  //btn->setClickCallback(kgmGuiButton::ClickEventCallback(this, (kgmGuiButton::ClickEventCallback::Function)&kViewOptionsForObstacle::onBox));
  y_coord += 23;

  g = new kgmGuiLabel(tobstacle, 0, y_coord, 50, 20);
  g->setText("Scale");
  g = new kgmGuiText(tobstacle, 51, y_coord, 50, 20);
  g->setSid("scale");
  //g->setText(kgmConvert::toString(n->obs->getScale()));
  //((kgmGuiText*)g)->setChangeEventCallback(kgmGuiText::ChangeEventCallback(this, (kgmGuiText::ChangeEventCallback::Function)&kViewOptionsForObstacle::onScale));
  ((kgmGuiText*)g)->setEditable(true);
  ((kgmGuiText*)g)->setNumeric(true);
  y_coord += 23;
}

void kViewOptionsForVisual::onParticlesLoop(bool s)
{
  node->visual()->getParticles()->loop(s);
}

void kViewOptionsForVisual::onParticlesFade(bool s)
{
  node->visual()->getParticles()->fade(s);
}

void kViewOptionsForVisual::onParticlesCount(kgmString s)
{
  node->visual()->getParticles()->count(kgmConvert::toInteger(s));
  node->visual()->getParticles()->build();
}

void kViewOptionsForVisual::onParticlesSpeed(kgmString s)
{
  node->visual()->getParticles()->speed(kgmConvert::toDouble(s));
}

void kViewOptionsForVisual::onParticlesAngle(kgmString s)
{
  if(s.length() < 1)
    return;

  node->visual()->getParticles()->angle(DEGTORAD(kgmConvert::toInteger(s)));
}

void kViewOptionsForVisual::onParticlesLife(kgmString s)
{
  node->visual()->getParticles()->life(kgmConvert::toDouble(s));
}

void kViewOptionsForVisual::onParticlesSize(kgmString s)
{
  node->visual()->getParticles()->size(kgmConvert::toDouble(s));
}

void kViewOptionsForVisual::onParticlesEsize(kgmString s)
{
  node->visual()->getParticles()->esize(kgmConvert::toDouble(s));
}

void kViewOptionsForVisual::onParticlesDivSpeed(kgmString s)
{
  if(s.length() < 1)
    return;

  float f = (float) kgmConvert::toInteger(s) / 100.0f;
  node->visual()->getParticles()->divspeed(f);
}

void kViewOptionsForVisual::onParticlesDivLife(kgmString s)
{
  if(s.length() < 1)
    return;

  float f = (float) kgmConvert::toInteger(s) / 100.0f;
  node->visual()->getParticles()->divlife(f);
}

kViewOptionsForLight::kViewOptionsForLight(kgmGameNode* n, int x, int y, int w, int h)
:kViewOptions(n, x, y, w, h)
{
  kgmLight* light = ((kgmGameLight*)n)->light();
  kgmGui* tlight = tab->addTab("Light");
  y_coord = 1;
  kgmGui* g = new kgmGuiLabel(tlight, 0, y_coord, 50, 20);
  g->setText("Intensity");

  g = new kgmGuiText(tlight, 51, y_coord, 70, 20);

  g->setText(kgmConvert::toString(light->intensity));
  ((kgmGuiText*)g)->setEditable(true);
  Slot<kViewOptionsForLight, kgmString> slotIntensisty;
  slotIntensisty.connect(this, (Slot<kViewOptionsForLight, kgmString>::FN) &kViewOptionsForLight::setIntencity, &((kgmGuiText*)g)->sigChange);
  y_coord += 23;

  g = new kgmGuiLabel(tlight, 0, y_coord, 50, 20);
  g->setText("Color");
  g = new kgmGuiText(tlight, 51, y_coord, 30, 20);
  g->setSid("ColorR");
  g->setText(kgmConvert::toString((s32)(light->color.x * 255)));
  ((kgmGuiText*)g)->setEditable(true);
  ((kgmGuiText*)g)->setNumeric(true);
  slotColorR.connect(this, (Slot<kViewOptionsForLight, kgmString>::FN) &kViewOptionsForLight::onColorR, &((kgmGuiText*)g)->sigChange);
  g = new kgmGuiText(tlight, 83, y_coord, 30, 20);
  g->setSid("ColorG");
  g->setText(kgmConvert::toString((s32)(light->color.y * 255)));
  ((kgmGuiText*)g)->setEditable(true);
  ((kgmGuiText*)g)->setNumeric(true);
  slotColorG.connect(this, (Slot<kViewOptionsForLight, kgmString>::FN) &kViewOptionsForLight::onColorG, &((kgmGuiText*)g)->sigChange);
  g = new kgmGuiText(tlight, 115, y_coord, 30, 20);
  g->setSid("ColorB");
  g->setText(kgmConvert::toString((s32)(light->color.z * 255)));
  ((kgmGuiText*)g)->setEditable(true);
  ((kgmGuiText*)g)->setNumeric(true);
  slotColorB.connect(this, (Slot<kViewOptionsForLight, kgmString>::FN) &kViewOptionsForLight::onColorB, &((kgmGuiText*)g)->sigChange);

  y_coord += 23;

  kgmGuiCheck* check = new kgmGuiCheck(tlight, 1, y_coord, 150, 20);
  check->setText("Shadows");
  check->setCheck(light->shadows);
  slotShadows.connect(this, (Slot<kViewOptionsForLight, bool>::FN) &kViewOptionsForLight::setShadows, &check->sigClick);

  y_coord += 23;
}

void kViewOptionsForLight::setIntencity(kgmString s)
{
  f32 in = kgmConvert::toDouble(s);

  ((kgmGameLight*)node)->light()->intensity = in;
}

void kViewOptionsForLight::setShadows(bool s)
{
  ((kgmGameLight*)node)->light()->shadows = s;
}

void kViewOptionsForLight::onColorR(kgmString c)
{
  if(c.length() < 1)
    return;

  u32 color = (u32)kgmConvert::toInteger(c);
  color = clamp<u32>(color, 0, 255);
  ((kgmGameLight*)node)->light()->color.x = color / 255;
}

void kViewOptionsForLight::onColorG(kgmString c)
{
  if(c.length() < 1)
    return;

  u32 color = (u32)kgmConvert::toInteger(c);
  color = clamp<u32>(color, 0, 255);
  ((kgmGameLight*)node)->light()->color.y = color / 255;
}

void kViewOptionsForLight::onColorB(kgmString c)
{
  if(c.length() < 1)
    return;

  u32 color = (u32)kgmConvert::toInteger(c);
  color = clamp<u32>(color, 0, 255);
  ((kgmGameLight*)node)->light()->color.z = color / 255;
}

kViewOptionsForUnit::kViewOptionsForUnit(kgmGameNode* n, int x, int y, int w, int h)
:kViewOptions(n, x, y, w, h)
{
  kgmGui* gcollision = tab->addTab("Body");
  y_coord = 1;

  kgmGuiCheck* chk = new kgmGuiCheck(gcollision, 1, y_coord, 150, 20);
  chk->setText("collide");
  //chk->setClickCallback(kgmGuiCheck::ClickEventCallback(this, (kgmGuiCheck::ClickEventCallback::Function)&kViewOptionsForUnit::onSelectCollision));
  y_coord += 23;

  kgmGuiCheck* chk_grp = new kgmGuiCheck(gcollision, 1, y_coord, 150, 20);
  chk_grp->setText("Shape Box");
  //chk_grp->setClickCallback(kgmGuiCheck::ClickEventCallback(this, (kgmGuiCheck::ClickEventCallback::Function)&kViewOptionsForUnit::onSelectShapeBox));

  //if(node->shp == "box") chk_grp->setCheck(true);
  y_coord += 23;

  chk = new kgmGuiCheck(gcollision, 1, y_coord, 150, 20);
  chk->setText("Shape Sphere");
  //chk->setClickCallback(kgmGuiCheck::ClickEventCallback(this, (kgmGuiCheck::ClickEventCallback::Function)&kViewOptionsForUnit::onSelectShapeSphere));
  chk_grp->addGroup(chk);

  //if(node->shp == "sphere") chk->setCheck(true);
  y_coord += 23;

  chk = new kgmGuiCheck(gcollision, 1, y_coord, 150, 20);
  chk->setText("Shape Mesh");
  //chk->setClickCallback(kgmGuiCheck::ClickEventCallback(this, (kgmGuiCheck::ClickEventCallback::Function)&kViewOptionsForUnit::onSelectShapeMesh));
  //chk_grp->addGroup(chk);
  //if(node->shp == "mesh") chk->setCheck(true);
  y_coord += 23;

  chk = new kgmGuiCheck(gcollision, 1, y_coord, 150, 20);
  chk->setText("Shape Plane");
  //chk->setClickCallback(kgmGuiCheck::ClickEventCallback(this, (kgmGuiCheck::ClickEventCallback::Function)&kViewOptionsForUnit::onSelectShapePlane));
  chk_grp->addGroup(chk);

  //if(node->shp == "plane") chk->setCheck(true);
  y_coord += 23;

  chk = new kgmGuiCheck(gcollision, 1, y_coord, 150, 20);
  chk->setText("Shape Convex");
  //chk->setClickCallback(kgmGuiCheck::ClickEventCallback(this, (kgmGuiCheck::ClickEventCallback::Function)&kViewOptionsForUnit::onSelectShapeConvex));
  chk_grp->addGroup(chk);

  //if(node->shp == "convex") chk->setCheck(true);

  y_coord += 23;

  kgmGui* g = new kgmGuiLabel(gcollision, 0, y_coord, 50, 20);
  g->setText("Bound");

  g = new kgmGuiText(gcollision, 51, y_coord, 50, 20);
  g->setSid("Length x");
  //g->setText(kgmConvert::toString(n->bnd.max.x - n->bnd.min.x));
  //((kgmGuiText*)g)->setChangeEventCallback(kgmGuiText::ChangeEventCallback(this, (kgmGuiText::ChangeEventCallback::Function)&kViewOptionsForUnit::onBoundX));
  ((kgmGuiText*)g)->setEditable(true);
  ((kgmGuiText*)g)->setNumeric(true);

  g = new kgmGuiText(gcollision, 102, y_coord, 50, 20);
  g->setSid("Width y");
  //g->setText(kgmConvert::toString(n->bnd.max.y - n->bnd.min.y));
  //((kgmGuiText*)g)->setChangeEventCallback(kgmGuiText::ChangeEventCallback(this, (kgmGuiText::ChangeEventCallback::Function)&kViewOptionsForUnit::onBoundY));
  ((kgmGuiText*)g)->setEditable(true);
  ((kgmGuiText*)g)->setNumeric(true);

  g = new kgmGuiText(gcollision, 154, y_coord, 50, 20);
  g->setSid("Height z");
  //g->setText(kgmConvert::toString(n->bnd.max.z - n->bnd.min.z));
  //((kgmGuiText*)g)->setChangeEventCallback(kgmGuiText::ChangeEventCallback(this, (kgmGuiText::ChangeEventCallback::Function)&kViewOptionsForUnit::onBoundZ));
  ((kgmGuiText*)g)->setEditable(true);
  ((kgmGuiText*)g)->setNumeric(true);
  y_coord += 23;

  kgmGui* tunit = tab->addTab("Unit");
  y_coord = 1;

  kgmGuiCheck* enable = new kgmGuiCheck(tunit, 1, y_coord, 150, 20);
  enable->setText("Enabled");
  //enable->setCheck(node->unt->valid());
  //enable->setClickCallback(kgmGuiCheck::ClickEventCallback(this, (kgmGuiCheck::ClickEventCallback::Function)&kViewOptionsForUnit::onSelectEnable));

  y_coord += 23;

  g = new kgmGuiLabel(tunit, 0, y_coord, 50, 20);
  g->setText("Visual");

  g = new kgmGuiText(tunit, 51, y_coord, 50, 20);
  g->setSid("Visual");
  ((kgmGuiText*)g)->setEditable(false);

  //if (n->unt && n->unt->visual() && n->unt->visual()->getMesh())
  //  g->setText(n->unt->visual()->getMesh()->getMesh()->id());

  y_coord += 23;

  g = new kgmGuiLabel(tunit, 0, y_coord, 50, 20);
  g->setText("Material");

  g = new kgmGuiText(tunit, 51, y_coord, 50, 20);
  g->setSid("Material");
  ((kgmGuiText*)g)->setEditable(false);

  //if (n->unt && n->unt->visual() && n->unt->visual()->getMaterial())
  //  g->setText(n->unt->visual()->getMaterial()->name());

  y_coord += 23;

  g = new kgmGuiLabel(tunit, 0, y_coord, 50, 20);
  g->setText("Action");

  g = new kgmGuiText(tunit, 51, y_coord, 50, 20);
  g->setSid("Action");
  ((kgmGuiText*)g)->setEditable(false);

  //if (n->unt)
  //  g->setText(n->unt->action());

  g = new kgmGuiButton(tunit, 102, y_coord, 50, 20);
  g->setSid("btnSelectAction");
  g->setText("Select");
  slotListActions.connect(this, (Slot<kViewOptionsForUnit, int>::FN) &kViewOptionsForUnit::onListActions, &((kgmGuiButton*)g)->sigClick);

  y_coord += 23;

  if(((kgmUnit*)node)->m_variables.length() > 0)
    y_coord += 23;

  for(int i = 0; i < ((kgmUnit*)node)->m_variables.length(); i++)
  {
    kgmVariable& var = ((kgmUnit*)node)->m_variables[i];

    kgmGui* g = new kgmGuiLabel(tunit, 0, y_coord, 50, 20);
    g->setText(var.getName());

    g = new kGuiText(tunit, 51, y_coord, 70, 20);
    g->setSid(var.getName());

    ((kGuiText*)g)->setEditable(true);
    //((kGuiText*)g)->setChangeEventCallback(kGuiText::kChangeEventCallback(this, (kGuiText::kChangeEventCallback::Function)&kViewOptionsForUnit::updateVariable));

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

void kViewOptionsForUnit::onSelectCollision(bool s)
{
  //node-> = s;
}

void kViewOptionsForUnit::onSelectShapeBox(bool s)
{
  //if(s) node->shp = "box";
}

void kViewOptionsForUnit::onSelectShapeMesh(bool s)
{
  //if(s) node->shp = "mesh";
}

void kViewOptionsForUnit::onSelectShapePlane(bool s)
{
  //if(s) node->shp = "plane";
}

void kViewOptionsForUnit::onSelectShapeSphere(bool s)
{
  //if(s) node->shp = "sphere";
}

void kViewOptionsForUnit::onSelectShapeConvex(bool s)
{
  //if(s) node->shp = "convex";
}

void kViewOptionsForUnit::onBoundX(kgmString s)
{
  double v = kgmConvert::toDouble(s);

  //node->bnd.min.x = -0.5 * v;
  //node->bnd.max.x =  0.5 * v;
}

void kViewOptionsForUnit::onBoundY(kgmString s)
{
  double v = kgmConvert::toDouble(s);

  //node->bnd.min.y = -0.5 * v;
  //node->bnd.max.y =  0.5 * v;
}

void kViewOptionsForUnit::onBoundZ(kgmString s)
{
  double v = kgmConvert::toDouble(s);

  //node->bnd.min.z = -0.5 * v;
  //node->bnd.max.z =  0.5 * v;
}

void kViewOptionsForUnit::onSelectEnable(bool state)
{
  if(state)
    node->enable();
  else
    node->disable();
}

void kViewOptionsForUnit::onListActions(int state)
{
  kViewObjects* vo = kViewObjects::getDialog();

  if(!vo)
    return;

  kgmList<kgmString> actions;

  kgmUnit::getAvailableActions(actions);

  for(int i = 0; i < actions.length(); i++)
      vo->addItem(actions[i]);

  kgmIGame::getGame()->guiAdd(vo);
}

void kViewOptionsForUnit::onSelectAction(kgmString id)
{
}

void kViewOptionsForUnit::updateVariable(kgmString id, kgmString data)
{
  /*for(int i = 0; i < node->unt->m_variables.length(); i++)
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
  }*/
}

kViewOptionsForSensor::kViewOptionsForSensor(kgmGameNode* n, int x, int y, int w, int h)
:kViewOptionsForUnit(n, x, y, w, h)
{
  kgmGui* tsensor = tab->addTab("Sensor");
  y_coord = 1;

  kgmGui* g = new kgmGuiLabel(tsensor, 0, y_coord, 50, 20);
  g->setText("Target");

  g = new kgmGuiText(tsensor, 51, y_coord, 70, 20);

  g->setText(((kgmSensor*)node)->getTarget());
  ((kgmGuiText*)g)->setEditable(true);
  //((kgmGuiText*)g)->setChangeEventCallback(kgmGuiText::ChangeEventCallback(this, (kgmGuiText::ChangeEventCallback::Function)&kViewOptionsForSensor::setTarget));

  y_coord += 23;
}

void kViewOptionsForSensor::setTarget(kgmString s)
{
  ((kgmSensor*)node)->setTarget(s);
}

kViewOptionsForActor::kViewOptionsForActor(kgmGameNode* n, int x, int y, int w, int h)
:kViewOptions(n, x, y, w, h)
{
  kgmGui* tactor = tab->addTab("Actor");
  y_coord = 1;

  kgmGui* g = new kgmGuiLabel(tactor, 0, y_coord, 50, 20);
  g->setText("State");
  g = guiState = new kgmGuiText(tactor, 51, y_coord, 70, 20);

  kgmString state = ((kgmActor*)node)->getState();

  if(state.length())
    g->setText(state);

  kgmGuiButton* btn = new kgmGuiButton(tactor, 125, y_coord, 50, 20);
  btn->setText("select");
  //btn->setClickCallback(kgmGuiButton::ClickEventCallback(this, (kgmGuiButton::ClickEventCallback::Function)&kViewOptionsForActor::showStates));

  y_coord += 23;

  kgmGuiCheck* enable = new kgmGuiCheck(tactor, 1, y_coord, 150, 20);
  enable->setText("Player");
  enable->setCheck(node == kgmIGame::getGame()->getLogic()->getPlayer());
  //enable->setClickCallback(kgmGuiCheck::ClickEventCallback(this, (kgmGuiCheck::ClickEventCallback::Function)&kViewOptionsForActor::onPlayer));

  y_coord += 23;
}

void kViewOptionsForActor::showStates()
{
  kViewObjects* vo = kViewObjects::getDialog();

  if(!vo)
    return;

  for(u32 i = 0; i < ((kgmActor*)node)->getStatesCount(); i++)
    vo->addItem(((kgmActor*)node)->getStateName(i));

  ((kgmGameBase*)kgmGameApp::gameApplication()->game())->guiAdd(vo);
}

void kViewOptionsForActor::onState(kgmString state)
{
  ((kgmActor*)node)->setState(state);

  guiState->setText(state);

  vo->erase();
  vo = null;
}

void kViewOptionsForActor::onPlayer(bool state)
{
  if(state)
    kgmIGame::getGame()->getLogic()->setPlayer(((kgmActor*)node));
  else
    kgmIGame::getGame()->getLogic()->setPlayer(null);
}

kViewOptionsForEffect::kViewOptionsForEffect(kgmGameNode* n, int x, int y, int w, int h)
:kViewOptionsForUnit(n, x, y, w, h)
{
  //kgmGui* teffect = tab->addTab("Effect");
  y_coord = 1;
}

kViewOptionsForTrigger::kViewOptionsForTrigger(kgmGameNode* n, int x, int y, int w, int h)
:kViewOptions(n, x, y, w, h)
{
  kgmGui* ttrigger = tab->addTab("Trigger");
  y_coord = 1;

  kgmGui* g = new kgmGuiLabel(ttrigger, 0, y_coord, 50, 20);
  g->setText("Channels");

  g = new kgmGuiText(ttrigger, 51, y_coord, 70, 20);

  g->setText(kgmConvert::toString((s32)((kgmTrigger*)node)->getCount()));
  ((kgmGuiText*)g)->setEditable(true);
  //((kgmGuiText*)g)->setChangeEventCallback(kgmGuiText::ChangeEventCallback(this, (kgmGuiText::ChangeEventCallback::Function)&kViewOptionsForTrigger::setChanels));

  y_coord += 23;

  g = new kgmGuiLabel(ttrigger, 0, y_coord, 50, 20);
  g->setText("Target");

  g = new kgmGuiText(ttrigger, 51, y_coord, 70, 20);

  g->setText(((kgmTrigger*)node)->getTarget());
  ((kgmGuiText*)g)->setEditable(true);
  //((kgmGuiText*)g)->setChangeEventCallback(kgmGuiText::ChangeEventCallback(this, (kgmGuiText::ChangeEventCallback::Function)&kViewOptionsForTrigger::setTarget));

  y_coord += 23;
}

void kViewOptionsForTrigger::setChanels(kgmString s)
{
  u32 ch = kgmConvert::toDouble(s);

  ((kgmTrigger*)node)->setCount(ch);
}

void kViewOptionsForTrigger::setTarget(kgmString s)
{
  ((kgmTrigger*)node)->setTarget(s);
}
