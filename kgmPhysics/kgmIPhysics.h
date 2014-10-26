#pragma once

class kgmBody;

class kgmIPhysics
{
public:
 virtual void clear() = 0;
 virtual void build() = 0;
 virtual void update(float time) = 0;

 virtual void add(vec3& a, vec3& b, vec3& c) = 0;	//static objects
 virtual void add(kgmBody*) = 0;                  //dynamic objects
 virtual bool remove(kgmBody*) = 0;	              //remove body
};
