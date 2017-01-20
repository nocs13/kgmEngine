#include "kgmGameVisual.h"

kgmGameVisual::kgmGameVisual(kgmIGame* g)
  :kgmGameNode(g)
{
  m_type = VISUAL;

  if(!m_visual)
    m_visual = new kgmVisual();
}
