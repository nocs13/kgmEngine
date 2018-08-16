#ifndef BASERENDER_H
#define BASERENDER_H

#include "../../kgmBase/kgmIGC.h"
#include "../kgmGui.h"
#include "../kgmIGraphics.h"

class kgmGraphics;
class kgmVisual;
class kgmLight;
class kgmMesh;
class kgmIcon;
class kgmShader;
class kgmMaterial;
class kgmParticles;
class kgmCamera;

class BaseRender
{
protected:
  kgmGraphics* gr;
  kgmIGC*      gc;
  kgmTexture*  font;

private:
  kgmShader* m_sh_base = null;

public:
  BaseRender(kgmGraphics* gr);
  virtual ~BaseRender(){}

  void render(kgmCamera*, kgmIGraphics::INode*);

  virtual void build() {}
  virtual void clear() {}
  virtual void render() {}

protected:
  void draw(kgmMesh*);
  void draw(kgmVisual*);
  void draw(kgmParticles*);
  void draw(kgmIcon*, kgmCamera*);
};

#endif // BASERENDER_H
