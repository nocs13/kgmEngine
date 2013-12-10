#ifndef ASPACEROBJECTS_H
#define ASPACEROBJECTS_H

//#include "../kgmGame/kgmGameObject.h"

struct ASp_Vertex
{
  vec3 v;
  u32  c;
  vec2 uv;
};

struct ASp_Face
{
  u16 a, b, c;
};

class ASp_Skybox: public kgmGameObject
{
  KGM_OBJECT(ASp_Skybox);

  kgmIGame* game;

public:
  ASp_Skybox(kgmIGame* g)
  {
    game = g;
    kgmMaterial*  mtl  = new kgmMaterial();

    mtl->m_type        = "simple";
    mtl->m_shader      = kgmMaterial::ShaderNone;
    mtl->m_2side       = true;

    kgmMesh* mesh = new kgmMesh();
    mesh->m_rtype = kgmMesh::RT_POINT;

    kgmMesh::Vertex_P_C* pc = (kgmMesh::Vertex_P_C*)mesh->vAlloc(2000, kgmMesh::FVF_P_C);

    for(int i = 0; i < 1000; i++)
    {
      vec3 v(pow(-1.0, rand()%2) * rand(), pow(-1.0, rand()%2) * rand(), pow(-1.0, rand()%2) * rand());
      v.normalize();
      v = v * 10;
      v.z *= 0.41;

      pc[i].pos = v;
      pc[i].col = 0xffffffaa;
    }

    m_visual = new kgmVisual();
    m_visual->set(mesh);

    mesh->release();
    mtl->release();
  }

  ~ASp_Skybox()
  {

  }

  virtual void update(u32 ms)
  {
    kgmGameObject::update(ms);

    if(game)
    {
      kgmCamera& cam = ((kgmGameBase*)game)->m_render->camera();
      mtx4 m, msc;

      msc.scale(80, 80, 80);
      m.translate(cam.mPos);
      m_visual->m_transform = msc * m;
    }
  }
};

class ASp_MotorA: public kgmGameObject
{
  KGM_OBJECT(ASp_MotorA);
  kgmIGame*     game;
  kgmMesh*      mesh;
  kgmMaterial*  mtl;
public:
  ASp_MotorA(kgmIGame* g)
  {
    game     = g;
    m_visual = new kgmVisual();

    mtl = new kgmMaterial();
    mtl->m_2side = true;

    mtl->m_blend = true;
    mtl->m_srcblend = gcblend_srcalpha;
    mtl->m_dstblend = gcblend_srcialpha;
    mtl->m_tex_color = g->getResources()->getTexture("point_a.tga");
    mtl->m_type = "simple";
    mtl->m_shader = kgmMaterial::ShaderNone;

    mesh = new kgmMesh();
    ASp_Vertex* v = (ASp_Vertex*)mesh->vAlloc(18, kgmMesh::FVF_P_C_T);

    v[0]   = { {0, -0.1, -0.1}, 0xffffffff, {0, 1}};
    v[1]   = { {0, -0.1, 0.1},  0xffffffff, {0, 0}};
    v[2]   = { {0, 0.1, 0.1},   0xffffffff, {1, 0}};
    v[3]   = { {0, 0.1, 0.1},   0xffffffff, {1, 0}};
    v[4]   = { {0, 0.1, -0.1},  0xffffffff, {1, 1}};
    v[5]   = { {0, -0.1, -0.1}, 0xffffffff, {0, 1}};

    v[6]   = { {0, -0.1, 0},  0xffffffff, {0, 1}};
    v[7]   = { {0,  0.1, 0},  0xffffffff, {0, 0}};
    v[8]   = { {-1,  0.1, 0}, 0xffffffff, {1, 0}};
    v[9]   = { {-1, 0.1, 0},  0xffffffff, {1, 0}};
    v[10]  = { {-1, -0.1, 0}, 0xffffffff, {1, 1}};
    v[11]  = { {0, -0.1, 0},  0xffffffff, {0, 1}};

    v[12]  = { {0, 0, -0.1},  0xffffffff, {0, 1}};
    v[13]  = { {0, 0, 0.1},   0xffffffff, {0, 0}};
    v[14]  = { {-1, 0, 0.1},  0xffffffff, {1, 0}};
    v[15]  = { {-1, 0, 0.1},  0xffffffff, {1, 0}};
    v[16]  = { {-1, 0, -0.1}, 0xffffffff, {1, 1}};
    v[17]  = { {0, 0, -0.1},  0xffffffff, {0, 1}};

    m_visual->set(mesh);
    m_visual->set(mtl);
  }

