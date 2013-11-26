#ifndef KGMBASE64_H
#define KGMBASE64_H

#include "kgmTypes.h"
#include "kgmArray.h"

class kgmBase64
{
public:
  kgmBase64();

  static bool encode(kgmByteArray& encoded, sint8* buffer, sint32 length);
  static bool decode(kgmByteArray& decoded, sint8* buffer, sint32 length);
};

#endif // KGMBASE64_H
