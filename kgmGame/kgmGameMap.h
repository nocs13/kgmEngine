#pragma once
#ifndef KGMGAMEMAP_H
#define KGMGAMEMAP_H

#include "../kgmBase/kgmObject.h"
#include "../kgmBase/kgmXml.h"

class kgmIGame;

class kgmGameMap : public kgmObject
{
  KGM_OBJECT(kgmGameMap);
public:
  enum OpenType
  {
    OpenRead,
    OpenWrite
  };

  OpenType type;

public:
  kgmGameMap(kgmIGame* g, OpenType ot = OpenRead);
};

#endif // KGMGAMEMAP_H
