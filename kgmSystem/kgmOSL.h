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
    public:
        _Sound();
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
public:
    kgmOSL();
    virtual ~kgmOSL();

    Sound*   create(FMT fmt, u16 freq, u32 size, void* data);
    void 	   listener(vec3& pos, vec3& vel, vec3& ort);
    void     release();
};
#endif

#endif // KGMOSL_H
