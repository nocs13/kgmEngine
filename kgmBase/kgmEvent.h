#pragma once
#include "kgmBase.h"
#include "kgmObject.h"
#include "kgmList.h"

// KEY BITS
#define KB_KEYCTRL    (0x01 << 0)
#define KB_KEYALT     (0x01 << 1)
#define KB_KEYSHFT    (0x01 << 2)
#define KB_MSBLEFT    (0x01 << 3)
#define KB_MSBRIGHT   (0x01 << 4)
#define KB_MSBMIDLE   (0x01 << 5)


//KEYBOARD
enum KEY_CODE{
 KEY_NONE,
 KEY_ESCAPE,
 KEY_1,
 KEY_2,
 KEY_3,
 KEY_4,
 KEY_5,
 KEY_6,
 KEY_7,
 KEY_8,
 KEY_9,
 KEY_0,
 KEY_SUB,
 KEY_EQU,
 KEY_BACK,
 KEY_TAB,
 KEY_Q,
 KEY_W,
 KEY_E,
 KEY_R,
 KEY_T,
 KEY_Y,
 KEY_U,
 KEY_I,
 KEY_O,
 KEY_P,
 KEY_ENTER = 28,
 KEY_A = 30,
 KEY_S,
 KEY_D,
 KEY_F,
 KEY_G,
 KEY_H,
 KEY_J,
 KEY_K,
 KEY_L,
 KEY_Z,
 KEY_X,
 KEY_C,
 KEY_V,
 KEY_B,
 KEY_N,
 KEY_M,
 KEY_LSHIFT = 42,
 KEY_RSHIFT,
 KEY_LCTRL,
 KEY_RCTRL,
 KEY_LALT,
 KEY_RALT,
 KEY_SPACE = 50,
 KEY_F1 = 59,
 KEY_F2,
 KEY_F3,
 KEY_F4,
 KEY_F5,
 KEY_F6,
 KEY_F7,
 KEY_F8,
 KEY_F9,
 KEY_F10,
 KEY_F11,
 KEY_F12,
 KEY_UP = 72,
 KEY_LEFT = 75,
 KEY_RIGHT = 77,
 KEY_DOWN = 80,
 KEY_PAUSE,

 KEY_MSBLEFT = 128,
 KEY_MSBRIGHT,
 KEY_MSBMIDDLE,
};


enum EVENTS{
 evtNone,
 evtMsWheel,
 evtMsMove,
 evtMsLeftDown,
 evtMsLeftUp,
 evtMsRightDown,
 evtMsRightUp,
 evtMsMiddleDown,
 evtMsMiddleUp,
 evtKeyDown,
 evtKeyUp,
 evtCreate,
 evtClose,
 evtResize,
 evtPaint,
 evtTimer,

 //global events for game app
 evtExtra = 0xff,
};

//base class for system and user events
class kgmEvent: public kgmObject{
  KGM_OBJECT(kgmEvent);
public:
typedef struct{
 u32  event;		//event id
 int x, y,
     width, height; //for rect
 int msx, msy, msz;	//mouse coordinates
 int key, keyMask;	//keyboard key and mask
 void* gc;			//graphical context
 void* xparam;		//additional parameter
} Event;

public:
private:
 kgmList<kgmEvent*> m_listens;	//targets for message delivery
public:
 kgmEvent(){
 }
 ~kgmEvent(){
  m_listens.clear();
 }

 bool addListener(kgmEvent* e){
  int i;
  for(i = 0; i < m_listens.length(); i++)
   if(m_listens[i] == e)
    return false;
  m_listens.add(e);
  return true;
 } 

 bool removeListener(kgmEvent* e){
  int i;
  for(i = 0; i < m_listens.length(); i++)
   if(m_listens[i] == e){
    m_listens.erase(i);
    return true;
   }
  return false;
 }

 virtual void onEvent(kgmEvent::Event* e){
  for(int i = 0; i < m_listens.length(); i++)
   m_listens[i]->onEvent(e);
  switch(e->event){
  case evtCreate:
	  onCreate();
	  break;
  case evtClose:
	  onClose();
	  break;
  case evtPaint:
	  onPaint();
	  break;
  case evtResize:
	  onResize(e->width, e->height);
	  break;
  case evtMsWheel:
	  onMsWheel(e->keyMask, e->msx, e->msy, e->msz);
	  break;
  case evtMsMove:
	  onMsMove(e->keyMask, e->msx, e->msy);
	  break;
  case evtMsLeftDown:
	  onMsLeftDown(e->keyMask, e->msx, e->msy);
	  break;
  case evtMsLeftUp:
	  onMsLeftUp(e->keyMask, e->msx, e->msy);
	  break;
  case evtMsRightDown:
	  onMsRightDown(e->keyMask, e->msx, e->msy);
	  break;
  case evtMsRightUp:
	  onMsRightUp(e->keyMask, e->msx, e->msy);
	  break;
  case evtKeyDown:
	  onKeyDown(e->key);
	  break;
  case evtKeyUp:
	  onKeyUp(e->key);
	  break;
  }
 }

protected:
  
 virtual void onCreate(){}
 virtual void onClose(){}
 virtual void onIdle(){}
 virtual void onPaint(){}
 virtual void onResize(int w, int h){}
 virtual void onMsWheel(int k, int x, int y, int z){}
 virtual void onMsMove(int k, int x, int y){}
 virtual void onMsLeftDown(int k, int x, int y){}
 virtual void onMsLeftUp(int k, int x, int y){}
 virtual void onMsRightDown(int k, int x, int y){}
 virtual void onMsRightUp(int k, int x, int y){}
 virtual void onKeyDown(int k){}
 virtual void onKeyUp(int k){}

};
