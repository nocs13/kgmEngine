#ifndef KGMOSL_H
#define KGMOSL_H

#include "../kgmBase/kgmList.h"
#include "../kgmMedia/kgmIAudio.h"
#include "kgmThread.h"

#ifdef OSL
#include <SLES/OpenSLES.h>

#ifdef ANDROID
#include <SLES/OpenSLES_Android.h>
#endif

class kgmOSL: public kgmIAudio
{
  class _Sound
  {
    kgmOSL*     osl;
  public:
    SLObjectItf audioPlayerObject;
    SLPlayItf   audioPlayer;
    SLVolumeItf audioVolume;
    SLSeekItf   audioSeek;
#ifdef ANDROID
    SLAndroidSimpleBufferQueueItf audioPlayerQueue;
#else
    SLBufferQueueItf audioPlayerQueue;
#endif
    void* buffer;
    uint  length;

    vec3  position;
    vec3  velocity;

    bool  remove;
    bool  loop;
    float vol;

  public:
    _Sound(kgmOSL*);
    virtual ~_Sound();
    void release();
    void stop();
    void play(bool loop);
    void pause();
    void volume(float vol);
    void emit(vec3& pos, vec3& vel);
  };

  SLObjectItf engineObject;
  SLEngineItf engineEngine;
  SLObjectItf outputMixObject;
  SLObjectItf listenerObject;

  kgmList<_Sound*> sounds;
  Mutex            mux;
  bool             active;

  vec3  position;
  vec3  velocity;
  vec3  orient;

public:
  kgmOSL();
  virtual ~kgmOSL();

  Sound    create(FMT fmt, u16 freq, u32 size, void* data);
  void     listener(vec3& pos, vec3& vel, vec3& ort);
  void     clear();

  void     run();

  static void OSL_sound_bufferQueue_callback(SLBufferQueueItf,void*);
};
#endif

#endif // KGMOSL_H
