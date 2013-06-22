#ifndef KGMILOGIC_H
#define KGMILOGIC_H

class kgmActor;
class kgmSensor;
class kgmTrigger;
class kgmGameObject;

class kgmILogic
{
public:
  virtual bool add(kgmActor*)                          = 0;
  virtual bool add(kgmSensor*)                         = 0;
  virtual bool add(kgmTrigger*)                        = 0;
  virtual bool add(kgmGameObject*)                     = 0;
  virtual void update(u32)                             = 0;
  virtual void collide(kgmGameObject*, kgmGameObject*) = 0;
};

#endif // KGMILOGIC_H
