#ifndef KGMGAMEOBJECT_H
#define KGMGAMEOBJECT_H

#include "../kgmBase/kgmTab.h"
#include "../kgmBase/kgmObject.h"
#include "../kgmBase/kgmVariable.h"
#include "../kgmPhysics/kgmBody.h"
#include "../kgmPhysics/kgmObstacle.h"
#include "../kgmGraphics/kgmVisual.h"

#include "../kgmGraphics/kgmIGraphics.h"
#include "../kgmPhysics/kgmIPhysics.h"

class kgmIGame;
class kgmSound;
class kgmTerrain;

class kgmUnit : public kgmIGraphics::INode, public kgmObject, public kgmIPhysics::IBody
{
  KGM_OBJECT(kgmUnit);

public:
  enum UnitType
  {
    Unit,
    Mesh,
    Light,
    Actor,
    Camera,
    Effect,
    Sensor,
    Trigger,
    Terrain,
    Obstacle,
    Particles
  };

  struct Action;

  typedef void (*ActionCallback)(kgmIGame*, kgmUnit*, Action*);

  struct Action
  {
    kgmString id;
    u32       time;
    u32       state;

    kgmList<kgmVariable> variables;

    ActionCallback callback = null;
  };

private:
  kgmIGame* m_game = null;

  u32       m_id;
  kgmString m_name;

  bool m_lock;
  bool m_valid;
  bool m_remove;
  bool m_culled;
  bool m_visible;
  bool m_gravity;

  u32 m_group;
  u32 m_birth;
  u32 m_living;

  vec3 m_scale;
  vec3 m_position;
  vec3 m_rotation;
  quat m_quaternion;

  mtx4 m_transform;

  f32 m_mass;

  box m_bound;

protected:
  union
  {
    kgmObject*   m_object = null;

    kgmMesh*      m_mesh;
    kgmLight*     m_light;
    kgmCamera*    m_camera;
    kgmTerrain*   m_terrain;
    kgmObstacle*  m_obstacle;
    kgmParticles* m_particles;
  };

  //kgmBody* m_body   = null;

  Action m_action;

  static kgmTab<kgmString, ActionCallback> g_list_action;

  kgmMaterial* m_material = null;

  UnitType m_type;

public:
  typedef kgmUnit* (*Generate)(kgmIGame*);

  static kgmTab<kgmString, Generate> g_typ_objects;

  kgmList<kgmVariable> m_variables;

public:
  kgmUnit(kgmIGame* g = null);
  kgmUnit(kgmIGame* g, kgmMesh* msh);
  kgmUnit(kgmIGame* g, kgmLight* lgt);
  kgmUnit(kgmIGame* g, kgmCamera* cam);
  kgmUnit(kgmIGame* g, kgmVisual* vis);
  kgmUnit(kgmIGame* g, kgmTerrain* ter);
  kgmUnit(kgmIGame* g, kgmObstacle* obs);
  kgmUnit(kgmIGame* g, kgmParticles* prt);
  ~kgmUnit();

  virtual void init();

  virtual void exit()
  { }

  virtual void event(kgmObject*, kgmString)
  { }

  virtual void update(u32);

  void remove();
  u32  timeout();

  kgmObject*             getNodeObject();
  kgmIGraphics::TypeNode getNodeType();
  bool                   isNodeValid();
  box3                   getNodeBound();
  vec3                   getNodePosition();
  mtx4                   getNodeTransform();
  kgmMaterial*           getNodeMaterial();
  void                   setNodeMaterial(kgmMaterial*);

  void set(kgmMesh* m)
  {
    if (m && m_type == Mesh)
      m_mesh = m;
  }

  void set(kgmMaterial* m)
  {
    if (m)
      m_material = m;
  }

private:
  virtual void clear()
  { }

public:
  kgmIGame* game() const
  {
    return m_game;
  }

  UnitType getType() const
  {
    return m_type;
  }

  //kgmBody* body() const
  //{
  //  return m_body;
  //}

  bool bodyIsValid()
  {
    return m_valid;
  }

  bool bodyGravity()
  {
    return m_gravity;
  }

  void bodyGravity(bool g)
  {
    m_gravity = g;
  }

