#include "kgmAlsa.h"
#include "../kgmBase/kgmLog.h"

KGMOBJECT_IMPLEMENT(kgmAlsa, kgmIAudio);

#ifdef ALSA
#include <alsa/asoundlib.h>
#include <alsa/mixer.h>

static const char alsaDevice[] = "default";

#pragma GCC diagnostic push
#pragma GCC diagnostic warning "-fpermissive"

//#pragma GCC diagnostic ignored "-pedantic"
//#pragma GCC diagnostic pop
//#pragma GCC diagnostic ignored "-fno-permissive"

typedef void* (*PCM_FUNC)(snd_pcm_t*,...);

//#define MAKE_FUNC(f) static typeof(f) p##f
#define MAKE_FUNC(f) static PCM_FUNC p##f \

MAKE_FUNC(snd_strerror);
MAKE_FUNC(snd_pcm_wait);
MAKE_FUNC(snd_pcm_open);
MAKE_FUNC(snd_pcm_close);
MAKE_FUNC(snd_pcm_drain);
MAKE_FUNC(snd_pcm_start);
MAKE_FUNC(snd_pcm_writei);
MAKE_FUNC(snd_pcm_prepare);
MAKE_FUNC(snd_pcm_recover);
MAKE_FUNC(snd_pcm_set_params);
MAKE_FUNC(snd_pcm_bytes_to_frames);
MAKE_FUNC(snd_pcm_frames_to_bytes);
MAKE_FUNC(snd_pcm_hw_params);
MAKE_FUNC(snd_pcm_hw_params_any);
MAKE_FUNC(snd_pcm_hw_params_free);
MAKE_FUNC(snd_pcm_hw_params_malloc);
MAKE_FUNC(snd_pcm_hw_params_alloca);
MAKE_FUNC(snd_pcm_hw_params_sizeof);
MAKE_FUNC(snd_pcm_hw_params_set_access);
MAKE_FUNC(snd_pcm_hw_params_set_format);
MAKE_FUNC(snd_pcm_hw_params_set_channels);
MAKE_FUNC(snd_pcm_hw_params_set_rate_near);
MAKE_FUNC(snd_pcm_hw_params_set_rate_resample);

#define snd_strerror     psnd_strerror
#define snd_pcm_wait     psnd_pcm_wait
#define snd_pcm_open     psnd_pcm_open
#define snd_pcm_close    psnd_pcm_close
#define snd_pcm_drain    psnd_pcm_drain
#define snd_pcm_start    psnd_pcm_start
#define snd_pcm_writei   psnd_pcm_writei
#define snd_pcm_prepare  psnd_pcm_prepare
#define snd_pcm_recover  psnd_pcm_recover
#define snd_pcm_set_params       psnd_pcm_set_params
#define snd_pcm_bytes_to_frames  psnd_pcm_bytes_to_frames
#define snd_pcm_frames_to_bytes  psnd_pcm_frames_to_bytes
#define snd_pcm_hw_params        psnd_pcm_hw_params
#define snd_pcm_hw_params_any    psnd_pcm_hw_params_any
#define snd_pcm_hw_params_free   psnd_pcm_hw_params_free
#define snd_pcm_hw_params_malloc psnd_pcm_hw_params_malloc
#define snd_pcm_hw_params_alloca psnd_pcm_hw_params_alloca
#define snd_pcm_hw_params_sizeof psnd_pcm_hw_params_sizeof
#define snd_pcm_hw_params_set_access psnd_pcm_hw_params_set_access
#define snd_pcm_hw_params_set_format psnd_pcm_hw_params_set_format
#define snd_pcm_hw_params_set_channels psnd_pcm_hw_params_set_channels
#define snd_pcm_hw_params_set_rate_near psnd_pcm_hw_params_set_rate_near
#define snd_pcm_hw_params_set_rate_resample psnd_pcm_hw_params_set_rate_resample

#endif //ALSA

struct _Sound
{
  enum State
  {
    StStop,
    StPlay,
    StPause
  };

