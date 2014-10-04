#include "kgmAlsa.h"
#include "../kgmBase/kgmLog.h"
#include "../kgmBase/kgmTime.h"

KGMOBJECT_IMPLEMENT(kgmAlsa, kgmIAudio);

#ifdef ALSA
#include <alsa/asoundlib.h>
//#include <alsa/mixer.h>

static const char alsaDevice[] = "default";

#pragma GCC diagnostic push
#pragma GCC diagnostic warning "-fpermissive"

//#pragma GCC diagnostic ignored "-pedantic"
//#pragma GCC diagnostic pop
//#pragma GCC diagnostic ignored "-fno-permissive"

typedef void* (*PCM_FUNC)(void*,...);

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
MAKE_FUNC(snd_async_add_pcm_handler);
MAKE_FUNC(snd_async_handler_get_callback_private);

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
#define snd_async_add_pcm_handler psnd_async_add_pcm_handler
#define snd_async_handler_get_callback_private psnd_async_handler_get_callback_private


void _callback(snd_async_handler_t *pcm_callback)
{
  kgmAlsa* obj = (kgmAlsa*)snd_async_handler_get_callback_private((snd_pcm_t*)pcm_callback);

  if(obj)
    obj->render();
  /*snd_pcm_t *pcm_handle = snd_async_handler_get_pcm(pcm_callback);
  snd_pcm_sframes_t avail;
  int err;

  avail = snd_pcm_avail_update(pcm_handle);
  while (avail >= period_size) {
     snd_pcm_writei(pcm_handle, MyBuffer, period_size);
     avail = snd_pcm_avail_update(handle);
     }
     */
 }

#endif //ALSA

struct _Sound
{
  enum State
  {
    StStop,
    StPlay,
    StPause
  };

  void *data;
  u32   size;
  u32   cursor;

  int bps;
  int rate;
  int frames;
  int channels;

  bool loop;
  bool remove;

  State state;

  _Sound(kgmIAudio::FMT fmt, u16 freq, u32 size, void* data)
  {
    this->data   = null;
    this->size   = 0;

    cursor = 0;

    bps      = 0;
    rate     = freq;
    frames   = 0;
    channels = 0;

    loop = false;
    state = StStop;

    remove = false;

    rate = freq;

    switch(fmt)
    {
    case kgmIAudio::FMT_MONO8:
      bps = 8;
      channels = 1;
      break;
    case kgmIAudio::FMT_MONO16:
      bps = 16;
      channels = 1;
      break;
    case kgmIAudio::FMT_STEREO8:
      bps = 8;
      channels = 2;
      break;
    case kgmIAudio::FMT_STEREO16:
    default:
      bps = 16;
      channels = 2;
    }

    if(data != null && size > 0)
    {
      this->data = malloc(size);

      memcpy(this->data, data, size);

      this->size = size;
    }
  }

  ~_Sound()
  {
    if(data)
      free(data);
  }

  void release()
  {
    remove = true;
  }

  void stop()
  {
    state  = StStop;

    cursor = 0;
  }

  void play(bool lp)
  {
    state = StPlay;

    loop = lp;
  }

  void pause(bool state)
  {
    if(state)
      state = StPause;
    else
      state = StPlay;
  }

