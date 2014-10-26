#ifndef KGMGAMEAUDIO_H
#define KGMGAMEAUDIO_H

#include "../kgmBase/kgmObject.h"
#include "../kgmMedia/kgmNullAudio.h"

class kgmGameAudio: public kgmObject, public kgmIAudio
{
  KGM_OBJECT(kgmGameAudio);

  struct Listener
  {
    vec3 pos;
    vec3 vel;
    vec3 ort;
  };

private:
  kgmIAudio*  m_audio;

  Listener m_listener;

public:
  kgmGameAudio();
  virtual ~kgmGameAudio();

  Sound create(FMT fmt, u16 freq, u32 size, void* data)
  {
    return m_audio->create(fmt, freq, size, data);
  }

  void remove(Sound snd)
  {
    m_audio->remove(snd);
  }

  void volume(Sound snd, u16 vol)
  {
    m_audio->volume(snd, vol);
  }

  void pause(Sound snd, bool stat)
  {
    m_audio->pause(snd, stat);
  }

  void play(Sound snd, bool loop)
  {
    m_audio->play(snd, loop);
  }

  void stop(Sound snd)
  {
    m_audio->stop(snd);
  }

  void channel(Sound snd, s16 pan)
  {
    m_audio->channel(snd, pan);
  }

  void transform(Sound snd, vec3& pos, vec3& vel)
  {

  }

  void listener(vec3& pos, vec3& vel, vec3& ort)
  {
    m_listener.pos = pos;
    m_listener.vel = vel;
    m_listener.ort = ort;
  }

  void background(Sound snd, bool stat)
  {

  }
};
#endif // KGMGAMEAUDIO_H
