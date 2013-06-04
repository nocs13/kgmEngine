#ifndef KGMGAMEOBJECT_H
#define KGMGAMEOBJECT_H

#include "../kgmBase/kgmObject.h"
#include "../kgmPhysics/kgmBody.h"
#include "../kgmGraphics/kgmVisual.h"

class kgmGameObject : public kgmObject
{
  KGM_OBJECT(kgmGameObject);

private:
  kgmBody*    m_body;
  kgmVisual*  m_visual;

public:
  kgmGameObject();

  void update(u32 mls);
};

#endif // KGMGAMEOBJECT_H
