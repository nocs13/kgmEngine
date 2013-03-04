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
    class Gui: public kgmGui{
    };
public:
    kgmGameGui();
    kgmGameGui(const kgmGameGui& orig);
    virtual ~kgmGameGui();
    
    static kgmGameGui* build(kgmXml& xml);
private:

};

#endif	/* KGMGAMEGUI_H */

