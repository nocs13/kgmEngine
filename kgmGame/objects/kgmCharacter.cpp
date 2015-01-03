#include "kgmCharacter.h"
#include "kgmIGame.h"
#include "../kgmGameBase.h"
#include "../kgmGameAudio.h"
#include "../kgmGraphics/kgmIGraphics.h"

KGMOBJECT_IMPLEMENT(kgmCharacter, kgmActor);

kgmCharacter::kgmCharacter(kgmIGame *g)
  :kgmActor(g)
{
  roll      = 0.0;
  yaaw      = 0.0;
  pich      = 0.5 * PI;

  speed_max = 0.05;
  speed_min = 0.01;

  c_dist    = 5.0f;
  z_dist    = 2.0f;

}

void kgmCharacter::update(u32 ms)
{
  kgmActor::update(ms);

  if(m_visual)
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
      kgmGameObject*  go = (kgmGameObject*)dm->linked();

      if(go && kgmObject::isValid(go) && go->getVisual())
      {
        mtx4 m;
        vec3 v = dm->m_shift;

        m.identity();
        m.translate(v);

        m_visual->set(m * m_visual->getTransform());
        go->update(ms);
      }
    }
  }

  if(getBody())
  {
    vec3 pos = getBody()->position();

    if(pos.x < -3000) pos.x =  3000;
    if(pos.x >  3000) pos.x = -3000;
    if(pos.y < -3000) pos.y =  3000;
    if(pos.y >  3000) pos.y = -3000;

    getBody()->translate(pos);
  }

  if(m_state)
  {
    bool ainput = false;

    if(m_state->id == "idle")
    {
      for(int i = 0; i < m_ainputs.size(); i++)
      {
        Input inp = m_ainputs[i];

        if(gbtns[inp.input])
        {
          ainput = true;
          setState(inp.state);
        }
      }

      if(!ainput && (roll != 0.0 || yaaw != 0.0))
      {
        setState("correct");

        return;
      }

      if(!ainput && (m_body->m_velocity > speed_min))
      {
        setState("slow");
        m_body->m_position.z = 0.0f;
      }
    }
    else if(m_state->id == "left")
    {
      vec3 vt = m_body->m_rotation;
      vt.z += (0.02f);
      m_body->rotate(vt.x, vt.y, vt.z);

      if(roll > -PI/4)
        roll -= 0.02f;
    }
    else if(m_state->id == "right")
    {
      vec3 vt = m_body->m_rotation;
      vt.z -= (0.02f);
      m_body->rotate(vt.x, vt.y, vt.z);

      if(roll < PI/4)
        roll += 0.02f;
    }
    else if(m_state->id == "up")
    {
      if(yaaw < PI/6)
      {
        vec3 vt = m_body->m_rotation;
        vt.x    = yaaw;
        m_body->rotate(vt.x, vt.y, vt.z);

        yaaw += 0.02f;
      }
    }
    else if(m_state->id == "down")
    {
      if(yaaw > -PI/6)
      {
        vec3 vt = m_body->m_rotation;
        vt.x    = yaaw;
        m_body->rotate(vt.x, vt.y, vt.z);

        yaaw -= 0.02f;
      }
    }
    else if(m_state->id == "fast")
    {
      if(m_body->m_velocity < speed_max)
      {
        m_body->m_velocity += 0.001f;

        if(m_body->m_velocity > speed_max)
          m_body->m_velocity = speed_max;
      }
    }
    else if(m_state->id == "slow")
    {
      if(m_body->m_velocity > speed_min)
      {
        m_body->m_velocity -= 0.001f;

        if(m_body->m_velocity < speed_min)
          m_body->m_velocity = speed_min;
      }
      else if(m_body->m_velocity <= speed_min)
        setState(m_state->switchto);

      for(int i = 0; i < m_dummies.size(); i++)
      {
        kgmGameObject* go = (kgmGameObject*)m_dummies[i]->linked();
      }
    }
    else if(m_state->id == "correct")
    {
      if(roll > 0.0f)
        roll -= 0.02f;
      else
        roll += 0.02f;

      if(fabs(roll) < 0.05f)
        roll = 0.0f;

      if(yaaw > 0.0f)
        yaaw -= 0.02f;
      else
        yaaw += 0.02f;

      if(fabs(yaaw) < 0.05f)
        yaaw = 0.0f;

      if(yaaw == 0.0 && roll == 0.0)
      {
        setState("idle", true);
      }
    }
    else if(m_state->id == "die")
    {
    }

    if(m_health < 1 && m_state->id != "dying")
    {
      setState("dying", true);

      for(int i = 0; i < m_dummies.size(); i++)
      {
        kgmGameObject* go = (kgmGameObject*)m_dummies[i]->linked();

        if(kgmObject::isValid(go))
        {
          go->event(this, "die");
          m_dummies[i]->detach();
        }
      }
    }
  }

  if(m_gameplayer)
  {
    kgmCamera& cam = game()->getGraphics()->camera();
    vec3 cpos = m_body->position() - m_body->direction() * c_dist;
    cpos.z = m_body->m_position.z + z_dist;
    cam.mPos = cpos;
    cam.mDir = m_body->direction();
    cam.update();

    if(kgmIGame::getGame()->getAudio())
    {
      vec3 no(0, 0, 0);
      vec3 spos = m_body->position();
      ((kgmGameAudio*)kgmIGame::getGame()->getAudio())->listener(spos, no, no);
    }
  }
}

void kgmCharacter::input(u32 in, int state)
{

}

