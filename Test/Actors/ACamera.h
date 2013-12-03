#ifndef ACAMERA_H
#define ACAMERA_H

#endif // ACAMERA_H
class ACamera: public kgmActor
{
  KGM_OBJECT(ACamera);

  kgmIGame* game;
  float     z_dist, c_dist;

public:
  ACamera(kgmIGame* g)
    :kgmActor()
  {
    game   = g;
    z_dist = 3.0f;
    c_dist = 1.0f;
  }

  void update(u32 mls)
  {
    kgmActor::update(mls);

    kgmCamera& cam = ((kgmGameBase*)game)->m_render->camera();
    vec3 cpos = m_body->m_position;
    cpos.y -= z_dist;

    cam.mPos = cpos;
    cam.mDir = m_body->m_position - cpos;
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
};
