#include "ShadowRender.h"
#include "../../kgmGraphics/kgmGraphics.h"

ShadowRender::ShadowRender(kgmGraphics* g)
  :BaseRender(g)
{

}

void ShadowRender::render()
{
  kgmCamera cam;

  for (u32 i = 0; i < gr->m_shadows.length(); i++)
  {
    kgmGraphics::Shadow* s = &gr->m_shadows[i];


  }
}
