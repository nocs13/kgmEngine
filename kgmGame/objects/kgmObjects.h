#ifndef KGMOBJECTS_H
#define KGMOBJECTS_H

#include "../kgmIGame.h"
#include "../kgmGameObject.h"
#include "../kgmGameResources.h"
#include "../../kgmGraphics/kgmMaterial.h"

class kgmResult: public kgmGameObject
{
  KGM_OBJECT(kgmResult);

  kgmIGame* game;
  kgmString text;
  s32       result;

public:
  kgmResult(kgmIGame* g, u32 time, s32 res, kgmString txt)
  {
    game = g;
    text = txt;
    result = res;
    timeout(time);

    int cx, cy, cw, ch, tw, tl;
    //game->getWindow()->getRect(cx, cy, cw, ch);

    tl = txt.length();

    if(tl < 1)
      tl = 1;

    tw = 300 / tl;

    m_visual  = new kgmVisual();
    kgmText* text = new kgmText();
    text->m_rect  = uRect(10, ch - 2 * tw, 300, 2 * tw);
    text->m_text  = txt;
    text->m_size  = tw;

    m_visual->set(text);
    text->release();
  }

  virtual ~kgmResult()
  {
  }

  s32 getResult()
  {
    return result;
  }

  void update(u32 t)
  {
    kgmGameObject::update(t);
  }

  void exit()
  {
    //game->getLogic()->action(kgmILogic::ACTION_GAMEOBJECT, this, "result");
  }
};

class kgmParticleObject: public kgmGameObject
{
  KGM_OBJECT(kgmParticleObject);

private:
  kgmIGame* game;

protected:
  kgmParticles* particles;
  kgmMaterial*  material;

public:
  kgmParticleObject(kgmIGame* g,
               vec3 pos = vec3(0, 0, 0), vec3 vol = vec3(1, 1, 1), vec3 dir = vec3(0, 0, 0),
               float speed = 0.0f, float div_speed = 0.0,
               float life = 1000, float div_life = 0.5f,
               float size_start = .1f, float size_end = 1.0f,
               u32 count = 1,
               kgmString tid = "",
               bool loop = false)
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
    particles->div_life  = div_life;

    particles->build();

    m_visual->set(material);
    m_visual->set(particles);

    setPosition(pos);
  }

  kgmParticleObject(kgmIGame* g, kgmParticles* pts, kgmMaterial* mtl, u32 life)
  {
    game = g;
    material = mtl;
    particles = pts;

    pts->increment();
    mtl->increment();

    timeout(life);
  }

  virtual ~kgmParticleObject()
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
             float size_start = .1f, float size_end = 2.0f,
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

class kgmLaser: public kgmGameObject
{
  KGM_OBJECT(kgmLaser);

  struct Vertex
  {
    vec3 v;
    u32  c;
    vec2 uv;
  };

  kgmMesh*      mesh;
  kgmMaterial*  mtl;

public:
  u32           power;

public:
  kgmLaser(kgmIGame* g, u32 time, vec3 pos, vec3 rot, float speed,
           float hlen=0.5, float hwid=0.1)
  {
    power = 1;
    timeout(time);
    this->setId("laser");
    m_visual  = new kgmVisual();

    mtl = new kgmMaterial();
    mtl->m_2side = true;
    mtl->m_depth = false;
    mtl->m_blend = true;
    mtl->m_srcblend = gcblend_one;
    mtl->m_dstblend = gcblend_one;
    mtl->m_tex_color = g->getResources()->getTexture("point_redd.tga");
    mtl->m_shader = kgmMaterial::ShaderBlend;

    mesh = new kgmMesh();
    Vertex* v = (Vertex*)mesh->vAlloc(18, kgmMesh::FVF_P_C_T);

    v[0]  = { {0, -hwid, -hwid}, 0xffffffff, {0, 1}};
    v[1]  = { {0, -hwid,  hwid}, 0xffffffff, {0, 0}};
    v[2]  = { {0,  hwid,  hwid}, 0xffffffff, {1, 0}};
    v[3]  = { {0,  hwid,  hwid}, 0xffffffff, {1, 0}};
    v[4]  = { {0,  hwid, -hwid}, 0xffffffff, {1, 1}};
    v[5]  = { {0, -hwid, -hwid}, 0xffffffff, {0, 1}};

    v[6]  = { {hlen,  -hwid, 0}, 0xffffffff, {0, 1}};
    v[7]  = { {hlen,   hwid, 0}, 0xffffffff, {0, 0}};
    v[8]  = { {-hlen,  hwid, 0}, 0xffffffff, {1, 0}};
    v[9]  = { {-hlen,  hwid, 0}, 0xffffffff, {1, 0}};
    v[10] = { {-hlen, -hwid, 0}, 0xffffffff, {1, 1}};
    v[11] = { {hlen,  -hwid, 0}, 0xffffffff, {0, 1}};

    v[12] = { {hlen,  0, -hwid}, 0xffffffff, {0, 1}};
    v[13] = { {hlen,  0, hwid},  0xffffffff, {0, 0}};
    v[14] = { {-hlen, 0, hwid},  0xffffffff, {1, 0}};
    v[15] = { {-hlen, 0, hwid},  0xffffffff, {1, 0}};
    v[16] = { {-hlen, 0, -hwid}, 0xffffffff, {1, 1}};
    v[17] = { {hlen,  0, -hwid}, 0xffffffff, {0, 1}};

    m_visual->set(mesh);
    m_visual->set(mtl);
    mesh->release();
    mtl->release();

    m_body = new kgmBody();

    m_body->translate(pos);
    m_body->rotate(rot);
    m_body->m_velocity  = speed;
    m_body->m_gravity   = false;
    m_body->m_udata     = this;
    m_body->m_mass      = 0.0f;
    m_body->m_bound.min = vec3(-hlen, -hwid, -hwid);
    m_body->m_bound.max = vec3( hlen,  hwid,  hwid);
  }

