#ifndef AITAOREN_H
#define AITAOREN_H

#include "../../kgmGame/kgmIGame.h"
#include "../../kgmGame/kgmGameLogic.h"
#include "../../kgmGame/kgmGameGraphics.h"

class AITaoRen: public kgmGameLogic::AI
{
  kgmIGame* game;
public:
  AITaoRen(kgmIGame* g)
  {
    game = g;
  }
  void update(kgmActor* a, u32 mls)
  {
    a->update(mls);

    if(a->m_gameplayer)
    {
      kgmCamera& cam = ((kgmGameBase*)game)->m_render->camera();
      vec3 cpos = a->m_body->m_position - a->m_body->m_direction * 3.5f;
      cpos.z += 1.0f;
      cam.mPos = cpos;
      cam.mDir = a->m_body->m_direction;
      cam.update();
    }
  }

  void input(kgmActor* a, u32 btn, int state)
  {
    vec3 vt;

    if(!a && !a->m_gameplayer)
      return;

    switch(btn)
    {
    case grot_x:
      vt = a->m_body->m_rotation;
      vt.z += (0.2f * state);
      a->m_body->rotate(0, 0, vt.z);
      break;
    case gbtn_down:
      vt = a->m_body->m_direction;
      break;
    case gbtn_up:
        if(state)
            a->m_body->m_velocity = 0.01f;
        else
            a->m_body->m_velocity = 0.0f;
      break;
    }
  }
};

#endif // AITAOREN_H
