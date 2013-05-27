#ifndef AITAOREN_H
#define AITAOREN_H

#include "../../kgmGame/kgmIGame.h"
#include "../../kgmGame/kgmGameLogic.h"
#include "../../kgmGame/kgmGameGraphics.h"

class AITaoRen: public kgmGameLogic::AI
{
  kgmIGame* game;

  float     c_dist;
  float     z_dist;
public:
  AITaoRen(kgmIGame* g)
  {
    game = g;
    c_dist = 5.0f;
    z_dist = 2.0f;
  }
  void update(kgmActor* a, u32 mls)
  {
    a->update(mls);

    if(a->m_gameplayer)
    {
      kgmCamera& cam = ((kgmGameBase*)game)->m_render->camera();
      vec3 cpos = a->m_body->m_position - a->m_body->m_direction * c_dist;
      cpos.z = z_dist;
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
    {
      static float alpha = 0.0f;
      alpha += (0.005f * state);
      vt = a->m_body->m_rotation;
      vt.z += (0.002f * state);
      a->m_body->rotate(0, 0, vt.z);
      break;
    }
    case grot_y:
    {
      z_dist -= (state * 0.005f);
      z_dist = (z_dist > 2.0)?(2.0):(z_dist);
      z_dist = (z_dist < .1)?(.1):(z_dist);

      break;
    }
    case grot_z:
    {
      c_dist += (state * 0.1f);

      break;
    }
    case gbtn_down:
      if(state)
          a->m_body->m_velocity = -0.01f;
      else
          a->m_body->m_velocity = 0.0f;
      break;
    case gbtn_up:
        if(state)
            a->m_body->m_velocity = 0.01f;
        else
            a->m_body->m_velocity = 0.0f;
      break;
    case gbtn_left:
      break;
    case gbtn_right:
      break;
    }
  }
};

#endif // AITAOREN_H
