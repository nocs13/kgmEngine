#ifndef KGMICON_H
#define KGMICON_H

#include "../kgmBase/kgmObject.h"
#include "../kgmBase/kgmPointer.h"
#include "kgmTexture.h"

class kgmTexture;

class kgmIcon: public kgmObject
{
  kgmTexture* icon = null;

  float       width,
              height;
  vec3        position;

public:
  kgmIcon()
  {
    icon = null;
    width = 0.2;
    height = 0.2;
    position = vec3(0, 0, 0);
  }

  kgmIcon(kgmTexture* c, float w = 0.2, float h = 0.2, vec3 v = vec3(0, 0, 0))
  {
    icon = c;
    width = w;
    height = h;
    position = v;
  }

  kgmIcon(const kgmIcon& icn);

  ~kgmIcon()
  {
  }

  kgmTexture* getIcon()
  {
    return icon;
  }

  float       getWidth() { return width; }
  float       getHeight() { return height; }
  vec3        getPosition() { return position; }
  void        setPosition(vec3 v) { position = v; }
};

#endif // KGMICON_H
