#ifndef KGMGUIWIDGET_H
#define KGMGUIWIDGET_H

#include "kgmGui.h"

class kgmGuiWidget : public kgmGui
{
  KGM_OBJECT(kgmGuiWidget);

  kgmGui*    m_focus;    //current child active window

  typedef    kgmList<kgmGui*> Guis;
  Guis       m_childs;

public:
  kgmGuiWidget();

  kgmGui*      getById(u32 id);
  kgmGui*      getBySid(kgmString sid);
  kgmGui*      getFocus();
  bool         setFocus(kgmGui*);

  //useful functions
  void          addChild(kgmGui *e);
  void          delChild(kgmGui *e);
  bool          isChild(kgmGui *e);
  kgmGui*       getFocus(Point pos);
  kgmGui*       getPointed();
};

#endif // KGMGUIWIDGET_H
