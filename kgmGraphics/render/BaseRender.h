#ifndef BASERENDER_H
#define BASERENDER_H

#include "../../kgmBase/kgmIGC.h"
#include "../kgmGui.h"
#include "../kgmIGraphics.h"
#include "../kgmMaterial.h"
#include "../kgmFont.h"

class kgmGraphics;
class kgmLight;
class kgmMesh;
class kgmText;
class kgmIcon;
class kgmShader;
class kgmMaterial;
class kgmParticles;
class kgmCamera;

class BaseRender: public kgmObject
{
  public:

  struct ClipPlane
  {
    bool on     = false;
    vec4 plane  = vec4(0, 0, 1, 0);
  };

  struct OLight
  {
    kgmIGraphics::INode* l;
    f32    f;
  };

  struct OMesh
  {
    kgmIGraphics::INode* l;
    f32    d;
  };

protected:
  kgmGraphics* gr;
  kgmIGC*      gc;
  kgmTexture*  font;
  kgmMaterial  mtl;

  ClipPlane  m_clip_planes[1];

private:
  kgmShader* m_sh_base = null;

public:
  BaseRender(kgmGraphics* gr);
  ~BaseRender(){}

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
  void draw(kgmText*);
  void draw(kgmParticles*);
  void draw(kgmIcon*, kgmCamera*);

  void gcDrawRect(kgmIGC* gc, kgmGui::Rect rc, u32 col, kgmTexture* tex);
  void gcDrawBorder(kgmIGC* gc, kgmGui::Rect rc, u32 col, kgmTexture* tex);
  void gcDrawText(kgmIGC* gc, kgmFont* font, u32 fwidth, u32 fheight, u32 fcolor,
                  kgmGui::Rect clip, kgmString& text);

  void material(kgmMaterial*);
  void shader(kgmShader* shd, kgmCamera* cam, kgmMaterial* mtl, kgmIGraphics::INode* nod);
};

#endif // BASERENDER_H
