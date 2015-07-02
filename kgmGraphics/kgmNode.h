#include "../kgmBase/kgmTypes.h"
#include "../kgmBase/kgmObject.h"
#include "../kgmBase/kgmString.h"
#include "../kgmBase/kgmList.h"
#include "../kgmMath/kgmBase.h"
#include "../kgmMath/kgmMatrix3d.h"

/* TODO
 * Must switch to 3d scene graph system.
 * Insted of independent light,mesh, etc
 * they must be part of Nodes.
 */

class kgmNode: public kgmObject
{
  KGM_OBJECT(kgmNode);

public:

  enum NodeType
  {
    NodeNone,
    NodeIcon,
    NodeLight,
    NodeShape
  };

private:
  kgmNode* m_parent;

  kgmString         m_name;
  kgmList<kgmNode*> m_childs;


protected:
  ~kgmNode();

public:
  kgmNode(kgmNode *parent = null);
};

