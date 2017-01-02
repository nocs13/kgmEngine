#ifndef KVIEWOPTIONS_H
#define KVIEWOPTIONS_H

#include "../../kgmGraphics/kgmGuiFrame.h"
#include "../../kgmGraphics/kgmGuiTab.h"
#include "../../kgmGraphics/kgmGuiLabel.h"
#include "../../kgmGraphics/kgmGuiCheck.h"
#include "../../kgmGraphics/kgmGuiSelect.h"
#include "../../kgmGraphics/kgmGuiFileDialog.h"
#include "../kgmGameNode.h"

namespace kgmGameEditor
{

class kViewObjects;

class kViewOptions : public kgmGuiFrame
{
  KGM_OBJECT(kViewOptions);

  kgmGuiButton* btn_close;

  Slot<kViewOptions, kgmString> slotRotationX;
  Slot<kViewOptions, kgmString> slotRotationY;
  Slot<kViewOptions, kgmString> slotRotationZ;

protected:
  static kViewOptions* single;

  kgmGuiTab*  tab;

  kgmGameNode*  node;
  u32     y_coord;

public:
  Signal<> sigClose;

protected:
  kViewOptions(kgmGameNode* n, int x, int y, int w, int h);
  ~kViewOptions() { kViewOptions::single = null; }

public:
  static kViewOptions* getDialog(kgmGameNode* n, int x, int y, int w, int h)
  {
    if(kViewOptions::single)
      return null;

    return (kViewOptions::single = new kViewOptions(n, x, y, w, h));
  }

  void onAction(kgmGui* from, u32 arg);
  void onCloseOptions();
  __stdcall void onNodeName(kgmString s);
  __stdcall void onPositionX(kgmString s);
  __stdcall void onPositionY(kgmString s);
  __stdcall void onPositionZ(kgmString s);
  __stdcall void onRotationX(kgmString s);
  __stdcall void onRotationY(kgmString s);
  __stdcall void onRotationZ(kgmString s);
  __stdcall void onSelectLock(bool);
};

class kViewOptionsForVisual : public kViewOptions
{
  Slot<kViewOptionsForVisual, int>       slotSelectMaterial;
  Slot<kViewOptionsForVisual, bool>      slotParticlesLoop;
  Slot<kViewOptionsForVisual, bool>      slotParticlesFade;
  Slot<kViewOptionsForVisual, kgmString> slotParticlesCount;
  Slot<kViewOptionsForVisual, kgmString> slotParticlesSpeed;
  Slot<kViewOptionsForVisual, kgmString> slotParticlesAngle;
  Slot<kViewOptionsForVisual, kgmString> slotParticlesLife;
  Slot<kViewOptionsForVisual, kgmString> slotParticlesSize;
  Slot<kViewOptionsForVisual, kgmString> slotParticlesEsize;
  Slot<kViewOptionsForVisual, kgmString> slotParticlesDivSpeed;
  Slot<kViewOptionsForVisual, kgmString> slotParticlesDivLife;

  kgmGuiText* vis_text = null;

protected:
  kViewOptionsForVisual(kgmGameNode* n, int x, int y, int w, int h);

public:
  static kViewOptionsForVisual* getDialog(kgmGameNode* n, int x, int y, int w, int h)
  {
    if(kViewOptions::single)
      return null;

    return (kViewOptionsForVisual*) (kViewOptions::single = new kViewOptionsForVisual(n, x, y, w, h));
  }

  __stdcall void onShowMaterials(int);
  __stdcall void onSelectMaterial(kgmString);
  __stdcall void onParticlesLoop(bool);
  __stdcall void onParticlesFade(bool);
  __stdcall void onParticlesCount(kgmString);
  __stdcall void onParticlesSpeed(kgmString);
  __stdcall void onParticlesAngle(kgmString);
  __stdcall void onParticlesLife(kgmString);
  __stdcall void onParticlesSize(kgmString);
  __stdcall void onParticlesEsize(kgmString);
  __stdcall void onParticlesDivSpeed(kgmString);
  __stdcall void onParticlesDivLife(kgmString);
};

class kViewOptionsForLight : public kViewOptions
{
  Slot<kViewOptionsForLight, kgmString> slotColorR;
  Slot<kViewOptionsForLight, kgmString> slotColorG;
  Slot<kViewOptionsForLight, kgmString> slotColorB;
  Slot<kViewOptionsForLight, bool>      slotShadows;

protected:
  kViewOptionsForLight(kgmGameNode* n, int x, int y, int w, int h);

public:
  static kViewOptionsForLight* getDialog(kgmGameNode* n, int x, int y, int w, int h)
  {
    if(kViewOptions::single)
      return null;

    return (kViewOptionsForLight*)(kViewOptions::single = new kViewOptionsForLight(n, x, y, w, h));
  }


