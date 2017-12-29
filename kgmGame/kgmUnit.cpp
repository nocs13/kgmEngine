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
}

kgmUnit::kgmUnit(kgmIGame* g, kgmMesh* msh)
{
  m_type = Mesh;

  m_game = g;

  m_object = msh;

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

kgmUnit::kgmUnit(kgmIGame* g, kgmVisual* vis)
{
  m_type = Visual;

  m_game = g;

  //m_visual = vis;

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

kgmUnit::~kgmUnit()
{
  if(m_body)
    delete m_body;

  //if(m_visual)
  //  delete m_visual;

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

void kgmUnit::update(u32 mls)
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

    if(m_body)
    {
      m_body->transform(tm);
    }
    else
    {
      mtx4 rt, tr;

      tr.identity();
      //tr.translate(m_position);
      rt.identity();
      //rt.rotate(m_rotation);

      tm = rt * tr;
    }

    //m_visual->getTransform() = tm;
    //m_visual->update();
  }
}

kgmObject* kgmUnit::getNodeObject()
{
  return m_object;
}

kgmIGraphics::TypeNode kgmUnit::getNodeType()
{
  if (m_type == Mesh)
    return kgmIGraphics::NodeMesh;

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

mtx4 kgmUnit::getNodeTransform()
{
  return mtx4();
}

kgmMaterial* kgmUnit::getNodeMaterial()
{
  return m_material;
}

void kgmUnit::setNodeMaterial(kgmMaterial*)
{

}

