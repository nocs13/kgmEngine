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
                               (float)pow(-1, rand() % 2) / (1 + rand()%30));
    m_body->m_direction.normalize();
    m_body->m_bound.min = vec3(-1, -1, -1);
    m_body->m_bound.max = vec3( 1,  1,  1);
  }

  ~ASp_Asteroid()
  {
    //msh->release();
    mtl->release();
  }

  void update(u32 t)
  {
    kgmGameObject::update(t);
  }
};

class ASp_AsteroidSpawner: public kgmActor
{
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

      as->timeout(10000 + rand() % 10000);
      as->setPosition(m_body->m_position);

      if(game->gAppend(as))
      {
        //addChild(as);
      }
      else
      {
        as->release();
      }

      m_time_prev = kgmTime::getTicks();
    }
  }
};

class ASp_Spacer: public kgmActor
{
protected:
  kgmIGame* game;
  float     speed_max;
  float     speed_min;
  float     roll;
  float     yaaw;

public:
  ASp_Spacer(kgmIGame* g)
  {
    game = g;

    roll = yaaw = 0.0f;

    m_body->m_gravity = false;
    m_body->m_bound.min = vec3(-1, -1, -1);
    m_body->m_bound.max = vec3( 1,  1,  1);
  }

  virtual ~ASp_Spacer()
  {

  }

  void update(u32 ms)
  {
    kgmActor::update(ms);

    if(m_visual)
    {
      vec3 vz(0, 0, 1), vy(0, 0, 0), vx(1, 0, 0);
      mtx4 mz, mx, my, mr;
      float pich = 0.5 * PI;

      mz.rotate(0.5 * PI, vz);
      mx.rotate(roll, vx);
      my.rotate(yaaw, vy);

      mr.rotate(-roll, yaaw, -pich);
      m_visual->m_transform = mr * m_visual->m_transform;

      for(int i = 0; i < m_dummies.length(); i++)
      {
        kgmDummy*       dm = m_dummies[i];
        kgmGameObject*  go = (kgmGameObject*)dm->m_linked;

        if(go && go->getVisual())
        {
          mtx4 m;
          vec3 v = dm->m_shift;

          m.identity();
          m.translate(v);

          go->getVisual()->m_transform = m * m_visual->m_transform;
        }
      }
    }

    if(m_state)
    {
      if(m_state->id == "idle")
      {
        if(m_body->m_velocity > speed_min)
        {
          setState("slow");
        }
      }
      else if(m_state->id == "left")
      {
        vec3 vt = m_body->m_rotation;
        vt.z += (0.02f);
        m_body->rotate(vt.x, vt.y, vt.z);

        if(roll > -PI/4)
          roll -= 0.02f;
      }
      else if(m_state->id == "right")
      {
        vec3 vt = m_body->m_rotation;
        vt.z -= (0.02f);
        m_body->rotate(vt.x, vt.y, vt.z);

        if(roll < PI/4)
          roll += 0.02f;
      }
      else if(m_state->id == "up")
      {
        if(yaaw < PI/6)
        {
          vec3 vt = m_body->m_rotation;
          vt.x    = yaaw;
          m_body->rotate(vt.x, vt.y, vt.z);

          yaaw += 0.02f;
        }
      }
      else if(m_state->id == "down")
      {
        if(yaaw > -PI/6)
        {
          vec3 vt = m_body->m_rotation;
          vt.x    = yaaw;
          m_body->rotate(vt.x, vt.y, vt.z);

          yaaw -= 0.02f;
        }
      }
      else if(m_state->id == "fast")
      {
        if(m_body->m_velocity < speed_max)
        {
          m_body->m_velocity += 0.001f;

          if(m_body->m_velocity > speed_max)
            m_body->m_velocity = speed_max;
        }
      }
      else if(m_state->id == "slow")
      {
        if(m_body->m_velocity > speed_min)
        {
          m_body->m_velocity -= 0.001f;

          if(m_body->m_velocity < speed_min)
            m_body->m_velocity = speed_min;
        }
        else if(m_body->m_velocity <= speed_min)
          setState(m_state->switchto);
      }
      else if(m_state->id == "correct")
      {
        if(roll > 0.0f)
          roll -= 0.02f;
        else
          roll += 0.02f;

        if(fabs(roll) < 0.05f)
          roll = 0.0f;

        if(yaaw > 0.0f)
          yaaw -= 0.02f;
        else
          yaaw += 0.02f;

        if(fabs(yaaw) < 0.05f)
          yaaw = 0.0f;

        if(yaaw == 0.0 && roll == 0.0)
        {
          setState("idle", true);
        }
      }
    }
  }
};

class ASp_SpacerSpawner: public kgmGameObject
{
public:
  ASp_SpacerSpawner(kgmIGame* g)
  {

  }
};

#endif // ASPACEROBJECTS_H
