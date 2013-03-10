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
#include "../kgmGraphics/kgmGuiTab.h"
#include "../kgmGraphics/kgmGuiList.h"
#include "../kgmGraphics/kgmGuiButton.h"

#include "kgmIGame.h"

class kgmGameGui: public kgmGui{
  class Base{
    kgmString m_sid;

  public:
    Base(kgmString& sid){
      m_sid = sid;
    }

    kgmString& sid(){
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

  kgmIGame* m_game;
public:
    kgmGameGui(kgmIGame* game, kgmXml& xml);
    virtual ~kgmGameGui();
private:

  void build(kgmGui*, kgmXml::Node*);
};

#endif	/* KGMGAMEGUI_H */

