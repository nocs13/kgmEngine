#include "kgmAlsa.h"
#include "../kgmBase/kgmLog.h"
#include "../kgmBase/kgmTime.h"
#include "../kgmBase/kgmPointer.h"

#undef DEBUG

#ifdef ALSA

#include "inc/alsa/asoundlib.h"

static const char alsaDevice[] = "default";

#pragma GCC diagnostic push
#pragma GCC diagnostic warning "-fpermissive"

//#pragma GCC diagnostic ignored "-pedantic"
//#pragma GCC diagnostic pop
//#pragma GCC diagnostic ignored "-fno-permissive"

typedef void* (*PCM_FUNC)(void*, ...);

//#define MAKE_FUNC(f) static typeof(f) p##f
#define MAKE_FUNC(f) static PCM_FUNC p##f \

MAKE_FUNC(snd_strerror);
MAKE_FUNC(snd_pcm_open);
MAKE_FUNC(snd_pcm_close);
MAKE_FUNC(snd_pcm_nonblock);
MAKE_FUNC(snd_pcm_frames_to_bytes);
MAKE_FUNC(snd_pcm_bytes_to_frames);
MAKE_FUNC(snd_pcm_hw_params_malloc);
MAKE_FUNC(snd_pcm_hw_params_free);
MAKE_FUNC(snd_pcm_hw_params_any);
MAKE_FUNC(snd_pcm_hw_params_current);
MAKE_FUNC(snd_pcm_hw_params_set_access);
MAKE_FUNC(snd_pcm_hw_params_set_format);
MAKE_FUNC(snd_pcm_hw_params_set_channels);
MAKE_FUNC(snd_pcm_hw_params_set_periods_near);
MAKE_FUNC(snd_pcm_hw_params_set_rate_near);
MAKE_FUNC(snd_pcm_hw_params_set_rate);
MAKE_FUNC(snd_pcm_hw_params_set_rate_resample);
MAKE_FUNC(snd_pcm_hw_params_set_buffer_time_near);
MAKE_FUNC(snd_pcm_hw_params_set_period_time_near);
MAKE_FUNC(snd_pcm_hw_params_set_buffer_size_near);
MAKE_FUNC(snd_pcm_hw_params_set_period_size_near);
MAKE_FUNC(snd_pcm_hw_params_set_buffer_size_min);
MAKE_FUNC(snd_pcm_hw_params_get_buffer_time_min);
MAKE_FUNC(snd_pcm_hw_params_get_buffer_time_max);
MAKE_FUNC(snd_pcm_hw_params_get_period_time_min);
MAKE_FUNC(snd_pcm_hw_params_get_period_time_max);
MAKE_FUNC(snd_pcm_hw_params_get_buffer_size);
MAKE_FUNC(snd_pcm_hw_params_get_period_size);
MAKE_FUNC(snd_pcm_hw_params_get_access);
MAKE_FUNC(snd_pcm_hw_params_get_periods);
MAKE_FUNC(snd_pcm_hw_params_test_format);
MAKE_FUNC(snd_pcm_hw_params_test_channels);
MAKE_FUNC(snd_pcm_hw_params);
MAKE_FUNC(snd_pcm_sw_params_malloc);
MAKE_FUNC(snd_pcm_sw_params_current);
MAKE_FUNC(snd_pcm_sw_params_set_avail_min);
MAKE_FUNC(snd_pcm_sw_params_set_stop_threshold);
MAKE_FUNC(snd_pcm_sw_params);
MAKE_FUNC(snd_pcm_sw_params_free);
MAKE_FUNC(snd_pcm_prepare);
MAKE_FUNC(snd_pcm_start);
MAKE_FUNC(snd_pcm_resume);
MAKE_FUNC(snd_pcm_reset);
MAKE_FUNC(snd_pcm_wait);
MAKE_FUNC(snd_pcm_delay);
MAKE_FUNC(snd_pcm_state);
MAKE_FUNC(snd_pcm_avail_update);
MAKE_FUNC(snd_pcm_areas_silence);
MAKE_FUNC(snd_pcm_mmap_begin);
MAKE_FUNC(snd_pcm_mmap_commit);
MAKE_FUNC(snd_pcm_readi);
MAKE_FUNC(snd_pcm_writei);
MAKE_FUNC(snd_pcm_drain);
MAKE_FUNC(snd_pcm_drop);
MAKE_FUNC(snd_pcm_recover);
MAKE_FUNC(snd_pcm_info_malloc);
MAKE_FUNC(snd_pcm_info_free);
MAKE_FUNC(snd_pcm_info_set_device);
MAKE_FUNC(snd_pcm_info_set_subdevice);
MAKE_FUNC(snd_pcm_info_set_stream);
MAKE_FUNC(snd_pcm_info_get_name);
MAKE_FUNC(snd_pcm_set_params);
MAKE_FUNC(snd_ctl_pcm_next_device);
MAKE_FUNC(snd_ctl_pcm_info);
MAKE_FUNC(snd_ctl_open);
MAKE_FUNC(snd_ctl_close);
MAKE_FUNC(snd_ctl_card_info_malloc);
MAKE_FUNC(snd_ctl_card_info_free);
MAKE_FUNC(snd_ctl_card_info);
MAKE_FUNC(snd_ctl_card_info_get_name);
MAKE_FUNC(snd_ctl_card_info_get_id);
MAKE_FUNC(snd_card_next);
MAKE_FUNC(snd_config_update_free_global);
#undef MAKE_FUNC

