#pragma once
#include "kgmGui.h"


class kgmGuiButton: public kgmGui
{
 enum Type{
  TypeNormal,
  TypeCheck,
  TypeRadio,
 };

 enum Act{
  ActNone,
  ActFocus,
  ActClick
 };

 Type  m_type;
 Act   m_act;
public:
 kgmGuiButton();
 kgmGuiButton(kgmGui *par, uint x, uint y, uint w, uint h);
 virtual ~kgmGuiButton();
 
protected:
 virtual void onClick();
 virtual void onActivated();
 virtual void onDeactivated();

 void onPaint(kgmIGraphics* gc);
 void onMsLeftUp(int key, int x, int y);
 void onMsLeftDown(int key, int x, int y);
 void onMsMove(int key, int x, int y);
};
