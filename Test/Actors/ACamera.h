#ifndef ACAMERA_H
#define ACAMERA_H

#endif // ACAMERA_H
class ACamera: public kgmActor
{
  KGM_GO_OBJECT(ACamera);

  kgmIGame* game;
  float     z_dist, c_dist;
  bool      ms_left, ms_right;

public:

  ACamera(kgmIGame* g)
    :kgmActor()
  {
    game   = g;
    z_dist = 3.0f;
    c_dist = 1.0f;
    ms_left = false;
    ms_right = false;

    m_body->m_mass = 0.0f;
  }

  void update(u32 mls)
  {
    kgmActor::update(mls);

    kgmCamera& cam = ((kgmGameBase*)game)->m_render->camera();
    vec3 cpos = m_body->m_position;
    cpos.y -= z_dist;

    cam.mPos = m_body->position();
    cam.mDir = m_body->direction();
    cam.mDir.normalize();
    cam.update();
  }

  void input(u32 btn, int state)
  {
    static float alpha = 0.0f;
    vec3 vt;

    kgmActor::input(btn, state);


    switch(btn)
    {
    case gbtn_up:
      if(state)
        m_body->m_position = m_body->m_position + m_body->direction() * 1.0f;
      break;
    case gbtn_down:
      if(state)
        m_body->m_position = m_body->m_position + m_body->direction() * -1.0f;
      break;
    case gbtn_a:
      ms_left = (state) ? (true) : (false);
      break;
    case gbtn_b:
      ms_right = (state) ? (true) : (false);
      break;
    case gbtn_left:
    case gbtn_right:
      break;
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

      if(ms_left)
      {
        m_body->m_position = m_body->m_position + m_body->direction() * (float)state * 0.1f;
      }
      else if(ms_right)
      {
        m_body->m_position.z += (float)state * 0.1f;
      }
      break;
    }
    case grot_z:
    {
      c_dist += (state * 0.1f);
      break;
    }
    }
  }
};
