#ifndef KVIEWOPTIONS_H
#define KVIEWOPTIONS_H

#include "kNode.h"
#include "../../kgmGraphics/kgmGui.h"
#include "../../kgmGraphics/kgmGuiTab.h"
#include "../../kgmGraphics/kgmGuiLabel.h"
#include "../../kgmGraphics/kgmGuiCheck.h"
#include "../../kgmBase/kgmCallback.h"

namespace kgmGameEditor
{

class kViewOptions : public kgmGui
{
//  kgmObject(kViewObject);

  kgmGuiButton* btn_close;
  kgmGuiTab*    tab;

protected:
  kNode*        node;
  u32           y_coord;

public:
  typedef kgmCallback<void(void*)> CloseCallback;

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
  void onSelectCollision(bool);
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
    typedef kgmCallback<void(void*, kgmString, kgmString)> kChangeEventCallback;

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

    void dataChange(kgmString s)
    {
      if(kcallback.hasObject() && kcallback.hasFunction())
        kcallback(kcallback.getObject(), getSid(), getText());
    }
  };
public:
  kViewOptionsForObject(kNode* n, int x, int y, int w, int h);

  void updateVariable(kgmString id, kgmString data);
};

class kViewOptionsForTrigger : public kViewOptions
{
public:
  kViewOptionsForTrigger(kNode* n, int x, int y, int w, int h);

  void setChanels(kgmString s);
  void setTarget(kgmString s);
};

}

#endif // KVIEWOPTIONS_H
