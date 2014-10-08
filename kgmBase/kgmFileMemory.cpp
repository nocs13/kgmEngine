#include "kgmFileMemory.h"

KGMOBJECT_IMPLEMENT(kgmFileMemory, kgmObject)

kgmFileMemory::kgmFileMemory()
{
  base   = null;
  length = 0;
  cursor = 0;
}

kgmFileMemory::kgmFileMemory(void *ptr, u32 len)
{
  base   = (u8*)ptr;
  length = len;
  cursor = 0;
}
