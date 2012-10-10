#include "kLua.h"
#include "../kgmBase/kgmTab.h"
#include "../kgmBase/kgmXml.h"
#include "../kgmGraphics/kgmGui.h"
#include "../kgmGame/kgmActor.h"

class Gui: public kgmGui{
 typedef struct{
  kgmGui* gui;
  kgmString id;
 }	GUI;
public:
 static const char className[];
 static kgmLuaBind<Gui>::RegType methods[];
public:
 Gui()
 :kgmGui(0, 0, 0, 100, 100){
  fprintf(stderr, "\nGui");
 }
 Gui(lua_State* l)
 :kgmGui(0, 0, 0, 100, 100){
  fprintf(stderr, "\nGui Lua");
 }
 ~Gui(){
 }

 int _class(lua_State* l){
  lua_pushlightuserdata(l, this);
  return 1;
 }

 int _show(lua_State* l){
  kgmString   id = (char*)lua_tostring(l, 2);
  int         st = (int)lua_tointeger(l, 3);
  kgmGui*     gu = 0;

  fprintf(stderr, "\nGui Show");

  if(id == "self")
   gu = this;

  if(gu)
   if(st)
	gu->show();
   else
	gu->hide();

  return 0;
 }

  void align(kgmGui* gui, double w, double h){
   Rect rc;
   gui->getRect(rc);
   rc.x *= w;
   rc.y *= h;
   rc.w *= w;
   rc.h *= h;
   gui->setRect(rc);
   for(int i = 0; i < gui->m_childs.length(); i++)
   align(gui->m_childs[i], w, h);
  }

  void onResize(int w, int h){
   Rect rc;
   getRect(rc);
   double x = (double)w / (double)rc.w;
   double y = (double)h / (double)rc.h;
   align(this, x, y);
  }
  void onAction(kgmGui* g, uint a){
  }
};
const char Gui::className[] = "Gui";
kgmLuaBind<Gui>::RegType Gui::methods[] = {
 {"class", &Gui::_class},
 {"show",  &Gui::_show},
 {0, 0}
};


class Actor: public kgmActor{
public:
 static const char className[];
 static kgmLuaBind<Actor>::RegType methods[];
public:
 Actor(lua_State* l):kgmActor(){

 }
 ~Actor(){

 }
 int _class(lua_State* l){
  lua_pushlightuserdata(l, this);
  return 1;
 }
};
const char Actor::className[] = "Actor";
kgmLuaBind<Actor>::RegType Actor::methods[] = {
 {"class", &Actor::_class},
 {0, 0}
};


void kgmLuaInit(lua_State* lua){
 kgmLuaBind<Gui>::Register(lua);
 kgmLuaBind<Actor>::Register(lua);
}