  f32   bodyMass()
  {
    return m_mass;
  }

  void  bodyMass(f32 m)
  {
    m_mass = m;
  }

  vec3  bodyForce()
  {
    return vec3(0, 0, 0);
  }

  vec3 bodyPosition()
  {
    return vec3(0, 0, 0);
  }

  void bodyPosition(vec3)
  {

  }

  box   bodyBound()
  {
    return m_bound;
  }

  void  bodyBound(box b)
  {
    m_bound = b;
  }

  kgmObstacle* obstacle()
  {
    if (m_type != Obstacle)
      return null;

    return m_obstacle;
  }

  bool valid() const
  {
    return m_valid;
  }

  bool culled()  const
  {
    return m_culled;
  }

  bool visible() const
  {
    return m_visible;
  }

  bool removed() const
  {
    return m_remove;
  }

  void enable()
  {
    m_valid  = true;
  }

  void disable()
  {
    m_valid  = false;
  }

  void show()
  {
    m_visible = true;
  }

  void hide()
  {
    m_visible = false;
  }

  bool lock() const
  {
    return m_lock;
  }

  void lock(bool l)
  {
    m_lock = l;
  }

  void setId(u32 c)
  {
    m_id = c;
  }

  u32 getId()
  {
    return m_id;
  }

  void setName(kgmString s)
  {
    m_name = s;
  }

  kgmString getName()
  {
    return m_name;
  }

  void setGroup(u32 g)
  {
    m_group = g;
  }

  u32 getGroup()
  {
    return m_group;
  }

  void setLiving(u32 l)
  {
    m_living = l;
  }

  u32 getLiving() const
  {
    return m_living;
  }

  vec3 position()
  {
    //if(m_body)
    //  return m_body->position();

    return m_position;
  }

  void position(vec3& v)
  {
    m_position = v;

    //if (m_object && m_type == Particles)
    //  ((kgmParticles*)m_object)->position(v);

    //m_transform.identity();
    m_transform.translate(v);
  }

  vec3 rotation()
  {
    //if(m_body)
    //  return m_body->rotation();

    return m_rotation;
  }

  void rotation(vec3& r)
  {
    //if(m_body)
    //  m_body->rotate(r.x, r.y, r.z);
    //else
      m_rotation = r;
  }

  quat quaternion()
  {
    //if(m_body)
    //  return m_body->quaternion();

    return quat();
  }

  void quaternion(quat& q)
  {
    //if(m_body)
    //  m_body->rotate(q);
  }

  quat scale()
  {
    return m_scale;
  }

  void scale(vec3& s)
  {
    m_scale = s;

    m_transform.scale(s);
  }

  mtx4 transform()
  {
    return m_transform;
  }

  u32 birth()
  {
    return m_birth;
  }

  UnitType type()
  {
    return m_type;
  }

  kgmLight* light() const
  {
    if(m_type != Light)
      return null;

    return m_light;
  }

  kgmCamera* camera() const
  {
    if(m_type != Camera)
      return null;

    return m_camera;
  }

  kgmString action() const
  {
    return m_action.id;
  }

  void action(kgmString a)
  {
    m_action.id = a;
  }

  void setParameter(kgmString name, kgmString value)
  {
    for(int i = 0; i < m_variables.length(); ++i)
    {
      kgmVariable& var = m_variables[i];

      if(var.getName() == name)
        var.fromString(value);
    }
  }

  static bool getAvailableActions(kgmList<kgmString>& actions)
  {
    if(g_list_action.length() < 1)
      return false;

    kgmTab<kgmString, ActionCallback>::iterator i;

    for(i = g_list_action.begin(); !i.end(); ++i)
    {
      actions.add(i.key());
    }

    return true;
  }

  static void setActionCallback(kgmString action, kgmUnit::ActionCallback callback)
  {
    g_list_action.set(action, callback);
  }

  static kgmUnit::ActionCallback getActionCallback(kgmString action)
  {
    kgmTab<kgmString, ActionCallback>::iterator i = g_list_action.get(action);

    if(i.isEnd())
      return null;

    return i.data();
  }
};

#endif // KGMGAMEOBJECT_H
