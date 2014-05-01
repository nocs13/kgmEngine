#include "kViewOptions.h"
#include "../../kgmBase/kgmConvert.h"
#include "../../kgmGame/kgmGameApp.h"
#include "../../kgmGame/kgmGameBase.h"

kViewOptions::kViewOptions(kNode* n, int x, int y, int w, int h)
  :kgmGui(null, x, y, w, h)
{
  //tab = new kgmGuiTab(this, 0, 0, w, h);
  tab = null;
  node = n;

  btn_close = new kgmGuiButton(this, w - 11, 1, 10, 10);
  btn_close->setText("X");

  if(n)
  {
    kgmGui* g;

    g = new kgmGuiLabel(this, 0, 0, 50, 10);
    g->setText(n->nam);
    g = new kgmGuiLabel(this, 0, 12, 50, 20);
    g->setText("Position");
    g = new kgmGuiText(this, 51, 12, 50, 20);
    g->setSid("position_x");
    g->setText(kgmConvert::toString(n->pos.x));
    ((kgmGuiText*)g)->setEditable(true);
    ((kgmGuiText*)g)->setNumeric(true);
    g = new kgmGuiText(this, 102, 12, 50, 20);
    g->setSid("position_y");
    g->setText(kgmConvert::toString(n->pos.y));
    ((kgmGuiText*)g)->setEditable(true);
    ((kgmGuiText*)g)->setNumeric(true);
    g = new kgmGuiText(this, 154, 12, 50, 20);
    g->setSid("position_z");
    g->setText(kgmConvert::toString(n->pos.z));
    ((kgmGuiText*)g)->setEditable(true);
    ((kgmGuiText*)g)->setNumeric(true);

    g = new kgmGuiLabel(this, 0, 35, 50, 20);
    g->setText("Rotation");
    g = new kgmGuiText(this, 51, 35, 50, 20);
    g->setSid("rotation_x");
    g->setText(kgmConvert::toString(n->rot.x));
    ((kgmGuiText*)g)->setEditable(true);
    ((kgmGuiText*)g)->setNumeric(true);
    g = new kgmGuiText(this, 102, 35, 50, 20);
    g->setSid("rotation_y");
    g->setText(kgmConvert::toString(n->rot.y));
    ((kgmGuiText*)g)->setEditable(true);
    ((kgmGuiText*)g)->setNumeric(true);
    g = new kgmGuiText(this, 154, 35, 50, 20);
    g->setSid("rotation_z");
    g->setText(kgmConvert::toString(n->rot.z));
    ((kgmGuiText*)g)->setEditable(true);
    ((kgmGuiText*)g)->setNumeric(true);
  }
}

void kViewOptions::onAction(kgmGui *from, u32 arg)
{
  kgmString txt;

  if(from)
  {
    txt = from->getText();
  }

  if(from == btn_close)
  {
    kgmGameApp* gapp = kgmGameApp::gameApplication();
    kgmIGame* igame = gapp->game();

    ((kgmGameBase*)igame)->removeListener(this);
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
}
