#include "kgmAlsa.h"

KGMOBJECT_IMPLEMENT(kgmAlsa, kgmIAudio);

#ifdef ALSA
#include <alsa/asoundlib.h>

static const char alsaDevice[] = "ALSA Default";

#pragma GCC diagnostic push
#pragma GCC diagnostic warning "-fpermissive"

//#pragma GCC diagnostic ignored "-pedantic"
//#pragma GCC diagnostic pop
//#pragma GCC diagnostic ignored "-fno-permissive"

typedef void* (*PCM_FUNC)(snd_pcm_t*,...);

//#define MAKE_FUNC(f) static typeof(f) p##f
#define MAKE_FUNC(f) static PCM_FUNC p##f

MAKE_FUNC(snd_strerror);
MAKE_FUNC(snd_pcm_open);
MAKE_FUNC(snd_pcm_close);

#define snd_strerror psnd_strerror
#define snd_pcm_open psnd_pcm_open
#define snd_pcm_close psnd_pcm_close

#endif //ALSA

kgmAlsa::kgmAlsa()
{
  m_handle = null;

  if (m_lib.open("libasound.so.2") || m_lib.open("libasound.so.1") ||
      m_lib.open("libasound.so.0") || m_lib.open("libasound.so"))
  {
#ifdef ALSA
#define LOAD_FUNC(f) p##f = m_lib.get(#f)
    LOAD_FUNC(snd_strerror);
    LOAD_FUNC(snd_pcm_open);
    LOAD_FUNC(snd_pcm_close);

    if(snd_pcm_open != null)
    {

    }
#endif
  }
}

kgmAlsa::~kgmAlsa()
{
#ifdef ALSA
#endif
}
