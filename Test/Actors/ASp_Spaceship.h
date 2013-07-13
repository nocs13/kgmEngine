#ifndef ASP_SPACESHIP_H
#define ASP_SPACESHIP_H
class ASp_Spaceship: public kgmActor
{
  KGM_OBJECT(ASp_Spaceship);

protected:
  kgmIGame* game;

  kgmVisual* vtext;

public:
  ASp_Spaceship(kgmIGame* g)
  {
    game = g;

    m_body->m_gravity = false;
    m_body->m_bound.min = vec3(-3, -3, -3);
    m_body->m_bound.max = vec3( 3,  3,  3);

    vtext = new kgmVisual();

    kgmText* text = new kgmText();
    text->m_rect  = uRect(10, 150, 500, 200);

    vtext->set(text);
    ((kgmGameBase*)game)->m_render->add(vtext);
    text->release();
  }

  virtual ~ASp_Spaceship()
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
      }

      logic(m_state->id);
    }
  }
};

class ASp_SpaceshipA: public ASp_Spaceship
{
public:
  ASp_SpaceshipA(kgmIGame* g)
    :ASp_Spaceship(g)
  {

  }
};
#endif // ASP_SPACESHIP_H
