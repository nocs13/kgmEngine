#include "BaseRender.h"

class PhongRender: public BaseRender
{
public:
  PhongRender(kgmGraphics* gr);
  ~PhongRender();

  void render();

};