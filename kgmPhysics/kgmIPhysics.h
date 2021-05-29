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
    BodyShapeCylinder,
    BodyShapePolyhedron
  };

  class IBody
  {
  public:
    virtual bool bodyIsValid() = 0;

    virtual bool bodyUseCollision() = 0;

    virtual bool bodyGravity()     = 0;
    virtual void bodyGravity(bool) = 0;

    virtual f32   bodyMass()     = 0;
    virtual void  bodyMass(f32)  = 0;

    virtual vec3  bodyForce() = 0;

    virtual vec3 bodyPosition()     = 0;
    virtual void bodyPosition(vec3) = 0;

    virtual box   bodyBound()     = 0;
    virtual void  bodyBound(box)  = 0;

    virtual BodyShape bodyShape()           = 0;
    virtual void      bodyShape(BodyShape ) = 0;
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
 virtual void update(u32 ms) = 0;

 virtual void add(vec3& a, vec3& b, vec3& c) = 0;
 virtual void add(IBody*)                    = 0;
 virtual void add(kgmObstacle*)              = 0;
 virtual void remove(IBody*)                 = 0;
 virtual void remove(kgmObstacle*)           = 0;
};
