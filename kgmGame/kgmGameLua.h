/*
 * kgmGameLua.h
 *
 *  Created on: Nov 3, 2010
 *      Author: goga
 */

/*
 *  Class for lua integration in kgm game
 */
#pragma once
#include "../kgmScript/kgmLua.h"

class kgmEvent;
class kgmGui;

class kgmGameLua: public kgmLua{
public:
 kgmGameLua();
 virtual ~kgmGameLua();
 //virtual void prepare(); //load libraries
 //virtual void onEvent(kgmEvent::Event*);
 //virtual void onAction(kgmGui*, int);

//base function based on command line
 static int command(lua_State*);

//implement game engine objects
 static int gameStart(lua_State*);
 static int gameLoop(lua_State*);
 static int gameQuit(lua_State*);
 static int gameLoad(lua_State*);
//Process

//Thread

//Widgets

//Sound

//Actors
};
