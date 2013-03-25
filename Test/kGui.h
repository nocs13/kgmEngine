#ifndef KGUI_H
#define KGUI_H

#include "../kgmBase/kgmObject.h"
#include "../kgmGame/kgmGameBase.h"

class kGui : public kgmObject, kgmEvent
{
public:
    kgmGameBase* m_game;
    kgmGui* m_guiMain;

public:
    kGui(kgmGameBase*);
    void onAction(kgmEvent*, int);
};

#endif // KGUI_H
