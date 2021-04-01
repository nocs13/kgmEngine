#ifndef KGMGPUMESH_H
#define KGMGPUMESH_H

#include "kgmIMesh.h"
#include "../kgmBase/kgmIGC.h"
#include "../kgmBase/kgmResource.h"

class kgmGpuMesh : public kgmIMesh, public kgmResource
{
  KGM_OBJECT(kgmGpuMesh);

  gchandle m_handle;

  u32 m_vcount;
  u32 m_fcount;
  u32 m_vsize;
  u32 m_fsize;

  u32 m_fvf;
  u32 m_fff;

  RenderType m_rtype;

  box m_bound;

public:
  kgmGpuMesh();
  kgmGpuMesh(gchandle h, u32 vcnt, u32 fcnt, u32 vsize, u32 fsize, u32 fvf, u32 fff, u32 rtype, box bound);

  Vertex* vertices()
  {
    return (Vertex*) m_handle;
  }

  Face*   faces()
  {
    return (Face*) m_handle;
  }

  u32 vsize()
  {
    return m_vsize;
  }

  u32 fsize()
  {
    return m_fsize;
  }

  u32 fvf()
  {
    return m_fvf;
  }

  u32 fff()
  {
    return m_fff;
  }

  u32 vcount() const
  {
    return m_vcount;
  }

  u32 fcount() const
  {
    return m_fcount;
  }

  RenderType rtype() const
  {
    return m_rtype;
  }

  box bound() const
  {
    return m_bound;
  }

  bool gpu() const
  {
    return true;
  }
};

#endif // KGMGPUMESH_H
