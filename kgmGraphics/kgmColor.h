#pragma once
#include "../kgmMath/kgmMath.h"
typedef unsigned char uchar;
typedef unsigned int u32;

#define KGM_RGB(r, g, b) (u32)(((u8)r << 0) | ((u8)g << 8) | ((u8)b << 16))
#define KGM_RGBA(r, g, b, a) (u32)(((u8)r << 0) | ((u8)g << 8) | ((u8)b << 16) | ((u8)a << 24))

class kgmColor
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
  kgmColor()
  {
    r = g = b = a = 0;
    color = 0x00000000;
  }

  kgmColor(uchar x, uchar y, uchar z, uchar w)
  {
    r = x, g = y, b = z, a = w;
  }

  kgmColor(u32 c)
  {
    color = c;
  }

  kgmColor(u16 c)
  {
    u8 r5 = ((c & 0xf800) >> 11);
    u8 g6 = ((c & 0x7e0) >> 5);
    u8 b5 = ((c & 0x1f));

    r = (r5 * 527 + 23) >> 6;
  }

  kgmColor(vec4 v)
  {
    r = u8(255.0 * v.x);
    g = u8(255.0 * v.y);
    b = u8(255.0 * v.z);
    a = u8(255.0 * v.w);
  }

  void get(vec4 &v)
  {
    v = vec4(r / 255.0, g / 255.0, b / 255.0, a / 255.0);
  }

  float gray()
  {
    return (r * 0.2125 + g * 0.7154 + b * 0.0721);
  }

  static vec4 toVector(u32 c)
  {
    u8 r, g, b, a;

    r = (u8)(0x000000ff & c);
    g = (u8)(0x000000ff & (c >> 8));
    b = (u8)(0x000000ff & (c >> 16));
    a = (u8)(0x000000ff & (c >> 24));

    return vec4((float)r / 255.f, (float)g / 255.f,
                (float)b / 255.f, (float)a / 255.f);
  }

  static vec3 toVector3(u32 c)
  {
    u8 r, g, b, a;

    r = (u8)(0x000000ff & c);
    g = (u8)(0x000000ff & (c >> 8));
    b = (u8)(0x000000ff & (c >> 16));

    return vec3((float)r / 255.f, (float)g / 255.f, (float)b / 255.f);
  }
};

typedef kgmColor color;
