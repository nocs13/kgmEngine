#pragma once
#include "../kgmBase/kgmObject.h"
#include "../kgmBase/kgmString.h"

#include "../kgmMath/kgmBase.h"
#include "../kgmMath/kgmVector3d.h"

class kgmDummy: public kgmObject
{
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
  vec3         m_orient;

private:
  kgmObject*   m_linked;

protected:
  ~kgmDummy()
  {
  }

public:
  kgmDummy()
  {
    m_linked = null;
    m_type   = AttachToNone;

    m_shift = m_orient = vec3(0, 0, 0);
  }

  void attach(kgmObject* o = null, Attach t = AttachToNone)
  {
    m_linked = o;
    m_type   = t;
  }

  void detach()
  {
    m_linked = null;
    m_type = AttachToNone;
  }

  void setId(kgmString s)
  {
    m_id = s;
  }

  void setShift(vec3& v)
  {
    m_shift = v;
  }

  void setOrient(vec3& r)
  {
    m_orient = r;
  }

  kgmObject* linked()
  {
    return m_linked;
  }
};
