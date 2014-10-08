#ifndef KGMFILEMEMORY_H
#define KGMFILEMEMORY_H

#include "kgmObject.h"

class kgmFileMemory : public kgmObject
{
  KGM_OBJECT(kgmFileMemory);

  u8*  base;
  u32  cursor;
  u32  length;

public:
  kgmFileMemory();
  kgmFileMemory(void *ptr, u32 len);

  bool eof()
  {
    return (cursor >= length);
  }
};

#endif // KGMFILEMEMORY_H
