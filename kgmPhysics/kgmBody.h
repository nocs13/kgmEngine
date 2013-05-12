#pragma once
#include "../kgmBase/kgmList.h"
#include "../kgmMath/kgmMath.h"

class kgmBody{
public:
 enum{ 
  ShapeNone,
  ShapeBox,
  ShapeSphere,
  ShapeCylinder,
  ShapePolyhedron,
 };

 struct Constraint{
   bool      position,
             rotation,
             scale;
   vec3      offset;
   kgmBody*  body;
 };


 u32	m_type;
 u32	m_shape;

 bool	m_physics;		//valid physics object
 bool	m_collision;	//calculate collisions for this body
 bool	m_gravity;		//gravity object or gravitation required
 bool	m_falling;		//additional information of falling

 double	m_mass;			//mass of body
 double m_friction;		//friction of a body
 double	m_force;		//persistent force  to body
 double m_upforce;		//upstear force
 double m_velocity;		//persistent speed

 vec3	m_position;		//current position 
 vec3	m_rotation;		//current rotation
 vec3	m_direction;	//current direction
 quat	m_quaternion;	//current rotation(in quaternions)

 //vec3	m_V;			//additional velocity
 //vec3	m_P;			//current impulse
 //vec3	m_F;			//current superposed, applied, imposed force
 //vec3   m_EF;  //external force

 box3	m_bound;		//bound box

 Constraint m_constraint;

 void*	    m_extra;		//extended data
 void*	    m_udata;		//user data

private:
 kgmList<polygon3*> m_convex;	//convex shape sides for simulation(collision, ...)

public:
 kgmBody();
 ~kgmBody();

 virtual void collision(kgmBody*); //callback when collision

 void enable(bool);
 void translate(float x, float y, float z);
 void rotate(float x, float y, float z);
 void rotate(quat& q);
 void transform(mtx4& mtr);

 void setShape(box3& b);
// void setShape(sphere3& s);
// void setShape(cylinder& c);
 void setShape(int c, polygon3* p);
 
 void setPosition(vec3& v);
 void setDirection(vec3& d);
};
