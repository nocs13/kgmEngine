/* 
 * File:   kgmGameGUI.h
 * Author: goga
 *
 * Created on March 3, 2013, 3:49 PM
 */

#ifndef KGMGAMEGUI_H
#define	KGMGAMEGUI_H

#include "../kgmBase/kgmXml.h"
#include "../kgmGraphics/kgmGui.h"

class kgmGameGui: public kgmGui{
  class Base{
    kgmString m_sid;

  public:
    Base(kgmString& sid){
      m_sid = sid;
    }

    kgmString& sid() const{
      return m_sid;
    }
  };

  class Gui: public kgmGui, public Base{
  };

  class GuiTab: public kgmGuiTab, public Base{
  };

  class GuiList: public kgmGuiList, public Base{
  };

  class GuiButton: public kgmGuiButton, public Base{
  };

public:
    kgmGameGui();
    kgmGameGui(const kgmGameGui& orig);
    virtual ~kgmGameGui();
    
    static kgmGameGui* build(kgmXml& xml);
private:

};

#endif	/* KGMGAMEGUI_H */

