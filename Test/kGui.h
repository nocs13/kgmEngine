#ifndef KGUI_H
#define KGUI_H

#include "../kgmBase/kgmObject.h"
#include "../kgmGame/kgmGameBase.h"

class kGui : public kgmObject
{
public:
    kgmGui* m_guiMain;
public:
    kGui(kgmGameBase*);
};

#endif // KGUI_H
