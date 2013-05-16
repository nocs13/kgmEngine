#include "../kgmBase/kgmObject.h"
#include "../kgmBase/kgmString.h"
#include "../kgmBase/kgmList.h"
#include "../kgmMath/kgmVector3d.h"
#include "../kgmMath/kgmQuaternion.h"

class kgmJoint: public kgmObject
{
  kgmString         name;
  kgmJoint*         parent;
  kgmList<kgmJoint> childs;

  vec3              position;   //parent base position, if parent null then world based
  vec3              rotation;   //euler based rotaton-- dont use
  quat              quaternion; //parent based rotations
public:
  kgmJoint(){
    name = "";
    parent = 0;
  }

  kgmJoint(kgmJoint* par, kgmString& n){
    name = n;
    parent = par;
  }

  void setPosition(vec3& v){
    position = v;
  }

  void setRotation(vec3& r){
    rotation = r;
  }

  void setQuaternion(quat& q){
    quaternion = q;
  }
};
