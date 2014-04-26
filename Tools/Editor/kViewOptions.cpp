#include "kViewOptions.h"
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

    g = new kgmGui(this, 0, 0, 50, 20);
    g->setText("Position");
    g = new kgmGui(this, 0, 25, 50, 20);
    g->setText("Rotation");
  }
}

void kViewOptions::onAction(kgmGui *from, u32 arg)
{
  if(from == btn_close)
  {
    kgmGameApp* gapp = kgmGameApp::gameApplication();
    kgmIGame* igame = gapp->game();

    ((kgmGameBase*)igame)->removeListener(this);
    erase();
    release();
  }
}
