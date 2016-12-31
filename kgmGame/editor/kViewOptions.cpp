#include "kViewOptions.h"
#include "../../kgmBase/kgmLog.h"
#include "../../kgmBase/kgmConvert.h"
#include "../../kgmGame/kgmGameApp.h"
#include "../../kgmGame/kgmGameBase.h"
#include "../../kgmGame/kgmGameBase.h"
#include "kViewObjects.h"
#include "kEditor.h"

using namespace kgmGameEditor;

kViewOptions* kViewOptions::single = null;

kViewOptions::kViewOptions(kNode* n, int x, int y, int w, int h)
  : kgmGuiFrame("Options", x, y, w, h)
{
  tab = null;
  node = n;
  y_coord = 0;

  if(n)
  {
    kgmGui* g;

    tab = new kgmGuiTab(getClient(), 0, 0, getClient()->getRect().width(), getClient()->getRect().height());

    kgmGui* tgeneral = tab->addTab("General");

    g = new kgmGuiLabel(tgeneral, 1, y_coord, 40, 20);
    g->setText("Name:");
    g = new kgmGuiText(tgeneral, 41, y_coord, w - 42, 20);
    g->setSid("node_name");
    g->setText(n->nam);
    Slot<kViewOptions, kgmString> slotName;
    slotName.connect(this, (Slot<kViewOptions, kgmString>::FN) &kViewOptions::onNodeName, &((kgmGuiText*)g)->sigChange);
    ((kgmGuiText*)g)->setEditable(true);
    y_coord += 22;

    if(n->typ == kNode::MATERIAL)
      return;

    g = new kgmGuiLabel(tgeneral, 0, y_coord, 50, 20);
    g->setText("Pos");
    g = new kgmGuiText(tgeneral, 51, y_coord, 50, 20);
    g->setSid("position_x");
    g->setText(kgmConvert::toString(n->pos.x));
    Slot<kViewOptions, kgmString> slotPosX;
    slotPosX.connect(this, (Slot<kViewOptions, kgmString>::FN) &kViewOptions::onPositionX, &((kgmGuiText*)g)->sigChange);
    ((kgmGuiText*)g)->setEditable(true);
    ((kgmGuiText*)g)->setNumeric(true);
    g = new kgmGuiText(tgeneral, 102, y_coord, 50, 20);
    g->setSid("position_y");
    g->setText(kgmConvert::toString(n->pos.y));
    Slot<kViewOptions, kgmString> slotPosY;
    slotPosY.connect(this, (Slot<kViewOptions, kgmString>::FN) &kViewOptions::onPositionY, &((kgmGuiText*)g)->sigChange);
    ((kgmGuiText*)g)->setEditable(true);
    ((kgmGuiText*)g)->setNumeric(true);
    g = new kgmGuiText(tgeneral, 154, y_coord, 50, 20);
    g->setSid("position_z");
    g->setText(kgmConvert::toString(n->pos.z));
    Slot<kViewOptions, kgmString> slotPosZ;
    slotPosZ.connect(this, (Slot<kViewOptions, kgmString>::FN) &kViewOptions::onPositionX, &((kgmGuiText*)g)->sigChange);
    ((kgmGuiText*)g)->setEditable(true);
    ((kgmGuiText*)g)->setNumeric(true);

    y_coord += 23;
    g = new kgmGuiLabel(tgeneral, 0, y_coord, 50, 20);
    g->setText("Rot");
    g = new kgmGuiText(tgeneral, 51, y_coord, 50, 20);
    g->setSid("rotation_x");
    g->setText(kgmConvert::toString((s32)RADTODEG(n->rot.x)));
    ((kgmGuiText*)g)->setEditable(true);
    ((kgmGuiText*)g)->setNumeric(true);
    slotRotationX.connect(this, (Slot<kViewOptions, kgmString>::FN) &kViewOptions::onRotationX, &((kgmGuiText*)g)->sigChange);
    g = new kgmGuiText(tgeneral, 102, y_coord, 50, 20);
    g->setSid("rotation_y");
    g->setText(kgmConvert::toString((s32)RADTODEG(n->rot.y)));
    ((kgmGuiText*)g)->setEditable(true);
    ((kgmGuiText*)g)->setNumeric(true);
    slotRotationY.connect(this, (Slot<kViewOptions, kgmString>::FN) &kViewOptions::onRotationY, &((kgmGuiText*)g)->sigChange);
    g = new kgmGuiText(tgeneral, 154, y_coord, 50, 20);
    g->setSid("rotation_z");
    g->setText(kgmConvert::toString((s32)RADTODEG(n->rot.z)));
    ((kgmGuiText*)g)->setEditable(true);
    ((kgmGuiText*)g)->setNumeric(true);
    slotRotationZ.connect(this, (Slot<kViewOptions, kgmString>::FN) &kViewOptions::onRotationZ, &((kgmGuiText*)g)->sigChange);

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

    kgmGuiCheck* lock = new kgmGuiCheck(tgeneral, 0, y_coord, 204, 20);
    lock->setText("Locked");
    lock->setCheck(node->lock);
    //lock->setClickCallback(kgmGuiCheck::ClickEventCallback(this, (kgmGuiCheck::ClickEventCallback::Function)&kViewOptions::onSelectLock));
  }
}

void kViewOptions::onCloseOptions()
{
  sigClose();

  erase();
}

void kViewOptions::onNodeName(kgmString s)
{
  if(s.length() < 1) return;

  node->nam = s;
}

void kViewOptions::onPositionX(kgmString s)
{
  if(s.length() < 1) return;

  node->pos.x = kgmConvert::toDouble(s);
  node->setPosition(node->pos);
}

void kViewOptions::onPositionY(kgmString s)
{
  if(s.length() < 1) return;

  node->pos.y = kgmConvert::toDouble(s);
  node->setPosition(node->pos);
}

void kViewOptions::onPositionZ(kgmString s)
{
  if(s.length() < 1) return;

  node->pos.z = kgmConvert::toDouble(s);
  node->setPosition(node->pos);
}

void kViewOptions::onRotationX(kgmString s)
{
  if(s.length() < 1) return;

  node->rot.x = DEGTORAD(kgmConvert::toInteger(s));
  node->setRotation(node->rot);
}

void kViewOptions::onRotationY(kgmString s)
{
  if(s.length() < 1) return;

  node->rot.y = DEGTORAD(kgmConvert::toInteger(s));
  node->setRotation(node->rot);
}

