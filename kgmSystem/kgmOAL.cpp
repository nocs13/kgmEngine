// kgmOALDevice.cpp: implementation of the kgmOALDevice class.
//
//////////////////////////////////////////////////////////////////////

#include "kgmOAL.h"
#include "../kgmBase/kgmLog.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#ifdef OAL

static u32 error = 0;

kgmOAL::_Sound::_Sound(u32 buf, u32 src)
{
  buffer = buf;
  source = src;
}

kgmOAL::_Sound::~_Sound()
{
}

void kgmOAL::_Sound::release()
{
  alDeleteSources(1, &source);
  alDeleteBuffers(1, &buffer);

  delete this;
}

void kgmOAL::_Sound::stop()
{
  alSourceStop(source);
}

void kgmOAL::_Sound::play(bool loop)
{
  alSourcei(source, AL_LOOPING, (loop)?(AL_TRUE):(AL_FALSE));
  alSourcePlay(source);

  if((error = alGetError()) != AL_NO_ERROR)
  {
#ifdef DEBUG
    kgm_log() << "error of playing sound\n";
#endif
  }
}

void kgmOAL::_Sound::volume(float vol)
{
}

void kgmOAL::_Sound::pause()
{
}

void kgmOAL::_Sound::emit(vec3& pos, vec3& vel)
{
  alSource3f(source, AL_POSITION, pos.x, pos.y, pos.z);
  alSource3f(source, AL_VELOCITY, vel.x, vel.y, vel.z);
}

void kgmOAL::_Sound::drop()
{
  delete this;
}

kgmOAL::kgmOAL()
{
  m_device = alcOpenDevice(NULL);

  if(m_device)
  {
    m_context = alcCreateContext(m_device, NULL);
    alcMakeContextCurrent(m_context);

    if(!m_context)
    {
#ifdef DEBUG
      kgm_log() << "Can't Create ALC Context\n";
#endif
    }
  }
  else
  {
#ifdef DEBUG
    kgm_log() << "Can't Open ALC Device\n";
#endif
  }
}

kgmOAL::~kgmOAL()
{
  alcMakeContextCurrent(NULL);
  alcDestroyContext(m_context);
  alcCloseDevice(m_device);
}

//kgmSound* kgmOAL::generic(kgmWave* wav)
kgmIAudio::Sound* kgmOAL::create(FMT fmt, u16 freq, u32 size, void* data)
{
  ALenum _fmt;

  switch(fmt)
  {
  case FMT_MONO8:
    _fmt = AL_FORMAT_MONO8;
    break;
  case FMT_MONO16:
    _fmt = AL_FORMAT_MONO16;
    break;
  case FMT_STEREO8:
    _fmt = AL_FORMAT_STEREO8;
    break;
  case FMT_STEREO16:
    _fmt = AL_FORMAT_STEREO16;
    break;
  }

  u32 buf = 0, src = 0;

  alGenBuffers(1, &buf);
  alGenSources(1, &src);
  alBufferData(buf, _fmt, data, size, freq);
  alSourceQueueBuffers(src, 1, &buf);

  if((error = alGetError()) != AL_NO_ERROR)
  {
#ifdef DEBUG
    kgm_log() << "error when create sound buffer\n";
#endif
  }

  if(!buf || !src)
    return 0;

  return new _Sound(buf, src);
}

void kgmOAL::listener(vec3& pos, vec3& vel, vec3& ort)
{
  float l = vel.length();
  float dirort[6] = {vel.x, vel.y, vel.z, ort.x, ort.y, ort.z};

  alListener3f(AL_POSITION, pos.x, pos.y, pos.z);
  alListener3f(AL_VELOCITY, vel.x, vel.y, vel.z);
  alListenerfv(AL_ORIENTATION, dirort);
}

void kgmOAL::clear()
{
}
#endif
