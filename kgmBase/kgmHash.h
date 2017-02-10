#pragma once
#include "kgmTypes.h"

class kgmHash
{
public:
  static u32  base(u8* buffer, u32 size);
  static u32  simple(u8* buffer, u32 size);
};
