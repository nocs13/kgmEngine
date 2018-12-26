#ifndef KGMBASE64_H
#define KGMBASE64_H

#include "../kgmBase/kgmObject.h"
#include "../kgmBase/kgmString.h"
#include "../kgmBase/kgmArray.h"


class kgmBase64 : public kgmObject
{
  KGM_OBJECT(kgmBase64);

  kgmString m_base64;

public:
  kgmBase64();
  kgmBase64(kgmString);
  kgmBase64(u8*, u32);

  static bool encode(kgmArray<u8>& source, kgmString& target);
  static bool decode(kgmString&    source, kgmArray<u8>& target);
  static s32  decode_size(s32 size);
  static s32  encode_size(s32 inbuffersize, s32 linesize);

private:

  static s32 b64_ntop(u8* src, u32 srclength, s8* target, u32 targsize);
  static s32 b64_pton(s8* src, u8* target, u32 targsize);
};

#endif // KGMBASE64_H
