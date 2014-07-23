#ifndef KINPUT_H
#define KINPUT_H

#include "../../kgmGame/objects/kgmObjects.h"
#include "../../kgmGame/objects/kgmSnInputListener.h"

class kInput: public kgmSnInputListener
{
  KGM_OBJECT(kInput);

  kgmIGame* game;

  u8 keys[128];

public:
  kInput(kgmIGame* g)
  {
    game = g;

    memset(keys, 0, 128);
  }

  void input(u32 btn, int state)
  {
    switch(btn)
    {
    case gbtn_a:
      if(state && keys[gbtn_x])
      {
        kgmExplode* e = new kgmExplode(game, vec3(0,0,0),vec3(1,1,1));
        e->setTexture("explode_b.tga");
        e->setSlideFrames(4, 4);
        game->gAppend(e);
        e->release();
      }
      else if(state && keys[gbtn_z])
      {
        kgmFlame* e = new kgmFlame(game);
        e->setTexture("fire_aa.tga");
        e->setSlideFrames(1, 3);
        game->gAppend(e);
        e->release();
      }
      break;
    }

    keys[btn] = (u8)state;
  }
};

#endif // KINPUT_H
