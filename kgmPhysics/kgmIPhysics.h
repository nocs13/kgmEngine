#pragma once

class kgmBody;
class kgmObstacle;

class kgmIPhysics
{
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
