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
  kgmGuiTab*    tab;

protected:
  kNode*        node;
  u32           y_coord;

public:
  typedef kgmCallback<void, kgmObject*> CloseCallback;

  CloseCallback callClose;
public:
  kViewOptions(kNode* n, int x, int y, int w, int h);

  void onAction(kgmGui* from, u32 arg);
  void onCloseOptions();
  void onPositionX(kgmString s);
  void onPositionY(kgmString s);
  void onPositionZ(kgmString s);
  void onRotationX(u32 s);
  void onRotationY(u32 s);
  void onRotationZ(u32 s);
  void onSelectLock(bool);
  void onSelectCollision(bool);

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
public:
  kViewOptionsForMesh(kNode* n, int x, int y, int w, int h);

  void onSelectFailed();
  void onSelectMaterial();
  void onSelectedMaterial();
};

class kViewOptionsForLight : public kViewOptions
{
public:
  kViewOptionsForLight(kNode* n, int x, int y, int w, int h);

  void setIntencity(kgmString s);
};

class kViewOptionsForActor : public kViewOptions
{
public:
  kViewOptionsForActor(kNode* n, int x, int y, int w, int h);
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

class kViewOptionsForTrigger : public kViewOptions
{
public:
  kViewOptionsForTrigger(kNode* n, int x, int y, int w, int h);

  __stdcall void setChanels(kgmString s);
  __stdcall void setTarget(kgmString s);
};

}

#endif // KVIEWOPTIONS_H