static int run_recovery(snd_pcm_t *handle, size_t err)
{
  if (err == -EPIPE)
  {
    err = (size_t) psnd_pcm_prepare(handle);

    if(err >= 0)
      err = (size_t) psnd_pcm_start(handle);

    if (err < 0)
      kgm_log() << "kgmAlsa: prepare failed: " << (char*)psnd_strerror((void*) err) << ".\n";
  }
  else if (err == -ESTRPIPE)
  {
    while ((err = (size_t) psnd_pcm_resume(handle)) == -EAGAIN)
      usleep(1);

    if (err < 0)
    {
      err = (size_t) psnd_pcm_prepare(handle);

      if(err >= 0)
        err = (size_t) psnd_pcm_start(handle);

      if (err < 0)
        kgm_log() << "kgmAlsa: prepare failed: " << (char*)psnd_strerror((void*) err) << ".\n";
    }
  }

  return err;
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

  //void *data;
  kgm_ptr<u8> data;
  u32   size;
  u32   cursor;

  s16   vol;
  s16   pan;

  int bps;
  int rate;
  int frames;
  int channels;

  bool loop;
  bool remove;

  State state;

  _Sound(kgmIAudio::FMT fmt, u16 freq, u32 size, void* data)
  {
    this->size = 0;

    cursor = 0;

    pan      = 0;
    vol      = 0;

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
      this->data = kgm_ptr<u8>(new u8[size]);

      memcpy(((u8*)this->data), data, size);

      this->size = size;
    }
  }

  ~_Sound()
  {
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
  m_mutex  = null;

  m_proceed = 0;


  if (m_lib.open((char*) "libasound.so.2") || m_lib.open((char*) "libasound.so.1") ||
      m_lib.open((char*) "libasound.so.0") || m_lib.open((char*) "libasound.so"))
  {
    size_t err = 0;

#ifdef ALSA
#define LOAD_FUNC(f) p##f = (PCM_FUNC) m_lib.get((char*) #f)
    LOAD_FUNC(snd_strerror);
    LOAD_FUNC(snd_pcm_open);
    LOAD_FUNC(snd_pcm_close);
    LOAD_FUNC(snd_pcm_nonblock);
    LOAD_FUNC(snd_pcm_frames_to_bytes);
    LOAD_FUNC(snd_pcm_hw_params_malloc);
    LOAD_FUNC(snd_pcm_hw_params_free);
    LOAD_FUNC(snd_pcm_hw_params_any);
    LOAD_FUNC(snd_pcm_hw_params_set_access);
    LOAD_FUNC(snd_pcm_hw_params_set_format);
    LOAD_FUNC(snd_pcm_hw_params_set_channels);
    LOAD_FUNC(snd_pcm_hw_params_set_periods_near);
    LOAD_FUNC(snd_pcm_hw_params_set_rate_near);
    LOAD_FUNC(snd_pcm_hw_params_set_rate);
    LOAD_FUNC(snd_pcm_hw_params_set_buffer_size_near);
    LOAD_FUNC(snd_pcm_hw_params_set_buffer_size_min);
    LOAD_FUNC(snd_pcm_hw_params_get_period_size);
    LOAD_FUNC(snd_pcm_hw_params_get_access);
    LOAD_FUNC(snd_pcm_hw_params);
    LOAD_FUNC(snd_pcm_set_params);
    LOAD_FUNC(snd_pcm_prepare);
    LOAD_FUNC(snd_pcm_start);
    LOAD_FUNC(snd_pcm_resume);
    LOAD_FUNC(snd_pcm_state);
    LOAD_FUNC(snd_pcm_reset);
    LOAD_FUNC(snd_pcm_avail_update);
    LOAD_FUNC(snd_pcm_areas_silence);
    LOAD_FUNC(snd_pcm_mmap_begin);
    LOAD_FUNC(snd_pcm_mmap_commit);
    LOAD_FUNC(snd_pcm_writei);
    LOAD_FUNC(snd_pcm_drain);
    LOAD_FUNC(snd_pcm_info_malloc);
    LOAD_FUNC(snd_pcm_info_free);
    LOAD_FUNC(snd_pcm_info_set_device);
    LOAD_FUNC(snd_pcm_info_set_subdevice);
    LOAD_FUNC(snd_pcm_info_set_stream);
    LOAD_FUNC(snd_pcm_info_get_name);
    LOAD_FUNC(snd_ctl_pcm_next_device);
    LOAD_FUNC(snd_ctl_pcm_info);
    LOAD_FUNC(snd_ctl_open);
    LOAD_FUNC(snd_ctl_close);
    LOAD_FUNC(snd_ctl_card_info_malloc);
    LOAD_FUNC(snd_ctl_card_info_free);
    LOAD_FUNC(snd_ctl_card_info);
    LOAD_FUNC(snd_ctl_card_info_get_name);
    LOAD_FUNC(snd_card_next);
#undef LOAD_FUNC

    if(snd_pcm_open)
    {
      char* device = (char*) "default";
      //char* device = "plughw:0,0";
      err = (size_t)psnd_pcm_open((void**)&m_handle, device, SND_PCM_STREAM_PLAYBACK, SND_PCM_NONBLOCK);

      if(err < 0)
      {
        usleep(200000);

        err = (size_t)psnd_pcm_open((void**)&m_handle, device, SND_PCM_STREAM_PLAYBACK, SND_PCM_NONBLOCK);

        if(err < 0)
        {
          kgm_log() << "kgmAlsa Error: Can't open alsa device.\n";
          kgm_log() << "kgmAlsa Error: Is " << (char*)kgmString((char*)psnd_strerror((void*)err)) << ".\n";

          return;
        }
      }

      if(err == 0)
      {
        err = (size_t) psnd_pcm_nonblock(m_handle, 0);

        if(err < 0)
        {
          psnd_pcm_close(m_handle);

          m_handle = null;

          return;
        }

        m_mixer.prepare(2, 16, 44100);

        snd_pcm_hw_params_t *params = null;

        psnd_pcm_hw_params_malloc(&params);

        if(params != null)
        {
          int periods = 4;
          int frequency = 44100;
          snd_pcm_uframes_t bufferSizeInFrames = m_mixer.getFrames();

          if((psnd_pcm_hw_params_any(m_handle, params) == 0) &&
             (psnd_pcm_hw_params_set_access(m_handle, params, SND_PCM_ACCESS_MMAP_INTERLEAVED) == 0) &&
             (psnd_pcm_hw_params_set_format(m_handle, params, SND_PCM_FORMAT_S16_LE) == 0) &&
             (psnd_pcm_hw_params_set_channels(m_handle, params, 2) == 0) &&
             (psnd_pcm_hw_params_set_periods_near(m_handle, params, &periods, NULL) == 0) &&
             (psnd_pcm_hw_params_set_rate_near(m_handle, params, &frequency, NULL) == 0) &&
             (psnd_pcm_hw_params_set_buffer_size_near(m_handle, params, &bufferSizeInFrames) == 0) &&
             (psnd_pcm_hw_params(m_handle, params) == 0))
          {
            kgm_log() << "kgmAlsa: wow set params :S \n";
          }
          else
          {
            printf("kgmAlsa: %d failed: %s\n", (int) err, (char*) psnd_strerror((void*) err));
          }

          psnd_pcm_hw_params_free(params);
          //psnd_pcm_close(m_handle);
        }

        if(err = psnd_pcm_set_params(m_handle, SND_PCM_FORMAT_S16_LE,
                                     SND_PCM_ACCESS_RW_INTERLEAVED, 2,
                                     44100, 1, m_mixer.getMsTime() * 1000) < 0)
        {
          kgm_log() << "kgmAlsa: Playback set param error: " << (char*)psnd_strerror((void*) err) << ".\n";

          return;
        }

        if(err = psnd_pcm_prepare(m_handle) < 0)
        {
          kgm_log() << "kgmAlsa ERROR: Can't prepare " << (char*)psnd_strerror((void*) err) << ".\n";

          return;
        }

        m_thread.start(this, &kgmAlsa::proceed);
        m_render.start(this, &kgmAlsa::render);

        m_thread.priority(kgmThread::PrIdle);
        m_render.priority(kgmThread::PrIdle);
        
        m_proceed = true;

        m_mutex = kgmThread::mutex();
      }
    }
#endif
  }
}

