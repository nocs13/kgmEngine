#ifndef KGMGAMEAUDIO_H
#define KGMGAMEAUDIO_H

#include "../kgmMedia/kgmNullAudio.h"

class kgmGameAudio: public kgmIAudio
{
  KGM_OBJECT(kgmGameAudio);

private:
  kgmIAudio*  m_audio;

public:
  kgmGameAudio();
  virtual ~kgmGameAudio();

  Sound* create(FMT fmt, u16 freq, u32 size, void* data)
  {
    return m_audio->create(fmt, freq, size, data);
  }

  void   listener(vec3& pos, vec3& vel, vec3& ort)
  {
    m_audio->listener(pos, vel, ort);
  }

  void   clear()
  {
    m_audio->clear();
  }
};
#endif // KGMGAMEAUDIO_H
