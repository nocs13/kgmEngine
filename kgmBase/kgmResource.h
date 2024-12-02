#pragma once
#include "kgmObject.h"
#include "kgmString.h"

class kgmIResources;

class kgmResource: public kgmObject
{
  KGM_OBJECT(kgmResource)

  friend class kgmIResources;

private:

public:
  enum  Type
  {
    TypeNone,
    TypeFile,
    TypeFont,
    TypeMesh,
    TypeSound,
    TypeBuffer,
    TypeShader,
    TypeTexture,
    TypeSkeleton,
    TypeAnimation,
  };

  kgmObject* m_resource;
  kgmString  m_id;
  u32	       m_uid;
  u32        m_type;
  u32        m_group;

public:
  kgmResource();

  void setId(kgmString id){ m_id = id; }

  kgmString id() const
  {
    return m_id;
  }

  void id(kgmString id)
  {
    m_id = id;
  }

  u32 group() const
  {
    return m_group;
  }

  void group(u32 group)
  {
    m_group = group;
  }

  Type type() const
  {
    return (Type) m_type;
  }

protected:
  ~kgmResource();
};

