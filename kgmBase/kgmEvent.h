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
enum KEY_CODE
{
  KEY_NONE,
  KEY_ESCAPE,
  KEY_0,
  KEY_1,
  KEY_2,
  KEY_3,
  KEY_4,
  KEY_5,
  KEY_6,
  KEY_7,
  KEY_8,
  KEY_9,
  KEY_SUB,
  KEY_EQU,
  KEY_BACK,
  KEY_TAB,
  KEY_DOT,
  KEY_COMMA,
  KEY_ENTER = 28,
  KEY_A = 30,
  KEY_B,
  KEY_C,
  KEY_D,
  KEY_E,
  KEY_F,
  KEY_G,
  KEY_H,
  KEY_I,
  KEY_J,
  KEY_K,
  KEY_L,
  KEY_M,
  KEY_N,
  KEY_O,
  KEY_P,
  KEY_Q,
  KEY_R,
  KEY_S,
  KEY_T,
  KEY_U,
  KEY_V,
  KEY_W,
  KEY_X,
  KEY_Y,
  KEY_Z,
  KEY_LSHIFT,
  KEY_RSHIFT,
  KEY_LCTRL,
  KEY_RCTRL,
  KEY_LALT,
  KEY_RALT,
  KEY_DELETE,
  KEY_SPACE,
  KEY_F1,
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
  KEY_UP,
  KEY_LEFT,
  KEY_RIGHT,
  KEY_DOWN,
  KEY_PAUSE,

  KEY_MSBLEFT = 128,
  KEY_MSBRIGHT,
  KEY_MSBMIDDLE,

  KEY_END = 131
};


enum EVENTS
{
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
class kgmEvent: public kgmObject
{
  KGM_OBJECT(kgmEvent);

public:
  typedef struct
  {
    u32  event;	       //event id
    int x, y,          //
    width, height;     //for rect
    int msx, msy, msz; //mouse coordinates
    int key, keyMask;	 //keyboard key and mask
    void* gc;	         //graphical context
    void* xparam;	     //additional parameter
  } Event;

public:
  kgmEvent()
  {
  }

  ~kgmEvent();

  virtual void onEvent(kgmEvent::Event* e)
  {
    switch(e->event)
    {
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

  virtual void onAction(kgmEvent* e, int a)
  {
  }

  virtual void onIdle()
  {
  }

protected:
  virtual void onCreate(){}
  virtual void onClose(){}
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
