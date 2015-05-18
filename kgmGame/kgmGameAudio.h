#ifndef KGMGAMEAUDIO_H
#define KGMGAMEAUDIO_H

#include "../kgmBase/kgmObject.h"
#include "../kgmBase/kgmPointer.h"
#include "../kgmMedia/kgmNullAudio.h"

class kgmGameAudio: public kgmIAudio
{
  KGM_OBJECT(kgmGameAudio);

  struct Listener
  {
    vec3 pos;
    vec3 vel;
    vec3 ort;
  };

private:
  kgm_ptr<kgmIAudio> m_audio;
  //kgmIAudio* m_audio;

  Listener m_listener;

public:
  kgmGameAudio();
  virtual ~kgmGameAudio();

  Sound create(FMT fmt, u16 freq, u32 size, void* data)
  {
    return ((kgmIAudio*)m_audio)->create(fmt, freq, size, data);
  }

  void remove(Sound snd)
  {
    ((kgmIAudio*)m_audio)->remove(snd);
  }

  void volume(Sound snd, u16 vol)
  {
    ((kgmIAudio*)m_audio)->volume(snd, vol);
  }

  void pause(Sound snd, bool stat)
  {
    ((kgmIAudio*)m_audio)->pause(snd, stat);
  }

  void play(Sound snd, bool loop)
  {
    ((kgmIAudio*)m_audio)->play(snd, loop);
  }

  void stop(Sound snd)
  {
    ((kgmIAudio*)m_audio)->stop(snd);
  }

  void channel(Sound snd, s16 pan)
  {
    ((kgmIAudio*)m_audio)->channel(snd, pan);
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
