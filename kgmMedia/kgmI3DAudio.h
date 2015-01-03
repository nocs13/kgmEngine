#ifndef KGMI3DAUDIO_H
#define KGMI3DAUDIO_H

#include "kgmIAudio.h"

class kgmI3DAudio: public kgmIAudio
{
public:
  virtual void transform(Sound snd, vec3& pos, vec3& vel) = 0;
  virtual void listener(vec3& pos, vec3& vel, vec3& ort) = 0;
};

#endif // KGMI3DAUDIO_H
