#ifndef ASP_SPACER_H
#define ASP_SPACER_H

class ASp_Spacer: public kgmActor
{
  KGM_OBJECT(ASp_Spacer);

protected:
  kgmIGame* game;

  float     speed_max;
  float     speed_min;
  float     chase_max;
  float     chase_min;
  float     roll;
  float     yaaw;

  bool      chase;

  kgmGameObject*  target;

public:
  ASp_Spacer(kgmIGame* g)
  {
    game = g;

    speed_max = 0.05;
    speed_min = 0.01;
    chase_max = 70.0;
    chase_min = 5.00;
    roll      = 0.0;
    yaaw      = 0.0;

    chase     = false;

    m_body->m_gravity = false;
    m_body->m_bound.min = vec3(-1, -1, -1);
    m_body->m_bound.max = vec3( 1,  1,  1);

    target = null;
  }

  virtual ~ASp_Spacer()
  {

  }

  void update(u32 ms)
  {
    kgmActor::update(ms);

    if(m_visual)
    {
      vec3 vz(0, 0, 1), vy(0, 0, 0), vx(1, 0, 0);
      mtx4 mz, mx, my, mr;
      float pich = 0.5 * PI;

      mz.rotate(0.5 * PI, vz);
      mx.rotate(roll, vx);
      my.rotate(yaaw, vy);

      mr.rotate(-roll, yaaw, -pich);
      m_visual->m_transform = mr * m_visual->m_transform;

      for(int i = 0; i < m_dummies.length(); i++)
      {
        kgmDummy*       dm = m_dummies[i];
        kgmGameObject*  go = (kgmGameObject*)dm->m_linked;

        if(go && go->getVisual())
        {
          mtx4 m;
          vec3 v = dm->m_shift;

          m.identity();
          m.translate(v);

          go->getVisual()->m_transform = m * m_visual->m_transform;
        }
      }
    }

    if(m_state)
    {
      logic(m_state->id);

      if(m_state->id == "idle")
      {
        if(m_body->m_velocity > speed_min)
        {
          setState("slow");
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
    }
  }
};

class ASp_SpacerA: public ASp_Spacer
{
  KGM_OBJECT(ASp_SpacerA);

  kgmString       aim;
  kgmGameObject*  target;

public:
  ASp_SpacerA(kgmIGame* g)
  :ASp_Spacer(g)
  {
    target = null;

    m_body->m_velocity = speed_min;
  }

  void update(u32 ms)
  {
    ASp_Spacer::update(ms);
  }

  void logic(kgmString s)
  {
    if(s == "idle")
    {
      if(target == null || aim.length() < 1)
      {
        kgmString sgo, saim;

        m_options.get("Aim", saim);
        m_options.get("Target", sgo);

        aim = saim;

        if(sgo.length() > 0)
          target = game->getLogic()->getObjectById(sgo);
      }
      else if(target)
      {
        vec3 tpos   = target->getBody()->m_position;
        vec3 tdir   = tpos - m_body->m_position;
        float dist  = tdir.length();
        float angle = m_body->m_direction.angle(tdir.normal());

        if(angle < (PI / 100))
          setState("laser");
        else
          setState("left");
      }
    }
  }

};
#endif // ASP_SPACER_H
