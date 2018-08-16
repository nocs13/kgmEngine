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
  struct ClipPlane
  {
    bool on     = false;
    vec4 plane  = vec4(0, 0, 1, 0);
  };

protected:
  kgmGraphics* gr;
  kgmIGC*      gc;
  kgmTexture*  font;

  ClipPlane  m_clip_planes[1];

private:
  kgmShader* m_sh_base = null;

public:
  BaseRender(kgmGraphics* gr);
  virtual ~BaseRender(){}

  void render(kgmCamera*, kgmIGraphics::INode*);

  virtual void build() {}
  virtual void clear() {}
  virtual void render() {}

  void setClipPlane(bool st, u32 id, vec4 cp)
  {
    id = 0;

    m_clip_planes[id].on = st;
    m_clip_planes[id].plane = cp;
  }

protected:
  void draw(kgmMesh*);
  void draw(kgmVisual*);
  void draw(kgmParticles*);
  void draw(kgmIcon*, kgmCamera*);
};

#endif // BASERENDER_H
