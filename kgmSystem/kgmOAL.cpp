// kgmOALDevice.cpp: implementation of the kgmOALDevice class.
//
//////////////////////////////////////////////////////////////////////

#include "kgmOAL.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

static uint error = 0;

kgmOAL::_Sound::_Sound(uint buf, uint src){
  buffer = buf;
  source = src;
}

kgmOAL::_Sound::~_Sound(){
  alDeleteSources(1, &source);
  alDeleteBuffers(1, &buffer);
}

void kgmOAL::_Sound::stop(){
 alSourceStop(source);
}

void kgmOAL::_Sound::play(bool loop){
  alSourcei(source, AL_LOOPING, (loop)?(AL_TRUE):(AL_FALSE));
  alSourcePlay(source);
  if((error = alGetError()) != AL_NO_ERROR){
  }
}

void kgmOAL::_Sound::volume(float vol){
}

void kgmOAL::_Sound::pause(){
}

void kgmOAL::_Sound::emit(vec3& pos, vec3& vel){
  alSource3f(source, AL_POSITION, pos.x, pos.y, pos.z);
  alSource3f(source, AL_VELOCITY, vel.x, vel.y, vel.z);
}


kgmOAL::kgmOAL()
{
 m_device = alcOpenDevice(NULL);
 if(m_device){
  m_context = alcCreateContext(m_device, NULL);
  alcMakeContextCurrent(m_context);
  if(!m_context){
     printf("Can't Create ALC Context\n");
  }
 }else{
     printf("Can't Open ALC Device\n");
 }
}

kgmOAL::~kgmOAL()
{
 alcMakeContextCurrent(NULL);
 alcDestroyContext(m_context);
 alcCloseDevice(m_device);
}

//kgmSound* kgmOAL::generic(kgmWave* wav)
kgmIAudio::Sound* kgmOAL::create(FMT fmt, ushort freq, uint size, void* data)
{
  ALenum _fmt;

  switch(fmt){
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

  uint buf = 0, src = 0;
  alGenBuffers(1, &buf);
  alGenSources(1, &src);
  alBufferData(buf, _fmt, data, size, freq);
  alSourceQueueBuffers(src, 1, &buf);
  if((error = alGetError()) != AL_NO_ERROR){
    int k = 0;
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