  void *handle;
  void *data;
  u32   size;

  int pcm_rate;
  int pcm_frames;
  int pcm_format;
  int pcm_channels;

  bool loop;

  State state;

  _Sound()
  {
    handle = null;
    data   = null;
    size   = 0;

    pcm_rate     = 0;
    pcm_frames   = 0;
    pcm_format   = 0;
    pcm_channels = 0;

    loop = false;
    state = StStop;
  }

  ~_Sound()
  {
    if(data)
      free(data);
  }

  void release()
  {

  }

  void stop()
  {
    state = StStop;
  }

  void play(bool lp)
  {
    state = StPlay;

    loop = lp;

#ifdef ALSA
    if(handle && data && size)
    {
      int pcm = 0;
      int bpp = 1;

      if(pcm_format == SND_PCM_FORMAT_S16_LE)
        bpp = 2;

      pcm_frames = size / (bpp * pcm_channels);

      if(pcm = snd_pcm_set_params(handle, pcm_format,
                                  SND_PCM_ACCESS_RW_INTERLEAVED, pcm_channels,
                                  pcm_rate, 1, 500000) < 0) {
          printf("Playback set param error: %s\n", snd_strerror(pcm));
       }

      if(pcm = snd_pcm_prepare(handle) < 0)
        printf("ERROR: Can't prepare. %s\n", snd_strerror(pcm));

      snd_pcm_sframes_t avail = snd_pcm_bytes_to_frames(handle, size);

      char* WritePtr = data;

      while(avail > 0)
      {
        int ret = snd_pcm_writei(handle, WritePtr, avail);

        switch (ret)
        {
        case -EAGAIN:
          continue;
        case -ESTRPIPE:
        case -EPIPE:
        case -EINTR:
          ret = snd_pcm_recover(handle, ret, 1);

          if(ret < 0)
            avail = 0;

          break;
        default:
          if(ret >= 0)
          {
            WritePtr += (u32)snd_pcm_frames_to_bytes(handle, ret);
            avail -= ret;
          }
          break;
        }

        if (ret < 0)
        {
          ret = snd_pcm_prepare(handle);

          if(ret < 0)
            break;
        }
      }

      //if(pcm = snd_pcm_writei(handle, data, pcm_frames) != pcm_frames)
      {
        //printf("ERROR: Can't write. %s\n", snd_strerror(pcm));
      }

      if(pcm = snd_pcm_drain(handle) < 0)
        printf("ERROR: Can't drain. %s\n", snd_strerror(pcm));

      //if(pcm = snd_pcm_close(handle) < 0)
      //  printf("ERROR: Can't drain. %s\n", snd_strerror(pcm));
    }
#endif
  }

  void pause()
  {

  }

  void volume(float vol)
  {

  }

  bool valid()
  {
    return (handle != null);
  }

  bool create(kgmIAudio::FMT fmt, u16 freq, u32 size, void* data)
  {
    if(!handle)
      return false;

#ifdef ALSA
    if(handle)
    {
      int pcm       = 0;
      pcm_rate      = 0;
      pcm_format    = 0;
      pcm_channels  = 0;

      pcm_rate = freq;

      switch(fmt)
      {
      case kgmIAudio::FMT_MONO8:
        pcm_format = SND_PCM_FORMAT_S8;
        pcm_channels = 1;
        break;
      case kgmIAudio::FMT_MONO16:
        pcm_format = SND_PCM_FORMAT_S16_LE;
        pcm_channels = 1;
        break;
      case kgmIAudio::FMT_STEREO8:
        pcm_format = SND_PCM_FORMAT_S8;
        pcm_channels = 2;
        break;
      case kgmIAudio::FMT_STEREO16:
      default:
        pcm_format = SND_PCM_FORMAT_S16_LE;
        pcm_channels = 2;
      }

      /*pcm = (int)snd_pcm_open((void*)&handle, "plughw:0,0", SND_PCM_STREAM_PLAYBACK, SND_PCM_NONBLOCK);

      if(pcm < 0)
      {
        kgm_log() << "Error: can't open alsa device.\n";
        kgm_log() << "Error: is " << (char*)kgmString((char*)snd_strerror(pcm)) << ".\n";
      }*/

        if(data && size)
        {
          this->data = malloc(size);
          memcpy(this->data, data, size);
          this->size = size;
        }
//      if(pcm = snd_pcm_drain(handle) < 0)
//        printf("ERROR: Can't drain. %s\n", snd_strerror(pcm));
    }
#endif

    return true;
  }


};

