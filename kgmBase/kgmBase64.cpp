#include "kgmBase64.h"

const char base64chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

kgmBase64::kgmBase64()
{
  (void)base64chars;
}

bool kgmBase64::encode(kgmByteArray& encoded, int8* buffer, int32 length)
{
  if(!buffer || !length)
    return false;

  return true;
}

bool kgmBase64::decode(kgmByteArray& decoded, int8* buffer, int32 length)
{
  if(!buffer || !length)
    return false;

  return true;
}
