#pragma once
#include "kgmObject.h"
#include "kgmString.h"

class kgmIResources;

class kgmResource: public kgmObject
{
  KGM_OBJECT(kgmResource)

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

  static kgmIResources* g_resources;

public:
  kgmObject* m_resource;
  kgmString  m_id;
  u32	       m_uid;
  u32        m_type;

private:
  unsigned int m_references;

public:
  kgmResource();
  ~kgmResource();

  void setId(kgmString id){ m_id = id; }

  void release();

  u32 references() const
  {
    return m_references;
  }

  void increment()
  {
    m_references += 1;
  }
};

