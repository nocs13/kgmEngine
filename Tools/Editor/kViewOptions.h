#ifndef KVIEWOPTIONS_H
#define KVIEWOPTIONS_H

#include "kNode.h"
#include "../../kgmGraphics/kgmGui.h"
#include "../../kgmGraphics/kgmGuiTab.h"

class kViewOptions : public kgmGui
{
  kgmGuiButton* btn_close;
  kgmGuiTab*    tab;
  kNode*        node;

public:
  kViewOptions(kNode* n, int x, int y, int w, int h);

  void onAction(kgmGui* from, u32 arg);
};

class kViewOptionsForMesh : public kViewOptions
{
public:
  kViewOptionsForMesh(kNode* n, int x, int y, int w, int h)
  :kViewOptions(n, x, y, w, h)
  {
  }
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


#endif // KVIEWOPTIONS_H
