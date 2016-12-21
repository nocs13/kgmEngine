#pragma once
#include "../kgmBase/kgmObject.h"
#include "../kgmMath/kgmMath.h"

class kgmBody: public kgmObject
{
  KGM_OBJECT(kgmBody);

public:
  enum
  {
    ShapeNone,
    ShapeBox,
    ShapeSphere,
    ShapePolyhedron,
  };

  struct Constraint
  {
    bool     position, rotation, scale;
    vec3     offset;
    kgmBody* body;
  };


  u32     m_type;
  u32     m_shape;

  bool    m_valid;      //valid physics object
  bool    m_remove;     //physics object should removed
  bool    m_collision;  //calculate collisions for this body
  bool    m_gravity;    //gravity object or gravitation required
  bool    m_falling;    //additional information of falling
#ifdef DEBUG
  bool    m_intersect;  //have intersection
#endif
  float   m_mass;          //mass of body
  float   m_friction;      //friction of a body
  float   m_velocity;      //persistent speed
  //float   m_speed_forward;
  //float   m_speed_side;
  //float   m_speed_up;

  vec3     m_position;    //current position
  vec3     m_rotation;    //current rotation
  quat     m_quaternion;  //current rotation(in quaternions)

  box3     m_bound;       //bound box

  Constraint         m_constraint;

  kgmList<triangle3> m_convex; //convex shape sides for simulation(collision, ...)

  void*	  m_udata;        //user data of linked object

private:
  vec3     m_direction;        //current direction

public:
  kgmBody();
  kgmBody(void* udata);
  virtual ~kgmBody();

  virtual void collision(kgmBody*); //callback when collision

  bool removed() { return m_remove; }
  void remove()  { m_remove = true; }
  bool valid()   { return m_valid;  }
  void enable()  { m_valid = true;  }
  void disable() { m_valid = false; }

  void translate(float x, float y, float z)
  {
    m_position.set(x, y, z);
  }

  void translate(vec3 v)
  {
    m_position = v;
  }

  void rotate(float x, float y, float z)
  {
    m_rotation.x = x;
    m_rotation.y = y;
    m_rotation.z = z;
    m_quaternion = quat(m_rotation);

    mtx4 m(m_quaternion);
    vec3 v(1, 0, 0);

    m_direction = m * v;
    m_direction.normalize();
  }

  void rotate(vec3 r)
  {
    rotate(r.x, r.y, r.z);
  }

  void rotate(quat& q)
  {
    m_quaternion = q;
    mtx4 m(m_quaternion);
    m.angles(m_rotation);
  }

  void transform(mtx4& mtr)
  {
    mtx4 tr(m_position), rt(m_quaternion);
    mtr = rt * tr;
  }

  mtx4 transform()
  {
    mtx4 tr(m_position), rt(m_quaternion);
    mtx4 m = rt * tr;

    return m;
  }

  void setShape(box3& b);
  void setShape(sphere3& s);
  void addShapeSide(vec3 v[3]);

  vec3 position()  { return m_position;  }
  vec3 rotation()  { return m_rotation;  }
  vec3 direction() { return m_direction; }

  kgmOBox3d<float> getOBox()
  {
    vec3 dim = m_bound.max - m_bound.min;

    return kgmOBox3d<float>(m_position, m_rotation, dim);
  }
};
