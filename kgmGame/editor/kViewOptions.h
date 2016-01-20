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

class kFileDialog;
class kViewObjects;

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

  void setCloseCallback(CloseCallback cfn)
  {
    callClose = cfn;
  }
};

class kViewOptionsForMaterial : public kViewOptions
{
  enum Mode
  {
    Mode_None,
    Mode_Shader,
    Mode_Color,
    Mode_Normal,
    Mode_Specular
  };

  Mode mode;

  Slot<kViewOptionsForMaterial, int> slotReset;
  Slot<kViewOptionsForMaterial, int> slotSelectShader;

  Slot<kViewOptionsForMaterial, int> slotSelectColor;
  Slot<kViewOptionsForMaterial, int> slotSelectNormal;
  Slot<kViewOptionsForMaterial, int> slotSelectSpecular;

  Slot<kViewOptionsForMaterial, bool> slotSelectAlpha;

  kgmGuiText*  guiTextTexColor;
  kgmGuiText*  guiTextTexNormal;
  kgmGuiText*  guiTextTexSpecular;
  kgmGuiText*  guiTextShader;

public:
  kViewOptionsForMaterial(kNode* n, int x, int y, int w, int h);

  __stdcall void onReset(int);
  __stdcall void onSelectFailed(kFileDialog*);
  __stdcall void onSelectSuccess(kFileDialog*);
  __stdcall void onSelectShader(int);
  __stdcall void onSelectTexColor(int);
  __stdcall void onSelectTexNormal(int);
  __stdcall void onSelectTexSpecular(int);
  __stdcall void onShininess(u32 s);
  __stdcall void onTransparency(u32 s);
  __stdcall void onAlpha(bool s);
};

class kViewOptionsForVisual : public kViewOptions
{
  Slot<kViewOptionsForVisual, int>       slotSelectMaterial;
  Slot<kViewOptionsForVisual, bool>      slotParticlesLoop;
  Slot<kViewOptionsForVisual, kgmString> slotParticlesCount;
  Slot<kViewOptionsForVisual, kgmString> slotParticlesSpeed;
  Slot<kViewOptionsForVisual, kgmString> slotParticlesAngle;
  Slot<kViewOptionsForVisual, kgmString> slotParticlesLife;
  Slot<kViewOptionsForVisual, kgmString> slotParticlesSize;

  kgmGuiText* vis_text = null;

public:
  kViewOptionsForVisual(kNode* n, int x, int y, int w, int h);

  __stdcall void onShowMaterials(int);
  __stdcall void onSelectMaterial(kgmString);
  __stdcall void onParticlesLoop(bool);
  __stdcall void onParticlesCount(kgmString);
  __stdcall void onParticlesSpeed(kgmString);
  __stdcall void onParticlesAngle(kgmString);
  __stdcall void onParticlesLife(kgmString);
  __stdcall void onParticlesSize(kgmString);
};

class kViewOptionsForLight : public kViewOptions
{
public:
  kViewOptionsForLight(kNode* n, int x, int y, int w, int h);

  __stdcall void setIntencity(kgmString s);
  __stdcall void setShadows(bool s);
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

  __stdcall void onSelectEnable(bool);
  __stdcall void updateVariable(kgmString id, kgmString data);
};

class kViewOptionsForSensor : public kViewOptionsForObject
{
public:
  kViewOptionsForSensor(kNode* n, int x, int y, int w, int h);

  void setTarget(kgmString s);
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

  __stdcall void onSelectPolygons();
  __stdcall void onSelectedPolygons();
  __stdcall void onScale(kgmString);
  __stdcall void onRect();
  __stdcall void onBox();
};

class kViewOptionsForUnit : public kViewOptionsForObject
{
public:
  kViewOptionsForUnit(kNode* n, int x, int y, int w, int h);
};

class kViewOptionsForEffect : public kViewOptionsForObject
{

public:
  kViewOptionsForEffect(kNode* n, int x, int y, int w, int h);
};

class kViewOptionsForActor : public kViewOptionsForObject
{
  kViewObjects *vo;
  kgmGuiText   *guiState;

public:
  kViewOptionsForActor(kNode* n, int x, int y, int w, int h);

  __stdcall void showStates();
  __stdcall void onState(kgmString);
  __stdcall void onPlayer(bool);
};

}

#endif // KVIEWOPTIONS_H
