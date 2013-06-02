#ifndef AKOMBLE_H
#define AKOMBLE_H

class AKomble: public kgmActor
{
  kgmIGame* game;
  float     z_dist;

public:
  AKomble(kgmIGame* g)
  :kgmActor()
  {
    game   = g;
    z_dist = 3.0f;
  }

  void update(u32 mls)
  {
    kgmActor::update(mls);

    if(m_gameplayer)
    {
      kgmCamera& cam = ((kgmGameBase*)game)->m_render->camera();
      vec3 cpos = m_body->m_position;
      cpos.y -= z_dist;

      cam.mPos = cpos;
      cam.mDir = m_body->m_position - cpos;
      cam.mDir.normalize();
      cam.update();
    }
  }

  void input(u32 btn, int state)
  {

  }
};

#endif // AKOMBLE_H
