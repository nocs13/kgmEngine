#pragma once

class kgmSprite: public kgmObject{
public:
 enum Type{
  Type_None,
  Type_Rect,
  Type_Billboard,
 };

public:
 kgmSprite();
 ~kgmSprite();
};