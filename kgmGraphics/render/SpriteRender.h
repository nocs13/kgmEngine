#ifndef SPRITERENDERER_H
#define SPRITERENDERER_H

#include "../../kgmBase/kgmIGC.h"
#include "BaseRender.h"

class kgmGraphics;
class kgmSprite;

class SpriteRender: public BaseRender
{
  mtx4 mi;

public:
  SpriteRender(kgmGraphics* gr);

  void render();
  void render(kgmSprite*);
};

#endif // SPRITERENDERER_H
