#pragma once
#include "../kgmMath/kgmMath.h"
typedef unsigned char uchar;
typedef unsigned int  u32;

#define KGM_RGB(r,g,b)    (u32)(((u8)r << 0) | ((u8)g << 8) | ((u8)b << 16))
#define KGM_RGBA(r,g,b,a) (u32)(((u8)r << 0) | ((u8)g << 8) | ((u8)b << 16) | ((u8)a << 24))

class kgm4Color
{
public:
 union
 {
  struct
  {
   uchar r, g, b, a;
  };

  u32 color;
 };

public:
 kgm4Color()
 {
  r = g = b = a = 0;
  color = 0x00000000;
 }

 kgm4Color(uchar x, uchar y, uchar z, uchar w)
 {
  r = x, g = y, b = z, a = w;
 }

 kgm4Color(u32 c)
 {
  color = c;
 }

 void get(vec4& v)
 {
  v = vec4(r / 255.0, g / 255.0, b / 255.0, a / 255.0);
 }
};

typedef kgm4Color color;
