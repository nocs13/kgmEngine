#pragma once
#include "kgmBase.h"
#include "../kgmBase/kgmTypes.h"
#include "../kgmBase/kgmEvent.h"
#include "../kgmBase/kgmBase.h"
#include "../kgmBase/kgmList.h"
#include "../kgmBase/kgmString.h"
#include "../kgmBase/kgmTab.h"
#include "../kgmMath/kgmMath.h"

//Class of GUI
class   kgmGui;
class   kgmIGuiDraw;

class kgmGui: public kgmEvent
{
  KGM_OBJECT(kgmGui);
 public:
  typedef void*            Option;
  typedef kgmVector2d<s32> Point;
  typedef kgmRect<s32>     Rect;
  
  struct Styles{
    u32 fgColor;
    u32 bgColor;
    u32 acColor;
  };
  
 protected:
  enum OptionValue{
    OptNone,
    OptRect,
    OptText,
    OptXData,
    OptColor,
    OptBgColor,
    OptFgColor,
    OptAcColor
  };

  struct Base{
    Rect rect;
    bool scale;
  };
  
 public:

 public:
  typedef  kgmList<kgmGui*> Guis;
  Guis	    m_childs;

 public:
  u32       m_id;
  kgmString m_sid;     //string id
  kgmGui*   m_parent;  //parent window whom send messages
  kgmGui*   m_focus;   //current child active window
  Rect      m_rect;    //rect of window
  bool      m_view;    //view status of window
  bool      m_hasMouse; //is mouse inside widget
  bool      m_hasInput; //is input(keyboard/joystick) inside widget
  kgmString  m_text;    //gui text
  void*     m_xdata;
protected:

private:
  Base  m_base;    
  kgmIGuiDraw*  m_draw;
  
public:
 kgmGui();
 kgmGui(kgmGui *par, int x, int y, int w, int h);
 virtual ~kgmGui();

 void show();
 void hide();
 void scale(float, float);
 void resize(int, int);
 void repaint(kgmGui*);
 void setParent(kgmGui*);
 void setRect(Rect&);
 void setRect(u32, u32, u32, u32);
 void getRect(Rect&, bool abs = false);
 void getRect(u32&, u32&, u32&, u32&, bool abs = false);
 void setText(kgmString&);
 void getText(kgmString&);

 //void setStyle(u32, void*);
 //void getStyle(u32, void*);

 void  setXdata(void* x){ m_xdata = x; }
 void* getXdata(){ return m_xdata; }

 // MESSAGE MANAGER
 virtual void onEvent(kgmEvent::Event* e);
 virtual void onAction(kgmGui* from, u32 type);
 virtual void onIdle();

public: //static
  Rect toAbsolute(Rect rect){
    Point pt = toAbsolute (Point(rect.x, rect.y));
    return Rect(pt.x, pt.y, rect.w, rect.h);
  }
  Point toAbsolute(Point pt){
    kgmGui* parent = m_parent;
    while(parent){
        pt.x += parent->m_rect.x;
        pt.y += parent->m_rect.y;
        parent = parent->m_parent;
    }
    return pt;
  }
protected:
 virtual void onMsWheel(int k, int x, int y, int z);
 virtual void onMsMove(int k, int x, int y);
 virtual void onMsLeftDown(int k, int x, int y);
 virtual void onMsLeftUp(int k, int x, int y);
 virtual void onMsRightDown(int k, int x, int y);
 virtual void onMsRightUp(int k, int x, int y);
 virtual void onMsInside();
 virtual void onMsOutside();
 virtual void onKeyDown(int k);
 virtual void onKeyUp(int k);

//useful functions
 void          addChild(kgmGui *e);
 void          delChild(kgmGui *e);
 bool          isChild(kgmGui *e);
 kgmGui*       getFocus(Point pos);

 //*************** DRAWING ***************
 //void gcDrawRect(kgmIGraphics* gc, Rect rc, u32 col, void* tex);
 //void gcDrawText(kgmIGraphics* gc, kgmFont* font, u32 fwidth, u32 fheight, u32 fcolor, Rect clip, kgmString& text);
};




