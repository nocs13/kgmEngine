// kgmOALDevice.h: interface for the kgmOALDevice class.
//
//////////////////////////////////////////////////////////////////////

#pragma once
#include "../kgmMain/kgmIAudio.h"
#include "../kgmMain/kgmMath.h"
#include "../kgmMain/kgmSound.h"
#include "../kgmMain/kgmWave.h"

#ifdef WIN32
#include "inc/al.h"
#include "inc/alc.h"
#endif

#ifdef LINUX
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>
#endif

class kgmOAL: public kgmIAudio  
{
  class _Sound: public Sound{
    uint	buffer;
    uint	source;
  public:
    _Sound(uint buf, uint src);
    virtual ~_Sound();
    void stop();
    void play(bool loop);
    void pause();
    void volume(float vol);
    void emit(vec3& pos, vec3& vel);
  };

  ALCdevice*		m_device;
  ALCcontext*		m_context;
public:
  kgmOAL();
  virtual ~kgmOAL();

  Sound* create(FMT fmt, ushort freq, uint size, void* data);
  void 	 listener(vec3& pos, vec3& vel, vec3& ort);
};

