#include "kgmGpuMesh.h"

kgmGpuMesh::kgmGpuMesh()
{
  m_handle = null;

  m_vcount = 0;
  m_fcount = 0;
  m_vsize = 0;
  m_fsize = 0;

  m_fvf = 0;
  m_fff = 0;

  m_rtype = RT_TRIANGLE;
}

kgmGpuMesh::kgmGpuMesh(gchandle h, u32 vcnt, u32 fcnt, u32 vsize, u32 fsize, u32 fvf, u32 fff, u32 rtype, box bound)
{
  m_handle = h;

  m_vcount = vcnt;
  m_fcount = fcnt;

  m_vsize = vsize;
  m_fsize = fsize;

  m_fvf = fvf;
  m_fff = fff;

  m_bound = bound;

  m_rtype = (RenderType) rtype;
}