kgmAlsa::~kgmAlsa()
{
  for(int i = m_sounds.length(); i > 0; i--)
    delete m_sounds[i - 1];

  m_sounds.clear();

#ifdef DEBUG
  kgm_log() << "kgmAlsa::~kgmAlsa.\n";
#endif
}

void kgmAlsa::clear()
{
  m_proceed = false;

  m_render.join();
  m_thread.join();

  kgmThread::mxfree(m_mutex);

  m_mutex = null;

#ifdef ALSA
  kgm_log() << "kgmAlsa snd_pcm_close.\n";

  if(m_handle)
  {
    psnd_pcm_reset(m_handle);
    psnd_pcm_close(m_handle);
  }
#endif

  m_lib.close();
}

kgmIAudio::Sound kgmAlsa::create(FMT fmt, u16 freq, u32 size, void* data)
{
  _Sound* sound = new _Sound(fmt, freq, size, data);

  sound->vol = kgmIAudio::VolMax;
  sound->pan = kgmIAudio::PanBalance;

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
  ((_Sound*)snd)->pan = pan;
}

void kgmAlsa::volume(Sound snd, u16 vol)
{
  ((_Sound*)snd)->vol = vol;
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

int kgmAlsa::render(int r)
{
  //(void*)r;
  
  while(m_proceed)
  {
#ifdef ALSA
    if(m_handle && m_mixer.getBuffer() && m_mixer.getFrames())
    {
      int pcm = 0;
      int bpp = 1;
      long avail = 0;

      snd_pcm_uframes_t offset, frames;
      const snd_pcm_channel_area_t *areas = NULL;

      avail = (long) psnd_pcm_avail_update((void*) m_handle);

      if(avail < 0)
        run_recovery((snd_pcm_t*) m_handle, avail);

      avail = m_mixer.getFrames();

      char* WritePtr = (char*) m_mixer.getBuffer();

      //kgm_log() << "Available frames " << avail << "\n";

      kgmThread::lock(m_mutex);

      u32 t1 = kgmTime::getTicks();

#ifdef DEBUG
      kgm_log() << "kgmAlsa:: render lock " << kgmTime::getTimeText() << "\n";
#endif

      while(avail > 0)
      {
        long ret;

        /*frames = avail;

        ret = psnd_pcm_mmap_begin(m_handle, &areas, &offset, &frames);

        if(ret < 0)
        {
          ret = run_recovery(m_handle, ret);

          if(ret < 0)
            break;
        }

        WritePtr = (char*)areas->addr + (offset * areas->step / 8);
        int WriteCnt = psnd_pcm_frames_to_bytes(m_handle, frames);

        int commitres = psnd_pcm_mmap_commit(m_handle, offset, frames);

        if (commitres < 0 || (commitres-frames) != 0)
        {
          kgm_log() << "mmap commit error: "
                    << (char*)psnd_strerror(commitres >= 0 ? -EPIPE : commitres) << "\n";
          break;
        }*/

        ret = (long) psnd_pcm_writei(m_handle, WritePtr, avail);

        switch (ret)
        {
        case -EAGAIN:
          continue;
        case -ESTRPIPE:
        case -EPIPE:
        case -EINTR:
          ret = (long) psnd_pcm_recover(m_handle, ret, 1);

          if(ret < 0)
            avail = 0;

          break;
        default:
          if(ret >= 0)
          {
            u32 k = ret * m_mixer.getBytesPerFrame();

            WritePtr += k;

            avail -= ret;

            if(avail < 1)
              break;
          }
          break;
        }

        if (ret < 0)
        {
          ret = (long) psnd_pcm_prepare(m_handle);

          if(ret < 0)
            break;
        }
      }

      //if(pcm = psnd_pcm_drain(m_handle) < 0)
      //  kgm_log() << "kgmAlsa ERROR: Can't drain. " << (char*)psnd_strerror(pcm) << "\n";

      u32 t2 = kgmTime::getTicks();

      u32 t3 = t2 - t1;

      if(t3 < m_mixer.getMsTime())
      {
        s32 wtime = m_mixer.getMsTime() - t3;

        kgmThread::sleep(wtime / 2);
      }

      m_mixer.clean();
      kgmThread::unlock(m_mutex);

#ifdef DEBUG
      kgm_log() << "kgmAlsa:: render unlock " << kgmTime::getTimeText() << "\n";
#endif
    }
    else
    {
      kgmThread::sleep(m_mixer.getMsTime() - 1);
    }
#endif
  }

  return 1;
}

int kgmAlsa::proceed(int p)
{
  static u32 max_sounds = 10;

  //(void*)p;
  
  m_proceed = true;

  while(m_proceed)
  {
    u32 snd_cound = 0;

    kgmThread::lock(m_mutex);

#ifdef DEBUG
    kgm_log() << "kgmAlsa:: proceed lock " << kgmTime::getTimeText() << "\n";
#endif

    m_mixer.clean();

    u32 t1 = kgmTime::getTicks();

    for(kgmList<_Sound*>::iterator i = m_sounds.begin(); i != m_sounds.end(); ++i)
    {
      _Sound* sound = (*i);

      if(snd_cound > max_sounds)
        break;

      if(sound->remove)
      {
        i = m_sounds.erase(i);

        delete sound;

        continue;
      }

      if(sound->state != _Sound::StPlay)
        continue;

      u32 size = m_mixer.mixdata((((u8*)sound->data) + sound->cursor),
                                 (sound->size - sound->cursor),
                                 sound->channels,
                                 sound->bps,
                                 sound->rate,
                                 sound->vol,
                                 sound->pan);

      if((sound->cursor + size) == sound->size)
      {
        sound->cursor = 0;

        if(!sound->loop)
          sound->stop();
      }
      else
      {
        sound->cursor += size;
      }

      snd_cound++;
    }

    kgmThread::unlock(m_mutex);

#ifdef DEBUG
    kgm_log() << "kgmAlsa:: proceed unlock " << kgmTime::getTimeText() << "\n";
#endif

    u32 t2 = kgmTime::getTicks();

    u32 t3 = t2 - t1;

    if(t3 < m_mixer.getMsTime())
    {
      s32 wtime = m_mixer.getMsTime() - t3;

#ifdef DEBUG
    kgm_log() << "kgmAlsa:: proceed sleeping " << wtime / 2 << "\n";
#endif

      kgmThread::sleep(wtime / 2);
    }
  }

  return 0;
}
