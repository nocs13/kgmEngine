#ifndef KGMGAMEOBJECT_H
#define KGMGAMEOBJECT_H

#include "../kgmBase/kgmObject.h"
#include "../kgmPhysics/kgmBody.h"
#include "../kgmGraphics/kgmVisual.h"

class kgmGameObject : public kgmObject
{
  KGM_OBJECT(kgmGameObject);

private:
  bool    m_enable;
  bool    m_active;
  bool    m_remove;
  bool    m_culled;
  bool    m_visible;

  kgmBody*    m_body;
  kgmVisual*  m_visual;

protected:
    kgmGameObject* 	         m_parent;
    kgmList<kgmGameObject*>  m_childs;


public:
  kgmGameObject();
  virtual ~kgmGameObject();

  void   addChild(kgmGameObject*);
  void   removeChild(kgmGameObject*);

  virtual void         update(u32 mls){}
  virtual kgmBody*     getBody(){ return m_body; }
  virtual kgmVisual*   getVisual(){ return m_visual; }

  bool isEnable() { return m_enable;  }
  bool isActive() { return m_active;  }
  bool isRemove() { return m_remove;  }
  bool isCulled() { return m_culled;  }
  bool isVisible(){ return m_visible; }
};

#endif // KGMGAMEOBJECT_H
