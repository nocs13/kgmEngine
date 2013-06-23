#ifndef KGMIAI_H
#define KGMIAI_H

class kgmActor;

class kgmIAI
{
public:
  virtual void clean() = 0;
  virtual void add(kgmActor*) = 0;
  virtual void update(u32)    = 0;
};

#endif // KGMIAI_H
