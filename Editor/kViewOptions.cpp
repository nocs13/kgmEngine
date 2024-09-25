#include "kViewOptions.h"
#include "../kgmBase/kgmLog.h"
#include "../kgmBase/kgmConvert.h"
#include "../kgmGame/kgmGameApp.h"
#include "../kgmGame/kgmGameBase.h"
#include "../kgmGame/kgmUnit.h"
#include "../kgmGraphics/kgmParticles.h"
#include "kViewObjects.h"
#include "kEditor.h"

kViewOptions*            kViewOptions::single            = null;
kViewOptionsForMaterial* kViewOptionsForMaterial::single = null;

kViewOptions::kViewOptions(kgmUnit* n, int x, int y, int w, int h)
  : kgmGuiFrame("Options", x, y, w, h)
{
  tab = null;
  node = n;
  y_coord = 0;

  if (!n)
    return;

  kgmGui* g;

  /*
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

  y_coord += 23;

  g = new kgmGuiLabel(tgeneral, 0, y_coord, 50, 20);
  ((kgmGuiLabel*)g)->setText("Material");
  g = new kgmGuiText(tgeneral, 51, y_coord, 100, 20);
  g->setSid("o_sel_material");

  if(n->getNode()->getNodeMaterial())
    ((kgmGuiText*)g)->setText(n->getNode()->getNodeMaterial()->id());

  kgmGuiButton* btn = new kgmGuiButton(tgeneral, 125, y_coord, 50, 20);
  btn->setText("select");
  slotSelectMaterial.connect(this, (Slot<kViewOptions, int>::FN) &kViewOptions::onShowMaterials, &btn->sigClick);

  y_coord += 23;

  kgmGuiCheck* lock = new kgmGuiCheck(tgeneral, 0, y_coord, 204, 20);
  lock->setText("Locked");
  lock->setCheck(node->lock());
  slotLock.connect(this, (Slot<kViewOptions, bool>::FN) &kViewOptions::onLock, &lock->sigClick);
  //lock->setClickCallback(kgmGuiCheck::ClickEventCallback(this, (kgmGuiCheck::ClickEventCallback::Function)&kViewOptions::onSelectLock));
  y_coord += 23;
  */

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

void kViewOptions::onLock(bool s)
{
  node->lock(s);
}

void kViewOptions::onShowMaterials(int s)
{
  kViewObjects* vo = kViewObjects::getDialog();

  if(!vo)
    return;

  slotMaterials.connect(this, (Slot<kViewOptions, kgmString>::FN) &kViewOptions::onSelectMaterial, &vo->sigSelect);

  kEditor* editor = null; //((kgmGameBase*)kgmGameApp::gameApp()->game())->getEditor();

  if (!editor)
    return;

  kgmList<kgmObject*> &ms = editor->getObjects();

  for(kgmList<kgmObject*>::iterator i = ms.begin(); i != ms.end(); i++)
  {
    if(kgmString((*i)->vClass()) == "kgmMaterial")
      vo->addItem(((kgmMaterial*)(*i))->id());
  }

  kgmGameApp::gameApp()->game()->guiAdd(vo);
}

void kViewOptions::onSelectMaterial(kgmString id)
{
  kEditor* editor = null; //((kgmGameBase*)kgmGameApp::gameApp()->game())->getEditor();

  if (!editor)
    return;

  kgmMaterial* m = editor->getMaterial(id);

  if (m)
  {
    node->getNode()->setNodeMaterial(m);
    kgmGuiText* g = (kgmGuiText*) getBySid("o_sel_material");

    if (g)
      g->setText(id);
  }
}

kViewOptionsForMaterial::kViewOptionsForMaterial(kgmMaterial* m, int x, int y, int w, int h)
  : kgmGuiFrame("Options", x, y, w, h)
{
  /*
  tab = new kgmGuiTab(getClient(), 0, 0, getClient()->getRect().width(), getClient()->getRect().height());

  kgmGui* tmaterial = tab->addTab("Material");
  y_coord = 1;

  kgmGuiButton* btn = new kgmGuiButton(tmaterial, 1, y_coord, 50, 20);
  btn->setText("Reset");
  slotReset.connect(this, (Slot<kViewOptionsForMaterial, int>::FN) &kViewOptionsForMaterial::onReset, &btn->sigClick);


  y_coord += 23;

  mtl = m;

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
  g->setText("TexColor");
  g = guiTextTexColor = new kgmGuiText(tmaterial, 51, y_coord, 70, 20);
  g->setSid("TexColor");

  if(mtl->getTexColor())
    g->setText(mtl->getTexColor()->m_id);

  btn = new kgmGuiButton(tmaterial, 125, y_coord, 50, 20);
  btn->setText("Select");
  slotSelectColor.connect(this, (Slot<kViewOptionsForMaterial, int>::FN) &kViewOptionsForMaterial::onSelectTexColor, &btn->sigClick);

  y_coord += 23;

  g = new kgmGuiLabel(tmaterial, 0, y_coord, 50, 20);
  g->setText("TexNormal");
  g = guiTextTexNormal = new kgmGuiText(tmaterial, 51, y_coord, 70, 20);
  g->setSid("TexNormal");

  if(mtl->getTexNormal())
    g->setText(mtl->getTexNormal()->m_id);

  btn = new kgmGuiButton(tmaterial, 125, y_coord, 50, 20);
  btn->setText("Select");
  slotSelectNormal.connect(this, (Slot<kViewOptionsForMaterial, int>::FN) &kViewOptionsForMaterial::onSelectTexNormal, &btn->sigClick);

  y_coord += 23;

  g = new kgmGuiLabel(tmaterial, 0, y_coord, 50, 20);
  g->setText("TexSpecular");
  g = guiTextTexSpecular = new kgmGuiText(tmaterial, 51, y_coord, 70, 20);
  g->setSid("TexSpecular");

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
  slotShininess.connect(this, (Slot<kViewOptionsForMaterial, u32>::FN) &kViewOptionsForMaterial::onShininess, &((kgmGuiScroll*)g)->sigChange);

  y_coord += 23;

  g = new kgmGuiLabel(tmaterial, 0, y_coord, 50, 20);
  g->setText("Transparency");
  g = new kgmGuiScroll(tmaterial, 51, y_coord, 140, 20);
  g->show();
  ((kgmGuiScroll*)g)->setOrientation(kgmGuiScroll::ORIENT_HORIZONTAL);
  ((kgmGuiScroll*)g)->setRange(100);
  ((kgmGuiScroll*)g)->setPosition(100.f * mtl->transparency());
  slotTranparency.connect(this, (Slot<kViewOptionsForMaterial, u32>::FN) &kViewOptionsForMaterial::onTransparency, &((kgmGuiScroll*)g)->sigChange);

  y_coord += 23;

  g = new kgmGuiLabel(tmaterial, 0, y_coord, 50, 20);
  g->setText("Shader");
  g = guiTextShader = new kgmGuiText(tmaterial, 51, y_coord, 70, 20);

  btn = new kgmGuiButton(tmaterial, 125, y_coord, 50, 20);
  btn->setText("select");
  slotSelectShader.connect(this, (Slot<kViewOptionsForMaterial, int>::FN) &kViewOptionsForMaterial::onSelectShader, &btn->sigClick);

  y_coord += 23;

  kgmGuiCheck* cull = new kgmGuiCheck(tmaterial, 1, y_coord, 60, 20);
  cull->setText("Cull");
  cull->setCheck(mtl->cull());
  slotSelectCull.connect(this, (Slot<kViewOptionsForMaterial, bool>::FN) &kViewOptionsForMaterial::onCull, &cull->sigClick);

  //y_coord += 23;

  //kgmGuiCheck* alpha = new kgmGuiCheck(tmaterial, 0, y_coord, 60, 20);
  //alpha->setText("Alpha");
  //alpha->setCheck(mtl->alpha());
  //slotSelectAlpha.connect(this, (Slot<kViewOptionsForMaterial, bool>::FN) &kViewOptionsForMaterial::onAlpha, &alpha->sigClick);
  kgmGuiCheck* shade = new kgmGuiCheck(tmaterial, 62, y_coord, 60, 20);
  shade->setText("Shade");
  //shade->setCheck(mtl->shade());
  slotSelectShade.connect(this, (Slot<kViewOptionsForMaterial, bool>::FN) &kViewOptionsForMaterial::onShade, &shade->sigClick);
  kgmGuiCheck* depth = new kgmGuiCheck(tmaterial, 124, y_coord, 60, 20);
  depth->setText("Depth");
  depth->setCheck(mtl->depth());
  slotSelectDepth.connect(this, (Slot<kViewOptionsForMaterial, bool>::FN) &kViewOptionsForMaterial::onDepth, &depth->sigClick);

  y_coord += 23;

  g = new kgmGuiLabel(tmaterial, 0, y_coord, 60, 20);
  g->setText("Blending");
  g = new kgmGuiSelect(tmaterial, 62, y_coord, 60, 20);
  g->setSid("Blending");
  */

  /* ((kgmGuiSelect*)g)->add(mtl->blendToString(kgmMaterial::Blend_None));
  ((kgmGuiSelect*)g)->add(mtl->blendToString(kgmMaterial::Blend_Add));
  ((kgmGuiSelect*)g)->add(mtl->blendToString(kgmMaterial::Blend_Mul));
  ((kgmGuiSelect*)g)->add(mtl->blendToString(kgmMaterial::Blend_Sub));
  ((kgmGuiSelect*)g)->add(mtl->blendToString(kgmMaterial::Blend_Inter));
  ((kgmGuiSelect*)g)->add(mtl->blendToString(kgmMaterial::Blend_CBurn));
  ((kgmGuiSelect*)g)->add(mtl->blendToString(kgmMaterial::Blend_LBurn));
  ((kgmGuiSelect*)g)->add(mtl->blendToString(kgmMaterial::Blend_CDodge));
  ((kgmGuiSelect*)g)->add(mtl->blendToString(kgmMaterial::Blend_LDodge));
  ((kgmGuiSelect*)g)->add(mtl->blendToString(kgmMaterial::Blend_Screen));
  ((kgmGuiSelect*)g)->add(mtl->blendToString(kgmMaterial::Blend_Darken));
  ((kgmGuiSelect*)g)->add(mtl->blendToString(kgmMaterial::Blend_Lighten)); */
  //((kgmGuiSelect*)g)->select(mtl->blendToString(mtl->blend()));
  //slotBlending.connect(this, (Slot<kViewOptionsForMaterial, kgmString>::FN) &kViewOptionsForMaterial::onBlending, &((kgmGuiSelect*)g)->sigSelect);

  //y_coord += 23;
}

void kViewOptionsForMaterial::onReset(int)
{
  mtl->setTexColor(null);
  mtl->setTexNormal(null);
  mtl->setTexSpecular(null);

  mtl->shininess(0.0);
  mtl->transparency(0.0);

  erase();
}

void kViewOptionsForMaterial::onColorR(kgmString c)
{
  if(c.length() < 1)
    return;

  u32 color = (u32)kgmConvert::toInteger(c);
  color = clamp<u32>(color, 0, 255);
  mtl->m_color.r = (float) color / 255.0f;
}

void kViewOptionsForMaterial::onColorG(kgmString c)
{
  if(c.length() < 1)
    return;

  u32 color = (u32)kgmConvert::toInteger(c);
  color = clamp<u32>(color, 0, 255);
  mtl->m_color.g = (float) color / 255.0f;
}

void kViewOptionsForMaterial::onColorB(kgmString c)
{
  if(c.length() < 1)
    return;

  u32 color = (u32)kgmConvert::toInteger(c);
  color = clamp<u32>(color, 0, 255);
  mtl->m_color.b = (float) color / 255.0f;
}

void kViewOptionsForMaterial::onBlending(kgmString c)
{
  if(c.length() < 1)
    return;

  mtl->blend(kgmMaterial::stringToBlend(c));
}

void kViewOptionsForMaterial::onSelectTexture(kgmGuiFileDialog* fd)
{
  switch (mode)
  {
    case Mode_Shader:
      //mtl->setShader(kgmIGame::getGame()->getResources()->getShader(fd->getFile()));
      guiTextShader->setText(fd->getFile());
      break;
    case Mode_Color:
      mtl->setTexColor(kgmGameApp::gameApp()->game()->getResources()->getTexture(fd->getFile()));
      guiTextTexColor->setText(fd->getFile());
      break;
    case Mode_Normal:
      mtl->setTexNormal(kgmGameApp::gameApp()->game()->getResources()->getTexture(fd->getFile()));
      guiTextTexNormal->setText(fd->getFile());
      break;
    case Mode_Specular:
      mtl->setTexSpecular(kgmGameApp::gameApp()->game()->getResources()->getTexture(fd->getFile()));
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
  slotTexColor.reset();
  slotTexColor.connect(this, (Slot<kViewOptionsForMaterial, kgmGuiFileDialog*>::FN) &kViewOptionsForMaterial::onSelectTexture, &fd->sigSelect);
  fd->forOpen(((kgmGameBase*)kgmGameApp::gameApplication()->game())->getSettings()->get((char*)"Data"));
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
  slotTexNormal.connect(this, (Slot<kViewOptionsForMaterial, kgmGuiFileDialog*>::FN) &kViewOptionsForMaterial::onSelectTexture, &fd->sigSelect);
  fd->forOpen(((kgmGameBase*)kgmGameApp::gameApplication()->game())->getSettings()->get((char*)"Data"));
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
  slotTexSpecular.connect(this, (Slot<kViewOptionsForMaterial, kgmGuiFileDialog*>::FN) &kViewOptionsForMaterial::onSelectTexture, &fd->sigSelect);
  fd->forOpen(((kgmGameBase*)kgmGameApp::gameApplication()->game())->getSettings()->get((char*)"Data"));
  ((kgmGameBase*)kgmGameApp::gameApplication()->game())->guiAdd(fd);
}

/*void kViewOptionsForMaterial::onSelectTexture(kgmGuiFileDialog* fd)
{
  kEditor* editor = ((kgmGameBase*)kgmIGame::getGame())->getEditor();

  kgmTexture* t = kgmIGame::getGame()->getResources()->getTexture(fd->getFile());

  if (!t)
    return;

  kgmGuiText* gt = null;

  switch(mode)
  {
  case Mode_Color:
    mtl->setTexColor(t);
    gt = getBySid("")
    break;
  }
}*/

void kViewOptionsForMaterial::onShininess(u32 s)
{
  if(!mtl)
    return;

  mtl->shininess(1.0f + (float)s);
}

void kViewOptionsForMaterial::onTransparency(u32 s)
{
  if(!mtl)
    return;

  mtl->transparency((float)s / 100.0f);
}

void kViewOptionsForMaterial::onAlpha(bool a)
{
  if(!mtl)
    return;

  //mtl->alpha(a);
}

void kViewOptionsForMaterial::onShade(bool a)
{
  if(!mtl)
    return;

  //mtl->shade(a);
}

void kViewOptionsForMaterial::onDepth(bool a)
{
  if(!mtl)
    return;

  mtl->depth(a);
}

void kViewOptionsForMaterial::onBlend(bool a)
{
  if(!mtl)
    return;

  //mtl->blend(a);
}

void kViewOptionsForMaterial::onCull(bool a)
{
  if(!mtl)
    return;

  //mtl->cull(a);
}

kViewOptionsForVisual::kViewOptionsForVisual(kgmUnit* n, int x, int y, int w, int h)
:kViewOptions(n, x, y, w, h)
{
  /*
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
*/
}


kViewOptionsForLight::kViewOptionsForLight(kgmUnit* n, int x, int y, int w, int h)
:kViewOptions(n, x, y, w, h)
{
  /*
  kgmLight* light = ((kgmLight*)node->getNode()->getNodeObject());
  kgmGui* tlight = tab->addTab("Light");
  y_coord = 1;
  kgmGui* g = new kgmGuiLabel(tlight, 0, y_coord, 50, 20);
  g->setText("Intensity");

  g = new kgmGuiText(tlight, 51, y_coord, 70, 20);

  g->setText(kgmConvert::toString(light->intensity()));
  ((kgmGuiText*)g)->setEditable(true);
  Slot<kViewOptionsForLight, kgmString> slotIntensisty;
  slotIntensisty.connect(this, (Slot<kViewOptionsForLight, kgmString>::FN) &kViewOptionsForLight::setIntencity, &((kgmGuiText*)g)->sigChange);
  y_coord += 23;

  g = new kgmGuiLabel(tlight, 0, y_coord, 50, 20);
  g->setText("Color");
  g = new kgmGuiText(tlight, 51, y_coord, 80, 20);
  g->setSid("Color");
  //g->setText(kgmConvert::toString((s32)(light->color()), true));
  ((kgmGuiText*)g)->setEditable(true);
  ((kgmGuiText*)g)->setHexnum(true);
  slotColor.connect(this, (Slot<kViewOptionsForLight, kgmString>::FN) &kViewOptionsForLight::onColor, &((kgmGuiText*)g)->sigChange);
  //g = new kgmGuiText(tlight, 83, y_coord, 30, 20);
  //g->setSid("ColorG");
  //g->setText(kgmConvert::toString((s32)(light->color.y * 255)));
  //((kgmGuiText*)g)->setEditable(true);
  //((kgmGuiText*)g)->setNumeric(true);
  //slotColorG.connect(this, (Slot<kViewOptionsForLight, kgmString>::FN) &kViewOptionsForLight::onColorG, &((kgmGuiText*)g)->sigChange);
  //g = new kgmGuiText(tlight, 115, y_coord, 30, 20);
  //g->setSid("ColorB");
  //g->setText(kgmConvert::toString((s32)(light->color.z * 255)));
  //((kgmGuiText*)g)->setEditable(true);
  //((kgmGuiText*)g)->setNumeric(true);
  //slotColorB.connect(this, (Slot<kViewOptionsForLight, kgmString>::FN) &kViewOptionsForLight::onColorB, &((kgmGuiText*)g)->sigChange);

  y_coord += 23;

  kgmGuiCheck* check = new kgmGuiCheck(tlight, 1, y_coord, 150, 20);
  check->setText("Shadows");
  check->setCheck(light->shadows());
  slotShadows.connect(this, (Slot<kViewOptionsForLight, bool>::FN) &kViewOptionsForLight::setShadows, &check->sigClick);

  y_coord += 23;
  */
}

void kViewOptionsForLight::setIntencity(kgmString s)
{
  f32 i = kgmConvert::toDouble(s);

  ((kgmLight*)node->getNode()->getNodeObject())->intensity(i);
}

void kViewOptionsForLight::setShadows(bool s)
{
  ((kgmLight*)node->getNode()->getNodeObject())->shadows(s);
}

void kViewOptionsForLight::onColor(kgmString c)
{
  if(c.length() < 1)
    return;

  u32 color = (u32)kgmConvert::toInteger(c, true);

  //node->light()->color(0xff000000 | color);
}

/*void kViewOptionsForLight::onColorG(kgmString c)
{
  if(c.length() < 1)
    return;

  u32 color = (u32)kgmConvert::toInteger(c);
  color = clamp<u32>(color, 0, 255);
  //node->light()->color.y = color / 255;
}

void kViewOptionsForLight::onColorB(kgmString c)
{
  if(c.length() < 1)
    return;

  u32 color = (u32)kgmConvert::toInteger(c);
  color = clamp<u32>(color, 0, 255);
  //node->light()->color.z = color / 255;
}*/

kViewOptionsForUnit::kViewOptionsForUnit(kgmUnit* n, int x, int y, int w, int h)
:kViewOptions(n, x, y, w, h)
{
  /*
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
  */
  /*
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
  */
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

  //kgmUnit::getAvailableActions(actions);

  for(int i = 0; i < actions.length(); i++)
      vo->addItem(actions[i]);

  kgmGameApp::gameApp()->game()->guiAdd(vo);
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

kViewOptionsForSensor::kViewOptionsForSensor(kgmUnit* n, int x, int y, int w, int h)
:kViewOptionsForUnit(n, x, y, w, h)
{
  /*
  kgmGui* tsensor = tab->addTab("Sensor");
  y_coord = 1;

  kgmGui* g = new kgmGuiLabel(tsensor, 0, y_coord, 50, 20);
  g->setText("Target");

  g = new kgmGuiText(tsensor, 51, y_coord, 70, 20);

  //g->setText(((kgmSensor*)node)->getTarget());
  ((kgmGuiText*)g)->setEditable(true);
  //((kgmGuiText*)g)->setChangeEventCallback(kgmGuiText::ChangeEventCallback(this, (kgmGuiText::ChangeEventCallback::Function)&kViewOptionsForSensor::setTarget));

  y_coord += 23;
  */
}

void kViewOptionsForSensor::setTarget(kgmString s)
{
  //((kgmSensor*)node)->setTarget(s);
}

kViewOptionsForActor::kViewOptionsForActor(kgmUnit* n, int x, int y, int w, int h)
:kViewOptions(n, x, y, w, h)
{
  /*
  kgmGui* tactor = tab->addTab("Actor");
  y_coord = 1;

  kgmGui* g = new kgmGuiLabel(tactor, 0, y_coord, 50, 20);
  g->setText("State");
  g = guiState = new kgmGuiText(tactor, 51, y_coord, 70, 20);

  kgmString state = "";//((kgmUnit*)node)->getState();

  if(state.length())
    g->setText(state);

  kgmGuiButton* btn = new kgmGuiButton(tactor, 125, y_coord, 50, 20);
  btn->setText("select");
  //btn->setClickCallback(kgmGuiButton::ClickEventCallback(this, (kgmGuiButton::ClickEventCallback::Function)&kViewOptionsForActor::showStates));

  y_coord += 23;

  kgmGuiCheck* enable = new kgmGuiCheck(tactor, 1, y_coord, 150, 20);
  enable->setText("Player");
  //enable->setCheck(node == kgmGameApp::gameApp()->game()->getLogic()->getPlayer());
  //enable->setClickCallback(kgmGuiCheck::ClickEventCallback(this, (kgmGuiCheck::ClickEventCallback::Function)&kViewOptionsForActor::onPlayer));

  y_coord += 23;
  */
}

void kViewOptionsForActor::showStates()
{
  kViewObjects* vo = kViewObjects::getDialog();

  if(!vo)
    return;

  //for(u32 i = 0; i < ((kgmActor*)node)->getStatesCount(); i++)
  //  vo->addItem(((kgmActor*)node)->getStateName(i));

  ((kgmGameBase*)kgmGameApp::gameApplication()->game())->guiAdd(vo);
}

void kViewOptionsForActor::onState(kgmString state)
{
  //((kgmUnit*)node)->setState(state);

  guiState->setText(state);

  vo->erase();
  vo = null;
}

void kViewOptionsForActor::onPlayer(bool state)
{
  //if(state)
  //  kgmGameApp::gameApp()->game()->getLogic()->setPlayer(((kgmActor*)node));
  //else
  //  kgmGameApp::gameApp()->game()->getLogic()->setPlayer(null);
}

kViewOptionsForEffect::kViewOptionsForEffect(kgmUnit* n, int x, int y, int w, int h)
:kViewOptionsForUnit(n, x, y, w, h)
{
  //kgmGui* teffect = tab->addTab("Effect");
  y_coord = 1;
}

kViewOptionsForTrigger::kViewOptionsForTrigger(kgmUnit* n, int x, int y, int w, int h)
:kViewOptions(n, x, y, w, h)
{
  /*
  kgmGui* ttrigger = tab->addTab("Trigger");
  y_coord = 1;

  kgmGui* g = new kgmGuiLabel(ttrigger, 0, y_coord, 50, 20);
  g->setText("Channels");

  g = new kgmGuiText(ttrigger, 51, y_coord, 70, 20);

  //g->setText(kgmConvert::toString((s32)((kgmTrigger*)node)->getCount()));
  ((kgmGuiText*)g)->setEditable(true);
  //((kgmGuiText*)g)->setChangeEventCallback(kgmGuiText::ChangeEventCallback(this, (kgmGuiText::ChangeEventCallback::Function)&kViewOptionsForTrigger::setChanels));

  y_coord += 23;

  g = new kgmGuiLabel(ttrigger, 0, y_coord, 50, 20);
  g->setText("Target");

  g = new kgmGuiText(ttrigger, 51, y_coord, 70, 20);

  //g->setText(((kgmTrigger*)node)->getTarget());
  ((kgmGuiText*)g)->setEditable(true);
  //((kgmGuiText*)g)->setChangeEventCallback(kgmGuiText::ChangeEventCallback(this, (kgmGuiText::ChangeEventCallback::Function)&kViewOptionsForTrigger::setTarget));

  y_coord += 23;
  */
}

void kViewOptionsForTrigger::setChanels(kgmString s)
{
  u32 ch = kgmConvert::toDouble(s);

  //((kgmTrigger*)node)->setCount(ch);
}

void kViewOptionsForTrigger::setTarget(kgmString s)
{
  //((kgmTrigger*)node)->setTarget(s);
}

kViewOptionsForParticles::kViewOptionsForParticles(kgmUnit* n, int x, int y, int w, int h)
:kViewOptions(n, x, y, w, h)
{
  kgmParticles* pr = (kgmParticles*) n->getNode()->getNodeObject();
  /*
  kgmGui* tparticles = tab->addTab("Particles");
  y_coord = 1;

  kgmGui* g = new kgmGuiLabel(tparticles, 0, y_coord, 50, 20);
  g->setText("Count");
  g = new kgmGuiText(tparticles, 51, y_coord, 70, 20);
  g->setText(kgmConvert::toString((s32)(pr->count())));
  slotCount.connect(this, (Slot<kViewOptionsForParticles, kgmString>::FN) &kViewOptionsForParticles::onCount, &((kgmGuiText*)g)->sigChange);
  ((kgmGuiText*)g)->setEditable(true);

  y_coord += 23;

  g = new kgmGuiLabel(tparticles, 0, y_coord, 50, 20);
  g->setText("Volume");
  g = new kgmGuiText(tparticles, 51,  y_coord, 50, 20);
  g->setText(kgmConvert::toString((f32)(pr->volume().x)));
  slotVolumeX.connect(this, (Slot<kViewOptionsForParticles, kgmString>::FN) &kViewOptionsForParticles::onVolumeX, &((kgmGuiText*)g)->sigChange);
  ((kgmGuiText*)g)->setEditable(true);
  g = new kgmGuiText(tparticles, 102, y_coord, 50, 20);
  g->setText(kgmConvert::toString((f32)(pr->volume().y)));
  slotVolumeY.connect(this, (Slot<kViewOptionsForParticles, kgmString>::FN) &kViewOptionsForParticles::onVolumeY, &((kgmGuiText*)g)->sigChange);
  ((kgmGuiText*)g)->setEditable(true);
  g = new kgmGuiText(tparticles, 154, y_coord, 50, 20);
  g->setText(kgmConvert::toString((f32)(pr->volume().z)));
  slotVolumeZ.connect(this, (Slot<kViewOptionsForParticles, kgmString>::FN) &kViewOptionsForParticles::onVolumeZ, &((kgmGuiText*)g)->sigChange);
  ((kgmGuiText*)g)->setEditable(true);

  y_coord += 23;

  g = new kgmGuiLabel(tparticles, 0, y_coord, 50, 20);
  g->setText("Speed");
  g = new kgmGuiText(tparticles, 51, y_coord, 50, 20);
  g->setText(kgmConvert::toString((s32)(pr->speed())));
  slotSpeed.connect(this, (Slot<kViewOptionsForParticles, kgmString>::FN) &kViewOptionsForParticles::onSpeed, &((kgmGuiText*)g)->sigChange);
  ((kgmGuiText*)g)->setEditable(true);
  g = new kgmGuiText(tparticles, 102,  y_coord, 50, 20);
  g->setText(kgmConvert::toString((f32)(pr->divspeed())));
  slotDivSpeed.connect(this, (Slot<kViewOptionsForParticles, kgmString>::FN) &kViewOptionsForParticles::onDivSpeed, &((kgmGuiText*)g)->sigChange);
  ((kgmGuiText*)g)->setEditable(true);

  y_coord += 23;

  g = new kgmGuiLabel(tparticles, 0, y_coord, 50, 20);
  g->setText("Size");
  g = new kgmGuiText(tparticles, 51, y_coord, 50, 20);
  g->setText(kgmConvert::toString((f32)(pr->size())));
  slotSize.connect(this, (Slot<kViewOptionsForParticles, kgmString>::FN) &kViewOptionsForParticles::onSize, &((kgmGuiText*)g)->sigChange);
  ((kgmGuiText*)g)->setEditable(true);
  g = new kgmGuiText(tparticles, 102,  y_coord, 50, 20);
  g->setText(kgmConvert::toString((f32)(pr->esize())));
  slotESize.connect(this, (Slot<kViewOptionsForParticles, kgmString>::FN) &kViewOptionsForParticles::onEsize, &((kgmGuiText*)g)->sigChange);
  ((kgmGuiText*)g)->setEditable(true);

  y_coord += 23;

  g = new kgmGuiLabel(tparticles, 0, y_coord, 50, 20);
  g->setText("Life");
  g = new kgmGuiText(tparticles, 51, y_coord, 50, 20);
  g->setText(kgmConvert::toString((f32)(pr->life())));
  slotLife.connect(this, (Slot<kViewOptionsForParticles, kgmString>::FN) &kViewOptionsForParticles::onLife, &((kgmGuiText*)g)->sigChange);
  ((kgmGuiText*)g)->setEditable(true);
  g = new kgmGuiText(tparticles, 102,  y_coord, 50, 20);
  g->setText(kgmConvert::toString((f32)(pr->divlife())));
  slotDivLife.connect(this, (Slot<kViewOptionsForParticles, kgmString>::FN) &kViewOptionsForParticles::onDivLife, &((kgmGuiText*)g)->sigChange);
  ((kgmGuiText*)g)->setEditable(true);

  y_coord += 23;

  g = new kgmGuiLabel(tparticles, 0, y_coord, 50, 20);
  g->setText("Direction");
  g = new kgmGuiText(tparticles, 51,  y_coord, 50, 20);
  g->setText(kgmConvert::toString((f32)(pr->direction().x)));
  slotDirectionX.connect(this, (Slot<kViewOptionsForParticles, kgmString>::FN) &kViewOptionsForParticles::onDirectionX, &((kgmGuiText*)g)->sigChange);
  ((kgmGuiText*)g)->setEditable(true);
  g = new kgmGuiText(tparticles, 102, y_coord, 50, 20);
  g->setText(kgmConvert::toString((f32)(pr->direction().y)));
  slotDirectionY.connect(this, (Slot<kViewOptionsForParticles, kgmString>::FN) &kViewOptionsForParticles::onDirectionY, &((kgmGuiText*)g)->sigChange);
  ((kgmGuiText*)g)->setEditable(true);
  g = new kgmGuiText(tparticles, 154, y_coord, 50, 20);
  g->setText(kgmConvert::toString((f32)(pr->direction().z)));
  slotDirectionZ.connect(this, (Slot<kViewOptionsForParticles, kgmString>::FN) &kViewOptionsForParticles::onDirectionZ, &((kgmGuiText*)g)->sigChange);
  ((kgmGuiText*)g)->setEditable(true);

  y_coord += 23;

  g = new kgmGuiLabel(tparticles, 0, y_coord, 50, 20);
  g->setText("Gravity");
  g = new kgmGuiText(tparticles, 51,  y_coord, 50, 20);
  g->setText(kgmConvert::toString((f32)(pr->gravity())));
  slotGravity.connect(this, (Slot<kViewOptionsForParticles, kgmString>::FN) &kViewOptionsForParticles::onGravity, &((kgmGuiText*)g)->sigChange);
  ((kgmGuiText*)g)->setEditable(true);

  y_coord += 23;

  g = new kgmGuiLabel(tparticles, 0, y_coord, 50, 20);
  g->setText("Mass");
  g = new kgmGuiText(tparticles, 51,  y_coord, 50, 20);
  g->setText(kgmConvert::toString((f32)(pr->mass())));
  slotMass.connect(this, (Slot<kViewOptionsForParticles, kgmString>::FN) &kViewOptionsForParticles::onMass, &((kgmGuiText*)g)->sigChange);
  ((kgmGuiText*)g)->setEditable(true);

  y_coord += 23;

  g = new kgmGuiLabel(tparticles, 0, y_coord, 50, 20);
  g->setText("Noise");
  g = new kgmGuiText(tparticles, 51,  y_coord, 50, 20);
  g->setText(kgmConvert::toString((f32)(pr->noise())));
  slotNoise.connect(this, (Slot<kViewOptionsForParticles, kgmString>::FN) &kViewOptionsForParticles::onNoise, &((kgmGuiText*)g)->sigChange);
  ((kgmGuiText*)g)->setEditable(true);

  y_coord += 23;

  g = new kgmGuiCheck(tparticles, 0, y_coord, 60, 20);
  ((kgmGuiCheck*)g)->setText("Loop");
  ((kgmGuiCheck*)g)->setCheck(pr->loop());
  slotLoop.connect(this, (Slot<kViewOptionsForParticles, bool>::FN) &kViewOptionsForParticles::onLoop, &((kgmGuiCheck*)g)->sigClick);

  g = new kgmGuiCheck(tparticles, 62, y_coord, 60, 20);
  ((kgmGuiCheck*)g)->setText("Fade");
  ((kgmGuiCheck*)g)->setCheck(pr->fade());
  slotFade.connect(this, (Slot<kViewOptionsForParticles, bool>::FN) &kViewOptionsForParticles::onFade, &((kgmGuiCheck*)g)->sigClick);

  y_coord += 23;
  */
}

void kViewOptionsForParticles::onLoop(bool s)
{
  ((kgmParticles*)node->getNode()->getNodeObject())->loop(s);
}

void kViewOptionsForParticles::onFade(bool s)
{
  ((kgmParticles*)node->getNode()->getNodeObject())->fade(s);
}

void kViewOptionsForParticles::onCount(kgmString s)
{
  ((kgmParticles*)node->getNode()->getNodeObject())->count(kgmConvert::toInteger(s));
  ((kgmParticles*)node->getNode()->getNodeObject())->build();
}

void kViewOptionsForParticles::onSpeed(kgmString s)
{
  ((kgmParticles*)node->getNode()->getNodeObject())->speed(kgmConvert::toDouble(s));
}

void kViewOptionsForParticles::onNoise(kgmString s)
{
  if(s.length() < 1)
    return;

  ((kgmParticles*)node->getNode()->getNodeObject())->noise((kgmConvert::toDouble(s)));
}

void kViewOptionsForParticles::onLife(kgmString s)
{
  ((kgmParticles*)node->getNode()->getNodeObject())->life(kgmConvert::toDouble(s));
}

void kViewOptionsForParticles::onMass(kgmString s)
{
  ((kgmParticles*)node->getNode()->getNodeObject())->mass(kgmConvert::toDouble(s));
}

void kViewOptionsForParticles::onSize(kgmString s)
{
  ((kgmParticles*)node->getNode()->getNodeObject())->size(kgmConvert::toDouble(s));
}

void kViewOptionsForParticles::onEsize(kgmString s)
{
  ((kgmParticles*)node->getNode()->getNodeObject())->esize(kgmConvert::toDouble(s));
}

void kViewOptionsForParticles::onDivSpeed(kgmString s)
{
  if(s.length() < 1)
    return;

  float f = (float) kgmConvert::toInteger(s) / 100.0f;
  ((kgmParticles*)node->getNode()->getNodeObject())->divspeed(f);
}

void kViewOptionsForParticles::onDivLife(kgmString s)
{
  if(s.length() < 1)
    return;

  float f = (float) kgmConvert::toInteger(s) / 100.0f;

  ((kgmParticles*)node->getNode()->getNodeObject())->divlife(f);
}

void kViewOptionsForParticles::onGravity(kgmString s)
{
  if(s.length() < 1)
    return;

  float f = (float) kgmConvert::toDouble(s);

  ((kgmParticles*)node->getNode()->getNodeObject())->gravity(f);
}

void kViewOptionsForParticles::onVolumeX(kgmString s)
{
  if(s.length() < 1)
    return;

  vec3 v = ((kgmParticles*)node->getNode()->getNodeObject())->volume();

  v.x = (float) kgmConvert::toDouble(s);

  ((kgmParticles*)node->getNode()->getNodeObject())->volume(v);
}

void kViewOptionsForParticles::onVolumeY(kgmString s)
{
  if(s.length() < 1)
    return;

  vec3 v = ((kgmParticles*)node->getNode()->getNodeObject())->volume();

  v.y = (float) kgmConvert::toDouble(s);

  ((kgmParticles*)node->getNode()->getNodeObject())->volume(v);
}

void kViewOptionsForParticles::onVolumeZ(kgmString s)
{
  if(s.length() < 1)
    return;

  vec3 v = ((kgmParticles*)node->getNode()->getNodeObject())->volume();

  v.z = (float) kgmConvert::toDouble(s);

  ((kgmParticles*)node->getNode()->getNodeObject())->volume(v);
}

void kViewOptionsForParticles::onDirectionX(kgmString s)
{
  if(s.length() < 1)
    return;

  vec3 v = ((kgmParticles*)node->getNode()->getNodeObject())->direction();

  v.x = (float) kgmConvert::toDouble(s);

  ((kgmParticles*)node->getNode()->getNodeObject())->direction(v);
}

void kViewOptionsForParticles::onDirectionY(kgmString s)
{
  if(s.length() < 1)
    return;

  vec3 v = ((kgmParticles*)node->getNode()->getNodeObject())->direction();

  v.y = (float) kgmConvert::toDouble(s);

  ((kgmParticles*)node->getNode()->getNodeObject())->direction(v);
}

void kViewOptionsForParticles::onDirectionZ(kgmString s)
{
  if(s.length() < 1)
    return;

  vec3 v = ((kgmParticles*)node->getNode()->getNodeObject())->direction();

  v.z = (float) kgmConvert::toDouble(s);

  ((kgmParticles*)node->getNode()->getNodeObject())->direction(v);
}

