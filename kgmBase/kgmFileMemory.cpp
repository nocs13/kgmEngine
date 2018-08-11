#include "kgmFileMemory.h"

kgmFileMemory::kgmFileMemory()
{
  base   = null;
  size   = 0;
  cursor = 0;
}

kgmFileMemory::kgmFileMemory(void *ptr, u32 len)
{
  base   = (u8*)ptr;
  size   = len;
  cursor = 0;
}

u32 kgmFileMemory::length()
{
  return size;
}

u32 kgmFileMemory::position()
{
  return cursor;
}

u32 kgmFileMemory::seek(u32 pos)
{
  if(pos < size)
    cursor = pos;

  return cursor;
}

u32 kgmFileMemory::read(void* dst, u32 len)
{
  if(len < (size - cursor))
  {
    memcpy(dst, base + cursor, len);

    cursor += len;

    return len;
  }

  return 0;
}

u32 kgmFileMemory::write(void* src, u32 len)
{
  if(len < (size - cursor))
  {
    memcpy(base + cursor, src, len);

    cursor += len;

    return len;
  }

  return 0;
}

bool kgmFileMemory::eof()
{
  return (cursor >= size);
}
