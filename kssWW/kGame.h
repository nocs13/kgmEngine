// kGame.h: interface for the kGame class.
//
//////////////////////////////////////////////////////////////////////

#pragma once
#include "../kgmMain/kgmString.h"
#include "../kgmScript/kgmLua.h"


class kGame: public kgmLua{
 class Game;
 class Gui;
 class Actor;

 Game* game;
 Gui*  gui;

 static kGame* m_game;
public:
 kGame();
 virtual ~kGame();

 static void onAction(Gui*, kgmString&, uint);
};

