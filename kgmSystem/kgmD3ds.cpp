// kgmOALDevice.cpp: implementation of the kgmOALDevice class.
//
//////////////////////////////////////////////////////////////////////

#include "kgmD3ds.h"
#include "../kgmBase/kgmLog.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

static u32 error = 0;

#ifdef D3DS

kgmD3ds::_Sound::_Sound(LPDIRECTSOUNDBUFFER sb){
  pSb = sb;
}

kgmD3ds::_Sound::~_Sound(){
}

void kgmD3ds::_Sound::release(){
  if(pSb)
    pSb->Release();
}

void kgmD3ds::_Sound::stop(){
}

void kgmD3ds::_Sound::play(bool loop){
  // should be check because don't play
  if(pSb)
  {
    pSb->Play(0, 0, (loop)?(DSBPLAY_LOOPING):(0));
  }
}

void kgmD3ds::_Sound::volume(float vol){
}

void kgmD3ds::_Sound::pause(){
}

void kgmD3ds::_Sound::emit(vec3& pos, vec3& vel){
}

void kgmD3ds::_Sound::drop()
{
  delete this;
}

kgmD3ds::kgmD3ds()
{
  m_pSnd = null;

  if(FAILED(DirectSoundCreate(0, &m_pSnd, 0)))
  {
#ifdef DEBUG
    kgm_log() << "Error: can't create direct sound\n";
#endif
    return;
  }

  if(m_pSnd->SetCooperativeLevel((HWND)GetDesktopWindow(), DSSCL_NORMAL) != DS_OK)
  {
#ifdef DEBUG
    kgm_log() << "Error: can't set cooperative level\n";
#endif
  }
}

kgmD3ds::~kgmD3ds()
{
  if(m_pSnd)
    m_pSnd->Release();
}

//kgmSound* kgmOAL::generic(kgmWave* wav)
kgmIAudio::Sound* kgmD3ds::create(FMT fmt, u16 freq, u32 size, void* data)
{
  DSBUFFERDESC dsb = {0};
  LPDIRECTSOUNDBUFFER pSb = 0;
  WAVEFORMATEX        wf = {0};

  if(!m_pSnd || !data || !size)
    return null;

  wf.cbSize          = 0;
  wf.nSamplesPerSec  = freq;
  wf.wFormatTag      = WAVE_FORMAT_PCM;

  switch(fmt){
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
    wf.wBitsPerSample = 8;
    break;
  }

  wf.nBlockAlign = wf.nChannels * ( wf.wBitsPerSample / 8 );
  wf.nAvgBytesPerSec = wf.nBlockAlign * wf.nSamplesPerSec;

  dsb.dwSize = sizeof(dsb);
  dsb.dwFlags = DSBCAPS_STATIC;
  dsb.dwBufferBytes = size;
  dsb.lpwfxFormat = &wf;

  if(FAILED(m_pSnd->CreateSoundBuffer(&dsb, &pSb, 0)))
  {
#ifdef DEBUG
    kgm_log() << "Error: can't create sound buffer\n";
#endif
    pSb = null;
  }

  if(pSb)
  {
    VOID   *ptr1, *ptr2;
    DWORD  size1, size2;

    if(DS_OK == pSb->Lock(0, size, &ptr1, &size1, &ptr2, &size2, 0L))
    {
      memcpy(ptr1, data, size);
      pSb->Unlock(ptr1, size, NULL, 0);
    }
#ifdef DEBUG
    else
    {
      kgm_log() << "Error: can't Lock sound buffer\n";
    }
#endif

  }

  return (pSb) ? (new _Sound(pSb)) : (null);
}

void kgmD3ds::listener(vec3& pos, vec3& vel, vec3& ort)
{
  float l = vel.length();
  float dirort[6] = {vel.x, vel.y, vel.z, ort.x, ort.y, ort.z};
}

void kgmD3ds::clear()
{
}
#endif
