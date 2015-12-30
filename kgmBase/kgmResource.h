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

  u32 m_references;

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

  kgmObject* m_resource;
  kgmString  m_id;
  u32	       m_uid;
  u32        m_type;

public:
  kgmResource();
  ~kgmResource();

  void setId(kgmString id){ m_id = id; }

  u32 references() const
  {
    return m_references;
  }

  void increment()
  {
    m_references++;
  }

  void release()
  {
    if (m_lock)
      return;

    if(m_references > 0)
      m_references--;
  }

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

