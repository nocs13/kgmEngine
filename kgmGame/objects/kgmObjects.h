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
             u32 count = 10,
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
    particles->m_count   = count;
    particles->m_life    = life;
    particles->m_loop    = loop;
    particles->st_size   = size_start;
    particles->en_size   = size_end;
    particles->div_life  = 0.8;

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

#endif // KGMOBJECTS_H
