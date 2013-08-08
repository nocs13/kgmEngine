#ifndef ASPACER_H
#define ASPACER_H

class ASpacer: public kgmActor
{
  KGM_OBJECT(ASpacer);

  kgmIGame* game;

  float     c_dist;
  float     z_dist;
  float     speed_max;
  float     speed_min;
  float     roll;
  float     yaaw;
  float     pich;

  bool      gbtns[65];

  ASp_Gui*   gui;
  bool       explode;
public:
  ASpacer(kgmIGame* g)
  {
    game      = g;
    c_dist    = 5.0f;
    z_dist    = 2.0f;
    speed_max = 0.05;
    speed_min = 0.01;
    roll      = 0.0;
    yaaw      = 0.0;
    pich      = 0.5 * PI;
    explode   = false;

    m_body->m_gravity = false;
    m_body->m_bound.min = vec3(-1, -1, -1);
    m_body->m_bound.max = vec3( 1,  1,  1);

    memset(gbtns, false, sizeof(gbtns));

    gui   = new ASp_Gui(game);

    m_health = 1;
    setId("MainPlayer");

    m_gameplayer = true;
  }

  ~ASpacer()
  {
    ((kgmGameBase*)game)->guiRemove(gui);
    gui->release();
  }

  void init()
  {
    if(m_gameplayer)
    {
      ASp_Skybox* sb = new ASp_Skybox(game);
      game->gAppend(sb);
      sb->release();

      setGroup(0);
      gui->set(this);
      gui->add(this);
      ((kgmGameBase*)game)->guiAdd(gui);

      kgmList<kgmGameObject*> objs;
      game->getLogic()->getObjectsByType(kgmActor::Class, objs);

      for(int i = 0; i < objs.size(); i++)
        gui->add((kgmActor*)objs[i]);

      objs.clear();

      getBody()->m_velocity = speed_min;
    }
  }

  void exit()
  {
  }

  void update(u32 mls)
  {
    kgmActor::update(mls);

    if(m_visual)
    {
      vec3 vz(0, 0, 1), vy(0, 0, 0), vx(1, 0, 0);
      mtx4 mz, mx, my, mr;
      pich = 0.0; //0.5 * PI;

      mz.rotate(pich, vz);
      mx.rotate(roll, vx);
      my.rotate(yaaw, vy);

      mr = my * mx;
      //mr.rotate(-roll, yaaw, -pich);
      m_visual->m_transform = mr * m_visual->m_transform;

      for(int i = 0; i < m_dummies.length(); i++)
      {
        kgmDummy*       dm = m_dummies[i];
        kgmGameObject*  go = (kgmGameObject*)dm->m_linked;

        if(go && kgmObject::isValid(go) && go->getVisual())
        {
          mtx4 m;
          vec3 v = dm->m_shift;

          //v.x -= 2.0f;
          m.identity();
          m.translate(v);

          go->getVisual()->m_transform = m * m_visual->m_transform;
        }
      }
    }

    if(getBody())
    {
      vec3 pos = getBody()->position();

      if(pos.x < -5000) pos.x =  5000;
      if(pos.x >  5000) pos.x = -5000;
      if(pos.y < -5000) pos.y =  5000;
      if(pos.x >  5000) pos.y = -5000;

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
          kgmGameObject* go = (kgmGameObject*)m_dummies[i]->m_linked;

          if(kgmObject::isValid(go))
          {
            go->event(this, "die");
            m_dummies[i]->m_linked = null;
          }
        }
      }
    }

    if(m_gameplayer)
    {
      kgmCamera& cam = ((kgmGameBase*)game)->m_render->camera();
      vec3 cpos = m_body->position() - m_body->direction() * c_dist;
      cpos.z = m_body->m_position.z + z_dist;
      cam.mPos = cpos;
      cam.mDir = m_body->direction();
      cam.update();

      gui->update();

      if(kgmIGame::getGame()->getAudio())
      {
        vec3 no(0, 0, 0);
        vec3 spos = m_body->position();
        kgmIGame::getGame()->getAudio()->listener(spos, no, no);
      }
    }
  }

  void input(u32 btn, int state)
  {
    static float alpha = 0.0f;
    vec3 vt;

    kgmActor::input(btn, state);

    gbtns[btn] = state;

    switch(btn)
    {
    case grot_x:
    {
      if(g_ms_camera)
      {
        alpha += (0.005f * state);
        vt = m_body->m_rotation;
        vt.z += (0.002f * state);
        m_body->rotate(0, 0, vt.z);
      }
      break;
    }
    case grot_y:
    {
      if(g_ms_camera)
      {
        z_dist -= (state * 0.005f);
        z_dist = (z_dist > 2.0)?(2.0):(z_dist);
        z_dist = (z_dist < .1)?(.1):(z_dist);
      }
      break;
    }
    case grot_z:
    {
      if(g_ms_camera)
      {
        c_dist += (state * 0.1f);
      }
      break;
    }
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
    else if(action == "dying")
    {
      action_dying();
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
                                        m_body->rotation(),
                                        m_body->m_velocity + 0.1f);

    dmy = dummy("Gun.2");

    if(dmy)
      pos = m_body->m_position + m * dmy->m_shift;
    else
      pos = m_body->m_position;

    kgmGameObject* go2 = new ASp_LaserA(game, 1000,
                                       pos,
                                       m_body->rotation(),
                                       m_body->m_velocity + 0.1f);

    go1->setId("laser1");
    go1->setGroup(getGroup());
    go2->setId("laser2");
    go2->setGroup(getGroup());

    game->gAppend(go1);
    game->gAppend(go2);

    go1->release();
    go2->release();
  }

  void action_shoot_rocket()
  {

  }

  void action_dying()
  {
    if(explode)
      return;

    box3  bnd = m_body->m_bound;
    vec3  pos = m_body->m_position;
    vec3  dim;  bnd.dimension(dim);

    kgmGameObject* go1 = new ASp_ExplodeA(game, pos, dim);
    kgmGameObject* go2 = new ASp_ExplodeC(game, pos, dim);


    go1->setId("explode1");
    go2->setId("explode1");

    game->gAppend(go1);
    game->gAppend(go2);

    go1->release();
    go2->release();

    explode = true;

    remove();
    game->getLogic()->action(kgmILogic::ACTION_GAMEOBJECT, this, "die");
  }
};
#endif // ASPACER_H
