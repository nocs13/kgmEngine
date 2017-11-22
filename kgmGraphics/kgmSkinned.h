#ifndef KGMSKINNED_H
#define KGMSKINNED_H

#include "kgmMesh.h"

class kgmAnimation;
class kgmSkeleton;

class kgmSkinned : public kgmMesh
{
  KGM_OBJECT(kgmSkinned);

  kgmAnimation* m_animation = null;
  kgmSkeleton*  m_skeleton  = null;

  u32 m_start = 0;
  u32 m_end   = -1;
  u32 m_set   = 0;

  bool m_loop = false;

  Vertex_P_N*   m_basic = null;

  kgmArray<mtx4>  m_tm_joints;

public:
  kgmSkinned();
  kgmSkinned(kgmSkeleton*, kgmAnimation*);

  Vertex* vAlloc(u32 count);

  void set(kgmSkeleton*);
  void set(kgmAnimation*);
  void update(u32);

  bool loop() const
  {
    return m_loop;
  }

  void loop(bool l)
  {
    m_loop = l;
  }
};

#endif // KGMSKINNED_H
