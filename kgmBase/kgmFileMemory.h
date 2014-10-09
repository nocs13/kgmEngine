#ifndef KGMFILEMEMORY_H
#define KGMFILEMEMORY_H

#include "kgmObject.h"

class kgmFileMemory : public kgmObject
{
  KGM_OBJECT(kgmFileMemory);

  u8*  base;
  u32  size;
  u32  cursor;

public:
  kgmFileMemory();
  kgmFileMemory(void *ptr, u32 len);

  u32 length();
  u32 position();
  u32 seek(u32);
  u32 read(void* dst, u32 len);
  u32 write(void* src, u32 len);

  bool eof();
};

#endif // KGMFILEMEMORY_H
