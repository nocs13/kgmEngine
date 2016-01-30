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
class   kgmTexture;

class kgmGui: public kgmEvent
{
  KGM_OBJECT(kgmGui);

public:
  typedef void*             Option;
  typedef kgmVector2d<s32>  Point;
  typedef kgmRect2d<s32>    Rect;
  typedef kgmTexture*       Icon;
  typedef kgmTexture*       Image;
  typedef u32               Color;
  
  struct Styles
  {
    u32 fgColor;
    u32 bgColor;
    u32 acColor;
  };
  
protected:
  enum OptionValue
  {
    OptNone,
    OptRect,
    OptText,
    OptXData,
    OptColor,
    OptBgColor,
    OptFgColor,
    OptAcColor
  };

  struct Base
  {
    Rect rect;
    bool scale;
  };
  
  bool       m_erase;
public:
  typedef    kgmList<kgmGui*> Guis;
  Guis       m_childs;

public:
  u32        m_id;       //numeric id
  kgmString  m_sid;      //string id
  kgmGui*    m_parent;   //parent window whom send messages
  kgmGui*    m_focus;    //current child active window
  Rect       m_rect;     //rect of window
  bool       m_view;     //view status of window
  bool       m_useStyle; //for draw manager, use predefined style
  bool       m_hasMouse; //is mouse inside widget
  bool       m_hasInput; //is input(keyboard/joystick) inside widget
  bool       m_alpha;    //test alpha in color
  bool       m_freeze;   //have or not input events
  bool       m_resize;   //should be gui resizable
  bool       m_point;    //should get input even point is out
  kgmString  m_text;     //gui text
  Color      m_color;    //gui bg color
  Image      m_image;    //gui bg texture.
  s32        m_fontsize; // font size. -1 default
  void*      m_xdata;

protected:

private:
  Base          m_base;
  kgmIGuiDraw*  m_draw;
  
public:
  kgmGui();
  kgmGui(kgmGui *par, int x, int y, int w, int h);
  virtual ~kgmGui();

  void show();
  void hide();
  void close();
  void scale(float, float);
  void resize(int, int);
  void repaint(kgmGui*);
  void setParent(kgmGui*);
  void setRect(Rect&);
  void setRect(u32, u32, u32, u32);
  void getRect(Rect&, bool abs = false);
  void getRect(u32&, u32&, u32&, u32&, bool abs = false);

  Rect getRect() const  { return m_rect; }

  void         erase() { m_erase = true; }
  bool         erased(){ return m_erase; }

  void         setText(kgmString s) { m_text = s;    }
  kgmString    getText()            { return m_text; }

  void         setSid(kgmString  sid){ m_sid = sid;  }
  kgmString    getSid()              { return m_sid; }

  kgmGui*      getRoot();
  kgmGui*      getById(u32 id);
  kgmGui*      getBySid(kgmString sid);
  kgmGui*      getFocus();
  bool         setFocus(kgmGui*);

  void         setXdata(void* x){ m_xdata = x;    }
  void*        getXdata()       { return m_xdata; }

  bool         alpha()        { return m_alpha; }
  void         alpha(bool  a) { m_alpha = a;    }

  void         setColor(Color c) { m_color = c;    }
  void         setImage(Image i) { m_image = i;    }
  void         useStyle(bool  s) { m_useStyle = s; }

  void         freeze(bool freeze) { m_freeze = freeze; }
  bool         visible(){ return m_view; }

  s32          getFontSize() const { return m_fontsize; }
  void         setFontSize(s32 sz) { m_fontsize = sz;   }

  // MESSAGE MANAGER
  virtual void onEvent(kgmEvent::Event* e);
  virtual void onIdle();

public:
  Rect toAbsolute(Rect rect)
  {
    Point pt = toAbsolute (Point(rect.x, rect.y));

    return Rect(pt.x, pt.y, rect.w, rect.h);
  }

  Point toAbsolute(Point pt)
  {
    kgmGui* parent = m_parent;

    while(parent)
    {
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
  virtual void onResize(int w, int h);

  //useful functions
  void          addChild(kgmGui *e);
  void          delChild(kgmGui *e);
  bool          isChild(kgmGui *e);
  kgmGui*       getFocus(Point pos);
  kgmGui*       getPointed();

  void          setId(u32 id){ this->m_id = id; };
  u32           getId(){ return this->m_id; };

  //*************** DRAWING ***************
  //void gcDrawRect(kgmIGC* gc, Rect rc, u32 col, void* tex);
  //void gcDrawText(kgmIGC* gc, kgmFont* font, u32 fwidth, u32 fheight, u32 fcolor, Rect clip, kgmString& text);
};