void kViewOptions::onRotationZ(kgmString s)
{
  if(s.length() < 1) return;

  node->rot.z = DEGTORAD(kgmConvert::toInteger(s));
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

  kgmGuiButton* btn = new kgmGuiButton(tmaterial, 1, y_coord, 50, 20);
  btn->setText("Reset");
  slotReset.connect(this, (Slot<kViewOptionsForMaterial, int>::FN) &kViewOptionsForMaterial::onReset, &btn->sigClick);


  y_coord += 23;
  kgmMaterial* mtl = n->mtl;

  m_srcblend = true;

  if(!mtl)
    return;

  kgmGui* g = null;

  g = new kgmGuiLabel(tmaterial, 0, y_coord, 50, 20);
  g->setText("Color");
  g = new kgmGuiText(tmaterial, 51, y_coord, 30, 20);
  g->setSid("ColorR");
  g->setText(kgmConvert::toString((s32)(mtl->m_color.r * 255)));
  ((kgmGuiText*)g)->setEditable(true);
  ((kgmGuiText*)g)->setNumeric(true);
  slotColorR.connect(this, (Slot<kViewOptionsForMaterial, kgmString>::FN) &kViewOptionsForMaterial::onColorR, &((kgmGuiText*)g)->sigChange);
  g = new kgmGuiText(tmaterial, 83, y_coord, 30, 20);
  g->setSid("ColorG");
  g->setText(kgmConvert::toString((s32)(mtl->m_color.g * 255)));
  ((kgmGuiText*)g)->setEditable(true);
  ((kgmGuiText*)g)->setNumeric(true);
  slotColorG.connect(this, (Slot<kViewOptionsForMaterial, kgmString>::FN) &kViewOptionsForMaterial::onColorG, &((kgmGuiText*)g)->sigChange);
  g = new kgmGuiText(tmaterial, 115, y_coord, 30, 20);
  g->setSid("ColorB");
  g->setText(kgmConvert::toString((s32)(mtl->m_color.b * 255)));
  ((kgmGuiText*)g)->setEditable(true);
  ((kgmGuiText*)g)->setNumeric(true);
  slotColorB.connect(this, (Slot<kViewOptionsForMaterial, kgmString>::FN) &kViewOptionsForMaterial::onColorB, &((kgmGuiText*)g)->sigChange);

  y_coord += 23;

  g = new kgmGuiLabel(tmaterial, 0, y_coord, 50, 20);
  g->setText("Specular");
  g = new kgmGuiText(tmaterial, 51, y_coord, 30, 20);
  g->setSid("SpecularR");
  g->setText(kgmConvert::toString((s32)(mtl->m_specular.r * 255)));
  ((kgmGuiText*)g)->setEditable(true);
  ((kgmGuiText*)g)->setNumeric(true);
  slotSpecularR.connect(this, (Slot<kViewOptionsForMaterial, kgmString>::FN) &kViewOptionsForMaterial::onSpecularR, &((kgmGuiText*)g)->sigChange);
  g = new kgmGuiText(tmaterial, 83, y_coord, 30, 20);
  g->setSid("SpecularG");
  g->setText(kgmConvert::toString((s32)(mtl->m_specular.g * 255)));
  ((kgmGuiText*)g)->setEditable(true);
  ((kgmGuiText*)g)->setNumeric(true);
  slotSpecularG.connect(this, (Slot<kViewOptionsForMaterial, kgmString>::FN) &kViewOptionsForMaterial::onSpecularG, &((kgmGuiText*)g)->sigChange);
  g = new kgmGuiText(tmaterial, 115, y_coord, 30, 20);
  g->setSid("SpecularB");
  g->setText(kgmConvert::toString((s32)(mtl->m_specular.b * 255)));
  ((kgmGuiText*)g)->setEditable(true);
  ((kgmGuiText*)g)->setNumeric(true);
  slotSpecularB.connect(this, (Slot<kViewOptionsForMaterial, kgmString>::FN) &kViewOptionsForMaterial::onSpecularB, &((kgmGuiText*)g)->sigChange);

  y_coord += 23;

  g = new kgmGuiLabel(tmaterial, 0, y_coord, 50, 20);
  g->setText("TexColor");
  g = guiTextTexColor = new kgmGuiText(tmaterial, 51, y_coord, 70, 20);

  if(mtl->getTexColor())
    g->setText(mtl->getTexColor()->m_id);

  btn = new kgmGuiButton(tmaterial, 125, y_coord, 50, 20);
  btn->setText("Select");
  slotSelectColor.connect(this, (Slot<kViewOptionsForMaterial, int>::FN) &kViewOptionsForMaterial::onSelectTexColor, &btn->sigClick);

  y_coord += 23;

  g = new kgmGuiLabel(tmaterial, 0, y_coord, 50, 20);
  g->setText("TexNormal");
  g = guiTextTexNormal = new kgmGuiText(tmaterial, 51, y_coord, 70, 20);

  if(mtl->getTexNormal())
    g->setText(mtl->getTexNormal()->m_id);

  btn = new kgmGuiButton(tmaterial, 125, y_coord, 50, 20);
  btn->setText("Select");
  slotSelectNormal.connect(this, (Slot<kViewOptionsForMaterial, int>::FN) &kViewOptionsForMaterial::onSelectTexNormal, &btn->sigClick);

  y_coord += 23;

  g = new kgmGuiLabel(tmaterial, 0, y_coord, 50, 20);
  g->setText("TexSpecular");
  g = guiTextTexSpecular = new kgmGuiText(tmaterial, 51, y_coord, 70, 20);

  if(mtl->getTexSpecular())
    g->setText(mtl->getTexSpecular()->m_id);

  btn = new kgmGuiButton(tmaterial, 125, y_coord, 50, 20);
  btn->setText("Select");
  slotSelectSpecular.connect(this, (Slot<kViewOptionsForMaterial, int>::FN) &kViewOptionsForMaterial::onSelectTexSpecular, &btn->sigClick);

  y_coord += 23;

  g = new kgmGuiLabel(tmaterial, 0, y_coord, 50, 20);
  g->setText("Shininess");
  g = new kgmGuiScroll(tmaterial, 51, y_coord, 140, 20);
  g->show();
  ((kgmGuiScroll*)g)->setOrientation(kgmGuiScroll::ORIENT_HORIZONTAL);
  ((kgmGuiScroll*)g)->setRange(512);
  ((kgmGuiScroll*)g)->setPosition(mtl->shininess());
  Slot<kViewOptionsForMaterial, u32> slotShininess;
  slotShininess.connect(this, (Slot<kViewOptionsForMaterial, u32>::FN) &kViewOptionsForMaterial::onShininess, &((kgmGuiScroll*)g)->sigChange);

  y_coord += 23;

  g = new kgmGuiLabel(tmaterial, 0, y_coord, 50, 20);
  g->setText("Transparency");
  g = new kgmGuiScroll(tmaterial, 51, y_coord, 140, 20);
  g->show();
  ((kgmGuiScroll*)g)->setOrientation(kgmGuiScroll::ORIENT_HORIZONTAL);
  ((kgmGuiScroll*)g)->setRange(100);
  ((kgmGuiScroll*)g)->setPosition(100.f * mtl->transparency());
  Slot<kViewOptionsForMaterial, u32> slotTranparency;
  slotTranparency.connect(this, (Slot<kViewOptionsForMaterial, u32>::FN) &kViewOptionsForMaterial::onTransparency, &((kgmGuiScroll*)g)->sigChange);

  y_coord += 23;

  g = new kgmGuiLabel(tmaterial, 0, y_coord, 50, 20);
  g->setText("Shader");
  g = guiTextShader = new kgmGuiText(tmaterial, 51, y_coord, 70, 20);

  if(mtl->getShader())
    g->setText(mtl->getShader()->m_id);

  btn = new kgmGuiButton(tmaterial, 125, y_coord, 50, 20);
  btn->setText("select");
  slotSelectShader.connect(this, (Slot<kViewOptionsForMaterial, int>::FN) &kViewOptionsForMaterial::onSelectShader, &btn->sigClick);

  y_coord += 23;

  kgmGuiCheck* cull = new kgmGuiCheck(tmaterial, 1, y_coord, 60, 20);
  cull->setText("Cull");
  cull->setCheck(mtl->cull());
  slotSelectCull.connect(this, (Slot<kViewOptionsForMaterial, bool>::FN) &kViewOptionsForMaterial::onCull, &cull->sigClick);

  y_coord += 23;

  kgmGuiCheck* alpha = new kgmGuiCheck(tmaterial, 0, y_coord, 60, 20);
  alpha->setText("Alpha");
  alpha->setCheck(mtl->alpha());
  slotSelectAlpha.connect(this, (Slot<kViewOptionsForMaterial, bool>::FN) &kViewOptionsForMaterial::onAlpha, &alpha->sigClick);
  kgmGuiCheck* shade = new kgmGuiCheck(tmaterial, 62, y_coord, 60, 20);
  shade->setText("Shade");
  shade->setCheck(mtl->shade());
  slotSelectShade.connect(this, (Slot<kViewOptionsForMaterial, bool>::FN) &kViewOptionsForMaterial::onShade, &shade->sigClick);
  kgmGuiCheck* depth = new kgmGuiCheck(tmaterial, 124, y_coord, 60, 20);
  depth->setText("Depth");
  depth->setCheck(mtl->depth());
  slotSelectDepth.connect(this, (Slot<kViewOptionsForMaterial, bool>::FN) &kViewOptionsForMaterial::onDepth, &depth->sigClick);

  y_coord += 23;

  kgmGuiCheck* blend = new kgmGuiCheck(tmaterial, 0, y_coord, 60, 20);
  blend->setText("Blend");
  blend->setCheck(mtl->blend());
  slotSelectBlend.connect(this, (Slot<kViewOptionsForMaterial, bool>::FN) &kViewOptionsForMaterial::onBlend, &blend->sigClick);
  g = new kgmGuiSelect(tmaterial, 62, y_coord, 60, 20);
  g->setSid("BlendSource");
  ((kgmGuiSelect*)g)->add("one");
  ((kgmGuiSelect*)g)->add("zero");
  ((kgmGuiSelect*)g)->add("srcalpha");
  ((kgmGuiSelect*)g)->add("isrcalpha");
  ((kgmGuiSelect*)g)->select(kgmMaterial::blendToString(mtl->srcblend()));
  slotBlendSource.connect(this, (Slot<kViewOptionsForMaterial, kgmString>::FN) &kViewOptionsForMaterial::onBlendSource, &((kgmGuiSelect*)g)->sigSelect);
  g = new kgmGuiSelect(tmaterial, 124, y_coord, 60, 20);
  g->setSid("BlendDestination");
  ((kgmGuiSelect*)g)->add("one");
  ((kgmGuiSelect*)g)->add("zero");
  ((kgmGuiSelect*)g)->add("srcalpha");
  ((kgmGuiSelect*)g)->add("isrcalpha");
  ((kgmGuiSelect*)g)->select(kgmMaterial::blendToString(mtl->dstblend()));
  slotBlendDestination.connect(this, (Slot<kViewOptionsForMaterial, kgmString>::FN) &kViewOptionsForMaterial::onBlendDestination, &((kgmGuiSelect*)g)->sigSelect);

  y_coord += 23;
}