  void volume(float vol)
  {

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
    //LOAD_FUNC(snd_async_add_pcm_handler);
    //LOAD_FUNC(snd_async_handler_get_callback_private);

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
      else
      {
        int pcm = 0;

        if(pcm = snd_pcm_set_params(m_handle, SND_PCM_FORMAT_S16_LE,
                                    SND_PCM_ACCESS_RW_INTERLEAVED, 2,
                                    44100, 1, 1500 * 1000) < 0)
        {
          printf("Playback set param error: %s\n", snd_strerror(pcm));
        }

        if(pcm = snd_pcm_prepare(m_handle) < 0)
          printf("ERROR: Can't prepare. %s\n", snd_strerror(pcm));

        m_mixer.prepare(2, 16, 44100);

        //m_callback = null;
        //pcm = snd_async_add_pcm_handler(&m_callback, m_handle, _callback, this);

        if(pcm < 0)
          printf("ERROR: Can't prepare handler %s.\n", snd_strerror(pcm));

        snd_pcm_start(m_handle);

        m_thread.start(this, (int(*)(kgmAlsa*))&kgmAlsa::proceed);
        //m_render.start(this, (int(*)(kgmAlsa*))&kgmAlsa::render);

        //m_thread.priority(kgmThread::PrIdle);
        //m_render.priority(kgmThread::PrIdle);
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

  m_proceed = false;
  m_render.join();
  m_thread.join();

  m_lib.close();
}

kgmIAudio::Sound kgmAlsa::create(FMT fmt, u16 freq, u32 size, void* data)
{
  _Sound* sound = new _Sound(fmt, freq, size, data);

  m_sounds.add(sound);

  return sound;
}

void kgmAlsa::remove(Sound snd)
{
  if(snd)
    ((_Sound*)snd)->remove = true;
}

void kgmAlsa::channel(Sound snd, s16 pan)
{

}

void kgmAlsa::volume(Sound snd, u16 vol)
{

}

void kgmAlsa::pause(Sound snd, bool stat)
{
  if(snd)
    ((_Sound*)snd)->pause(stat);
}

void kgmAlsa::play(Sound snd, bool loop)
{
  if(snd)
    ((_Sound*)snd)->play(loop);
}

void kgmAlsa::stop(Sound snd)
{
  if(snd)
    ((_Sound*)snd)->stop();
}

int kgmAlsa::render()
{
  //while(m_proceed)
  {
#ifdef ALSA
    if(m_handle && m_mixer.getBuffer())
    {
      int pcm = 0;
      int bpp = 1;

      int frames = m_mixer.getFrames();

      int avail = m_mixer.getLength();

      char* WritePtr = m_mixer.getBuffer();

      avail = frames;

      while(avail > 0)
      {
        int ret = snd_pcm_writei(m_handle, WritePtr, avail);
        //int ret = snd_pcm_writei(m_handle, WritePtr, 100);

        switch (ret)
        {
        case -EAGAIN:
          continue;
        case -ESTRPIPE:
        case -EPIPE:
        case -EINTR:
          ret = snd_pcm_recover(m_handle, ret, 1);

          if(ret < 0)
            avail = 0;

          break;
        default:
          if(ret >= 0)
          {
            //u32 k = (u32)snd_pcm_frames_to_bytes(m_handle, ret);
            u32 k = ret * m_mixer.getBytesPerFrame();
            WritePtr += k;
            avail -= ret;

            //if((WritePtr - (char*)m_mixer.getBuffer()) >= m_mixer.getLength())
            if(avail < 1)
              break;
          }
          break;
        }

        if (ret < 0)
        {
          ret = snd_pcm_prepare(m_handle);

          if(ret < 0)
            break;
        }
      }

      //if(pcm = snd_pcm_writei(handle, data, pcm_frames) != pcm_frames)
      //printf("ERROR: Can't write. %s\n", snd_strerror(pcm));

      //if(pcm = snd_pcm_drain(m_handle) < 0)
      //  printf("ERROR: Can't drain. %s\n", snd_strerror(pcm));
    }

    //kgmThread::sleep(1000);
#endif
  }

  return 1;
}

int kgmAlsa::proceed()
{
  m_proceed = true;

  while(m_proceed)
  {
    m_mixer.clean();

    for(kgmList<_Sound*>::iterator i = m_sounds.begin(); i != m_sounds.end(); ++i)
    {
      _Sound* sound = (*i);

      if(sound->remove)
      {
        i = m_sounds.erase(i);

        delete sound;

        continue;
      }

      if(sound->state != _Sound::StPlay)
        continue;

      //continue;

      u32 size = m_mixer.mixdata((sound->data + sound->cursor), (sound->size - sound->cursor),
                                 sound->channels, sound->bps, sound->rate);

      if((sound->cursor + size) >= sound->size)
      {
        sound->cursor = 0;

        if(!sound->loop)
          sound->stop();
      }
      else
      {
        sound->cursor += size;
      }
    }

    //kgm_log() << "Start audio render " << kgmTime::getTimeText() << "\n";
    render();
    //kgmThread::sleep(1);
    //kgm_log() << "End   audio render " << kgmTime::getTimeText() << "\n";
  }

  return 0;
}