  ~ASp_MotorA()
  {
    mesh->release();
    mtl->release();
  }

  void update(u32 mls)
  {
    //kgmGameObject::update(mls);
  }

  void event(kgmObject* o, kgmString arg)
  {
    if(arg == "die" && o == m_parent)
    {
      remove();
    }
  }
};

class ASp_Laser: public kgmGameObject
{
  KGM_OBJECT(ASp_Laser);

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
  ASp_Laser(kgmIGame* g, u32 time, vec3 pos, vec3 rot, float speed, float len=0.2)
  {
    power = 1;
    timeout(time);
    this->setId("laser");
    m_visual  = new kgmVisual();

    mtl = new kgmMaterial();
    mtl->m_2side = true;

    mtl->m_blend = true;
    mtl->m_srcblend = gcblend_srcalpha;
    mtl->m_dstblend = gcblend_srcialpha;
    mtl->m_tex_color = g->getResources()->getTexture("point_a.tga");
    mtl->m_shader = kgmMaterial::ShaderNone;

    mesh = new kgmMesh();
    Vertex* v = (Vertex*)mesh->vAlloc(18, kgmMesh::FVF_P_C_T);

    v[0]  = { {0, -len, -len}, 0xffffffff, {0, 1}};
    v[1]  = { {0, -len, len},  0xffffffff, {0, 0}};
    v[2]  = { {0, len, len},   0xffffffff, {1, 0}};
    v[3]  = { {0, len, len},   0xffffffff, {1, 0}};
    v[4]  = { {0, len, -len},  0xffffffff, {1, 1}};
    v[5]  = { {0, -len, -len}, 0xffffffff, {0, 1}};

    v[6]  = { {len, -len, 0},  0xffffffff, {0, 1}};
    v[7]  = { {len,  len, 0},  0xffffffff, {0, 0}};
    v[8]  = { {-len,  len, 0}, 0xffffffff, {1, 0}};
    v[9]  = { {-len, len, 0},  0xffffffff, {1, 0}};
    v[10] = { {-len, -len, 0}, 0xffffffff, {1, 1}};
    v[11] = { {len, -len, 0},  0xffffffff, {0, 1}};

    v[12] = { {len, 0, -len},  0xffffffff, {0, 1}};
    v[13] = { {len, 0, len},   0xffffffff, {0, 0}};
    v[14] = { {-len, 0, len},  0xffffffff, {1, 0}};
    v[15] = { {-len, 0, len},  0xffffffff, {1, 0}};
    v[16] = { {-len, 0, -len}, 0xffffffff, {1, 1}};
    v[17] = { {len, 0, -len},  0xffffffff, {0, 1}};

    m_visual->set(mesh);
    m_visual->set(mtl);
    mesh->release();
    mtl->release();

    m_body = new kgmBody();

    //body->m_collision = false;
    m_body->translate(pos);
    m_body->rotate(rot);
    m_body->m_velocity  = speed;
    m_body->m_gravity   = false;
    m_body->m_udata     = this;
    m_body->m_mass      = 0.0f;
    m_body->m_bound.min = vec3(-len, -len, -len);
    m_body->m_bound.max = vec3( len,  len,  len);
  }

  ~ASp_Laser()
  {
  }

  void update(u32 t)
  {
    kgmGameObject::update(t);
  }
};

class ASp_LaserA: public ASp_Laser
{
  KGM_OBJECT(ASp_LaserA);

public:
  ASp_LaserA(kgmIGame* g, u32 time, vec3 pos, vec3 rot, float speed, float len=0.2)
    :ASp_Laser(g, time, pos, rot, speed, len)
  {
  }
};

