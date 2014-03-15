#ifndef KGMOBJECTS_H
#define KGMOBJECTS_H

#include "../kgmIGame.h"
#include "../kgmGameObject.h"
#include "../kgmGameResources.h"
#include "../../kgmGraphics/kgmMaterial.h"

class kgmFlame: public kgmGameObject
{
  KGM_OBJECT(kgmFlame);

protected:
  kgmParticles* particles;
  kgmMaterial*  material;

public:
  kgmFlame(kgmIGame* g)
  {
    particles = new kgmParticles();
    m_visual  = new kgmVisual();

    material = new kgmMaterial();
    material->m_depth        = false;
    material->m_blend        = true;
    material->m_srcblend     = gcblend_srcalpha;
    material->m_dstblend     = gcblend_one;
    material->m_type         = "simple";
    material->m_shader       = kgmMaterial::ShaderBlend;
    material->m_tex_color    = g->getResources()->getTexture("fire_a.tga");

    particles->m_count   = 10;
    particles->m_speed   = 1.1;
    particles->div_speed = 1.0;
    particles->m_life    = 2000;
    particles->div_life  = 1.0;

    particles->build();
    m_visual->set(material);
    m_visual->set(particles);
  }

  virtual ~kgmFlame()
  {
    if(particles)
      particles->release();

    if(material)
      material->release();
  }
};

class kgmFlameA: public kgmFlame
{
  KGM_OBJECT(kgmFlameA);
public:
  kgmFlameA(kgmIGame* g)
    :kgmFlame(g)
  {

  }
};

class kgmSmoke: public kgmGameObject
{
  KGM_OBJECT(kgmSmoke);

protected:
  kgmParticles* particles;
  kgmMaterial*  material;
public:
  kgmSmoke(kgmIGame* g)
  {
    particles = new kgmParticles();
    m_visual  = new kgmVisual();

    material = new kgmMaterial();
    material->m_blend        = true;
    material->m_srcblend     = gcblend_srcalpha;
    material->m_dstblend     = gcblend_one;
    material->m_type         = "simple";
    material->m_shader       = kgmMaterial::ShaderBlend;
    material->m_tex_color    = g->getResources()->getTexture("smoke_a.tga");

    particles->m_speed = .01;
    particles->m_count = 10;
    particles->m_life  = 5000;
    particles->div_speed = .5;
    particles->build();

    m_visual->set(material);
    m_visual->set(particles);
  }

  virtual ~kgmSmoke()
  {
    if(particles)
      particles->release();

    if(material)
      material->release();
  }
};

class kgmSmokeA: public kgmSmoke
{
  KGM_OBJECT(kgmSmokeA);
public:
  kgmSmokeA(kgmIGame* g)
    :kgmSmoke(g)
  {

  }
};

class kgmExplode: public kgmGameObject
{
  KGM_OBJECT(kgmExplode);

private:
  kgmIGame* game;

protected:
  kgmParticles* particles;
  kgmMaterial*  material;

public:
  kgmExplode(kgmIGame* g,
             vec3 pos = vec3(0, 0, 0), vec3 vol = vec3(1, 1, 1), vec3 dir = vec3(0, 0, 0),
             float speed = 1.0f, float life = 1000,
             float size_start = 0.1f, float size_end = 2.0f,
             u32 count = 20,
             bool loop = false,
             kgmString tid="fire_a.tga")
  {
    timeout(life);

    game = g;

    particles = new kgmParticles();
    m_visual  = new kgmVisual();

    material = new kgmMaterial();
    material->m_depth        = false;
    material->m_blend        = true;
    material->m_srcblend     = gcblend_srcalpha;
    material->m_dstblend     = gcblend_one;
    material->m_type         = "simple";
    material->m_shader       = kgmMaterial::ShaderBlend;
    material->m_tex_color    = g->getResources()->getTexture(tid);

    particles->direction = dir;
    particles->volume    = vol;
    particles->m_speed   = speed;
    particles->m_count = count;
    particles->m_life  = life;
    particles->m_loop  = loop;
    particles->st_size = size_start;
    particles->en_size = size_end;
    particles->div_life = 0.8;

    particles->build();

    m_visual->set(material);
    m_visual->set(particles);

    setPosition(pos);
  }

  virtual ~kgmExplode()
  {
    if(particles)
      particles->release();

    if(material)
      material->release();
  }

  void setTexture(kgmString tid)
  {
    if(material->m_tex_color)
      material->m_tex_color->release();

    material->m_tex_color = game->getResources()->getTexture(tid);
  }

  void setSlideFrames(u32 rows, u32 cols)
  {
    particles->tex_slide_rows = rows;
    particles->tex_slide_cols = cols;
  }
};

class kgmExplodeA: public kgmExplode
{
  KGM_OBJECT(kgmExplodeA);
public:
  kgmExplodeA(kgmIGame* g, vec3 pos, vec3 vol)
    :kgmExplode(g, pos, vol)
  {
  }
};

class kgmExplodeB: public kgmExplode
{
  KGM_OBJECT(kgmExplodeB);
public:
  kgmExplodeB(kgmIGame* g, vec3 pos, vec3 vol)
    :kgmExplode(g, pos, vol)
  {
    material->m_tex_color  = g->getResources()->getTexture("smoke_a.tga");
    particles->m_count     = 10;
    particles->build();
  }
};

class kgmExplodeC: public kgmExplode
{
  KGM_OBJECT(kgmExplodeC);
public:
  kgmExplodeC(kgmIGame* g, vec3 pos, vec3 vol)
    :kgmExplode(g, pos, vol)
  {
    particles->m_fade      = false;
    particles->m_count     = 50;
    particles->st_size     = 0.01;
    particles->en_size     = 0.05;
    particles->m_speed     = 60.0;
    particles->div_speed     = .5;

    particles->build();
  }
};

#endif // KGMOBJECTS_H