  virtual ~kgmLaser()
  {
  }
};

class kgmIonNozzle: public kgmGameObject
{
  KGM_OBJECT(kgmIonNozzle);

  struct Vertex
  {
    vec3 v;
    u32  c;
    vec2 uv;
  };

  kgmIGame*      game;
  kgmMaterial*   mtl;
  kgmParticles*  ptl;

  kgmVisual*     visual;
  kgmMesh*       mesh;
public:
  kgmIonNozzle(kgmIGame* g)
  {
    game     = g;
    m_visual = new kgmVisual();

    mtl = new kgmMaterial();
    mtl->m_2side = true;
    mtl->m_blend = true;
    mtl->m_depth = false;
    mtl->m_srcblend = gcblend_srcalpha;
    mtl->m_dstblend = gcblend_one;
    mtl->m_tex_color = g->getResources()->getTexture("point_d.tga");
    mtl->m_type = "simple";
    mtl->m_shader = kgmMaterial::ShaderBlend;

    ptl = new kgmParticles();
    ptl->m_typerender  = kgmParticles::RTypePoint;
    ptl->m_count       = 25;
    ptl->m_speed       = 2.0f;
    ptl->m_life        = 200;
    ptl->div_life      = 0.95;
    ptl->div_speed     = 0.95;
    ptl->direction     = vec3(-1,0,0);
    ptl->div_direction = 0.5;
    ptl->st_size       = .08f;
    ptl->en_size       = .02f;
    ptl->build();

    visual = new kgmVisual();
    mesh = new kgmMesh();
    Vertex* v = (Vertex*)mesh->vAlloc(18, kgmMesh::FVF_P_C_T);

    v[0]   = { {0, -0.1, -0.1}, 0xffffffff, {0, 1} };
    v[1]   = { {0, -0.1, 0.1},  0xffffffff, {0, 0} };
    v[2]   = { {0, 0.1, 0.1},   0xffffffff, {1, 0} };
    v[3]   = { {0, 0.1, 0.1},   0xffffffff, {1, 0} };
    v[4]   = { {0, 0.1, -0.1},  0xffffffff, {1, 1} };
    v[5]   = { {0, -0.1, -0.1}, 0xffffffff, {0, 1} };

    v[6]   = { {0, -0.1, 0},  0xffffffff, {0.5, 1} };
    v[7]   = { {0,  0.1, 0},  0xffffffff, {0.5, 0} };
    v[8]   = { {-0.5,  0.1, 0}, 0xffffffff, {1, 0} };
    v[9]   = { {-0.5, 0.1, 0},  0xffffffff, {1, 0} };
    v[10]  = { {-0.5, -0.1, 0}, 0xffffffff, {1, 1} };
    v[11]  = { {0, -0.1, 0},  0xffffffff, {0.5, 1} };

    v[12]  = { {0, 0, -0.1},  0xffffffff, {0.5, 1} };
    v[13]  = { {0, 0, 0.1},   0xffffffff, {0.5, 0} };
    v[14]  = { {-0.5, 0, 0.1},  0xffffffff, {1, 0} };
    v[15]  = { {-0.5, 0, 0.1},  0xffffffff, {1, 0} };
    v[16]  = { {-0.5, 0, -0.1}, 0xffffffff, {1, 1} };
    v[17]  = { {0, 0, -0.1},  0xffffffff, {0.5, 1} };

    m_visual->set(mesh);
    m_visual->set(mtl);
    visual->set(ptl);
    visual->set(mtl);
  }

  virtual ~kgmIonNozzle()
  {
    mesh->release();
    mtl->release();
    ptl->release();
    visual->release();
  }

  void update(u32 mls)
  {
    if(!valid())
      return;

    visual->m_transform = m_visual->m_transform;
    visual->update();
  }

  void event(kgmObject* o, kgmString arg)
  {
    if(arg == "die" && o == m_parent)
    {
      visual->disable();
      visual->remove();
      remove();
    }
  }

  void setSpeed(float speed)
  {
    ptl->m_speed = 2.0 + 25 * speed;
  }
};

#endif // KGMOBJECTS_H
