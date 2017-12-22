#pragma once
#include "../kgmBase/kgmTypes.h"
#include "../kgmMath/kgmMath.h"

class kgmBody;
class kgmObstacle;

class kgmIPhysics
{
public:
  enum BodyShape
  {
    BodyShapeBox,
    BodyShapeSphere,
    BodyShapeCylinder
  };

  class IBody
  {
  public:
    virtual bool isBodyValid()      = 0;
    virtual f32  getBodyMass()      = 0;
    virtual f32  getBodyVelocity()  = 0;
    virtual vec3 getBodyPosition()  = 0;
    virtual vec3 getBodyDirection() = 0;

    virtual void  setBodyMass(f32)       = 0;
    virtual void  setBodyVelocity(f32)   = 0;
    virtual void  setBodyPosition(vec3)  = 0;
    virtual void  setBodyDirection(vec3) = 0;
  };

  class IObstacle
  {
  public:
    virtual void      set(mtx4&)               = 0;
    virtual void      add(vec3&, vec3&, vec3&) = 0;
    virtual u32       count()                  = 0;
    virtual triangle3 get(u32 i)               = 0;
  };

public:
 virtual void clear()  = 0;
 virtual void build()  = 0;
 virtual void update() = 0;

 virtual void add(vec3& a, vec3& b, vec3& c) = 0;
 virtual void add(kgmBody*)                  = 0;
 virtual void add(kgmObstacle*)              = 0;
 virtual void remove(kgmBody*)               = 0;
 virtual void remove(kgmObstacle*)           = 0;
};