void kViewOptionsForMaterial::onReset(int)
{
  kgmMaterial* m = (kgmMaterial*)node->obj;

  m->setShader(null);
  m->setTexColor(null);
  m->setTexNormal(null);
  m->setTexSpecular(null);

  m->shininess(0.0);
  m->transparency(0.0);

  erase();
}

void kViewOptionsForMaterial::onColorR(kgmString c)
{
  if(c.length() < 1)
    return;

  u32 color = (u32)kgmConvert::toInteger(c);
  color = clamp<u32>(color, 0, 255);
  node->mtl->m_color.r = color / 255;
}

void kViewOptionsForMaterial::onColorG(kgmString c)
{
  if(c.length() < 1)
    return;

  u32 color = (u32)kgmConvert::toInteger(c);
  color = clamp<u32>(color, 0, 255);
  node->mtl->m_color.g = color / 255;
}

void kViewOptionsForMaterial::onColorB(kgmString c)
{
  if(c.length() < 1)
    return;

  u32 color = (u32)kgmConvert::toInteger(c);
  color = clamp<u32>(color, 0, 255);
  node->mtl->m_color.b = color / 255;
}

void kViewOptionsForMaterial::onSpecularR(kgmString c)
{
  if(c.length() < 1)
    return;

  u32 color = (u32)kgmConvert::toInteger(c);
  color = clamp<u32>(color, 0, 255);
  node->mtl->m_specular.r = color / 255;
}

void kViewOptionsForMaterial::onSpecularG(kgmString c)
{
  if(c.length() < 1)
    return;

  u32 color = (u32)kgmConvert::toInteger(c);
  color = clamp<u32>(color, 0, 255);
  node->mtl->m_specular.g = color / 255;
}

void kViewOptionsForMaterial::onSpecularB(kgmString c)
{
  if(c.length() < 1)
    return;

  u32 color = (u32)kgmConvert::toInteger(c);
  color = clamp<u32>(color, 0, 255);
  node->mtl->m_specular.b = color / 255;
}

void kViewOptionsForMaterial::onBlendSource(kgmString c)
{
  if(c.length() < 1)
    return;

  ((kgmMaterial*)node->mtl)->srcblend(kgmMaterial::stringToBlend(c));
}

void kViewOptionsForMaterial::onBlendDestination(kgmString c)
{
  if(c.length() < 1)
    return;

  ((kgmMaterial*)node->mtl)->dstblend(kgmMaterial::stringToBlend(c));
}

