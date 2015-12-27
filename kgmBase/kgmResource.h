#pragma once
#include "kgmObject.h"
#include "kgmString.h"

class kgmIResources;

class kgmResource: public kgmObject
{
  KGM_OBJECT(kgmResource);

  friend class kgmIResources;

private:
  bool m_lock;

public:
  enum  Type
  {
    TypeNone,
    TypeFont,
    TypeTexture,
    TypeShader,
    TypeMesh,
    TypeSound,
    TypeAnimation,
    TypeSkeleton,
  };

public:
  kgmObject* m_resource;
  kgmString  m_id;
  u32	       m_uid;
  u32        m_type;

protected:
  ~kgmResource();

public:
  kgmResource();

  void setId(kgmString id){ m_id = id; }

  /*void release()
  {
    if((references() > 1) || (!m_lock && references() == 1))
      ((kgmObject*)this)->release();
  }*/

private:
  void lock()
  {
    m_lock = true;
  }

  void unlock()
  {
    m_lock = false;
  }
};

