#include "BaseRender.h"

class SceneRenderBasic: public BaseRender
{
  public:
  SceneRenderBasic(kgmGraphics* gr);
  ~SceneRenderBasic();

  virtual void preRender();
  virtual void Render();
  virtual void postRender();

};