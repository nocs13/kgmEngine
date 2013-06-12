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
public:
  ASp_Skybox(kgmIGame* game)
  {
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

    if(m_parent && m_parent->getBody())
    {
      mtx4 m, msc;

      msc.scale(50, 50, 50);
      m.translate(m_parent->getBody()->m_position);
      m_visual->m_transform = msc * m;
    }
  }
};

class ASp_MotorA: public kgmGameObject
{
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
  struct Vertex
  {
    vec3 v;
    vec2 uv;
  };

  kgmMesh*      mesh;
  kgmMaterial*  mtl;

public:
  ASp_LaserA(kgmIGame* g, u32 time, vec3& pos, vec3& dir)
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

    kgmBody* body = new kgmBody();

    body->m_collision = false;
    body->m_position  = pos;
    body->m_direction = dir;
    body->m_velocity  = 0.1f;
    body->m_gravity   = false;

    m_body = body;
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

};

class ASp_AsteroidSpawner: public kgmGameObject
{

};

class ASp_SpacerSpawner: public kgmGameObject
{

};

#endif // ASPACEROBJECTS_H
