#ifndef ASPACEROBJECTS_H
#define ASPACEROBJECTS_H

#include "../kgmGame/kgmGameObject.h"

struct ASp_Vertex
{
  vec3 v;
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
    mtl->m_tex_color   = game->getResources()->getTexture("sky_1.tga");

    kgmMesh* mesh = game->getResources()->getMesh("skybox_1.kgm");

    m_visual = new kgmVisual();
    m_visual->addVisual(mesh, mtl);

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

      msc.scale(50, 50, 50);
      m.translate(cam.mPos);
      m_visual->m_transform = msc * m;
    }
  }
};

class ASp_MotorA: public kgmGameObject
{
  KGM_OBJECT(ASp_MotorA);

  kgmMesh*      mesh;
  kgmMaterial*  mtl;
public:
  ASp_MotorA(kgmIGame* g)
  {
    m_visual = new kgmVisual();

    mtl = new kgmMaterial();
    mtl->m_2side = true;

    mtl->m_transparency = 0.5;
    mtl->m_alpha = 0.5;
    mtl->m_tex_color = g->getResources()->getTexture("point_a.tga");
    mtl->m_type = "simple";
    mtl->m_shader = kgmMaterial::ShaderNone;

    mesh = new kgmMesh();
    ASp_Vertex* v = (ASp_Vertex*)mesh->vAlloc(18, kgmMesh::FVF_P_T);

    v[0]   = { {0, -0.1, -0.1}, {0, 1}};
    v[1]   = { {0, -0.1, 0.1},  {0, 0}};
    v[2]   = { {0, 0.1, 0.1},   {1, 0}};
    v[3]   = { {0, 0.1, 0.1},   {1, 0}};
    v[4]   = { {0, 0.1, -0.1},  {1, 1}};
    v[5]   = { {0, -0.1, -0.1}, {0, 1}};

    v[6]   = { {0, -0.1, 0},  {0, 1}};
    v[7]   = { {0,  0.1, 0},  {0, 0}};
    v[8]   = { {-1,  0.1, 0}, {1, 0}};
    v[9]   = { {-1, 0.1, 0},  {1, 0}};
    v[10]  = { {-1, -0.1, 0}, {1, 1}};
    v[11]  = { {0, -0.1, 0},  {0, 1}};

    v[12]  = { {0, 0, -0.1},  {0, 1}};
    v[13]  = { {0, 0, 0.1},   {0, 0}};
    v[14]  = { {-1, 0, 0.1},  {1, 0}};
    v[15]  = { {-1, 0, 0.1},  {1, 0}};
    v[16]  = { {-1, 0, -0.1}, {1, 1}};
    v[17]  = { {0, 0, -0.1},  {0, 1}};

    m_visual->addVisual(mesh, mtl);
  }

  ~ASp_MotorA()
  {
    mesh->release();
    mtl->release();
  }
};

class ASp_LaserA: public kgmGameObject
{
  KGM_OBJECT(ASp_LaserA);

  struct Vertex
  {
    vec3 v;
    vec2 uv;
  };

  kgmMesh*      mesh;
  kgmMaterial*  mtl;

public:
  ASp_LaserA(kgmIGame* g, u32 time, vec3& pos, vec3& dir, float speed)
  {
    timeout(time);
    m_visual  = new kgmVisual();

    mtl = new kgmMaterial();
    mtl->m_2side = true;

    mtl->m_transparency = 0.5;
    mtl->m_alpha = 0.5;
    mtl->m_tex_color = g->getResources()->getTexture("point_a.tga");
    mtl->m_shader = kgmMaterial::ShaderNone;

    mesh = new kgmMesh();
    Vertex* v = (Vertex*)mesh->vAlloc(18, kgmMesh::FVF_P_T);

    v[0]  = { {0, -0.2, -0.2}, {0, 1}};
    v[1]  = { {0, -0.2, 0.2},  {0, 0}};
    v[2]  = { {0, 0.2, 0.2},   {1, 0}};
    v[3]  = { {0, 0.2, 0.2},   {1, 0}};
    v[4]  = { {0, 0.2, -0.2},  {1, 1}};
    v[5]  = { {0, -0.2, -0.2}, {0, 1}};

    v[6]  = { {0.2, -0.2, 0},  {0, 1}};
    v[7]  = { {0.2,  0.2, 0},  {0, 0}};
    v[8]  = { {-0.2,  0.2, 0}, {1, 0}};
    v[9]  = { {-0.2, 0.2, 0},  {1, 0}};
    v[10] = { {-0.2, -0.2, 0}, {1, 1}};
    v[11] = { {0.2, -0.2, 0},  {0, 1}};

    v[12] = { {0.2, 0, -0.2},  {0, 1}};
    v[13] = { {0.2, 0, 0.2},   {0, 0}};
    v[14] = { {-0.2, 0, 0.2},  {1, 0}};
    v[15] = { {-0.2, 0, 0.2},  {1, 0}};
    v[16] = { {-0.2, 0, -0.2}, {1, 1}};
    v[17] = { {0.2, 0, -0.2},  {0, 1}};

    m_visual->addVisual(mesh, mtl);

    m_body = new kgmBody();

    //body->m_collision = false;
    m_body->m_position  = pos;
    m_body->m_direction = dir;
    m_body->m_velocity  = speed;
    m_body->m_gravity   = false;
    m_body->m_udata     = this;
    m_body->m_bound.min = vec3(-.1, -.1, -.1);
    m_body->m_bound.max = vec3( .1,  .1,  .1);
  }

  ~ASp_LaserA()
  {
    mesh->release();
    mtl->release();
  }

  void update(u32 t)
  {
    kgmGameObject::update(t);
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

    m_visual->addVisual(msh, mtl);

    m_body   = new kgmBody();
    m_body->m_udata = this;
    m_body->m_gravity = false;
    m_body->m_velocity = 0.01 + 0.02 * 1.0f / (1 + rand()%30);
    m_body->m_direction = vec3((float)pow(-1, rand() % 2) / (1 + rand()%30),
                               (float)pow(-1, rand() % 2) / (1 + rand()%30),
                               //(float)pow(-1, rand() % 2) / (1 + rand()%30)
                               0);
    m_body->m_direction.normalize();
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

#endif // ASPACEROBJECTS_H
