#ifndef KGMNULLAUDIO_H
#define KGMNULLAUDIO_H

#include "kgmIAudio.h"

class kgmNullAudio : public kgmIAudio
{
public:
    kgmNullAudio()
    {

    }

    Sound* create(FMT fmt, u16 freq, u32 size, void* data)
    {
        return null;
    }

    void   listener(vec3& pos, vec3& vel, vec3& ort)
    {
    }

    void   clear()
    {
    }
};

#endif // KGMNULLAUDIO_H
