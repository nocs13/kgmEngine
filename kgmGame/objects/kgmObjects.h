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

protected:
  kgmParticles* particles;
  kgmMaterial*  material;

public:
  kgmExplode(kgmIGame* g, vec3 pos, vec3 vol, kgmString tid = "fire_a.tga")
  {
    timeout(3000);

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

    particles->direction = vec3(1, 1, 0.4);
    particles->volume    = vol;
    particles->m_speed = .9;
    particles->m_count = 10;
    particles->m_life  = 2000;
    particles->m_loop  = false;
    particles->st_size = .1;
    particles->en_size = 2.0;
    particles->div_life = 1.0;

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
