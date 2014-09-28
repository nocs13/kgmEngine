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
struct _Sound
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

kgmDSound::kgmDSound()
{
  m_pSnd = null;

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
}

kgmDSound::~kgmDSound()
{
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

//kgmSound* kgmOAL::generic(kgmWave* wav)
kgmIAudio::Sound kgmDSound::create(FMT fmt, u16 freq, u32 size, void* data)
{
  DSBUFFERDESC dsb = {0};
  LPDIRECTSOUNDBUFFER pSb = 0;
  WAVEFORMATEX        wf = {0};

  if(!m_pSnd || !data || !size)
  {
#ifdef DEBUG
    kgm_log() << "Error: not valid direct sound or data.\n";
#endif

    return null;
  }

  wf.cbSize          = sizeof(WAVEFORMATEX);
  wf.nSamplesPerSec  = freq;
  wf.wFormatTag      = WAVE_FORMAT_PCM;

  switch(fmt)
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

  if(FAILED(m_pSnd->CreateSoundBuffer(&dsb, &pSb, 0)))
  {
#ifdef DEBUG
    kgm_log() << "Error: can't create sound buffer.\n";
#endif
    pSb = null;
  }

  if(pSb)
  {
    VOID   *ptr1, *ptr2;
    DWORD  size1, size2;

    if(DS_OK == pSb->Lock(0, 0, &ptr1, &size1, NULL, NULL, DSBLOCK_ENTIREBUFFER))
    {
      memcpy(ptr1, data, size);

      if(FAILED(pSb->Unlock(ptr1, size, NULL, 0)))
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

  return (pSb) ? (new _Sound(pSb)) : (null);
}
#endif
