#ifndef AITAOREN_H
#define AITAOREN_H

#include "../../kgmGame/kgmIGame.h"
#include "../../kgmGame/kgmGameLogic.h"

class AITaoRen: public kgmGameLogic::AI
{
public:
    void update(kgmActor* a, u32 mls)
    {
        a->update(mls);
    }

    void input(kgmActor* a, u32 btn, int state)
    {
      vec3 vt;

      if(!a)
        return;

      switch(btn)
      {
      case grot_z:
        vt = a->m_body->m_rotation;
        vt.z += (0.2f * state);
        a->m_body->setRotation(vt);
        break;
      case gbtn_down:
        vt = a->m_body->m_direction;
        a->setForce(10.f, vt);
        break;
      case gbtn_up:
        //a->m_body->m_position.x += 0.5f;
        break;
      }
    }
};

#endif // AITAOREN_H
