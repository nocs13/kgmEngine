#ifndef ASPACER_H
#define ASPACER_H

class ASpacer: public kgmActor
{
  kgmIGame* game;

  float     c_dist;
  float     z_dist;
  float     speed_max;
  float     speed_min;
  float     roll;
  float     yaaw;

  bool      gbtns[65];
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

    m_body->m_gravity = false;
    m_body->m_bound.min = vec3(-1, -1, -1);
    m_body->m_bound.max = vec3( 1,  1,  1);

    memset(gbtns, false, sizeof(gbtns));
  }

  ~ASpacer()
  {

  }

  void init()
  {
    if(m_gameplayer)
    {
      ASp_Skybox* sb = new ASp_Skybox(game);
      game->gAppend(sb);
      addChild(sb);
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
      float pich = 0.5 * PI;

      mz.rotate(0.5 * PI, vz);
      mx.rotate(roll, vx);
      my.rotate(yaaw, vy);

//      mr = my * mx;
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

          //v.x -= 2.0f;
          m.identity();
          m.translate(v);

          go->getVisual()->m_transform = m * m_visual->m_transform;
        }
      }
    }

    if(m_gameplayer)
    {
      kgmCamera& cam = ((kgmGameBase*)game)->m_render->camera();
      vec3 cpos = m_body->m_position - m_body->m_direction * c_dist;
      cpos.z = m_body->m_position.z + z_dist;
      cam.mPos = cpos;
      cam.mDir = m_body->m_direction;
      cam.update();

      if(gbtns[gbtn_left] && yaaw == 0.0)
      {
        vec3 vt = m_body->m_rotation;
        vt.z += (0.02f);
        m_body->rotate(vt.x, vt.y, vt.z);

        if(roll > -PI/4)
          roll -= 0.02f;
      }
      else if(gbtns[gbtn_right] && yaaw == 0.0)
      {
        vec3 vt = m_body->m_rotation;
        vt.z -= (0.02f);
        m_body->rotate(vt.x, vt.y, vt.z);

        if(roll < PI/4)
          roll += 0.02f;
      }
      else if(!gbtns[gbtn_left] && !gbtns[gbtn_left] && roll != 0.0f)
      {
        if(roll > 0.0f)
          roll -= 0.02f;
        else
          roll += 0.02f;

        if(fabs(roll) < 0.02f)
          roll = 0.0f;
      }

      if(gbtns[gbtn_up] && gbtns[gbtn_n] && yaaw < PI/6 && roll == 0.0)
      {
        yaaw += 0.02f;

        vec3 vt = m_body->m_rotation;
        vt.y += (0.02f);
        m_body->rotate(vt.y, 0, vt.z);
      }
      else if(gbtns[gbtn_up] && m_body->m_velocity < speed_max)
      {
        m_body->m_velocity += 0.001f;

        if(m_body->m_velocity > speed_max)
          m_body->m_velocity = speed_max;
      }
      else if(!gbtns[gbtn_up] && m_body->m_velocity > speed_min)
      {
        m_body->m_velocity -= 0.001f;

        if(m_body->m_velocity < speed_min)
          m_body->m_velocity = speed_min;
      }
      else if(gbtns[gbtn_down] && gbtns[gbtn_n] && yaaw > -PI/6 && roll == 0.0)
      {
        yaaw -= 0.02f;

        vec3 vt = m_body->m_rotation;
        vt.y -= (0.02f);
        m_body->rotate(vt.y, 0, vt.z);
      }
      else if(!gbtns[gbtn_up] && !gbtns[gbtn_down] && yaaw != 0.0)
      {
        if(yaaw > 0.0f)
          yaaw -= 0.02f;
        else
          yaaw += 0.02f;

        if(fabs(yaaw) < 0.02f)
          yaaw = 0.0f;
      }
      else if(gbtns[gbtn_down])
      {

      }
    }
  }

  void input(u32 btn, int state)
  {
    static float alpha = 0.0f;
    vec3 vt;

    if(!m_gameplayer)
      return;

    kgmActor::input(btn, state);

    switch(btn)
    {
    case grot_x:
    {
      alpha += (0.005f * state);
      vt = m_body->m_rotation;
      vt.z += (0.002f * state);
      m_body->rotate(0, 0, vt.z);
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
      gbtns[gbtn_down] = (state)?(true):(false);
      break;
    case gbtn_up:
      gbtns[gbtn_up] = (state)?(true):(false);
      break;
    case gbtn_left:
      gbtns[gbtn_left] = (state)?(true):(false);
      break;
    case gbtn_right:
      gbtns[gbtn_right] = (state)?(true):(false);
      break;
    case gbtn_n:
      gbtns[gbtn_n] = (state)?(true):(false);
      break;
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

    game->gAppend(go1);
    game->gAppend(go2);
    addChild(go1);
    addChild(go2);
    go1->release();
    go2->release();
  }

  void action_shoot_rocket()
  {

  }
};

#endif // ASPACER_H
