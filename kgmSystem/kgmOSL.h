#ifndef KGMOSL_H
#define KGMOSL_H

#include "../kgmBase/kgmIAudio.h"
#include "../kgmBase/kgmList.h"
#include "kgmThread.h"

#ifdef OSL
#include <SLES/OpenSLES.h>

#ifdef ANDROID
#include <SLES/OpenSLES_Android.h>
#endif

class kgmOSL: public kgmIAudio, public kgmThread
{
    class _Sound: public Sound{
      kgmOSL*     osl;
    public:
      SLObjectItf audioPlayerObject;
      SLPlayItf   audioPlayer;
      SLVolumeItf audioVolume;
#ifdef ANDROID
      SLAndroidSimpleBufferQueueItf audioPlayerQueue;
#else
      SLBufferQueueItf audioPlayerQueue;
#endif
        void* buffer;
        uint  length;

        vec3  position;
        vec3  velosity;
    public:
        _Sound(kgmOSL*);
        virtual ~_Sound();
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
    vec3  velosity;
    vec3  orientation;
public:
    kgmOSL();
    virtual ~kgmOSL();

    Sound*   create(FMT fmt, u16 freq, u32 size, void* data);
    void     listener(vec3& pos, vec3& vel, vec3& ort);
    void     release();
    void     remove(_Sound*);

    void     run();
};
#endif

#endif // KGMOSL_H
