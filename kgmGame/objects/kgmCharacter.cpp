#include "kgmCharacter.h"
#include "kgmIGame.h"
#include "../kgmGameBase.h"
#include "../kgmGameAudio.h"
#include "../kgmGraphics/kgmIGraphics.h"

#include "../../kgmBase/kgmLog.h"

kgmCharacter::kgmCharacter(kgmIGame *g)
  :kgmUnit(g)
{
  roll      = 0.0;
  yaaw      = 0.0;
  pich      = 0.5 * PI;

  speed_idl = 0.0;
  speed_wlk = 0.5;
  speed_run = 0.8;
}

void kgmCharacter::update(u32 ms)
{
  kgmUnit::update(ms);

  /*if(m_visual)
  {
    vec3 vz(0, 0, 1), vy(0, 0, 0), vx(1, 0, 0);
    mtx4 mz, mx, my, mr;
    pich = 0.0;

    mz.rotate(pich, vz);
    mx.rotate(roll, vx);
    my.rotate(yaaw, vy);

    mr = my * mx;
    m_visual->set(mr * m_visual->getTransform());

    // Update dummies.
    for(int i = 0; i < m_dummies.length(); i++)
    {
      kgmDummy*       dm = m_dummies[i];
      kgmUnit*  go = (kgmUnit*)dm->linked();

      if(go && go->visual())
      {
        mtx4 m;
        vec3 v = dm->m_shift;

        m.identity();
        m.translate(v);

        m_visual->set(m * m_visual->getTransform());
        go->update(ms);
      }
    }
  }*/

  vec3 pos = getNode()->getNodeTransform().position();

  if(pos.x < -3000) pos.x =  3000;
  if(pos.x >  3000) pos.x = -3000;
  if(pos.y < -3000) pos.y =  3000;
  if(pos.y >  3000) pos.y = -3000;

  //bodyPosition(pos);
}

void kgmCharacter::action(kgmString &a)
{
  kgm_log() << "kgmCharacter action: " << (char*)a << "\n";

  if(a == "idle")
  {
    //m_body->m_velocity = speed_idl;
  }
  else if(a == "walk")
  {
    //m_body->m_velocity = speed_wlk;
  }
  else if(a == "run")
  {
    //m_body->m_velocity = speed_run;
  }
  else if(a == "jump")
  {

  }
  else if(a == "fall")
  {

  }
  else if(a == "die")
  {

  }
}
