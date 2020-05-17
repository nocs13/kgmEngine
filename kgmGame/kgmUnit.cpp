#include "kgmUnit.h"
#include "kgmIGame.h"

kgmTab<kgmString, kgmUnit*(*)(kgmIGame*)> kgmUnit::g_typ_objects;

kgmTab<kgmString, kgmUnit::ActionCallback> kgmUnit::g_list_action;

kgmUnit::kgmUnit(kgmIGame* g)
{
  m_type = Unit;

  m_game = g;

  m_valid   = true;
  m_remove  = false;
  m_culled  = false;
  m_visible = true;

  m_position   = vec3(0, 0, 0);
  m_rotation   = vec3(0, 0, 0);
  m_quaternion = quat(0, 0, 0, 1);

  m_birth   = kgmTime::getTicks();
  m_living  = -1;

  m_name  = "None";
  m_class = "None";
}

kgmUnit::kgmUnit(kgmIGame* g, kgmIMesh* msh)
  :kgmUnit(g)
{
  m_type = Mesh;

  m_game = g;

  m_mesh = msh;
  m_vobject = msh;

  m_valid   = true;
  m_remove  = false;
  m_culled  = false;
  m_visible = true;

  m_position   = vec3(0, 0, 0);
  m_rotation   = vec3(0, 0, 0);
  m_quaternion = quat(0, 0, 0, 1);

  m_birth   = kgmTime::getTicks();
  m_living  = -1;
}

kgmUnit::kgmUnit(kgmIGame* g, kgmLight* lgt)
  :kgmUnit(g)
{
  m_type = Light;

  m_game = g;

  m_light = lgt;

  m_valid   = true;
  m_remove  = false;
  m_culled  = false;
  m_visible = true;

  m_position   = vec3(0, 0, 0);
  m_rotation   = vec3(0, 0, 0);
  m_quaternion = quat(0, 0, 0, 1);

  m_birth   = kgmTime::getTicks();
  m_living  = -1;
}

kgmUnit::kgmUnit(kgmIGame* g, kgmCamera* cam)
  :kgmUnit(g)
{
  m_type = Camera;

  m_game = g;

  m_camera = cam;

  m_valid   = true;
  m_remove  = false;
  m_culled  = false;
  m_visible = true;

  m_position   = vec3(0, 0, 0);
  m_rotation   = vec3(0, 0, 0);
  m_quaternion = quat(0, 0, 0, 1);

  m_birth   = kgmTime::getTicks();
  m_living  = -1;
}

kgmUnit::kgmUnit(kgmIGame* g, kgmTerrain* ter)
  :kgmUnit(g)
{
  m_type = Terrain;

  m_game = g;

  m_terrain = ter;

  m_valid   = true;
  m_remove  = false;
  m_culled  = false;
  m_visible = true;

  m_position   = vec3(0, 0, 0);
  m_rotation   = vec3(0, 0, 0);
  m_quaternion = quat(0, 0, 0, 1);

  m_birth   = kgmTime::getTicks();
  m_living  = -1;
}

kgmUnit::kgmUnit(kgmIGame* g, kgmObstacle* obs)
  :kgmUnit(g)
{
  m_type = Obstacle;

  m_game = g;

  m_obstacle = obs;

  m_valid   = true;
  m_remove  = false;
  m_culled  = false;
  m_visible = true;

  m_position   = vec3(0, 0, 0);
  m_rotation   = vec3(0, 0, 0);
  m_quaternion = quat(0, 0, 0, 1);

  m_birth   = kgmTime::getTicks();
  m_living  = -1;
}

kgmUnit::kgmUnit(kgmIGame* g, kgmParticles* par)
  :kgmUnit(g)
{
  m_type = Particles;

  m_game = g;

  m_particles = par;

  m_valid   = true;
  m_remove  = false;
  m_culled  = false;
  m_visible = true;

  m_position   = vec3(0, 0, 0);
  m_rotation   = vec3(0, 0, 0);
  m_quaternion = quat(0, 0, 0, 1);

  m_birth   = kgmTime::getTicks();
  m_living  = -1;
}

kgmUnit::~kgmUnit()
{
  if(m_remove)
    remove();
}

void kgmUnit::remove()
{
  clear();

  m_remove  = true;
  m_valid   = false;
  m_visible = false;
}

u32 kgmUnit::timeout()
{
  return (kgmTime::getTicks() - m_birth);
}

void kgmUnit::update(u32 ms)
{
  if(removed())
    return;

  if((getLiving() != (u32) -1) && (timeout() > getLiving()))
  {
    remove();

    return;
  }

  if(m_action.id.length() > 0)
  {
    if(!m_action.callback)
      m_action.callback = getActionCallback(m_action.id);

    if(m_action.callback)
      m_action.callback(game(), this, &m_action);
  }

  if(null)
  {
    mtx4 tm;

    //if(m_body)
    //{
    //  m_body->transform(tm);
    //}
    //else
    {
      mtx4 rt, tr, sc;

      tr.identity();
      tr.translate(m_position);
      rt.identity();
      rt.rotate(m_rotation);
      sc.identity();
      sc.scale(m_scale);

      tm = rt * tr * sc;
    }
  }

  if (m_type == Particles && m_particles)
    m_particles->update(ms);
}

void kgmUnit::init()
{
  if (m_type == Particles && m_particles)
    m_particles->build();
}

void* kgmUnit::getNodeObject()
{
  return m_object;
}

kgmIGraphics::TypeNode kgmUnit::getNodeType()
{
  switch(m_type)
  {
  case Mesh:
    return kgmIGraphics::NodeMesh;
  case  Light:
    return kgmIGraphics::NodeLight;
  case  Terrain:
    return kgmIGraphics::NodeTerrain;
  case  Particles:
    return kgmIGraphics::NodeParticles;
  };

  return kgmIGraphics::NodeNone;
}

bool kgmUnit::isNodeValid()
{
  return true;
}

box3 kgmUnit::getNodeBound()
{
  return box3();
}

vec3 kgmUnit::getNodePosition()
{
  return m_position;
}

mtx4 kgmUnit::getNodeTransform()
{
  return m_transform;
}

kgmMaterial* kgmUnit::getNodeMaterial()
{
  return m_material;
}

void kgmUnit::setNodeMaterial(kgmMaterial* m)
{
  m_material = m;
}

