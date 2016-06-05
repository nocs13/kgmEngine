#ifndef SPRITERENDERER_H
#define SPRITERENDERER_H

#include "../../kgmBase/kgmIGC.h"
#include "../../kgmBase/kgmArray.h"

class kgmGraphics;
class kgmVisual;
class kgmSprite;

class SpriteRender
{
  kgmGraphics* gr;

  mtx4 mi;

public:
  SpriteRender(kgmGraphics* gr);

  void render();
  void render(kgmVisual*);
  void render(kgmSprite*);
};

#endif // SPRITERENDERER_H
