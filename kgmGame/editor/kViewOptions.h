#ifndef KVIEWOPTIONS_H
#define KVIEWOPTIONS_H

#include "kNode.h"
#include "../../kgmGraphics/kgmGuiFrame.h"
#include "../../kgmGraphics/kgmGuiTab.h"
#include "../../kgmGraphics/kgmGuiLabel.h"
#include "../../kgmGraphics/kgmGuiCheck.h"
#include "../../kgmGraphics/kgmGuiSelect.h"
#include "../../kgmGraphics/kgmGuiFileDialog.h"

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

  kNode*  node;
  u32     y_coord;

public:
  Signal<> sigClose;

protected:
  kViewOptions(kNode* n, int x, int y, int w, int h);
  ~kViewOptions() { kViewOptions::single = null; }

public:
  static kViewOptions* getDialog(kNode* n, int x, int y, int w, int h)
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

  Slot<kViewOptionsForMaterial, bool> slotSelectCull;
  Slot<kViewOptionsForMaterial, bool> slotSelectAlpha;
  Slot<kViewOptionsForMaterial, bool> slotSelectShade;
  Slot<kViewOptionsForMaterial, bool> slotSelectDepth;
  Slot<kViewOptionsForMaterial, bool> slotSelectBlend;

  Slot<kViewOptionsForMaterial, kgmString> slotColorR;
  Slot<kViewOptionsForMaterial, kgmString> slotColorG;
  Slot<kViewOptionsForMaterial, kgmString> slotColorB;
  Slot<kViewOptionsForMaterial, kgmString> slotSpecularR;
  Slot<kViewOptionsForMaterial, kgmString> slotSpecularG;
  Slot<kViewOptionsForMaterial, kgmString> slotSpecularB;
  Slot<kViewOptionsForMaterial, kgmString> slotBlendSource;
  Slot<kViewOptionsForMaterial, kgmString> slotBlendDestination;

  kgmGuiText*  guiTextTexColor;
  kgmGuiText*  guiTextTexNormal;
  kgmGuiText*  guiTextTexSpecular;
  kgmGuiText*  guiTextShader;

  bool m_srcblend;

protected:
  kViewOptionsForMaterial(kNode* n, int x, int y, int w, int h);

public:
  static kViewOptionsForMaterial* getDialog(kNode* n, int x, int y, int w, int h)
  {
    if(kViewOptions::single)
      return null;

    return (kViewOptionsForMaterial*) (kViewOptions::single = new kViewOptionsForMaterial(n, x, y, w, h));
  }


  __stdcall void onReset(int);
  __stdcall void onColorR(kgmString);
  __stdcall void onColorG(kgmString);
  __stdcall void onColorB(kgmString);
  __stdcall void onSpecularR(kgmString);
  __stdcall void onSpecularG(kgmString);
  __stdcall void onSpecularB(kgmString);
  __stdcall void onBlendSource(kgmString);
  __stdcall void onBlendDestination(kgmString);
  __stdcall void onSelectFailed(kgmGuiFileDialog*);
  __stdcall void onSelectSuccess(kgmGuiFileDialog*);
  __stdcall void onSelectShader(int);
  __stdcall void onSelectTexColor(int);
  __stdcall void onSelectTexNormal(int);
  __stdcall void onSelectTexSpecular(int);
  __stdcall void onShininess(u32 s);
  __stdcall void onTransparency(u32 s);
  __stdcall void onCull(bool s);
  __stdcall void onBlend(bool s);
  __stdcall void onAlpha(bool s);
  __stdcall void onShade(bool s);
  __stdcall void onDepth(bool s);

private:
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
  kViewOptionsForVisual(kNode* n, int x, int y, int w, int h);

public:
  static kViewOptionsForVisual* getDialog(kNode* n, int x, int y, int w, int h)
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
  kViewOptionsForLight(kNode* n, int x, int y, int w, int h);

public:
  static kViewOptionsForLight* getDialog(kNode* n, int x, int y, int w, int h)
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
  kViewOptionsForUnit(kNode* n, int x, int y, int w, int h);

public:
  static kViewOptionsForUnit* getDialog(kNode* n, int x, int y, int w, int h)
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
  kViewOptionsForSensor(kNode* n, int x, int y, int w, int h);

public:
  static kViewOptionsForSensor* getDialog(kNode* n, int x, int y, int w, int h)
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
  kViewOptionsForTrigger(kNode* n, int x, int y, int w, int h);

public:
  static kViewOptionsForTrigger* getDialog(kNode* n, int x, int y, int w, int h)
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
  kViewOptionsForObstacle(kNode* n, int x, int y, int w, int h);

public:
  static kViewOptionsForObstacle* getDialog(kNode* n, int x, int y, int w, int h)
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
  kViewOptionsForEffect(kNode* n, int x, int y, int w, int h);

public:
  static kViewOptionsForEffect* getDialog(kNode* n, int x, int y, int w, int h)
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
  kViewOptionsForActor(kNode* n, int x, int y, int w, int h);

public:
  static kViewOptionsForActor* getDialog(kNode* n, int x, int y, int w, int h)
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