class ASp_LaserB: public ASp_Laser
{
  KGM_OBJECT(ASp_LaserB);

public:
  ASp_LaserB(kgmIGame* g, u32 time, vec3 pos, vec3 rot, float speed, float len=0.5)
    :ASp_Laser(g, time, pos, rot, speed, len)
  {
    mtl->m_tex_color = g->getResources()->getTexture("point_c.tga");
  }
};

class ASp_Asteroid: public kgmGameObject
{
  KGM_OBJECT(ASp_Asteroid);

  kgmMesh* msh;
  kgmMaterial* mtl;

public:
  ASp_Asteroid(kgmIGame* g, u32 type)
  {
    m_visual = new kgmVisual();
    msh = g->getResources()->getMesh("kasteroid_o2.kgm");
    mtl = new kgmMaterial();
    mtl->m_shader = kgmMaterial::ShaderBase;
    mtl->m_tex_color = g->getResources()->getTexture("asteroid_0.tga");

    m_visual->set(msh);
    m_visual->set(mtl);

    m_body   = new kgmBody();
    m_body->m_udata = this;
    m_body->m_gravity = false;
    m_body->m_velocity = 0.01 + 0.02 * 1.0f / (1 + rand()%30);
    //m_body->m_direction = vec3((float)pow(-1, rand() % 2) / (1 + rand()%30),
    //                           (float)pow(-1, rand() % 2) / (1 + rand()%30),
                               //(float)pow(-1, rand() % 2) / (1 + rand()%30)
    //                           0);
    //m_body->m_direction.normalize();
    m_body->m_bound.min = vec3(-1, -1, -1);
    m_body->m_bound.max = vec3( 1,  1,  1);
  }

  ~ASp_Asteroid()
  {
    msh->release();
    mtl->release();
  }

  void update(u32 t)
  {
    kgmGameObject::update(t);
  }
};

class ASp_AsteroidSpawner: public kgmGameObject
{
  KGM_OBJECT(ASp_AsteroidSpawner);

  kgmIGame*  game;
  u32 m_time_prev;
public:
  ASp_AsteroidSpawner(kgmIGame* g)
  {
    game        = g;
    m_time_prev = kgmTime::getTicks();

    m_body            = new kgmBody;
    m_body->m_gravity = false;
  }

  void update(u32 ms)
  {
    kgmGameObject::update(ms);
    return;

    u32 ctick = kgmTime::getTicks();

    if((ctick - m_time_prev > 5000) /*&& (m_childs.size() < 50)*/)
    {
      ASp_Asteroid* as = new ASp_Asteroid(game, 0);
      as->setId("asteroid");

      as->timeout(10000 + rand() % 10000);
      as->setPosition(m_body->m_position);
      as->setParent(this);
      game->gAppend(as);
      as->release();

      m_time_prev = kgmTime::getTicks();
    }
  }
};

class ASp_SpacerSpawner: public kgmGameObject
{
  KGM_OBJECT(ASp_SpacerSpawner);

public:
  ASp_SpacerSpawner(kgmIGame* g)
  {

  }
};

class ASp_Flame: public kgmGameObject
{
  KGM_OBJECT(ASp_Flame);

protected:
  kgmParticles* particles;
  kgmMaterial*  material;
public:
  ASp_Flame(kgmIGame* g)
  {
    particles = new kgmParticles();
    m_visual  = new kgmVisual();

    material = new kgmMaterial();
    material->m_blend        = true;
    material->m_srcblend     = gcblend_srcalpha;
    material->m_dstblend     = gcblend_one;
    material->m_type         = "simple";
    material->m_shader       = kgmMaterial::ShaderNone;
    material->m_tex_color    = g->getResources()->getTexture("fire_a.tga");

    particles->m_count   = 10;
    particles->m_speed   = 1.1;
    particles->div_speed = 1.0;
    particles->m_life    = 2000;
    particles->div_life  = 1.0;
    particles->build();
    particles->set(material);
    m_visual->set(particles);
  }

