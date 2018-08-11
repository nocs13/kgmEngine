#ifndef KVIEWOPTIONS_H
#define KVIEWOPTIONS_H

#include "../../kgmGraphics/kgmLight.h"
#include "../../kgmGraphics/kgmGuiFrame.h"
#include "../../kgmGraphics/kgmGuiTab.h"
#include "../../kgmGraphics/kgmGuiLabel.h"
#include "../../kgmGraphics/kgmGuiCheck.h"
#include "../../kgmGraphics/kgmGuiSelect.h"
#include "../../kgmGraphics/kgmGuiFileDialog.h"
#include "../kgmUnit.h"

namespace kgmGameEditor
{

class kViewObjects;

class kViewOptions: public kgmGuiFrame
{
  KGM_OBJECT(kViewOptions);

  kgmGuiButton* btn_close;

  Slot<kViewOptions, kgmString> slotRotationX;
  Slot<kViewOptions, kgmString> slotRotationY;
  Slot<kViewOptions, kgmString> slotRotationZ;
  Slot<kViewOptions, kgmString> slotMaterials;
  Slot<kViewOptions, int>       slotSelectMaterial;
  Slot<kViewOptions, bool>      slotLock;

protected:
  static kViewOptions* single;

  kgmGuiTab*  tab;

  kgmUnit*  node;
  u32     y_coord;

public:
  Signal<> sigClose;

protected:
  kViewOptions(kgmUnit* n, int x, int y, int w, int h);
  ~kViewOptions() { kViewOptions::single = null; }

public:
  static kViewOptions* getDialog(kgmUnit* n, int x, int y, int w, int h)
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
  __stdcall void onLock(bool);
  __stdcall void onShowMaterials(int);
  __stdcall void onSelectMaterial(kgmString);
};

class kViewOptionsForMaterial : public kgmGuiFrame
{
  enum Mode
  {
    Mode_None,
    Mode_Shader,
    Mode_Color,
    Mode_Normal,
    Mode_Specular
  };

  static kViewOptionsForMaterial* single;

  Mode mode;

  kgmGuiTab*  tab;

  u32     y_coord;

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
  Slot<kViewOptionsForMaterial, kgmString> slotBlending;

  Slot<kViewOptionsForMaterial, u32> slotShininess;
  Slot<kViewOptionsForMaterial, u32> slotTranparency;
  Slot<kViewOptionsForMaterial, u32> slotReflection;
  Slot<kViewOptionsForMaterial, u32> slotRefraction;

  Slot<kViewOptionsForMaterial, kgmGuiFileDialog*> slotTexColor;
  Slot<kViewOptionsForMaterial, kgmGuiFileDialog*> slotTexNormal;
  Slot<kViewOptionsForMaterial, kgmGuiFileDialog*> slotTexSpecular;

  kgmGuiText*  guiTextTexColor;
  kgmGuiText*  guiTextTexNormal;
  kgmGuiText*  guiTextTexSpecular;
  kgmGuiText*  guiTextShader;

  bool m_srcblend;

  kgmMaterial* mtl = null;

protected:
  kViewOptionsForMaterial(kgmMaterial* m, int x, int y, int w, int h);
  ~kViewOptionsForMaterial() {
    kViewOptionsForMaterial::single = null;
  }

public:
  static kViewOptionsForMaterial* getDialog(kgmMaterial* m, int x, int y, int w, int h)
  {
    if(kViewOptionsForMaterial::single)
      return null;

    return (kViewOptionsForMaterial*) (kViewOptionsForMaterial::single = new kViewOptionsForMaterial(m, x, y, w, h));
  }

  __stdcall void onReset(int);
  __stdcall void onColorR(kgmString);
  __stdcall void onColorG(kgmString);
  __stdcall void onColorB(kgmString);
  __stdcall void onSpecularR(kgmString);
  __stdcall void onSpecularG(kgmString);
  __stdcall void onSpecularB(kgmString);
  __stdcall void onBlending(kgmString);
  __stdcall void onSelectShader(int);
  __stdcall void onSelectTexColor(int);
  __stdcall void onSelectTexNormal(int);
  __stdcall void onSelectTexSpecular(int);
  __stdcall void onSelectTexture(kgmGuiFileDialog*);
  __stdcall void onShininess(u32 s);
  __stdcall void onTransparency(u32 s);
  __stdcall void onReflection(u32 s);
  __stdcall void onRefraction(u32 s);
  __stdcall void onCull(bool s);
  __stdcall void onBlend(bool s);
  __stdcall void onAlpha(bool s);
  __stdcall void onShade(bool s);
  __stdcall void onDepth(bool s);
};

class kViewOptionsForVisual : public kViewOptions
{
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
  kViewOptionsForVisual(kgmUnit* n, int x, int y, int w, int h);

public:
  static kViewOptionsForVisual* getDialog(kgmUnit* n, int x, int y, int w, int h)
  {
    if(kViewOptions::single)
      return null;

    return (kViewOptionsForVisual*) (kViewOptions::single = new kViewOptionsForVisual(n, x, y, w, h));
  }

