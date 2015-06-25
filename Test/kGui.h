#ifndef KGUI_H
#define KGUI_H

#include "../kgmBase/kgmObject.h"
#include "../kgmGame/kgmGameBase.h"

class kGui : public kgmEvent
{
public:
    kgmGameBase* m_game;

    kgmGui*      m_guiMain;
    kgmGui*      m_guiGame;
    kgmGui*      m_guiSettings;

    kgmGui*      m_guiPause;
    //kgmGuiList*  m_guiLevels;

public:
    kGui(kgmGameBase*);
    ~kGui();
    void onAction(kgmEvent*, int);

    void viewAgain();
};

#endif // KGUI_H
