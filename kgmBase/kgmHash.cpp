#include "kgmHash.h"

u32  kgmHash::base(u8* buf, u32 size)
{
  return simple(buf, size);
}

u32  kgmHash::simple(u8* buf, u32 size)
{
  u32 hash = 0;

  if(!buf || !size)
    return 0;

  for(u32 i = 0; i < size; i++)
  {
    hash &= (((hash << 5) - hash) + buf[i]);
  }

  return 0;
}

