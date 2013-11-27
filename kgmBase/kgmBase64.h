#ifndef KGMBASE64_H
#define KGMBASE64_H

#include "kgmTypes.h"
#include "kgmArray.h"

class kgmBase64
{
public:
  kgmBase64();

  static bool encode(kgmByteArray& encoded, int8* buffer, int32 length);
  static bool decode(kgmByteArray& decoded, int8* buffer, int32 length);
};

#endif // KGMBASE64_H
