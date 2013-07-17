#ifndef ASP_GUN_H
#define ASP_GUN_H

class ASp_Gun: public kgmActor
{
  KGM_OBJECT(ASp_Gun);

protected:
  kgmIGame* game;

  kgmVisual*      vtext;
  kgmGameObject*  target;

  float           shoot_distance;

  vec3            rotate;
  bool            shoot;
public:
  ASp_Gun(kgmIGame* g)
  {
    game = g;
    target = null;

    shoot_distance = 30.0;
    shoot          = false;

    m_body->m_gravity = false;
    m_body->m_bound.min = vec3(-1, -1, -1);
    m_body->m_bound.max = vec3( 1,  1,  1);

    vtext = new kgmVisual();

    kgmText* text = new kgmText();
    text->m_rect  = uRect(30, 200, 700, 200);

    vtext->set(text);
    ((kgmGameBase*)game)->m_render->add(vtext);
    text->release();
  }

  virtual ~ASp_Gun()
  {
    vtext->remove();
    vtext->release();
  }

  void init()
  {
    setState("idle");
  }

  void update(u32 ms)
  {
    kgmActor::update(ms);

    if(m_visual)
    {
      vec3 vz(0, 0, 1), vy(0, 0, 0), vx(1, 0, 0);
      mtx4 mz, mx, my, mr;

      mz.rotate(0.5 * PI, vz);

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

      kgmString ts = "ASp_Gun state: ";

      if(m_state)
        vtext->getText()->m_text = ts + m_state->id;
      else
        vtext->getText()->m_text = ts;
    }

    if(m_state)
    {
      if(m_state->id == "idle")
      {
      }
      else if(m_state->id == "shoot")
      {
      }

      logic(m_state->id);
    }
  }

  void logic(kgmString s)
  {
    float dist = -1, angle = 0, side = 0;
    vec3  left, orin(0, 0, 1);
    kgmString ts = "ASp_Spacer state x aim: ";

    vtext->getText()->m_text = ts + s;

    if(target)
    {
      vec3 tpos   = target->getBody()->m_position;
      vec3 tdir   = tpos - m_body->m_position;
      dist  = tdir.length();
      angle = m_body->direction().angle(tdir.normal());

      left = orin.cross(tdir);

      plane3 plan(left, getBody()->position());
      side = plan.distance(tpos);
    }
    else
    {
      kgmString sgo, saim;

      m_options.get("Aim", saim);
      m_options.get("Target", sgo);

      if(sgo.length() > 0)
      {
        target = game->getLogic()->getObjectById(sgo);
      }
      else if(m_parent)
      {
        sgo = ((kgmActor*)m_parent)->getOption("Target");

        if(sgo.length() > 0)
        {
          target = game->getLogic()->getObjectById(sgo);
        }
      }
    }

    if(s == "idle")
    {
      if(dist > 0 && dist < shoot_distance && angle < (PI / 4))
      {
        setState("aiming");
      }
    }
    else if(s == "aiming")
    {
      if(!shoot)
      {
        vec3 rot;

        rot = rotate = getBody()->m_rotation;

        if(side < 0)
          rot.z += angle;
        else
          rot.z -= angle;

        shoot = true;
        getBody()->rotate(rot);
        setState("shoot");
      }
      else
      {
        shoot = false;
        getBody()->rotate(rotate);

        setState("idle");
      }
    }
  }

  void action(kgmString action)
  {
    if(action == "shoot")
    {
      action_shoot_laser();
    }
  }

  void action_shoot_laser()
  {
    vec3 r, v(0, 0, 1);
    mtx4 m;
    m.rotate(0.5 * PI, v);

    vec3      pos;
    kgmDummy* dmy = null;

    dmy = dummy("shoot");

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

    go1->setId("laser1");
    go1->setParent(this);
    go1->setGroup(getGroup());

    game->gAppend(go1);

    go1->release();
  }
};

class ASp_GunA: public ASp_Gun
{
  KGM_OBJECT(ASp_GunA);
public:
  ASp_GunA(kgmIGame* g)
  :ASp_Gun(g)
  {

  }
};

class ASp_GunFA: public ASp_Gun
{
  KGM_OBJECT(ASp_GunFA);
public:
  ASp_GunFA(kgmIGame* g)
  :ASp_Gun(g)
  {

  }

  void update(u32 ms)
  {
    ASp_Gun::update(ms);
  }
};

#endif // ASP_GUN_H
