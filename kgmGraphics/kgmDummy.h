#pragma once
#include "../kgmBase/kgmObject.h"
#include "../kgmBase/kgmString.h"

#include "../kgmMath/kgmBase.h"
#include "../kgmMath/kgmVector3d.h"

class kgmDummy: public kgmObject{
  KGM_OBJECT(kgmDummy);

public:
  enum Attach
  {
    AttachToNone = 0,
    AttachToDummy,
    AttachToJoint,
    AttachToObject
  };

public:
  kgmString    m_id;
  Attach       m_type;
  vec3         m_shift;
  kgmObject*   m_linked;

public:
  ~kgmDummy()
  {
  }

  kgmDummy()
  {
    m_linked = 0;
    m_type   = AttachToNone;
  }

  void attach(kgmObject* o = null, Attach t = AttachToNone)
  {
    m_linked = o;
    m_type   = t;
  }

  void setId(kgmString s)
  {
    m_id = s;
  }

  void setShift(vec3& v)
  {
    m_shift = v;
  }
};