kgmAlsa::kgmAlsa()
{
  m_handle = null;

  if (m_lib.open("libasound.so.2") || m_lib.open("libasound.so.1") ||
      m_lib.open("libasound.so.0") || m_lib.open("libasound.so"))
  {
    int err = 0;

#ifdef ALSA
#define LOAD_FUNC(f) p##f = m_lib.get(#f)
    LOAD_FUNC(snd_strerror);
    LOAD_FUNC(snd_pcm_wait);
    LOAD_FUNC(snd_pcm_open);
    LOAD_FUNC(snd_pcm_close);
    LOAD_FUNC(snd_pcm_drain);
    LOAD_FUNC(snd_pcm_start);
    LOAD_FUNC(snd_pcm_writei);
    LOAD_FUNC(snd_pcm_prepare);
    LOAD_FUNC(snd_pcm_recover);
    LOAD_FUNC(snd_pcm_set_params);
    LOAD_FUNC(snd_pcm_bytes_to_frames);
    LOAD_FUNC(snd_pcm_frames_to_bytes);
    LOAD_FUNC(snd_pcm_hw_params);
    LOAD_FUNC(snd_pcm_hw_params_any);
    LOAD_FUNC(snd_pcm_hw_params_free);
    LOAD_FUNC(snd_pcm_hw_params_malloc);
    LOAD_FUNC(snd_pcm_hw_params_alloca);
    LOAD_FUNC(snd_pcm_hw_params_sizeof);
    LOAD_FUNC(snd_pcm_hw_params_set_access);
    LOAD_FUNC(snd_pcm_hw_params_set_format);
    LOAD_FUNC(snd_pcm_hw_params_set_channels);
    LOAD_FUNC(snd_pcm_hw_params_set_rate_near);
    LOAD_FUNC(snd_pcm_hw_params_set_rate_resample);

    if(snd_pcm_open)
    {
      char* device = "default";
      //char* device = "plughw:0,0";
      err = (int)snd_pcm_open((void*)&m_handle, device, SND_PCM_STREAM_PLAYBACK, SND_PCM_NONBLOCK);

      if(err < 0)
      {
        kgm_log() << "Error: can't open alsa device.\n";
        kgm_log() << "Error: is " << (char*)kgmString((char*)snd_strerror(err)) << ".\n";
      }
    }
#endif
  }
}

kgmAlsa::~kgmAlsa()
{
#ifdef ALSA
  snd_pcm_close(m_handle);
#endif

  m_lib.close();
}

kgmIAudio::Sound kgmAlsa::create(FMT fmt, u16 freq, u32 size, void* data)
{
#ifdef ALSA
  if(1)
  {
    _Sound* sound = new _Sound();

    if(sound->create(fmt, freq, size, data))
      return sound;
    else
      delete sound;
  }
#endif

  return null;
}

void kgmAlsa::remove(Sound snd)
{
 if(snd)
 {
#ifdef ALSA
   delete snd;
#endif
 }
}

void kgmAlsa::channel(Sound snd, s16 pan)
{

}

void kgmAlsa::volume(Sound snd, u16 vol)
{

}

void kgmAlsa::pause(Sound snd, bool stat)
{

}

void kgmAlsa::play(Sound snd, bool loop)
{
  if(snd)
    ((_Sound*)snd)->play(loop);
}

void kgmAlsa::stop(Sound snd)
{

}
