#ifndef ASPACER_H
#define ASPACER_H

class ASpacer: public kgmActor
{
  kgmIGame* game;

  float     c_dist;
  float     z_dist;

public:
  ASpacer(kgmIGame* g)
  {
    game = g;
    c_dist = 5.0f;
    z_dist = 2.0f;
  }

  ~ASpacer()
  {

  }

  void update(u32 mls)
  {
    kgmActor::update(mls);

    if(m_visual)
    {
      vec3 v(0, 0, 1);
      mtx4 m;
      m.rotate(0.5 * PI, v);
      m_visual->m_transform = m * m_visual->m_transform;

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
      cpos.z = z_dist;
      cam.mPos = cpos;
      cam.mDir = m_body->m_direction;
      cam.update();
    }
  }

  void input(u32 btn, int state)
  {
    vec3 vt;

    if(!m_gameplayer)
      return;

    switch(btn)
    {
    case grot_x:
    {
      static float alpha = 0.0f;
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
      if(state)
          m_body->m_velocity = -0.01f;
      else
          m_body->m_velocity = 0.0f;
      break;
    case gbtn_up:
        if(state)
            m_body->m_velocity = 0.01f;
        else
            m_body->m_velocity = 0.0f;
      break;
    case gbtn_left:
      break;
    case gbtn_right:
      break;
    }
  }
};

#endif // ASPACER_H
