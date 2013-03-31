/*
 * kgmNode.h
 *
 *  Created on: Jun 2, 2011
 *      Author: goga
 */

#include "../kgmBase/kgmTypes.h"
#include "../kgmBase/kgmObject.h"
#include "../kgmBase/kgmString.h"
#include "../kgmBase/kgmList.h"
#include "../kgmMath/kgmBase.h"
#include "../kgmMath/kgmMatrix3d.h"

#include <stdio.h>

class kgmMesh;
class kgmMaterial;
class kgmSkeleton;
class kgmAnimation;

class kgmNode: public kgmObject
{
    kgmString     sid;
public:

    kgmMesh*      mesh;
    kgmMaterial*  material;
    kgmAnimation* animation;
    kgmSkeleton*  skeleton;

    mtx4          transform;


public:
  kgmNode();
  virtual ~kgmNode();

};

