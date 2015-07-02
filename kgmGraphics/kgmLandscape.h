// kgmLandscape.h: interface for the kgmLandscape class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "../kgmBase/kgmObject.h"

class kgmLandscape: public kgmObject
{
  KGM_OBJECT(kgmLandscape);

private:
  struct Heightmap
  {
    //kgmPicture* map;
  };

protected:
  ~kgmLandscape();

public:
  kgmLandscape();
};

