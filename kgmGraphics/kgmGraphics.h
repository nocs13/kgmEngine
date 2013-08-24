#pragma once
#include "../kgmBase/kgmInterface.h"
#include "../kgmBase/kgmIGraphics.h"
#include "../kgmBase/kgmIResources.h"
#include "../kgmBase/kgmTab.h"
#include "../kgmSystem/kgmWindow.h"

#include "kgmNode.h"
#include "kgmGui.h"
#include "kgmGuiTab.h"
#include "kgmGuiList.h"
#include "kgmGuiButton.h"
#include "kgmGuiScroll.h"
#include "kgmGuiProgress.h"
#include "kgmGuiStyle.h"
#include "kgmIGuiDraw.h"

#include "kgmLight.h"
#include "kgmCamera.h"


class kgmGraphics : public kgmInterface, public kgmIGuiDraw, public kgmObject
{
public:
  kgmIResources* rc;
  kgmIGraphics* gc;
  kgmWindow* wnd;
  kgmFont* font;

  kgmNode* node;

  kgmList<kgmMaterial*> materials;
  kgmList<kgmLight*>    lights;
  kgmList<kgmMesh*>     meshes;
  kgmList<kgmNode*>     nodes;
  kgmList<kgmGui*>      guis;
  kgmGuiStyle*          gui_style;

  int fwidth, fheight;
  float znear, zfar;

  kgmLight*     light;
  kgmCamera*    camera;
  kgmMaterial*  material;
  kgmSkeleton*  skeleton;
  kgmAnimation* animation;

  mtx4  mtransform;

  void* tdiffuse;
  void* tspecular;
  void* tnormal;
  void* tdepth;
  void* shader;

  kgmTab<u16, kgmShader*> shaders;
  kgmTab<u16, void*> textures;

  kgmCamera  mcamera;

public:
  kgmGraphics(kgmWindow* w, kgmIResources* r = 0);
  ~kgmGraphics();

  void
  build();
  void
  render();
  void
  resize(int, int);

  void
  erase(kgmGui*);

  void
  setActiveCamera(kgmCamera*);

  void
  setDefaultFont(kgmFont*);

  bool
  registration(u32 id, kgmShader* sh)
  {
    shaders.add((u16) id, sh);
    return true;
  }

  //virtuals
  void
  update(kgmGui* gui)
  {
    render(gui);
  }

  kgmCamera& mainCamera(){
      return mcamera;
  }

  void add(kgmGui* g){ guis.add(g); }
  void add(kgmNode* n){ if(n) this->nodes.add(n); }
  void add(kgmMesh* m){ if(m) this->meshes.add(m); }
  void add(kgmLight* l){ if(l) this->lights.add(l); }
  void add(kgmMaterial* m){ if(m) this->materials.add(m); }

protected:
  void
  set(kgmCamera*);
  void
  set(kgmMaterial*);
  void
  set(kgmShader*, u32);

  void
  render(kgmNode*);
  void
  render(kgmGui*);
  void
  render(kgmGuiTab*);
  void
  render(kgmGuiList*);
  void
  render(kgmGuiButton*);
  void
  render(kgmGuiScroll*);
  void
  render(kgmGuiProgress*);

  //
  void
  gcDrawRect(kgmGui::Rect rc, u32 col, void* tex);
  void
  gcDrawText(kgmFont* font, u32 fwidth, u32 fheight, u32 fcolor,
    kgmGui::Rect clip, kgmString& text);
};
