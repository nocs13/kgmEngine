#ifndef KGMOBSTACLE_H
#define KGMOBSTACLE_H

#include "../kgmBase/kgmObject.h"

class kgmObstacle : public kgmObject
{
  KGM_OBJECT(kgmObstacle);

  enum
  {
    ShapeNone,
    ShapeBox,
    ShapeRect
  };

  u32 shape;

  float sx, sy,sz;

public:
  kgmObstacle();
  kgmObstacle(float x, float y, float z);

  void setSx(float x) { sx = x; }
  void setSy(float y) { sy = y; }
  void setSz(float z) { sz = z; }

  float getSx() const { return sx; }
  float getSy() const { return sy; }
  float getSz() const { return sz; }
};

#endif // KGMOBSTACLE_H
