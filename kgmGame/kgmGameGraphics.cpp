#include "kgmGameGraphics.h"
#include "kgmGameApp.h"
#include "kgmIGame.h"
#include "kgmUnit.h"

kgmGameGraphics::kgmGameGraphics(kgmIGC *g, kgmIResources* r)
  :kgmGraphics(g, r)
{
  m_game = ((kgmGameApp*)kgmGameApp::application())->game();
}

kgmGameGraphics::~kgmGameGraphics()
{
}

void kgmGameGraphics::render_3d()
{
}

void kgmGameGraphics::render_2d()
{
}

void kgmGameGraphics::setBgColor(kgmColor c)
{
  m_bg_color = c.color;
}

