#include "BaseRender.h"
#include "../../kgmBase/kgmIGC.h"
#include "../kgmGui.h"

class INode;
class Mesh;


class GlassRender: public BaseRender
{
  struct Node
  {
    INode *node;
    Mesh  *mesh;
  };

 public:
  GlassRender(kgmGraphics* gr);

  void clear();
  void render();

  void add(INode*);
};
