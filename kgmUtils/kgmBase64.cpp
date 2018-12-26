#include "kgmBase64.h"
#include <string.h>
#include <ctype.h>

static const u8 Base64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static const s8 Pad64 = '=';


kgmBase64::kgmBase64()
{

}

kgmBase64::kgmBase64(kgmString s)
{

}

kgmBase64::kgmBase64(u8*, u32)
{

}

bool kgmBase64::encode(kgmArray<u8>& source, kgmString& target)
{
  if (source.length() < 1)
    return false;

  s32 esize = kgmBase64::encode_size(source.length(), source.length());

  target.alloc(esize);

  esize = kgmBase64::b64_ntop(source.data(), source.length(), target.data(), esize);

  if (esize < 1)
    return false;

  return true;
}

bool kgmBase64::decode(kgmString& source, kgmArray<u8>& target)
{
  s32 dsize = kgmBase64::decode_size(source.length());

  if (dsize < 1)
    return false;

  target.alloc(dsize);

  dsize = kgmBase64::b64_pton(source.data(), target.data(), dsize);

  if (dsize < 1)
    return false;

  return true;
}

s32 kgmBase64::decode_size(s32 size)
{
    return size / 4 * 3;
}

s32 kgmBase64::encode_size(s32 inbuffersize, s32 linesize)
{
  s32 needed;

  /*
     * Determine how many 4 bytes blocks are
     * needed. We need a block every 3 bytes
     * plus padding.
     */

  needed = inbuffersize / 3 * 4;

  if ( inbuffersize % 3 != 0 )
  {
    needed += 4;
  }

  /*
     * Determine how many bytes we need for
     * the CR/LR.
     */

  if ( linesize > 0 )
  {
    s32 controlsneeded;
    controlsneeded = ( needed / linesize ) * 2;

    if ( needed % linesize != 0 )
    {
      controlsneeded += 2;
    }

    needed += controlsneeded;
  }

  /*
     * Account for the terminating zero we
     * will add to the output buffer.
     */

  return needed + 1;
}

s32 kgmBase64::b64_ntop(u8 *src, u32 srclength, s8* target, u32 targsize)
{
  s32 datalength = 0;
  u8  input[3];
  u8  output[4];
  u32 i;

  while (2 < srclength)
  {
    input[0] = *src++;
    input[1] = *src++;
    input[2] = *src++;
    srclength -= 3;

    output[0] = input[0] >> 2;
    output[1] = ((input[0] & 0x03) << 4) + (input[1] >> 4);
    output[2] = ((input[1] & 0x0f) << 2) + (input[2] >> 6);
    output[3] = input[2] & 0x3f;

    if (datalength + 4 > targsize)
      return (-1);

    target[datalength++] = Base64[output[0]];
    target[datalength++] = Base64[output[1]];
    target[datalength++] = Base64[output[2]];
    target[datalength++] = Base64[output[3]];
  }

  /* Now we worry about padding. */
  if (0 != srclength)
  {
    /* Get what's left. */
    input[0] = input[1] = input[2] = '\0';

    for (i = 0; i < srclength; i++)
      input[i] = *src++;

    output[0] = input[0] >> 2;
    output[1] = ((input[0] & 0x03) << 4) + (input[1] >> 4);
    output[2] = ((input[1] & 0x0f) << 2) + (input[2] >> 6);

    if (datalength + 4 > targsize)
      return (-1);

    target[datalength++] = Base64[output[0]];
    target[datalength++] = Base64[output[1]];

    if (srclength == 1)
      target[datalength++] = Pad64;
    else
      target[datalength++] = Base64[output[2]];

    target[datalength++] = Pad64;
  }

  if (datalength >= targsize)
    return (-1);

  target[datalength] = '\0';	/* Returned value doesn't count \0. */

  return (datalength);
}

s32 kgmBase64::b64_pton(s8* src, u8* target, u32 targsize)
{
  u32 tarindex, state;
  s32 ch;
  s8 *pos;

  state = 0;
  tarindex = 0;

  while ((ch = *src++) != '\0')
  {
    if (isspace(ch)) /* Skip whitespace anywhere. */
      continue;

    if (ch == Pad64)
      break;

    pos = strchr((s8*) Base64, ch);

    if (pos == 0)  /* A non-base64 character. */
      return (-1);

    switch (state)
    {
    case 0:
      if (target)
      {
        if (tarindex >= targsize)
          return (-1);

        target[tarindex] = (pos - (s8*) Base64) << 2;
      }
      state = 1;
      break;
    case 1:
      if (target)
      {
        if (tarindex + 1 >= targsize)
          return (-1);

        target[tarindex]   |=  (pos - (s8*) Base64) >> 4;
        target[tarindex+1]  = ((pos - (s8*) Base64) & 0x0f) << 4 ;
      }
      tarindex++;
      state = 2;
      break;
    case 2:
      if (target)
      {
        if (tarindex + 1 >= targsize)
          return (-1);

        target[tarindex]   |=  (pos - (s8*) Base64) >> 2;
        target[tarindex+1]  = ((pos - (s8*) Base64) & 0x03) << 6;
      }
      tarindex++;
      state = 3;
      break;
    case 3:
      if (target)
      {
        if (tarindex >= targsize)
          return (-1);

        target[tarindex] |= (pos - (s8*) Base64);
      }

      tarindex++;
      state = 0;
      break;
    }
  }

  /*
     * We are done decoding Base-64 chars.  Let's see if we ended
     * on a byte boundary, and/or with erroneous trailing characters.
     */

  if (ch == Pad64)
  {
    /* We got a pad char. */
    ch = *src++; /* Skip it, get next. */

    switch (state)
    {
    case 0:		/* Invalid = in first position */
    case 1:		/* Invalid = in second position */
      return (-1);

    case 2:		/* Valid, means one byte of info */
      /* Skip any number of spaces. */
      for (; ch != '\0'; ch = *src++)
        if (!isspace(ch))
          break;
      /* Make sure there is another trailing = sign. */
      if (ch != Pad64)
        return (-1);
      ch = *src++;		/* Skip the = */
      /* Fall through to "single trailing =" case. */
      /* FALLTHROUGH */

    case 3:		/* Valid, means two bytes of info */
      /*
         * We know this char is an =.  Is there anything but
         * whitespace after it?
         */
      for (; ch != '\0'; ch = *src++)
        if (!isspace(ch))
          return (-1);

      /*
         * Now make sure for cases 2 and 3 that the "extra"
         * bits that slopped past the last full byte were
         * zeros.  If we don't check them, they become a
         * subliminal channel.
         */
      if (target && target[tarindex] != 0)
        return (-1);
    }
  }
  else
  {
    /*
       * We ended by seeing the end of the string.  Make sure we
       * have no partial bytes lying around.
       */
    if (state != 0)
      return (-1);
  }

  return (tarindex);
}
