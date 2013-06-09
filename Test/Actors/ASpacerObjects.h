#ifndef ASPACEROBJECTS_H
#define ASPACEROBJECTS_H

#include "../kgmGame/kgmGameObject.h"

class ASp_MotorA: public kgmGameObject
{
  struct Vertex
  {
    vec3 v;
    vec2 uv;
  };

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

    mesh = new kgmMesh();
    Vertex* v = (Vertex*)mesh->vAlloc(18, kgmMesh::FVF_P_T);

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
  ASp_LaserA(kgmIGame* g, u32 time, vec3 pos, vec3 dir)
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

    v[0]   = { {0, -0.1, -0.1}, {0, 1}};
    v[1]   = { {0, -0.1, 0.1},  {0, 0}};
    v[2]   = { {0, 0.1, 0.1},   {1, 0}};
    v[3]   = { {0, 0.1, 0.1},   {1, 0}};
    v[4]   = { {0, 0.1, -0.1},  {1, 1}};
    v[5]   = { {0, -0.1, -0.1}, {0, 1}};

    v[0]  = { {0, -0.1, 0},  {0, 1}};
    v[1]  = { {0,  0.1, 0},  {0, 0}};
    v[2]  = { {-1,  0.1, 0}, {1, 0}};
    v[3]  = { {-1, 0.1, 0},  {1, 0}};
    v[4]  = { {-1, -0.1, 0}, {1, 1}};
    v[5]  = { {0, -0.1, 0},  {0, 1}};

    v[6]  = { {0, 0, -0.1},  {0, 0}};
    v[7]  = { {0, 0, 0.1},   {0, 0}};
    v[8]  = { {-1, 0, 0.1},  {0, 0}};
    v[9]  = { {-1, 0, 0.1},  {0, 0}};
    v[10] = { {-1, 0, -0.1}, {0, 0}};
    v[11] = { {0, 0, -0.1},  {0, 0}};


    m_visual->addVisual(mesh, mtl);

    kgmBody* body = new kgmBody();

    body->m_collision = false;
    body->m_position  = pos;
    body->m_direction = dir;
    body->m_velocity  = 0.1f;

    m_body = body;
  }

  ~ASp_LaserA()
  {
    mesh->release();
    mtl->release();
  }
};
#endif // ASPACEROBJECTS_H