  __stdcall void onColorR(kgmString);
  __stdcall void onColorG(kgmString);
  __stdcall void onColorB(kgmString);
  __stdcall void setIntencity(kgmString s);
  __stdcall void setShadows(bool s);
};

class kViewOptionsForUnit : public kViewOptions
{
  class kGuiText: public kgmGuiText
  {
  public:
    Slot<kGuiText, kgmString>  slotChange;

  public:
    kGuiText()
      : kgmGuiText()
    {
    }

    kGuiText(kgmGui* parent, int x, int y, int w, int h)
      : kgmGuiText(parent, x, y, w, h)
    {
    }

    __stdcall void dataChange(kgmString s)
    {
    }
  };

  Slot<kViewOptionsForUnit, int>  slotListMeshes;
  Slot<kViewOptionsForUnit, int>  slotListActions;
  Slot<kViewOptionsForUnit, int>  slotListMaterials;

protected:
  kViewOptionsForUnit(kgmGameNode* n, int x, int y, int w, int h);

public:
  static kViewOptionsForUnit* getDialog(kgmGameNode* n, int x, int y, int w, int h)
  {
    if(kViewOptions::single)
      return null;

    return (kViewOptionsForUnit*)(kViewOptions::single = new kViewOptionsForUnit(n, x, y, w, h));
  }


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

  __stdcall void onListActions(int);

  __stdcall void onSelectAction(kgmString);
};

class kViewOptionsForSensor : public kViewOptionsForUnit
{
protected:
  kViewOptionsForSensor(kgmGameNode* n, int x, int y, int w, int h);

public:
  static kViewOptionsForSensor* getDialog(kgmGameNode* n, int x, int y, int w, int h)
  {
    if(kViewOptions::single)
      return null;

    return (kViewOptionsForSensor*)(kViewOptions::single = new kViewOptionsForSensor(n, x, y, w, h));
  }


  void setTarget(kgmString s);
};

class kViewOptionsForTrigger : public kViewOptions
{
protected:
  kViewOptionsForTrigger(kgmGameNode* n, int x, int y, int w, int h);

public:
  static kViewOptionsForTrigger* getDialog(kgmGameNode* n, int x, int y, int w, int h)
  {
    if(kViewOptions::single)
      return null;

    return (kViewOptionsForTrigger*)(kViewOptions::single = new kViewOptionsForTrigger(n, x, y, w, h));
  }


  __stdcall void setChanels(kgmString s);
  __stdcall void setTarget(kgmString s);
};

class kViewOptionsForObstacle : public kViewOptions
{
  kgmGuiFileDialog* fd;
  kgmGuiText*  guiCnvText;

protected:
  kViewOptionsForObstacle(kgmGameNode* n, int x, int y, int w, int h);

public:
  static kViewOptionsForObstacle* getDialog(kgmGameNode* n, int x, int y, int w, int h)
  {
    if(kViewOptions::single)
      return null;

    return (kViewOptionsForObstacle*)(kViewOptions::single = new kViewOptionsForObstacle(n, x, y, w, h));
  }


  __stdcall void onSelectPolygons();
  __stdcall void onSelectedPolygons();
  __stdcall void onScale(kgmString);
  __stdcall void onRect();
  __stdcall void onBox();
};

class kViewOptionsForEffect : public kViewOptionsForUnit
{
protected:
  kViewOptionsForEffect(kgmGameNode* n, int x, int y, int w, int h);

public:
  static kViewOptionsForEffect* getDialog(kgmGameNode* n, int x, int y, int w, int h)
  {
    if(kViewOptions::single)
      return null;

    return (kViewOptionsForEffect*)(kViewOptions::single = new kViewOptionsForEffect(n, x, y, w, h));
  }
};

class kViewOptionsForActor : public kViewOptions
{
  kViewObjects *vo;
  kgmGuiText   *guiState;

protected:
  kViewOptionsForActor(kgmGameNode* n, int x, int y, int w, int h);

public:
  static kViewOptionsForActor* getDialog(kgmGameNode* n, int x, int y, int w, int h)
  {
    if(kViewOptions::single)
      return null;

    return (kViewOptionsForActor*)(kViewOptions::single = new kViewOptionsForActor(n, x, y, w, h));
  }


  __stdcall void showStates();
  __stdcall void onState(kgmString);
  __stdcall void onPlayer(bool);
};

}

#endif // KVIEWOPTIONS_H
