#ifndef KINPUT_H
#define KINPUT_H

#include "../../kgmGame/actors/kgmActors.h"
#include "../../kgmGame/objects/kgmObjects.h"

class kInput: public kgmInputListen
{
  KGM_OBJECT(kInput);

  kgmIGame* game;

public:
  kInput(kgmIGame* g)
  {
    game = g;
  }

  void input(u32 btn, int state)
  {
    switch(btn)
    {
    case gbtn_a:
      if(state)
      {
        kgmExplode* e = new kgmExplode(game, vec3(0,0,0),vec3(1,1,1));
        e->setTexture("explode_b.tga");
        e->setSlideFrames(4, 4);
        game->gAppend(e);
        e->release();
      }
      break;
    }
  }
};

#endif // KINPUT_H
