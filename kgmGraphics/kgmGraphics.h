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

#include "kgmCamera.h"


class kgmGraphics : public kgmInterface, public kgmIGuiDraw, public kgmObject
{
public:
  /*class Node
  {
  public:
    enum Type
    {
      TypeNone,
      TypeGroup,
      TypeMesh,
      TypeLight,
      TypeCamera,
      TypeMaterial,
      TypeTransform,
      TypeAnimation,
    };
    Type type;

    union
    {
      kgmMesh* mesh;
      kgmTransform* transform;
      kgmCamera* camera;
      kgmAnimation* animation;
      kgmMaterial* material;
      kgmLight* light;
    };

  private:
    kgmList<Node*> m_nodes;

  public:
    Node() :
      type(TypeNone)
    {
    }

    Node(kgmMesh* m) :
      type(TypeMesh), mesh(m)
    {
    }

    Node(kgmLight* m) :
      type(TypeLight), light(m)
    {
    }

    Node(kgmMaterial* m) :
      type(TypeMaterial), material(m)
    {
    }

    Node(kgmTransform* m) :
      type(TypeTransform), transform(m)
    {
    }

    Node(kgmCamera* m) :
      type(TypeCamera), camera(m)
    {
    }

    Node(kgmAnimation* m) :
      type(TypeAnimation), animation(m)
    {
    }

    virtual
    ~Node()
    {
      for (int i = 0; i < m_nodes.size(); i++)
        delete m_nodes[i];
      m_nodes.clear();
    }

    virtual void add(Node* n) { m_nodes.add(n); }

    virtual void erase(Node* n) { }

    virtual int nodes() { return m_nodes.size(); }

    virtual Node* node(int i) { return (i < m_nodes.size()) ? (m_nodes[i]) : (0); }
  };*/

  kgmIResources* rc;
  kgmIGraphics* gc;
  kgmWindow* wnd;
  kgmFont* font;

  kgmNode* node;
  kgmList<kgmGui*> guis;
  kgmGuiStyle* gui_style;

  int fwidth, fheight;
  float znear, zfar;

  kgmCamera* camera;
  kgmMaterial* material;
  kgmAnimation* animation;
  kgmSkeleton* skeleton;
  mtx4 mtransform;
  void* tdiffuse;
  void* tspecular;
  void* tnormal;
  void* tdepth;
  void* shader;

  kgmTab<u16, kgmShader*> shaders;
  kgmTab<u16, void*> textures;

  kgmList<kgmLight*> a_lights;

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
  add(kgmNode*);
  void
  erase(kgmNode*);

  void
  add(kgmGui*);
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
