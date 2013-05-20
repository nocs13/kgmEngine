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
      //kgmCamera& cam = ((kgmGameBase*)game)->m_render->camera();
      //vec3 cpos = a->m_body->m_position - a->m_body->m_direction * 15.f;
      //cpos.z += 2.0f;
      //cam.mPos = cpos;
      //cam.mDir = a->m_body->m_direction;
      //cam.update();
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
      alpha += (0.05f * state);
//      vt = a->m_body->m_rotation;
//      vt.z += (0.2f * state);
//      a->m_body->rotate(0, 0, vt.z);
      vec3 vr(cos(alpha), sin(alpha), 0);
      kgmCamera& cam = ((kgmGameBase*)game)->m_render->camera();
      vec3 cpos = a->m_body->m_position + vr * 15.f;
      cpos.z += 1.0f;
      vec3 cdir = a->m_body->m_position - cpos;
      cdir.normalize();
      cam.mPos = cpos;
      cam.mDir = cdir;
      cam.update();
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
