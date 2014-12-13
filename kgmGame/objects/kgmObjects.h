#ifndef KGMOBJECTS_H
#define KGMOBJECTS_H

#include "../kgmIGame.h"
#include "../kgmActor.h"
#include "../kgmGameResources.h"
#include "../../kgmGraphics/kgmMaterial.h"

class kgmResult: public kgmActor
{
  KGM_GO_OBJECT(kgmResult);
public:

  kgmIGame* game;
  kgmString text;
  s32       result;

public:
  kgmResult(kgmIGame* g, u32 time = 0, s32 res = 0, kgmString txt = "")
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
  }
};

class kgmParticlesObject: public kgmActor
{
  KGM_GO_OBJECT(kgmParticlesObject);

private:
  kgmIGame* game;

protected:
  kgmParticles* particles;
  kgmMaterial*  material;

  //For editor only

  kgmString     idTex;

public:
  kgmParticlesObject(kgmIGame* g,
               vec3 pos = vec3(0, 0, 0), vec3 vol = vec3(1, 1, 1), vec3 dir = vec3(0, 0, 0),
               float speed = 0.0f, float div_speed = 0.0,
               float life = 1000,  float div_life = 0.5f,
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
    material->setShader(g->getResources()->getShader("blend"));
    material->setTexColor(g->getResources()->getTexture(tid));

    particles->direction = dir;
    particles->volume    = vol;
    particles->m_speed   = speed;
    particles->m_count   = count;
    particles->m_life    = life;
    particles->m_loop    = loop;
    particles->st_size   = size_start;
    particles->en_size   = size_end;
    particles->div_life  = div_life;

    m_visual->set(material);
    m_visual->set(particles);

    setPosition(pos);

    kgmVariable var;

    var = kgmVariable("Count",  0,     &particles->m_count);
    m_variables.add(var);
    var = kgmVariable("Speed",  0.0f,   &particles->m_speed);
    m_variables.add(var);
    var = kgmVariable("dSpeed", 0.0f,   &particles->div_speed);
    m_variables.add(var);
    var = kgmVariable("Life",   0.0f,     &particles->m_life);
    m_variables.add(var);
    var = kgmVariable("dLife",  0.0f,     &particles->div_life);
    m_variables.add(var);
    var = kgmVariable("Loop",   false, &particles->m_loop);
    m_variables.add(var);
    idTex = tid;
    var = kgmVariable("tId",   kgmString(""),     &idTex);
    m_variables.add(var);
  }

  kgmParticlesObject(kgmIGame* g, kgmParticles* pts, kgmMaterial* mtl, u32 life)
  {
    game = g;
    material = mtl;
    particles = pts;

    pts->increment();
    mtl->increment();

    timeout(life);
  }

  virtual ~kgmParticlesObject()
  {
    if(particles)
      particles->release();

    if(material)
      material->release();
  }

  void setTexture(kgmString tid)
  {
    material->setTexColor(game->getResources()->getTexture(tid));
  }

  void setSlideFrames(u32 rows, u32 cols)
  {
    particles->tex_slide_rows = rows;
    particles->tex_slide_cols = cols;
  }

  void init()
  {
    particles->build();
  }

#ifdef EDITOR
  void eupdate()
  {
    material->setTexColor(game->getResources()->getTexture(idTex));
    particles->build();
  }
#endif
};

class kgmFlame: public kgmParticlesObject
{
  KGM_GO_OBJECT(kgmFlame);

public:
  kgmFlame(kgmIGame* g,
               vec3 pos = vec3(0, 0, 0), vec3 vol = vec3(1, 1, 1), vec3 dir = vec3(0, 0, 0),
               float speed = 0.0f, float div_speed = 0.0,
               float life = 1000,  float div_life = 0.5f,
               float size_start = .1f, float size_end = 1.0f,
               u32 count = 1,
               kgmString tid = "flame_a.tga",
               bool loop = true)
    :kgmParticlesObject(g, pos, vol, dir, speed, div_speed, life, div_life, size_start, size_end, count, tid, loop)
  {
  }

  virtual ~kgmFlame()
  {
  }
};

class kgmSmoke: public kgmParticlesObject
{
  KGM_GO_OBJECT(kgmSmoke);

protected:
  kgmParticles* particles;
  kgmMaterial*  material;
public:
  kgmSmoke(kgmIGame* g,
           vec3 pos = vec3(0, 0, 0), vec3 vol = vec3(1, 1, 1), vec3 dir = vec3(0, 0, 0),
           float speed = 0.01f, float div_speed = 0.5,
           float life = 5000,  float div_life = 0.5f,
           float size_start = .1f, float size_end = 1.0f,
           u32 count = 10,
           kgmString tid = "smoke_a.tga",
           bool loop = false)
    :kgmParticlesObject(g, pos, vol, dir, speed, div_speed, life, div_life, size_start, size_end, count, tid, loop)
  {
    material->m_blend        = true;
    material->m_srcblend     = gcblend_srcalpha;
    material->m_dstblend     = gcblend_one;
    material->m_type         = "simple";
    material->setShader(g->getResources()->getShader("blend"));
  }

  virtual ~kgmSmoke()
  {
  }
};

class kgmExplode: public kgmParticlesObject
{
  KGM_GO_OBJECT(kgmExplode);

private:
  kgmIGame* game;

public:
  kgmExplode(kgmIGame* g,
             vec3 pos = vec3(0, 0, 0), vec3 vol = vec3(1, 1, 1), vec3 dir = vec3(0, 0, 0),
             float speed = 1.0f, float div_speed = 0.1f, float life = 1000, float div_life = 0.2f,
             float size_start = .1f, float size_end = 2.0f,
             u32 count = 10,
             bool loop = false,
             kgmString tid="fire_a.tga")
    :kgmParticlesObject(g, pos, vol, dir, speed, div_speed, life, div_life, size_start, size_end, count, tid, loop)
  {
    timeout(life);

    particles->div_life  = 0.8;

    particles->build();

    m_visual->set(material);
    m_visual->set(particles);

    setPosition(pos);
  }

  virtual ~kgmExplode()
  {
  }

  void setTexture(kgmString tid)
  {
    material->setTexColor(game->getResources()->getTexture(tid));
  }

  void setSlideFrames(u32 rows, u32 cols)
  {
    particles->tex_slide_rows = rows;
    particles->tex_slide_cols = cols;
  }
};

class kgmLaser: public kgmActor
{
  KGM_GO_OBJECT(kgmLaser);

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
  kgmLaser(kgmIGame* g = null)
    :kgmActor(g)
  {

  }

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
    mtl->setTexColor(g->getResources()->getTexture((char*)"point_redd.tga"));
    mtl->setShader(g->getResources()->getShader("blend"));

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

class kgmIonNozzle: public kgmActor
{
  KGM_GO_OBJECT(kgmIonNozzle);

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
    mtl->setTexColor(g->getResources()->getTexture((char*)"point_d.tga"));
    mtl->m_type = "simple";
    mtl->setShader(g->getResources()->getShader("blend"));

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

    visual->getTransform() = m_visual->getTransform();
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