  __stdcall void onShowMaterials(int);
  __stdcall void onSelectMaterial(kgmString);
};

class kViewOptionsForLight : public kViewOptions
{
  //Slot<kViewOptionsForLight, kgmString> slotColorR;
  //Slot<kViewOptionsForLight, kgmString> slotColorG;
  //Slot<kViewOptionsForLight, kgmString> slotColorB;
  Slot<kViewOptionsForLight, kgmString> slotColor;
  Slot<kViewOptionsForLight, kgmString> slotIntencity;
  Slot<kViewOptionsForLight, bool>      slotShadows;

protected:
  kViewOptionsForLight(kgmUnit* n, int x, int y, int w, int h);

public:
  static kViewOptionsForLight* getDialog(kgmUnit* n, int x, int y, int w, int h)
  {
    if(kViewOptions::single)
      return null;

    return (kViewOptionsForLight*)(kViewOptions::single = new kViewOptionsForLight(n, x, y, w, h));
  }


  //__stdcall void onColorR(kgmString);
  //__stdcall void onColorG(kgmString);
  //__stdcall void onColorB(kgmString);
  __stdcall void onColor(kgmString);
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
  kViewOptionsForUnit(kgmUnit* n, int x, int y, int w, int h);

public:
  static kViewOptionsForUnit* getDialog(kgmUnit* n, int x, int y, int w, int h)
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
  kViewOptionsForSensor(kgmUnit* n, int x, int y, int w, int h);

public:
  static kViewOptionsForSensor* getDialog(kgmUnit* n, int x, int y, int w, int h)
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
  kViewOptionsForTrigger(kgmUnit* n, int x, int y, int w, int h);

public:
  static kViewOptionsForTrigger* getDialog(kgmUnit* n, int x, int y, int w, int h)
  {
    if(kViewOptions::single)
      return null;

    return (kViewOptionsForTrigger*)(kViewOptions::single = new kViewOptionsForTrigger(n, x, y, w, h));
  }


  __stdcall void setChanels(kgmString s);
  __stdcall void setTarget(kgmString s);
};

class kViewOptionsForEffect : public kViewOptionsForUnit
{
protected:
  kViewOptionsForEffect(kgmUnit* n, int x, int y, int w, int h);

public:
  static kViewOptionsForEffect* getDialog(kgmUnit* n, int x, int y, int w, int h)
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
  kViewOptionsForActor(kgmUnit* n, int x, int y, int w, int h);

public:
  static kViewOptionsForActor* getDialog(kgmUnit* n, int x, int y, int w, int h)
  {
    if(kViewOptions::single)
      return null;

    return (kViewOptionsForActor*)(kViewOptions::single = new kViewOptionsForActor(n, x, y, w, h));
  }

  __stdcall void showStates();
  __stdcall void onState(kgmString);
  __stdcall void onPlayer(bool);
};

class kViewOptionsForParticles : public kViewOptions
{
  kViewObjects *vo;
  kgmGuiText   *guiState;

  Slot<kViewOptionsForParticles, bool>       slotLoop;
  Slot<kViewOptionsForParticles, bool>       slotFade;
  Slot<kViewOptionsForParticles, kgmString>  slotCount;
  Slot<kViewOptionsForParticles, kgmString>  slotSpeed;
  Slot<kViewOptionsForParticles, kgmString>  slotNoise;
  Slot<kViewOptionsForParticles, kgmString>  slotLife;
  Slot<kViewOptionsForParticles, kgmString>  slotMass;
  Slot<kViewOptionsForParticles, kgmString>  slotSize;
  Slot<kViewOptionsForParticles, kgmString>  slotESize;
  Slot<kViewOptionsForParticles, kgmString>  slotDivSpeed;
  Slot<kViewOptionsForParticles, kgmString>  slotDivLife;
  Slot<kViewOptionsForParticles, kgmString>  slotGravity;
  Slot<kViewOptionsForParticles, kgmString>  slotVolumeX;
  Slot<kViewOptionsForParticles, kgmString>  slotVolumeY;
  Slot<kViewOptionsForParticles, kgmString>  slotVolumeZ;
  Slot<kViewOptionsForParticles, kgmString>  slotDirectionX;
  Slot<kViewOptionsForParticles, kgmString>  slotDirectionY;
  Slot<kViewOptionsForParticles, kgmString>  slotDirectionZ;

protected:
  kViewOptionsForParticles(kgmUnit* n, int x, int y, int w, int h);

public:
  static kViewOptionsForParticles* getDialog(kgmUnit* n, int x, int y, int w, int h)
  {
    if(kViewOptions::single)
      return null;

    return (kViewOptionsForParticles*)(kViewOptions::single = new kViewOptionsForParticles(n, x, y, w, h));
  }

  __stdcall void onLoop(bool);
  __stdcall void onFade(bool);
  __stdcall void onCount(kgmString);
  __stdcall void onSpeed(kgmString);
  __stdcall void onNoise(kgmString);
  __stdcall void onLife(kgmString);
  __stdcall void onMass(kgmString);
  __stdcall void onSize(kgmString);
  __stdcall void onEsize(kgmString);
  __stdcall void onDivSpeed(kgmString);
  __stdcall void onDivLife(kgmString);
  __stdcall void onGravity(kgmString);
  __stdcall void onVolumeX(kgmString);
  __stdcall void onVolumeY(kgmString);
  __stdcall void onVolumeZ(kgmString);
  __stdcall void onDirectionX(kgmString);
  __stdcall void onDirectionY(kgmString);
  __stdcall void onDirectionZ(kgmString);
};

}

#endif // KVIEWOPTIONS_H
