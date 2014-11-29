#ifndef KGMSURFACE_H
#define KGMSURFACE_H

#include "../kgmBase/kgmObject.h"

class kgmSurface : public kgmObject
{
  KGM_OBJECT(kgmSurface);

  float x_size, y_size;
  u32   x_divs, y_divs;

  kgmArray<float> z_parts;

protected:
  ~kgmSurface();

public:
  kgmSurface();
  kgmSurface(float xsize, float ysize, u32 xdivs, u32 ydivs, float* data);

  void reset();

  bool setDimension(float xsize, float ysize, u32 xdivs, u32 ydivs);
  bool setHeight(float height, u32 x, u32 y);
};

#endif // KGMSURFACE_H
