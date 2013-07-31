#ifndef ASP_SPACESHIP_H
#define ASP_SPACESHIP_H
class ASp_Spaceship: public kgmActor
{
  KGM_OBJECT(ASp_Spaceship);

protected:
  kgmIGame* game;

  bool       explode;

public:
  ASp_Spaceship(kgmIGame* g)
  {
    game = g;
    explode = false;
    m_health = 10;

    m_body->m_gravity = false;
    m_body->m_bound.min = vec3(-3, -3, -3);
    m_body->m_bound.max = vec3( 3,  3,  3);

    setGroup(1);
  }

  virtual ~ASp_Spaceship()
  {
  }

  void init()
  {
    for(int i = 0; i < m_dummies.size(); i++)
    {
      kgmGameObject* go = (kgmGameObject*)m_dummies[i]->m_linked;

      ((kgmActor*)go)->setGroup(getGroup());
    }

    setState("idle");
  }

  void exit()
  {
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

        if(go && kgmObject::isValid(go) && go->getBody())
        {
          vec3 v = getBody()->m_position +  dm->m_shift;
          vec3 r = getBody()->m_rotation +  dm->m_orient;

          go->getBody()->translate(v);
          go->getBody()->rotate(r);
        }
      }
    }

    if(m_state)
    {
      if(m_state->id == "idle")
      {
      }
      else if(m_state->id == "die")
      {
        remove();
        game->getLogic()->action(kgmILogic::ACTION_GAMEOBJECT, this, "die");
      }

      if(this->m_health < 1 && m_state->id != "dying")
      {
        setState("dying", true);

        m_visual->disable();
      }

      logic(m_state->id);
    }
  }

  void action(kgmString action)
  {
    if(action == "dying")
    {
      action_dying();
    }
  }

  void action_dying()
  {
    if(explode)
      return;

    vec3      pos = m_body->m_position;

    kgmGameObject* go1 = new ASp_ExplodeA(game, pos);

    go1->setId("explode1");

    game->gAppend(go1);

    go1->release();
    explode = true;
  }
};

class ASp_SpaceshipA: public ASp_Spaceship
{
  KGM_OBJECT(ASp_SpaceshipA);
public:
  ASp_SpaceshipA(kgmIGame* g)
    :ASp_Spaceship(g)
  {

  }

  void init()
  {
    ASp_Spaceship::init();
  }

  void exit()
  {
    ASp_Spaceship::exit();
  }
};
#endif // ASP_SPACESHIP_H
