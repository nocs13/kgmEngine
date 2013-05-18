#pragma once
#include <math.h>
#include "../kgmBase/kgmResource.h"
#include "../kgmBase/kgmList.h"
#include "../kgmMath/kgmVector3d.h"
#include "../kgmMath/kgmVector4d.h"
#include "../kgmMath/kgmQuaternion.h"
#include "../kgmMath/kgmMatrix3d.h"
#include "../kgmMath/kgmBase.h"

class kgmSkeleton: public kgmResource{
  KGM_OBJECT(kgmSkeleton);
public:

  struct Joint
  {
    char  n[32];
    int   i;
    vec3  v;
    quat  r;
  };

public:
  kgmList<Joint*> m_joints;
  kgmList<mtx4>   m_matrices;
  kgmList<mtx4>   m_imatrices;
public:
  kgmSkeleton(){
  }
  virtual ~kgmSkeleton(){
    for(int i = 0; i < m_joints.size(); i++)
      delete m_joints[i];
    m_joints.clear();
    m_matrices.clear();
    m_imatrices.clear();
  }
  void update(){
    m_matrices.clear();
    m_imatrices.clear();
    for(u32 i = 0; i < m_joints.size(); i++){
      mtx4 m = mtx4(m_joints[i]->r, m_joints[i]->v);
      //mtx4 m = mtx4(m_joints[i]->v);
      m_matrices.add(m);
      m.invert();
      m_imatrices.add(m);
    }
  }
  void getTMBones(mtx4 *pbm, u32 cnt){
    if(!pbm || (cnt != m_joints.size()))
      return;

    for(u32 i = 0; i < m_joints.size(); i++){
      pbm[i] = mtx4(m_joints[i]->r, m_joints[i]->v);
    }
  }

  void getTM(mtx4& m, u32 i){

  }
};
