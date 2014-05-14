#pragma once
#include "kgmMain.h"
#include "kgmTypes.h"
#include <stdlib.h>
#include <string.h>

class kgmApp;
class kgmObject;

struct kgmRuntime
{
  const char* nClass; //class id
  u32         sClass; //size of class
  kgmRuntime* pClass; //parent class
};

#define KGM_OBJECT(o_class)                     \
  public:                                	\
  static  kgmRuntime   Class;			\
  static  o_class*     cast(kgmObject*);	\
  virtual kgmRuntime&  runtime();



#define KGMOBJECT_IMPLEMENT(o_class, o_parent)	\
  kgmRuntime  o_class::Class = {  #o_class, sizeof(class o_class), &o_parent::Class}; \
  o_class*    o_class::cast(kgmObject* o) { return (o_class*)o; } \
  kgmRuntime& o_class::runtime() { return o_class::Class; }

//base class for kgm_engine project objects

class kgmObject
{
  KGM_OBJECT(kgmObject);

private:
  unsigned int m_references;

protected:
  virtual ~kgmObject()
  {
  }

public:
  kgmObject()
  {
    m_references = 1;
  }

  bool isClass(kgmObject& o)
  {
    return !strcmp(runtime().nClass, o.runtime().nClass);
  }

  bool isClass(kgmRuntime& o)
  {
    return !strcmp(runtime().nClass, o.nClass);
  }

  bool isClass(const char* o)
  {
    return !strcmp(runtime().nClass, o);
  }

  bool isType(kgmObject& o)
  {
    kgmRuntime* r = &runtime();

    while(r != 0)
    {
      if(!strcmp(r->nClass, o.runtime().nClass))
        return true;

      if(r == r->pClass)
        break;

      r = r->pClass;
    }

    return false;
  }

  bool isType(kgmRuntime& o)
  {
    kgmRuntime* r = &runtime();

    while(r != 0)
    {
      if(!strcmp(r->nClass, o.nClass))
        return true;

      if(r == r->pClass)
        break;

      r = r->pClass;
    }

    return false;
  }

  bool isType(const char* o)
  {
    kgmRuntime* r = &runtime();

    while(r != 0){
      if(!strcmp(r->nClass, o))
        return true;

      if(r == r->pClass)
        break;

      r = r->pClass;
    }

    return false;
  }

  u32 references()
  {
    return m_references;
  }

  void increment()
  {
    m_references += 1;
  }

  void release()
  {
    if(!kgmObject::isValid(this))
      return;

    m_references--;

    if(m_references < 1)
      delete this;
  }

  virtual kgmObject* clone()
  {
    return new kgmObject();
  }

  void* operator new(size_t size);
  void  operator delete(void* p);

  friend class kgmApp;
  //private:
  static void releaseObjects();
  static int  objectCount();
  static bool isValid(kgmObject*);
#ifdef DEBUG
  static void listObjects();
#endif
};
///////////////////




