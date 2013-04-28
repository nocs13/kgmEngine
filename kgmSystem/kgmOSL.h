#ifndef KGMOSL_H
#define KGMOSL_H

#include "../kgmBase/kgmIAudio.h"

#ifdef OSL
#include <SLES/OpenSLES.h>

#ifdef ANDROID
#include <SLES/OpenSLES_Android.h>
#endif

class kgmOSL: public kgmIAudio
{
  class _Sound: public Sound{
    u32	buffer;
    u32	source;
  public:
    _Sound(u32 buf, u32 src);
    virtual ~_Sound();
    void stop();
    void play(bool loop);
    void pause();
    void volume(float vol);
    void emit(vec3& pos, vec3& vel);
  };

  SLObjectItf engineObject;
  SLEngineItf engineEngine;
public:
  kgmOSL();
  virtual ~kgmOSL();

  Sound* create(FMT fmt, u16 freq, u32 size, void* data);
  void 	 listener(vec3& pos, vec3& vel, vec3& ort);
};
#endif

#endif // KGMOSL_H