void kViewOptionsForMaterial::onSelectSuccess(kgmGuiFileDialog* fd)
{
  switch (mode)
  {
    case Mode_Shader:
      this->node->mtl->setShader(kgmIGame::getGame()->getResources()->getShader(fd->getFile()));
      guiTextShader->setText(fd->getFile());
      break;
    case Mode_Color:
      this->node->mtl->setTexColor(kgmIGame::getGame()->getResources()->getTexture(fd->getFile()));
      guiTextTexColor->setText(fd->getFile());
      break;
    case Mode_Normal:
      this->node->mtl->setTexNormal(kgmIGame::getGame()->getResources()->getTexture(fd->getFile()));
      guiTextTexNormal->setText(fd->getFile());
      break;
    case Mode_Specular:
      this->node->mtl->setTexSpecular(kgmIGame::getGame()->getResources()->getTexture(fd->getFile()));
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

  kgmGuiFileDialog* fd = kgmGuiFileDialog::getDialog();

  if(!fd)
    return;

  fd->m_rect.x = 300;
  fd->showHidden(false);
  fd->show();
  fd->setFilter("glsl");
  Slot<kViewOptionsForMaterial, kgmGuiFileDialog*> slotShader;
  slotShader.connect(this, (Slot<kViewOptionsForMaterial, kgmGuiFileDialog*>::FN) &kViewOptionsForMaterial::onSelectShader, &fd->sigSelect);
  fd->forOpen(((kgmGameBase*)kgmGameApp::gameApplication()->game())->getSettings()->get((char*)"Path"));
  ((kgmGameBase*)kgmGameApp::gameApplication()->game())->guiAdd(fd);
}

void kViewOptionsForMaterial::onSelectTexColor(int)
{
  mode = Mode_Color;

  kgmGuiFileDialog* fd = kgmGuiFileDialog::getDialog();

  if(!fd)
    return;

  fd->m_rect.x = 300;
  fd->showHidden(false);
  fd->show();
  fd->setFilter("tga");
  Slot<kViewOptionsForMaterial, kgmGuiFileDialog*> slotTexColor;
  slotTexColor.connect(this, (Slot<kViewOptionsForMaterial, kgmGuiFileDialog*>::FN) &kViewOptionsForMaterial::onSelectTexColor, &fd->sigSelect);
  fd->forOpen(((kgmGameBase*)kgmGameApp::gameApplication()->game())->getSettings()->get((char*)"Path"));
  ((kgmGameBase*)kgmGameApp::gameApplication()->game())->guiAdd(fd);
}

void kViewOptionsForMaterial::onSelectTexNormal(int)
{
  mode = Mode_Normal;

  kgmGuiFileDialog* fd = kgmGuiFileDialog::getDialog();

  if(!fd)
    return;

  fd->m_rect.x = 300;
  fd->showHidden(false);
  fd->show();
  fd->setFilter("tga");
  Slot<kViewOptionsForMaterial, kgmGuiFileDialog*> slotTexNormal;
  slotTexNormal.connect(this, (Slot<kViewOptionsForMaterial, kgmGuiFileDialog*>::FN) &kViewOptionsForMaterial::onSelectTexNormal, &fd->sigSelect);
  fd->forOpen(((kgmGameBase*)kgmGameApp::gameApplication()->game())->getSettings()->get((char*)"Path"));
  ((kgmGameBase*)kgmGameApp::gameApplication()->game())->guiAdd(fd);
}

void kViewOptionsForMaterial::onSelectTexSpecular(int)
{
  mode = Mode_Specular;

  kgmGuiFileDialog* fd = kgmGuiFileDialog::getDialog();

  if(!fd)
    return;

  fd->m_rect.x = 300;
  fd->showHidden(false);
  fd->show();
  fd->setFilter("tga");
  Slot<kViewOptionsForMaterial, kgmGuiFileDialog*> slotTexSpecular;
  slotTexSpecular.connect(this, (Slot<kViewOptionsForMaterial, kgmGuiFileDialog*>::FN) &kViewOptionsForMaterial::onSelectTexSpecular, &fd->sigSelect);
  fd->forOpen(((kgmGameBase*)kgmGameApp::gameApplication()->game())->getSettings()->get((char*)"Path"));
  ((kgmGameBase*)kgmGameApp::gameApplication()->game())->guiAdd(fd);
}

void kViewOptionsForMaterial::onShininess(u32 s)
{
  kgmMaterial* mtl = node->mtl;

  if(!mtl)
    return;

  mtl->shininess(1.0f + (float)s);
}

void kViewOptionsForMaterial::onTransparency(u32 s)
{
  kgmMaterial* mtl = node->mtl;

  if(!mtl)
    return;

  mtl->transparency((float)s / 100.0f);
}

void kViewOptionsForMaterial::onAlpha(bool a)
{
  kgmMaterial* mtl = node->mtl;

  if(!mtl)
    return;

  mtl->alpha(a);
}

void kViewOptionsForMaterial::onShade(bool a)
{
  kgmMaterial* mtl = node->mtl;

  if(!mtl)
    return;

  mtl->shade(a);
}

void kViewOptionsForMaterial::onDepth(bool a)
{
  kgmMaterial* mtl = node->mtl;

  if(!mtl)
    return;

  mtl->depth(a);
}

void kViewOptionsForMaterial::onBlend(bool a)
{
  kgmMaterial* mtl = node->mtl;

  if(!mtl)
    return;

  mtl->blend(a);
}

void kViewOptionsForMaterial::onCull(bool a)
{
  kgmMaterial* mtl = node->mtl;

  if(!mtl)
    return;

  mtl->cull(a);
}

kViewOptionsForVisual::kViewOptionsForVisual(kNode* n, int x, int y, int w, int h)
:kViewOptions(n, x, y, w, h)
{
  kgmGui* tvisual = tab->addTab("Visual");
  y_coord = 1;

  kgmGui* g = new kgmGuiLabel(tvisual, 0, y_coord, 50, 20);
  g->setText("Material");
  g = vis_text = new kgmGuiText(tvisual, 51, y_coord, 100, 20);

  if(n->vis->getMaterial())
    vis_text->setText(n->vis->getMaterial()->name());

  kgmGuiButton* btn = new kgmGuiButton(tvisual, 125, y_coord, 50, 20);
  btn->setText("select");
  slotSelectMaterial.connect(this, (Slot<kViewOptionsForVisual, int>::FN) &kViewOptionsForVisual::onShowMaterials, &btn->sigClick);

  y_coord += 23;

  if(n->vis->type() == kgmVisual::TypeMesh)
  {
  }
  else if (n->vis->type() == kgmVisual::TypeParticles)
  {
    kgmGui* g = new kgmGuiLabel(tvisual, 0, y_coord, 50, 20);
    g->setText("Count");
    g = new kgmGuiText(tvisual, 51, y_coord, 100, 20);
    g->setSid("Count");
    g->setText(kgmConvert::toString((s32)n->vis->getParticles()->count()));
    ((kgmGuiText*)g)->setEditable(true);
    ((kgmGuiText*)g)->setNumeric(true);

    slotParticlesCount.connect(this, (Slot<kViewOptionsForVisual, kgmString>::FN) &kViewOptionsForVisual::onParticlesCount, &((kgmGuiText*)g)->sigChange);

    y_coord += 23;

    g = new kgmGuiLabel(tvisual, 0, y_coord, 50, 20);
    g->setText("Speed");
    g = new kgmGuiText(tvisual, 51, y_coord, 50, 20);
    g->setSid("Speed");
    g->setText(kgmConvert::toString((s32)n->vis->getParticles()->speed()));
    ((kgmGuiText*)g)->setEditable(true);
    ((kgmGuiText*)g)->setNumeric(true);
    slotParticlesSpeed.connect(this, (Slot<kViewOptionsForVisual, kgmString>::FN) &kViewOptionsForVisual::onParticlesSpeed, &((kgmGuiText*)g)->sigChange);
    g = new kgmGuiText(tvisual, 102, y_coord, 50, 20);
    g->setSid("DivSpeed");
    g->setText(kgmConvert::toString((s32)(100 * n->vis->getParticles()->divspeed())));
    ((kgmGuiText*)g)->setEditable(true);
    ((kgmGuiText*)g)->setNumeric(true);
    slotParticlesDivSpeed.connect(this, (Slot<kViewOptionsForVisual, kgmString>::FN) &kViewOptionsForVisual::onParticlesDivSpeed, &((kgmGuiText*)g)->sigChange);


    y_coord += 23;

    g = new kgmGuiLabel(tvisual, 0, y_coord, 50, 20);
    g->setText("Angle");
    g = new kgmGuiText(tvisual, 51, y_coord, 100, 20);
    g->setSid("Angle");
    g->setText(kgmConvert::toString((s32)RADTODEG(n->vis->getParticles()->angle())));
    ((kgmGuiText*)g)->setEditable(true);
    ((kgmGuiText*)g)->setNumeric(true);
    slotParticlesAngle.connect(this, (Slot<kViewOptionsForVisual, kgmString>::FN) &kViewOptionsForVisual::onParticlesAngle, &((kgmGuiText*)g)->sigChange);

    y_coord += 23;

    g = new kgmGuiLabel(tvisual, 0, y_coord, 50, 20);
    g->setText("Life");
    g = new kgmGuiText(tvisual, 51, y_coord, 50, 20);
    g->setSid("Life");
    g->setText(kgmConvert::toString((s32)n->vis->getParticles()->life()));
    ((kgmGuiText*)g)->setEditable(true);
    ((kgmGuiText*)g)->setNumeric(true);
    slotParticlesLife.connect(this, (Slot<kViewOptionsForVisual, kgmString>::FN) &kViewOptionsForVisual::onParticlesLife, &((kgmGuiText*)g)->sigChange);
    g = new kgmGuiText(tvisual, 102, y_coord, 50, 20);
    g->setSid("DivLife");
    g->setText(kgmConvert::toString((s32)(100 * n->vis->getParticles()->divlife())));
    ((kgmGuiText*)g)->setEditable(true);
    ((kgmGuiText*)g)->setNumeric(true);
    slotParticlesDivLife.connect(this, (Slot<kViewOptionsForVisual, kgmString>::FN) &kViewOptionsForVisual::onParticlesDivLife, &((kgmGuiText*)g)->sigChange);

    y_coord += 23;

    g = new kgmGuiLabel(tvisual, 0, y_coord, 50, 20);
    g->setText("Size");
    g = new kgmGuiText(tvisual, 51, y_coord, 50, 20);
    g->setSid("Size");
    g->setText(kgmConvert::toString((s32)n->vis->getParticles()->size()));
    ((kgmGuiText*)g)->setEditable(true);
    ((kgmGuiText*)g)->setNumeric(true);
    slotParticlesSize.connect(this, (Slot<kViewOptionsForVisual, kgmString>::FN) &kViewOptionsForVisual::onParticlesSize, &((kgmGuiText*)g)->sigChange);
    g = new kgmGuiText(tvisual, 103, y_coord, 50, 20);
    g->setSid("Esize");
    g->setText(kgmConvert::toString((s32)n->vis->getParticles()->esize()));
    ((kgmGuiText*)g)->setEditable(true);
    ((kgmGuiText*)g)->setNumeric(true);
    slotParticlesEsize.connect(this, (Slot<kViewOptionsForVisual, kgmString>::FN) &kViewOptionsForVisual::onParticlesEsize, &((kgmGuiText*)g)->sigChange);

    y_coord += 23;

    kgmGuiCheck* loop = new kgmGuiCheck(tvisual, 0, y_coord, 74, 20);
    loop->setText("Loop");
    loop->setCheck(node->vis->getParticles()->loop());
    slotParticlesLoop.connect(this, (Slot<kViewOptionsForVisual, bool>::FN) &kViewOptionsForVisual::onParticlesLoop, &loop->sigClick);
    kgmGuiCheck* fade = new kgmGuiCheck(tvisual, 76, y_coord, 74, 20);
    fade->setText("Fade");
    fade->setCheck(node->vis->getParticles()->fade());
    slotParticlesFade.connect(this,(Slot<kViewOptionsForVisual, bool>::FN)  &kViewOptionsForVisual::onParticlesFade, &fade->sigClick);

    y_coord += 23;
  }
}

void kViewOptionsForVisual::onShowMaterials(int s)
{
  kViewObjects* vo = kViewObjects::getDialog();

  if(!vo)
    return;

  Slot<kViewOptionsForVisual, kgmString> slotMaterials;
  slotMaterials.connect(this, (Slot<kViewOptionsForVisual, kgmString>::FN) &kViewOptionsForVisual::onSelectMaterial, &vo->sigSelect);

  kEditor* editor = ((kgmGameBase*)kgmIGame::getGame())->getEditor();

  kgmList<kNode*>& nodes = editor->getNodes();

  for(int i = 0; i < nodes.length(); i++)
    if (nodes[i]->typ == kNode::MATERIAL)
      vo->addItem(nodes[i]->nam);

  kgmIGame::getGame()->guiAdd(vo);
}

void kViewOptionsForVisual::onSelectMaterial(kgmString id)
{
  kEditor* editor = ((kgmGameBase*)kgmIGame::getGame())->getEditor();

  kgmList<kNode*>& nodes = editor->getNodes();

  for(int i = 0; i < nodes.length(); i++)
  {
    if (nodes[i]->typ == kNode::MATERIAL)
    {
      if (nodes[i]->nam == id)
      {
        node->vis->set(nodes[i]->mtl);

        vis_text->setText(nodes[i]->mtl->name());

        break;
      }
    }
  }
}

void kViewOptionsForVisual::onParticlesLoop(bool s)
{
  node->vis->getParticles()->loop(s);
}

void kViewOptionsForVisual::onParticlesFade(bool s)
{
  node->vis->getParticles()->fade(s);
}

void kViewOptionsForVisual::onParticlesCount(kgmString s)
{
  node->vis->getParticles()->count(kgmConvert::toInteger(s));
  node->vis->getParticles()->build();
}

void kViewOptionsForVisual::onParticlesSpeed(kgmString s)
{
  node->vis->getParticles()->speed(kgmConvert::toDouble(s));
}

void kViewOptionsForVisual::onParticlesAngle(kgmString s)
{
  if(s.length() < 1)
    return;

  node->vis->getParticles()->angle(DEGTORAD(kgmConvert::toInteger(s)));
}

void kViewOptionsForVisual::onParticlesLife(kgmString s)
{
  node->vis->getParticles()->life(kgmConvert::toDouble(s));
}

void kViewOptionsForVisual::onParticlesSize(kgmString s)
{
  node->vis->getParticles()->size(kgmConvert::toDouble(s));
}

void kViewOptionsForVisual::onParticlesEsize(kgmString s)
{
  node->vis->getParticles()->esize(kgmConvert::toDouble(s));
}

void kViewOptionsForVisual::onParticlesDivSpeed(kgmString s)
{
  if(s.length() < 1)
    return;

  float f = (float) kgmConvert::toInteger(s) / 100.0f;
  node->vis->getParticles()->divspeed(f);
}

void kViewOptionsForVisual::onParticlesDivLife(kgmString s)
{
  if(s.length() < 1)
    return;

  float f = (float) kgmConvert::toInteger(s) / 100.0f;
  node->vis->getParticles()->divlife(f);
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
  Slot<kViewOptionsForLight, kgmString> slotIntensisty;
  slotIntensisty.connect(this, (Slot<kViewOptionsForLight, kgmString>::FN) &kViewOptionsForLight::setIntencity, &((kgmGuiText*)g)->sigChange);
  y_coord += 23;

  g = new kgmGuiLabel(tlight, 0, y_coord, 50, 20);
  g->setText("Color");
  g = new kgmGuiText(tlight, 51, y_coord, 30, 20);
  g->setSid("ColorR");
  g->setText(kgmConvert::toString((s32)(node->lgt->color.x * 255)));
  ((kgmGuiText*)g)->setEditable(true);
  ((kgmGuiText*)g)->setNumeric(true);
  slotColorR.connect(this, (Slot<kViewOptionsForLight, kgmString>::FN) &kViewOptionsForLight::onColorR, &((kgmGuiText*)g)->sigChange);
  g = new kgmGuiText(tlight, 83, y_coord, 30, 20);
  g->setSid("ColorG");
  g->setText(kgmConvert::toString((s32)(node->lgt->color.y * 255)));
  ((kgmGuiText*)g)->setEditable(true);
  ((kgmGuiText*)g)->setNumeric(true);
  slotColorG.connect(this, (Slot<kViewOptionsForLight, kgmString>::FN) &kViewOptionsForLight::onColorG, &((kgmGuiText*)g)->sigChange);
  g = new kgmGuiText(tlight, 115, y_coord, 30, 20);
  g->setSid("ColorB");
  g->setText(kgmConvert::toString((s32)(node->lgt->color.z * 255)));
  ((kgmGuiText*)g)->setEditable(true);
  ((kgmGuiText*)g)->setNumeric(true);
  slotColorB.connect(this, (Slot<kViewOptionsForLight, kgmString>::FN) &kViewOptionsForLight::onColorB, &((kgmGuiText*)g)->sigChange);

  y_coord += 23;

  kgmGuiCheck* check = new kgmGuiCheck(tlight, 1, y_coord, 150, 20);
  check->setText("Shadows");
  check->setCheck(node->lgt->shadows);
  slotShadows.connect(this, (Slot<kViewOptionsForLight, bool>::FN) &kViewOptionsForLight::setShadows, &check->sigClick);

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

void kViewOptionsForLight::onColorR(kgmString c)
{
  if(c.length() < 1)
    return;

  u32 color = (u32)kgmConvert::toInteger(c);
  color = clamp<u32>(color, 0, 255);
  node->lgt->color.x = color / 255;
}

void kViewOptionsForLight::onColorG(kgmString c)
{
  if(c.length() < 1)
    return;

  u32 color = (u32)kgmConvert::toInteger(c);
  color = clamp<u32>(color, 0, 255);
  node->lgt->color.y = color / 255;
}

void kViewOptionsForLight::onColorB(kgmString c)
{
  if(c.length() < 1)
    return;

  u32 color = (u32)kgmConvert::toInteger(c);
  color = clamp<u32>(color, 0, 255);
  node->lgt->color.z = color / 255;
}

kViewOptionsForUnit::kViewOptionsForUnit(kNode* n, int x, int y, int w, int h)
:kViewOptions(n, x, y, w, h)
{
  kgmGui* gcollision = tab->addTab("Body");
  y_coord = 1;

  kgmGuiCheck* chk = new kgmGuiCheck(gcollision, 1, y_coord, 150, 20);
  chk->setText("collide");
  chk->setCheck(node->col);
  //chk->setClickCallback(kgmGuiCheck::ClickEventCallback(this, (kgmGuiCheck::ClickEventCallback::Function)&kViewOptionsForUnit::onSelectCollision));
  y_coord += 23;

  kgmGuiCheck* chk_grp = new kgmGuiCheck(gcollision, 1, y_coord, 150, 20);
  chk_grp->setText("Shape Box");
  //chk_grp->setClickCallback(kgmGuiCheck::ClickEventCallback(this, (kgmGuiCheck::ClickEventCallback::Function)&kViewOptionsForUnit::onSelectShapeBox));

  if(node->shp == "box") chk_grp->setCheck(true);
  y_coord += 23;

  chk = new kgmGuiCheck(gcollision, 1, y_coord, 150, 20);
  chk->setText("Shape Sphere");
  //chk->setClickCallback(kgmGuiCheck::ClickEventCallback(this, (kgmGuiCheck::ClickEventCallback::Function)&kViewOptionsForUnit::onSelectShapeSphere));
  chk_grp->addGroup(chk);

  if(node->shp == "sphere") chk->setCheck(true);
  y_coord += 23;

  chk = new kgmGuiCheck(gcollision, 1, y_coord, 150, 20);
  chk->setText("Shape Mesh");
  //chk->setClickCallback(kgmGuiCheck::ClickEventCallback(this, (kgmGuiCheck::ClickEventCallback::Function)&kViewOptionsForUnit::onSelectShapeMesh));
  chk_grp->addGroup(chk);
  if(node->shp == "mesh") chk->setCheck(true);
  y_coord += 23;

  chk = new kgmGuiCheck(gcollision, 1, y_coord, 150, 20);
  chk->setText("Shape Plane");
  //chk->setClickCallback(kgmGuiCheck::ClickEventCallback(this, (kgmGuiCheck::ClickEventCallback::Function)&kViewOptionsForUnit::onSelectShapePlane));
  chk_grp->addGroup(chk);

  if(node->shp == "plane") chk->setCheck(true);
  y_coord += 23;

  chk = new kgmGuiCheck(gcollision, 1, y_coord, 150, 20);
  chk->setText("Shape Convex");
  //chk->setClickCallback(kgmGuiCheck::ClickEventCallback(this, (kgmGuiCheck::ClickEventCallback::Function)&kViewOptionsForUnit::onSelectShapeConvex));
  chk_grp->addGroup(chk);

  if(node->shp == "convex") chk->setCheck(true);

  y_coord += 23;

  kgmGui* g = new kgmGuiLabel(gcollision, 0, y_coord, 50, 20);
  g->setText("Bound");

  g = new kgmGuiText(gcollision, 51, y_coord, 50, 20);
  g->setSid("Length x");
  g->setText(kgmConvert::toString(n->bnd.max.x - n->bnd.min.x));
  //((kgmGuiText*)g)->setChangeEventCallback(kgmGuiText::ChangeEventCallback(this, (kgmGuiText::ChangeEventCallback::Function)&kViewOptionsForUnit::onBoundX));
  ((kgmGuiText*)g)->setEditable(true);
  ((kgmGuiText*)g)->setNumeric(true);

  g = new kgmGuiText(gcollision, 102, y_coord, 50, 20);
  g->setSid("Width y");
  g->setText(kgmConvert::toString(n->bnd.max.y - n->bnd.min.y));
  //((kgmGuiText*)g)->setChangeEventCallback(kgmGuiText::ChangeEventCallback(this, (kgmGuiText::ChangeEventCallback::Function)&kViewOptionsForUnit::onBoundY));
  ((kgmGuiText*)g)->setEditable(true);
  ((kgmGuiText*)g)->setNumeric(true);

  g = new kgmGuiText(gcollision, 154, y_coord, 50, 20);
  g->setSid("Height z");
  g->setText(kgmConvert::toString(n->bnd.max.z - n->bnd.min.z));
  //((kgmGuiText*)g)->setChangeEventCallback(kgmGuiText::ChangeEventCallback(this, (kgmGuiText::ChangeEventCallback::Function)&kViewOptionsForUnit::onBoundZ));
  ((kgmGuiText*)g)->setEditable(true);
  ((kgmGuiText*)g)->setNumeric(true);
  y_coord += 23;

  kgmGui* tunit = tab->addTab("Unit");
  y_coord = 1;

  kgmGuiCheck* enable = new kgmGuiCheck(tunit, 1, y_coord, 150, 20);
  enable->setText("Enabled");
  enable->setCheck(node->unt->valid());
  //enable->setClickCallback(kgmGuiCheck::ClickEventCallback(this, (kgmGuiCheck::ClickEventCallback::Function)&kViewOptionsForUnit::onSelectEnable));

  y_coord += 23;

  g = new kgmGuiLabel(tunit, 0, y_coord, 50, 20);
  g->setText("Visual");

  g = new kgmGuiText(tunit, 51, y_coord, 50, 20);
  g->setSid("Visual");
  ((kgmGuiText*)g)->setEditable(false);

  if (n->unt && n->unt->visual() && n->unt->visual()->getMesh())
    g->setText(n->unt->visual()->getMesh()->getMesh()->id());

  g = new kgmGuiButton(tunit, 102, y_coord, 50, 20);
  g->setSid("btnSelectVisual");
  g->setText("Select");
  slotListMeshes.connect(this, (Slot<kViewOptionsForUnit, int>::FN) &kViewOptionsForUnit::onListMeshes, &((kgmGuiButton*)g)->sigClick);

  y_coord += 23;

  g = new kgmGuiLabel(tunit, 0, y_coord, 50, 20);
  g->setText("Material");

  g = new kgmGuiText(tunit, 51, y_coord, 50, 20);
  g->setSid("Material");
  ((kgmGuiText*)g)->setEditable(false);

  if (n->unt && n->unt->visual() && n->unt->visual()->getMaterial())
    g->setText(n->unt->visual()->getMaterial()->name());

  g = new kgmGuiButton(tunit, 102, y_coord, 50, 20);
  g->setSid("btnSelectMaterial");
  g->setText("Select");
  slotListMaterials.connect(this, (Slot<kViewOptionsForUnit, int>::FN) &kViewOptionsForUnit::onListMaterials, &((kgmGuiButton*)g)->sigClick);

  y_coord += 23;

  g = new kgmGuiLabel(tunit, 0, y_coord, 50, 20);
  g->setText("Action");

  g = new kgmGuiText(tunit, 51, y_coord, 50, 20);
  g->setSid("Action");
  ((kgmGuiText*)g)->setEditable(false);

  if (n->unt)
    g->setText(n->unt->action());

  g = new kgmGuiButton(tunit, 102, y_coord, 50, 20);
  g->setSid("btnSelectAction");
  g->setText("Select");
  slotListActions.connect(this, (Slot<kViewOptionsForUnit, int>::FN) &kViewOptionsForUnit::onListActions, &((kgmGuiButton*)g)->sigClick);

  y_coord += 23;

  if(node->unt->m_variables.length() > 0)
    y_coord += 23;

  for(int i = 0; i < node->unt->m_variables.length(); i++)
  {
    kgmVariable& var = node->unt->m_variables[i];

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
  node->col = s;
}

void kViewOptionsForUnit::onSelectShapeBox(bool s)
{
  if(s)
    node->shp = "box";
}

void kViewOptionsForUnit::onSelectShapeMesh(bool s)
{
  if(s)
    node->shp = "mesh";
}

void kViewOptionsForUnit::onSelectShapePlane(bool s)
{
  if(s)
    node->shp = "plane";
}

void kViewOptionsForUnit::onSelectShapeSphere(bool s)
{
  if(s)
    node->shp = "sphere";
}

void kViewOptionsForUnit::onSelectShapeConvex(bool s)
{
  if(s)
    node->shp = "convex";
}

void kViewOptionsForUnit::onBoundX(kgmString s)
{
  double v = kgmConvert::toDouble(s);

  node->bnd.min.x = -0.5 * v;
  node->bnd.max.x =  0.5 * v;
}

void kViewOptionsForUnit::onBoundY(kgmString s)
{
  double v = kgmConvert::toDouble(s);

  node->bnd.min.y = -0.5 * v;
  node->bnd.max.y =  0.5 * v;
}

void kViewOptionsForUnit::onBoundZ(kgmString s)
{
  double v = kgmConvert::toDouble(s);

  node->bnd.min.z = -0.5 * v;
  node->bnd.max.z =  0.5 * v;
}

void kViewOptionsForUnit::onSelectEnable(bool state)
{
  if(state)
    node->unt->enable();
  else
    node->unt->disable();
}

void kViewOptionsForUnit::onListMeshes(int state)
{
  kgmGuiFileDialog* fd = kgmGuiFileDialog::getDialog();

  if(!fd)
    return;

  fd->m_rect.x = 300;
  fd->showHidden(false);
  fd->show();
  fd->setFilter(".msh");
  //fd->forOpen(((kgmGameBase*)kgmGameApp::gameApplication()->game())->getSettings()->get("Path"), kgmGuiFileDialog::ClickEventCallback(this, (kgmGuiFileDialog::ClickEventCallback::Function)&kViewOptionsForUnit::onSelectMesh));
  ((kgmGameBase*)kgmGameApp::gameApplication()->game())->guiAdd(fd);

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

void kViewOptionsForUnit::onListMaterials(int state)
{
  kViewObjects* vo = kViewObjects::getDialog();

  if(!vo)
    return;

  kEditor* editor = ((kgmGameBase*)kgmIGame::getGame())->getEditor();

  kgmList<kNode*>& nodes = editor->getNodes();

  for(int i = 0; i < nodes.length(); i++)
    if (nodes[i]->typ == kNode::MATERIAL)
      vo->addItem(nodes[i]->nam);

  kgmIGame::getGame()->guiAdd(vo);
}

void kViewOptionsForUnit::onSelectMesh(kgmGuiFileDialog* fd)
{
  kgmMesh* m = kgmIGame::getGame()->getResources()->getMesh(fd->getFile());

  if(m)
  {
    if(!node->unt->visual())
    {
      node->unt->visual(new kgmVisual());
      node->unt->visual()->set(m);
      ((kgmGameBase*)kgmIGame::getGame())->getRender()->add(node->unt->visual());
    }

    kgmGui* txt = this->getBySid("Visual");

    if(txt)
      txt->setText(fd->getFile());
  }
}

void kViewOptionsForUnit::onSelectAction(kgmString id)
{
  if(id.length() < 1)
    return;

  node->unt->action(id);

  kgmGui* act = this->getBySid("Action");

  if(act)
    act->setText(id);
}

void kViewOptionsForUnit::onSelectMaterial(kgmString id)
{
  kEditor* editor = ((kgmGameBase*)kgmIGame::getGame())->getEditor();

  kgmList<kNode*>& nodes = editor->getNodes();

  for(int i = 0; i < nodes.length(); i++)
  {
    if (nodes[i]->typ == kNode::MATERIAL)
    {
      if (nodes[i]->nam == id)
      {
        if(node->unt->visual())
          node->unt->visual()->set(nodes[i]->mtl);

        kgmGui* mtl = this->getBySid("Material");

        if(mtl)
          mtl->setText(id);

        break;
      }
    }
  }
}

void kViewOptionsForUnit::updateVariable(kgmString id, kgmString data)
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
:kViewOptionsForUnit(n, x, y, w, h)
{
  kgmGui* tsensor = tab->addTab("Sensor");
  y_coord = 1;

  kgmGui* g = new kgmGuiLabel(tsensor, 0, y_coord, 50, 20);
  g->setText("Target");

  g = new kgmGuiText(tsensor, 51, y_coord, 70, 20);

  g->setText(node->sns->getTarget());
  ((kgmGuiText*)g)->setEditable(true);
  //((kgmGuiText*)g)->setChangeEventCallback(kgmGuiText::ChangeEventCallback(this, (kgmGuiText::ChangeEventCallback::Function)&kViewOptionsForSensor::setTarget));

  y_coord += 23;
}

void kViewOptionsForSensor::setTarget(kgmString s)
{
  node->sns->setTarget(s);
}

kViewOptionsForActor::kViewOptionsForActor(kNode* n, int x, int y, int w, int h)
:kViewOptions(n, x, y, w, h)
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
  //btn->setClickCallback(kgmGuiButton::ClickEventCallback(this, (kgmGuiButton::ClickEventCallback::Function)&kViewOptionsForActor::showStates));

  y_coord += 23;

  kgmGuiCheck* enable = new kgmGuiCheck(tactor, 1, y_coord, 150, 20);
  enable->setText("Player");
  enable->setCheck(node->unt == kgmIGame::getGame()->getLogic()->getPlayer());
  //enable->setClickCallback(kgmGuiCheck::ClickEventCallback(this, (kgmGuiCheck::ClickEventCallback::Function)&kViewOptionsForActor::onPlayer));

  y_coord += 23;
}

void kViewOptionsForActor::showStates()
{
  kViewObjects* vo = kViewObjects::getDialog();

  if(!vo)
    return;

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
:kViewOptionsForUnit(n, x, y, w, h)
{
  //kgmGui* teffect = tab->addTab("Effect");
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
  //((kgmGuiText*)g)->setChangeEventCallback(kgmGuiText::ChangeEventCallback(this, (kgmGuiText::ChangeEventCallback::Function)&kViewOptionsForTrigger::setChanels));

  y_coord += 23;

  g = new kgmGuiLabel(ttrigger, 0, y_coord, 50, 20);
  g->setText("Target");

  g = new kgmGuiText(ttrigger, 51, y_coord, 70, 20);

  g->setText(node->trg->getTarget());
  ((kgmGuiText*)g)->setEditable(true);
  //((kgmGuiText*)g)->setChangeEventCallback(kgmGuiText::ChangeEventCallback(this, (kgmGuiText::ChangeEventCallback::Function)&kViewOptionsForTrigger::setTarget));

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
  g->setText(kgmConvert::toString(n->obs->getScale()));
  //((kgmGuiText*)g)->setChangeEventCallback(kgmGuiText::ChangeEventCallback(this, (kgmGuiText::ChangeEventCallback::Function)&kViewOptionsForObstacle::onScale));
  ((kgmGuiText*)g)->setEditable(true);
  ((kgmGuiText*)g)->setNumeric(true);
  y_coord += 23;
}

void kViewOptionsForObstacle::onSelectPolygons()
{
  if(fd)
    return;

  kgmGuiFileDialog* fd = kgmGuiFileDialog::getDialog();

  if(!fd)
    return;

  fd->m_rect.x = 300;
  fd->showHidden(false);
  fd->show();
  fd->setFilter("plg");
  //fd->forOpen(((kgmGameBase*)kgmGameApp::gameApplication()->game())->getSettings()->get("Path"), kgmGuiFileDialog::ClickEventCallback(this, (kgmGuiFileDialog::ClickEventCallback::Function)&kViewOptionsForObstacle::onSelectedPolygons));
  ((kgmGameBase*)kgmGameApp::gameApplication()->game())->guiAdd(fd);
}

void kViewOptionsForObstacle::onSelectedPolygons()
{
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
