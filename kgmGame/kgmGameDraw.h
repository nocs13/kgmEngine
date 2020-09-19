#ifndef KGMGAMEDRAW_H
#define KGMGAMEDRAW_H

#include "../kgmBase/kgmObject.h"
#include "../kgmMath/kgmMath.h"

class kgmMesh;
class kgmMaterial;

class kgmGameDraw : public kgmObject
{
  KGM_OBJECT(kgmGameDraw);

  enum Type
  {
    TypeNone,
    TypeMesh,
    TypeText,
    TypeSprite,
    TypeBillboard,
    TypeParticles
  };

  struct Draw {

  };

  struct  DrawMesh: Draw
  {
    kgmMesh*     msh;
    kgmMaterial* mtl;
    mtx4         trn;

    DrawMesh()
    {

    }
  };

  struct DrawText: Draw
  {

    DrawText()
    {

    }
  };

  struct DrawSprite: Draw
  {

    DrawSprite()
    {

    }
  };

  struct DrawParticles: Draw
  {

    DrawParticles()
    {

    }
  };

  struct DrawBillboard: Draw
  {

    DrawBillboard()
    {

    }
  };

public:
  kgmGameDraw();
};

#endif // KGMGAMEDRAW_H
