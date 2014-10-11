// kgmOALDevice.cpp: implementation of the kgmOALDevice class.
//
//////////////////////////////////////////////////////////////////////

#include "kgmDSound.h"
#include "../kgmBase/kgmLog.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

static u32 error = 0;

#ifdef DSOUND
/*struct _Sound
{
  LPDIRECTSOUNDBUFFER pSb;

  _Sound(LPDIRECTSOUNDBUFFER sb);
  ~_Sound();

  void release();
  void stop();
  void play(bool loop);
  void pause();
  void volume(float vol);
  void emit(vec3& pos, vec3& vel);

  void drop();
};

_Sound::_Sound(LPDIRECTSOUNDBUFFER sb)
{
  pSb = sb;
}

_Sound::~_Sound()
{
}

void _Sound::release()
{
  if(pSb)
    pSb->Release();
}

void _Sound::stop()
{
}

void _Sound::play(bool loop)
{
  // should be check because don't play
  if(pSb)
  {
    if(FAILED(pSb->Play(0, 0, (loop) ? (DSBPLAY_LOOPING) : (0))))
    {
#ifdef DEBUG
    kgm_log() << "Error: can't play sound buffer.\n";
#endif
    }
  }
}

void _Sound::volume(float vol)
{
}

void _Sound::pause()
{
}
*/
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
    this->data   = null;
    this->size   = 0;

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

kgmDSound::kgmDSound()
{
  m_pSnd = null;
  m_pSbuf = null;

  if(FAILED(DirectSoundCreate(0, &m_pSnd, 0)))
  {
#ifdef DEBUG
    kgm_log() << "Error: can't create direct sound.\n";
#endif
    return;
  }

  //Set Cooperative Level
   HWND hWnd = GetForegroundWindow();

   if (hWnd == NULL)
   {
    hWnd = GetDesktopWindow();
   }

   if(m_pSnd->SetCooperativeLevel((HWND)hWnd, DSSCL_PRIORITY) != DS_OK)
  {
#ifdef DEBUG
    kgm_log() << "Error: can't set cooperative level.\n";
#endif
  }

  m_mixer.prepare(2, 16, 44100);

  u32   size = m_mixer.getLength();
  void* data = m_mixer.getBuffer();

   DSBUFFERDESC dsb = {0};
   WAVEFORMATEX wf = {0};

   if(!m_pSnd)
   {
 #ifdef DEBUG
     kgm_log() << "Error: not valid direct sound or data.\n";
 #endif

     return;
   }

   wf.cbSize          = sizeof(WAVEFORMATEX);
   wf.nSamplesPerSec  = 44100;
   wf.wFormatTag      = WAVE_FORMAT_PCM;

   switch(FMT_STEREO16)
   {
   case FMT_MONO8:
     wf.nChannels = 1;
     wf.wBitsPerSample = 8;
     break;
   case FMT_MONO16:
     wf.nChannels = 1;
     wf.wBitsPerSample = 16;
     break;
   case FMT_STEREO8:
     wf.nChannels = 2;
     wf.wBitsPerSample = 8;
     break;
   case FMT_STEREO16:
     wf.nChannels = 2;
     wf.wBitsPerSample = 16;
     break;
   }

   wf.nBlockAlign = wf.nChannels * ( wf.wBitsPerSample / 8 );
   wf.nAvgBytesPerSec = wf.nBlockAlign * wf.nSamplesPerSec;

   dsb.dwSize = sizeof(DSBUFFERDESC);
   dsb.dwFlags = DSBCAPS_STATIC | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPAN;
   dsb.dwBufferBytes = size;
   dsb.lpwfxFormat = &wf;

   if(FAILED(m_pSnd->CreateSoundBuffer(&dsb, &m_pSbuf, 0)))
   {
 #ifdef DEBUG
     kgm_log() << "Error: can't create sound buffer.\n";
 #endif
     m_pSbuf = null;
   }

   if(m_pSbuf)
   {
     VOID   *ptr1, *ptr2;
     DWORD  size1, size2;

     if(DS_OK == m_pSbuf->Lock(0, 0, &ptr1, &size1, NULL, NULL, DSBLOCK_ENTIREBUFFER))
     {
       memcpy(ptr1, data, size);

       if(FAILED(m_pSbuf->Unlock(ptr1, size, NULL, 0)))
       {
    #ifdef DEBUG
        kgm_log() << "Error: can't unlock sound buffer.\n";
    #endif
       }
     }
 #ifdef DEBUG
     else
     {
       kgm_log() << "Error: can't Lock sound buffer.\n";
     }
 #endif

   }
}

kgmDSound::~kgmDSound()
{
  if(m_pSbuf)
  {
    if(FAILED(m_pSbuf->Release()))
    {
#ifdef DEBUG
      kgm_log() << "Error: can't release direct sound buffer.\n";
#endif
    }
  }

  if(m_pSnd)
  {
    if(FAILED(m_pSnd->Release()))
    {
#ifdef DEBUG
      kgm_log() << "Error: can't release direct sound.\n";
#endif
    }
  }
}

kgmIAudio::Sound kgmDSound::create(FMT fmt, u16 freq, u32 size, void* data)
{
  _Sound* sound = new _Sound(fmt, freq, size, data);

  sound->vol = kgmIAudio::VolMax;
  sound->pan = kgmIAudio::PanBalance;

  m_sounds.add(sound);

  return sound;
}

void kgmDSound::remove(Sound snd)
{
  if(snd)
    ((_Sound*)snd)->remove = true;
}

void kgmDSound::channel(Sound snd, s16 pan)
{
  ((_Sound*)snd)->pan = pan;
}

void kgmDSound::volume(Sound snd, u16 vol)
{
  ((_Sound*)snd)->vol = vol;
}

void kgmDSound::pause(Sound snd, bool stat)
{
  if(snd)
    ((_Sound*)snd)->pause(stat);
}

void kgmDSound::play(Sound snd, bool loop)
{
  if(snd)
    ((_Sound*)snd)->play(loop);
}

void kgmDSound::stop(Sound snd)
{
  if(snd)
    ((_Sound*)snd)->stop();
}

#endif
