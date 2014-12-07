#include "kgmCompress.h"

KGMOBJECT_IMPLEMENT(kgmCompress, kgmObject);

kgmCompress::kgmCompress()
{
  type = TypeNone;

  compressed.clear();
}

bool kgmCompress::compress(u32 ctype, void *data, u32 length)
{
  switch (ctype)
  {
  case TypeRle0:
    return rle0_compress(data, length);
    break;
  }

  return false;
}

bool kgmCompress::extract(kgmArray<u8> &data)
{
  switch (type)
  {
  case TypeRle0:
    return rle0_extract(data);
    break;
  }

  return false;
}

bool kgmCompress::rle0_compress(void *data, u32 length)
{
  type = TypeNone;

  compressed.clear();

  if(data == null || length < 1)
    return false;

  //Calculate allocate data count.

  u32 size = 0;

  u8* p = (u8*)data;

  for(u32 i = 0; i < length; i++)
  {
    u8 reps = 0;
    u8 c = p[i];

    size += 2;

    while(reps < 255 && p[i] == c)
      reps++;

    i--;
  }

  if(size < 2)
    return false;

  compressed.alloc(size);

  u8* pc = compressed.data();

  for(u32 i = 0; i < length; i++)
  {
    u8 reps = 0;
    u8 c = p[i];

    while(reps < 255 && p[i] == c)
    {
      reps++;
    }

    *pc = c;
    pc++;
    *pc = reps;
    pc++;

    i--;
  }

  type = TypeRle0;

  return true;
}

bool kgmCompress::rle0_extract(kgmArray<u8> &data)
{
  if(type != TypeRle0 || compressed.length() < 2)
    return false;


}
