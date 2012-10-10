/* 
 * File:   kgmEditor.h
 * Author: goga
 *
 * Created on January 7, 2012, 9:00 PM
 */

#ifndef KGMEDITOR_H
#define	KGMEDITOR_H

#include "../kgmGraphics/kgmGui.h"
#include "../kgmGraphics/kgmGuiMenu.h"
#include "../kgmGame/kgmGameBase.h"
#include "../kgmGame/kgmGameLua.h"

class kgmEditor: public kgmGameBase {
	kgmGameLua* lua;
public:
    kgmEditor();
    kgmEditor(const kgmEditor& orig);
    virtual ~kgmEditor();
    
    bool luaRun(kgmString script);

private:

};

#endif	/* KGMEDITOR_H */

