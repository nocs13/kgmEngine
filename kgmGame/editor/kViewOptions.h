#ifndef KVIEWOPTIONS_H
#define KVIEWOPTIONS_H

#include "kNode.h"
#include "../../kgmGraphics/kgmGuiFrame.h"
#include "../../kgmGraphics/kgmGuiTab.h"
#include "../../kgmGraphics/kgmGuiLabel.h"
#include "../../kgmGraphics/kgmGuiCheck.h"
#include "../../kgmBase/kgmCallback.h"

namespace kgmGameEditor
{

class kViewOptions : public kgmGuiFrame
{
  KGM_OBJECT(kViewOptions);

  kgmGuiButton* btn_close;

protected:
  kgmGuiTab*    tab;

  kNode*        node;
  u32           y_coord;

public:
  typedef kgmCallback<void, kgmObject*> CloseCallback;

  CloseCallback callClose;
public:
  kViewOptions(kNode* n, int x, int y, int w, int h);

  void onAction(kgmGui* from, u32 arg);
  void onCloseOptions();
  __stdcall void onPositionX(kgmString s);
  __stdcall void onPositionY(kgmString s);
  __stdcall void onPositionZ(kgmString s);
  __stdcall void onRotationX(u32 s);
  __stdcall void onRotationY(u32 s);
  __stdcall void onRotationZ(u32 s);
  __stdcall void onSelectLock(bool);
  __stdcall void onSelectCollision(bool);
  __stdcall void onSelectShapeBox(bool);
  __stdcall void onSelectShapeMesh(bool);
  __stdcall void onSelectShapePlane(bool);
  __stdcall void onSelectShapeSphere(bool);
  __stdcall void onSelectShapeCylinder(bool);
  __stdcall void onSelectShapeConvex(bool);
  __stdcall void onBoundX(kgmString s);
  __stdcall void onBoundY(kgmString s);
  __stdcall void onBoundZ(kgmString s);

  void setCloseCallback(CloseCallback cfn)
  {
    callClose = cfn;
  }


};

class kFileDialog;

class kViewOptionsForMesh : public kViewOptions
{
  kFileDialog* fd;
  kgmGuiText*  guiMtlText;
  kgmGuiText*  guiShdText;

public:
  kViewOptionsForMesh(kNode* n, int x, int y, int w, int h);

  __stdcall void onSelectFailed();
  __stdcall void onSelectMaterial();
  __stdcall void onSelectedMaterial();
  __stdcall void onSelectShader();
  __stdcall void onSelectedShader();
};

class kViewOptionsForLight : public kViewOptions
{
public:
  kViewOptionsForLight(kNode* n, int x, int y, int w, int h);

  __stdcall void setIntencity(kgmString s);
  __stdcall void setShadows(bool s);
};

class kViewOptionsForSensor : public kViewOptions
{
public:
  kViewOptionsForSensor(kNode* n, int x, int y, int w, int h);

  void setTarget(kgmString s);
};

class kViewOptionsForObject : public kViewOptions
{
  class kGuiText: public kgmGuiText
  {
  public:
    typedef kgmCallback<void, kgmObject*, kgmString, kgmString> kChangeEventCallback;

  private:
    kChangeEventCallback kcallback;

  public:
    kGuiText()
      : kgmGuiText(), kcallback(null, null)
    {
      ((kgmGuiText*)this)->setChangeEventCallback(kgmGuiText::ChangeEventCallback(this, (kgmGuiText::ChangeEventCallback::Function)&kGuiText::dataChange));
    }

    kGuiText(kgmGui* parent, int x, int y, int w, int h)
      : kgmGuiText(parent, x, y, w, h), kcallback(null, null)
    {
      ((kgmGuiText*)this)->setChangeEventCallback(kgmGuiText::ChangeEventCallback(this, (kgmGuiText::ChangeEventCallback::Function)&kGuiText::dataChange));
    }

    void setChangeEventCallback(kChangeEventCallback call)
    {
      kcallback = call;
    }

    __stdcall void dataChange(kgmString s)
    {
      if(kcallback.hasFunction())
        kcallback(getSid(), getText());
    }
  };
public:
  kViewOptionsForObject(kNode* n, int x, int y, int w, int h);

  __stdcall void updateVariable(kgmString id, kgmString data);
};

class kViewOptionsForActor : public kViewOptionsForObject
{
public:
  kViewOptionsForActor(kNode* n, int x, int y, int w, int h);
};

class kViewOptionsForTrigger : public kViewOptions
{
public:
  kViewOptionsForTrigger(kNode* n, int x, int y, int w, int h);

  __stdcall void setChanels(kgmString s);
  __stdcall void setTarget(kgmString s);
};

class kViewOptionsForObstacle : public kViewOptions
{
  kFileDialog* fd;
  kgmGuiText*  guiCnvText;

public:
  kViewOptionsForObstacle(kNode* n, int x, int y, int w, int h);

  __stdcall void onSelectConvex();
  __stdcall void onSelectedConvex();
};
}

#endif // KVIEWOPTIONS_H
