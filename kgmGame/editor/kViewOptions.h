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
  typedef kgmCallback<void(kgmObject*)> CloseCallback;

  CloseCallback callClose;
public:
  kViewOptions(kNode* n, int x, int y, int w, int h);

  void onAction(kgmGui* from, u32 arg);
  void onCloseOptions();
  void onPositionX(kgmString s);
  void onPositionY(kgmString s);
  void onPositionZ(kgmString s);
  void onRotationZ(u32 s);

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
  kViewOptionsForLight(kNode* n, int x, int y, int w, int h)
  :kViewOptions(n, x, y, w, h)
  {
  }
};

class kViewOptionsForActor : public kViewOptions
{
public:
  kViewOptionsForActor(kNode* n, int x, int y, int w, int h)
  :kViewOptions(n, x, y, w, h)
  {
  }
};

class kViewOptionsForMaterial : public kViewOptions
{
public:
  kViewOptionsForMaterial(kNode* n, int x, int y, int w, int h)
  :kViewOptions(n, x, y, w, h)
  {
  }
};

}

#endif // KVIEWOPTIONS_H
