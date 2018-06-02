#include "BaseRender.h"
#include "../../kgmBase/kgmIGC.h"
#include "../kgmGui.h"

class INode;
class Mesh;


class GlassRender: public BaseRender
{
  enum TypeReflect
  {
    TypeNone,
    TypePlane,
    TypeCube
  };

  struct Node
  {
    INode *node;
    Mesh  *mesh;

    void *reflect;

    TypeReflect type;
  };

  void *tex_refract;

 public:
  GlassRender(kgmGraphics* gr);

  void clear();
  void render();
  void prepare();

  void add(INode*);
};
