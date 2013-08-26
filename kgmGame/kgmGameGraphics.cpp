/*
 * kgmGameRenderer.cpp
 *
 *  Created on: May 27, 2012
 *      Author: goga
 */

#include "kgmGameGraphics.h"

KGMOBJECT_IMPLEMENT(kgmGameGraphics, kgmGraphics);

kgmGameGraphics::kgmGameGraphics(kgmIGraphics *g, kgmIResources* r)
  :kgmGraphics(g, r)
{
}

kgmGameGraphics::~kgmGameGraphics()
{
}
