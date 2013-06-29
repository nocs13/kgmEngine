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

  kgmVisual* vtext;

public:
  ASp_Spacer(kgmIGame* g)
  {
    game = g;

    speed_max = 0.05;
    speed_min = 0.01;
    chase_max = 70.0;
    chase_min = 15.0;
    roll      = 0.0;
    yaaw      = 0.0;

    chase     = false;

    m_body->m_gravity = false;
    m_body->m_bound.min = vec3(-1, -1, -1);
    m_body->m_bound.max = vec3( 1,  1,  1);

    target = null;
    vtext = new kgmVisual();

    kgmText* text = new kgmText();
    text->m_rect  = uRect(10, 150, 500, 200);

    vtext->set(text);
    ((kgmGameBase*)game)->m_render->add(vtext);
    text->release();
  }

  virtual ~ASp_Spacer()
  {
    vtext->remove();
    vtext->release();
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

      kgmString ts = "ASp_Spacer state: ";

      if(m_state)
        vtext->getText()->m_text = ts + m_state->id;
      else
        vtext->getText()->m_text = ts;
    }

    if(m_state)
    {
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

      logic(m_state->id);
    }
  }
};

class ASp_SpacerA: public ASp_Spacer
{
  KGM_OBJECT(ASp_SpacerA);

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
    float dist = 0, angle = 0;
    kgmString ts = "ASp_Spacer state x aim: ";
    vtext->getText()->m_text = ts + s;

    if(target)
    {
      vec3 tpos   = target->getBody()->m_position;
      vec3 tdir   = tpos - m_body->m_position;
      dist  = tdir.length();
      angle = m_body->m_direction.angle(tdir.normal());
    }
    else
    {
      kgmString sgo, saim;

      m_options.get("Aim", saim);
      m_options.get("Target", sgo);

      if(sgo.length() > 0)
        target = game->getLogic()->getObjectById(sgo);
    }

    if(s == "idle")
    {
      if(dist < chase_min)
      {
        setState("evade");
      }
      else if(dist > chase_max)
      {
        setState("chase");
      }
      else if(dist < chase_max)
      {
        if(angle < (PI / 10))
          setState("laser");
        else
          setState("chase");
      }
      else if(yaaw != 0.0 || roll != 0.0)
      {
        setState("correct");
      }
    }
    else if(s == "left" || s == "right")
    {
      if(dist < chase_min)
      {
        setState("idle");
      }
      else if(dist > chase_max && angle < (PI / 10))
      {
        setState("fast");
      }
      else if(angle < (PI / 20))
      {
        setState("laser");
      }
    }
    else if(s == "chase")
    {
      if(dist < chase_min)
      {
        setState("correct");
      }
      if(dist > chase_max)
      {
        setState("idle");
      }
      else if(angle < (PI / 20))
      {
        setState("laser");
      }
      else if(angle > (PI / 6))
      {
        if(rand() % 2)
          setState("left");
        else
          setState("right");
      }
    }
    else if(s == "evade")
    {
      if(dist > chase_min)
        setState("idle");
    }
    else if(s == "fast")
    {
      if(dist < chase_max)
        setState("slow");
    }
  }

  void action(kgmString action)
  {
    if(action == "laser")
    {
      action_shoot_laser();
    }
    else if(action == "rocket")
    {
      action_shoot_rocket();
    }
  }

  void action_shoot_laser()
  {
    vec3 r, v(0, 0, 1);
    mtx4 m;
    m.rotate(0.5 * PI, v);

    vec3      pos;
    kgmDummy* dmy = null;

    dmy = dummy("Gun.1");

    m_visual->m_transform.angles(r);
    m.rotate(r);

    if(dmy)
      pos = m_body->m_position + m * dmy->m_shift;
    else
      pos = m_body->m_position;

    kgmGameObject* go1 = new ASp_LaserA(game, 1000,
                                       pos,
                                       m_body->m_direction,
                                       m_body->m_velocity + 0.1f);

    dmy = dummy("Gun.2");

    if(dmy)
      pos = m_body->m_position + m * dmy->m_shift;
    else
      pos = m_body->m_position;

    kgmGameObject* go2 = new ASp_LaserA(game, 1000,
                                       pos,
                                       m_body->m_direction,
                                       m_body->m_velocity + 0.1f);

    go1->setId("laser1");
    go1->setParent(this);
    go1->setGroup(getGroup());
    go2->setId("laser2");
    go2->setParent(this);
    go2->setGroup(getGroup());

    game->gAppend(go1);
    game->gAppend(go2);

    go1->release();
    go2->release();
  }

  void action_shoot_rocket()
  {

  }
};
#endif // ASP_SPACER_H