  virtual ~ASp_Flame()
  {
    if(particles)
      particles->release();

    if(material)
      material->release();
  }
};

class ASp_FlameA: public ASp_Flame
{
  KGM_OBJECT(ASp_FlameA);
public:
  ASp_FlameA(kgmIGame* g)
    :ASp_Flame(g)
  {

  }
};

class ASp_Smoke: public kgmGameObject
{
  KGM_OBJECT(ASp_Smoke);

protected:
  kgmParticles* particles;
  kgmMaterial*  material;
public:
  ASp_Smoke(kgmIGame* g)
  {
    particles = new kgmParticles();
    m_visual  = new kgmVisual();

    material = new kgmMaterial();
    material->m_blend        = true;
    material->m_srcblend     = gcblend_srcalpha;
    material->m_dstblend     = gcblend_one;
    material->m_type         = "simple";
    material->m_shader       = kgmMaterial::ShaderNone;
    material->m_tex_color    = g->getResources()->getTexture("smoke_a.tga");

    particles->m_speed = .01;
    particles->m_count = 10;
    particles->m_life  = 5000;
    particles->div_speed = .5;
    particles->build();
    particles->set(material);
    m_visual->set(particles);
  }

  virtual ~ASp_Smoke()
  {
    if(particles)
      particles->release();

    if(material)
      material->release();
  }
};

class ASp_SmokeA: public ASp_Smoke
{
  KGM_OBJECT(ASp_SmokeA);
public:
  ASp_SmokeA(kgmIGame* g)
    :ASp_Smoke(g)
  {

  }
};

class ASp_Explode: public kgmGameObject
{
  KGM_OBJECT(ASp_Explode);

protected:
  kgmParticles* particles;
  kgmMaterial*  material;

public:
  ASp_Explode(kgmIGame* g, vec3 pos, vec3 vol, kgmString tid = "fire_a.tga")
  {
    timeout(3000);

    particles = new kgmParticles();
    m_visual  = new kgmVisual();

    material = new kgmMaterial();
    material->m_blend        = true;
    material->m_srcblend     = gcblend_srcalpha;
    material->m_dstblend     = gcblend_one;
    material->m_type         = "simple";
    material->m_shader       = kgmMaterial::ShaderNone;
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
    particles->set(material);
    m_visual->set(particles);

    setPosition(pos);
  }

  virtual ~ASp_Explode()
  {
    if(particles)
      particles->release();

    if(material)
      material->release();
  }
};

class ASp_ExplodeA: public ASp_Explode
{
  KGM_OBJECT(ASp_ExplodeA);
public:
  ASp_ExplodeA(kgmIGame* g, vec3 pos, vec3 vol)
    :ASp_Explode(g, pos, vol)
  {
  }
};

class ASp_ExplodeB: public ASp_Explode
{
  KGM_OBJECT(ASp_ExplodeB);
public:
  ASp_ExplodeB(kgmIGame* g, vec3 pos, vec3 vol)
    :ASp_Explode(g, pos, vol)
  {
    material->m_tex_color  = g->getResources()->getTexture("smoke_a.tga");
    particles->m_count     = 10;
    particles->build();
  }
};

class ASp_ExplodeC: public ASp_Explode
{
  KGM_OBJECT(ASp_ExplodeC);
public:
  ASp_ExplodeC(kgmIGame* g, vec3 pos, vec3 vol)
    :ASp_Explode(g, pos, vol)
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

class ASp_Result: public kgmGameObject
{
  KGM_OBJECT(ASp_Result);

  kgmIGame* game;
  kgmString text;
  s32       result;

public:
  ASp_Result(kgmIGame* g, u32 time, s32 res, kgmString txt)
  {
    game = g;
    text = txt;
    result = res;
    timeout(time);

    int cx, cy, cw, ch, tw, tl;
    game->getWindow()->getRect(cx, cy, cw, ch);

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

  ~ASp_Result()
  {
    int k=0;
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
    game->getLogic()->action(kgmILogic::ACTION_GAMEOBJECT, this, "result");    
  }
};

#endif // ASPACEROBJECTS_H
