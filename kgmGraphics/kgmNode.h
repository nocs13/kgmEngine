/*
 * kgmNode.h
 *
 *  Created on: Jun 2, 2011
 *      Author: goga
 */

#include "../kgmBase/kgmObject.h"
#include "../kgmBase/kgmString.h"
#include "../kgmBase/kgmList.h"
#include "../kgmMath/kgmBase.h"

#include <stdio.h>

class kgmGui;
class kgmMesh;
class kgmMaterial;
class kgmLight;
class kgmCamera;
class kgmAnimation;
class kgmSkeleton;

class kgmNode: public kgmObject
{
protected:
public:
  enum{
    NodeNone,
    NodeObject,
    NodeGui,
    NodeMesh,
    NodeMaterial,
    NodeLight,
    NodeCamera,
    NodeMatrix,
    NodeAnimation,
    NodeSkeleton,
    NodeString,

    NodeExtention,
  };

  union{
    kgmObject*    object;
    kgmGui*       gui;
    kgmMesh*      mesh;
    kgmMaterial*  material;
    kgmLight*     light;
    kgmCamera*    camera;
    kgmAnimation* animation;
    kgmSkeleton*  skeleton;
    kgmString*    string;

    mtx4*         transform;
  };

  u32              type;
  kgmList<kgmNode*> nodes;

public:
  kgmNode()                     { object = 0;    type = NodeNone;      }
  kgmNode(kgmObject* o)         { object = o;    type = NodeObject;    }
  kgmNode(kgmGui* o)            { gui = o;       type = NodeGui;       }
  kgmNode(kgmMesh* o)           { mesh = o;      type = NodeMesh;      }
  kgmNode(kgmMaterial* o)       { material = o;  type = NodeMaterial;  }
  kgmNode(kgmLight* o)          { light = o;     type = NodeLight;     }
  kgmNode(kgmCamera* o)         { camera = o;    type = NodeCamera;    }
  kgmNode(kgmAnimation* o)      { animation = o; type = NodeAnimation; }
  kgmNode(kgmSkeleton* o)       { skeleton = o;  type = NodeSkeleton;  }
  kgmNode(kgmString* o)         { string = o;    type = NodeString;    }
  kgmNode(mtx4*        o)       { transform = o; type = NodeMatrix;    }
  virtual ~kgmNode();

  void     print();
  bool     erase(kgmNode* node);
  kgmNode* find(u32 type, void* o);
  
  //virtual void add(kgmNode* n) { nodes.add(n); }
  virtual void     add(kgmNode* n);
  virtual int count() { return nodes.size(); }
  virtual kgmNode* node(int i) { return (i < nodes.size()) ? (nodes[i]) : (0); }

  
  operator kgmObject* () { return object; }       
  operator kgmMesh* () { return mesh; }       
  operator kgmLight* () { return light; }       
  operator kgmCamera* () { return camera; }       
  operator kgmMaterial* () { return material; }       
  operator kgmSkeleton* () { return skeleton; }       
  operator kgmAnimation* () { return animation; }       
  
};

