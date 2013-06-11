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
    kgmMesh*      mesh = new kgmMesh();
    kgmMaterial*  mtl  = new kgmMaterial();

    mtl->m_type        = "simple";
    mtl->m_2side       = true;
    mtl->m_tex_color   = game->getResources()->getTexture("sky_a.tga");

    ASp_Vertex* v = (ASp_Vertex*)mesh->vAlloc(36, kgmMesh::FVF_P_T);
    v[0]   = { {-100, -100, -100}, {0, 1}};
    v[1]   = { { 100, -100, -100}, {1, 1}};
    v[2]   = { { 100,  100, -100}, {1, 0}};
    v[3]   = { {-100,  100, -100}, {1, 0}};
    v[4]   = { {-100, -100,  100}, {0, 0}};
    v[5]   = { { 100, -100,  100}, {1, 0}};
    v[6]   = { { 100,  100,  100}, {1, 0}};
    v[7]   = { {-100,  100,  100}, {1, 0}};
    /*ASp_Face*   f = (ASp_Face*)mesh->fAlloc(12, kgmMesh::FFF_16);
    f[0]   = {0, 1, 2};
    f[1]   = {2, 3, 0};
    f[2]   = {4, 5, 6};
    f[3]   = {6, 7, 4};
    f[4]   = {0, 3, 7};
    f[5]   = {7, 4, 0};
    f[6]   = {1, 5, 6};
    f[7]   = {6, 2, 1};
    f[8]   = {0, 4, 5};
    f[9]   = {5, 1, 0};
    f[10]  = {2, 6, 7};
    f[11]  = {7, 3, 2};*/

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
      mtx4 m;

      m.translate(m_parent->getBody()->m_position);
      m_visual->m_transform = m;
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

    mesh = new kgmMesh();
    ASp_Vertex* v = (ASp_Vertex*)mesh->vAlloc(18, kgmMesh::FVF_P_T);

    v[0]   = { {0, -0.1, -0.1}, {0, 1}};
    v[1]   = { {0, -0.1, 0.1}, {0, 0}};
    v[2]   = { {0, 0.1, 0.1}, {1, 0}};
    v[3]   = { {0, 0.1, 0.1}, {1, 0}};
    v[4]   = { {0, 0.1, -0.1}, {1, 1}};
    v[5]   = { {0, -0.1, -0.1}, {0, 1}};

    v[6]   = { {0, -0.1, 0}, {0, 1}};
    v[7]   = { {0,  0.1, 0}, {0, 0}};
    v[8]   = { {-1,  0.1, 0}, {1, 0}};
    v[9]   = { {-1, 0.1, 0}, {1, 0}};
    v[10]  = { {-1, -0.1, 0}, {1, 1}};
    v[11]  = { {0, -0.1, 0}, {0, 1}};

    v[12]  = { {0, 0, -0.1}, {0, 0}};
    v[13]  = { {0, 0, 0.1}, {0, 0}};
    v[14]  = { {-1, 0, 0.1}, {0, 0}};
    v[15]  = { {-1, 0, 0.1}, {0, 0}};
    v[16]  = { {-1, 0, -0.1}, {0, 0}};
    v[17]  = { {0, 0, -0.1}, {0, 0}};


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

    mesh = new kgmMesh();
    Vertex* v = (Vertex*)mesh->vAlloc(12, kgmMesh::FVF_P_T);

    v[0]   = { {0, -0.2, -0.2}, {0, 1}};
    v[1]   = { {0, -0.2, 0.2},  {0, 0}};
    v[2]   = { {0, 0.2, 0.2},   {1, 0}};
    v[3]   = { {0, 0.2, 0.2},   {1, 0}};
    v[4]   = { {0, 0.2, -0.2},  {1, 1}};
    v[5]   = { {0, -0.2, -0.2}, {0, 1}};

    v[0]  = { {0, -0.2, 0},  {0, 1}};
    v[1]  = { {0,  0.2, 0},  {0, 0}};
    v[2]  = { {-0.4,  0.2, 0}, {1, 0}};
    v[3]  = { {-0.4, 0.2, 0},  {1, 0}};
    v[4]  = { {-0.4, -0.2, 0}, {1, 1}};
    v[5]  = { {0, -0.2, 0},  {0, 1}};

    v[6]  = { {0, 0, -0.2},  {0, 0}};
    v[7]  = { {0, 0, 0.2},   {0, 0}};
    v[8]  = { {-0.4, 0, 0.2},  {0, 0}};
    v[9]  = { {-0.4, 0, 0.2},  {0, 0}};
    v[10] = { {-0.4, 0, -0.2}, {0, 0}};
    v[11] = { {0, 0, -0.2},  {0, 0}};


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
    if(m_visual)
    {
      vec3 v(0, 0, 1);
      mtx4 m;
      //m.rotate(-0.5 * PI, v);
      //m_visual->m_transform = m * m_visual->m_transform;
    }
  }
};
#endif // ASPACEROBJECTS_H
