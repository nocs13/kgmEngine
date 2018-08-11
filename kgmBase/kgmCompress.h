#ifndef KGMCOMPRESS_H
#define KGMCOMPRESS_H

#include "kgmObject.h"
#include "kgmArray.h"

class kgmCompress : public kgmObject
{
  KGM_OBJECT(kgmCompress);

public:
  enum
  {
    TypeNone,
    TypeRle0
  };

private:
  u32 type;

  kgmArray<u8> compressed;

public:
  kgmCompress();

  void* getCompress()
  {
    return (void*)compressed.data();
  }

  u32 getLenght()
  {
    return compressed.length();
  }

  u32 getType() const
  {
    return type;
  }

  bool compress(u32 ctype, void* data, u32 length);
  bool extract(kgmArray<u8>& data);

private:
  bool rle0_compress(void* data, u32 length);
  bool rle0_extract(kgmArray<u8>& data);
};

#endif // KGMCOMPRESS_H
