#pragma once
#include "../kgmMain/kgmInterface.h"
#include "../kgmMain/kgmIGraphics.h"
#include "../kgmSystem/kgmWindow.h"
#include "kgmNode.h"

class kgmGraphics: public kgmInterface
{
  kgmIGraphics*     gc;
  kgmWindow*        wnd;
  kgmNode*          node;
  kgmList<kgmGui*>  guis;

  int               fwidth,
                    fheight;
  float             znear,
                    zfar;

  kgmCamera*        camera;
  kgmAnimation*     animation;
  kgmSkeleton*      skeleton;
  mtx4*             mtransform;
  void*             tdiffuse;
  void*             tspecular;
  void*             tnormal;
  void*             tshadow;
  void*             shader;
public:
  kgmGraphics(kgmWindow* w);
  ~kgmGraphics();

  void optimize();
  void render();
  void resize(int, int);

  void add(kgmNode*);
  void add(kgmGui*);

  void erase(kgmNode*);
  void erase(kgmGui*);

protected:
  void render(kgmNode*);
};
