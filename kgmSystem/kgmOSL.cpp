#include "kgmOSL.h"
#include "../kgmBase/kgmLog.h"
#include "kgmThread.h"
#include "kgmSystem.h"

#ifdef OSL


kgmOSL::_Sound::_Sound(kgmOSL* sl){
  audioPlayerObject = null;
  audioPlayer = null;
  audioVolume = null;
  audioSeek   = null;

  osl = sl;
  vol = .8f;
  loop = false;
  remove = false;
  buffer = null;
}

kgmOSL::_Sound::~_Sound(){

#ifdef DEBUG
  kgm_log() << "OSL delete sound \n";
#endif

  if(audioPlayerObject)
  {
    stop();

    (*audioPlayerObject)->Destroy(audioPlayerObject);
  }

  if(buffer)
  {
#ifdef DEBUG
    kgm_log() << "OSL delete sound buffer\n";
#endif

    free(buffer);
  }

#ifdef DEBUG
  kgm_log() << "OSL sound deleted...\n";
#endif
}

void kgmOSL::_Sound::release()
{
  remove = true;

#ifdef DEBUG
  kgm_log() << "OSL remove flag set \n";
#endif
}

void kgmOSL::_Sound::stop(){
  SLresult result;

  result = (*audioPlayer)->SetPlayState(audioPlayer, SL_PLAYSTATE_STOPPED);

#ifdef DEBUG
  kgm_log() << "OSL sound stopped\n";
#endif
}

void kgmOSL::_Sound::play(bool loop){
  SLresult result;

  result = (*audioPlayerQueue)->Enqueue(audioPlayerQueue, buffer, length);

  if(result != SL_RESULT_SUCCESS)
    return;

  this->loop = loop;

  if(loop && audioSeek)
  {
    SLmillisecond msec;
    result = (*audioPlayer)->GetDuration(audioPlayer, &msec);

    if(SL_RESULT_SUCCESS == result)
    {
      result = (*audioSeek)->SetLoop(audioSeek, SL_BOOLEAN_TRUE, 0, msec);
    }
  }

#ifdef DEBUG
  kgm_log() << "OSL sound played\n";
#endif
}

void kgmOSL::_Sound::volume(float v)
{
  SLresult result;

  if(audioVolume)
  {
    SLmillibel minvol = -900;
    SLmillibel maxvol;
    SLmillibel newvol = minvol + 900 * v; //((minvol - maxvol) * (1.0 - vol)) + maxvol;

    result = (*audioVolume)->GetMaxVolumeLevel(audioVolume, &maxvol);
    result = (*audioVolume)->SetVolumeLevel(audioVolume, newvol);

    if(SL_RESULT_SUCCESS != result)
    {
#ifdef DEBUG
      kgm_log() << "OSL can't set volume: " << newvol << "from max volume: " << maxvol << "\n";
#endif
    }

    vol = v;
  }
}

void kgmOSL::_Sound::pause()
{
  SLresult result;

  result = (*audioPlayer)->SetPlayState(audioPlayer, SL_PLAYSTATE_PAUSED);

#ifdef DEBUG
  if(SL_RESULT_SUCCESS != result)
  {
    kgm_log() << "OSL error while sound pause\n";
  }
#endif
}

void kgmOSL::_Sound::emit(vec3& pos, vec3& vel)
{
  position = pos;
  velocity = vel;
}

//
kgmOSL::kgmOSL()
{
  SLresult result;

  engineObject = null;
  engineEngine = null;
  outputMixObject = null;
  listenerObject = null;

#ifdef DEBUG
  kgm_log() << "OSL init engineObject \n";
#endif

  result = slCreateEngine(&engineObject, 0, NULL, 0, NULL, NULL);
  result = (*engineObject)->Realize(engineObject, SL_BOOLEAN_FALSE);

  if(result != SL_RESULT_SUCCESS)
    return;

  // get the engine interface, which is needed in order to create other objects
#ifdef DEBUG
  kgm_log() << "OSL init engineEngine \n";
#endif

  result = (*engineObject)->GetInterface(engineObject, SL_IID_ENGINE, &engineEngine);

  if(result != SL_RESULT_SUCCESS)
    return;

  const SLInterfaceID ids[1] = {SL_IID_NULL}; //{SL_IID_ENVIRONMENTALREVERB};
  const SLboolean req[1] = {SL_BOOLEAN_FALSE};

#ifdef DEBUG
  kgm_log() << "OSL init outputMixObject \n";
#endif

  result = (*engineEngine)->CreateOutputMix(engineEngine, &outputMixObject, 1, ids, req);

  if(result != SL_RESULT_SUCCESS)
    return;

  // realize the output mix
#ifdef DEBUG
  kgm_log() << "OSL realize outputMixObject \n";
#endif

  result = (*outputMixObject)->Realize(outputMixObject, SL_BOOLEAN_FALSE);

  if(result != SL_RESULT_SUCCESS)
    return;

  const SLInterfaceID listener_ids[] = {SL_IID_3DLOCATION, SL_IID_3DSOURCE};
  const SLboolean     listener_req[] = {SL_BOOLEAN_TRUE,  SL_BOOLEAN_TRUE};
  result = (*engineEngine)->CreateListener(engineEngine, &listenerObject, 1, listener_ids, listener_req);

  if(result != SL_RESULT_SUCCESS)
  {

  }

  mux = mutex();

#ifdef DEBUG
  kgm_log() << "OSL start control thread\n";
#endif

  exec();
  priority(-2);
}

kgmOSL::~kgmOSL()
{
  active = false;

#ifdef DEBUG
  kgm_log() << "OSL stoping thread\n";
#endif

  join();

#ifdef DEBUG
  kgm_log() << "OSL thread stoped\n";
#endif

  if(outputMixObject)
  {
#ifdef DEBUG
    kgm_log() << "OSL delete outputMixObject \n";
#endif
    (*outputMixObject)->Destroy(outputMixObject);
  }

  if(engineObject)
  {
#ifdef DEBUG
    kgm_log() << "OSL delete engineObject \n";
#endif
    (*engineObject)->Destroy(engineObject);
  }

  mxfree(mux);

#ifdef DEBUG
  kgm_log() << "OSL finish\n";
#endif
}

kgmIAudio::Sound* kgmOSL::create(FMT fmt, u16 freq, u32 size, void* data)
{
  if(!engineObject)
    return null;

  SLuint32 bitsPerSample = 0xffffffff;

#ifdef DEBUG
  kgm_log() << "OSL create buffer: " << (s32)fmt << " " << (s32)freq << "\n";
#endif

  switch(fmt)
  {
  case FMT_MONO8:
    bitsPerSample = SL_PCMSAMPLEFORMAT_FIXED_8;
    break;
  case FMT_MONO16:
    bitsPerSample = SL_PCMSAMPLEFORMAT_FIXED_16;
    break;
  case FMT_STEREO8:
    bitsPerSample = SL_PCMSAMPLEFORMAT_FIXED_8;
    break;
  case FMT_STEREO16:
    bitsPerSample = SL_PCMSAMPLEFORMAT_FIXED_16;
    break;
  }

  SLuint32 samplingRate = freq * 1000;

  if(!data || !size ||
     (samplingRate == 0xffffffff) ||
     (bitsPerSample == 0xffffffff))
    return null;

  _Sound*  sound = new _Sound(this);
  SLresult result;

  sound->buffer = malloc(size);
  memcpy(sound->buffer, data, size);
  sound->length = size;

  /*const SLuint32 lBGMPlayerIIDCount = 1;
  const SLInterfaceID lBGMPlayerIIDs[] =
  { SL_IID_PLAY, SL_IID_SEEK };
  const SLboolean lBGMPlayerReqs[] =
  { SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE };

  SLDataLocator_Address loc_bufq = {SL_DATALOCATOR_ADDRESS, sound->buffer, size};
  SLDataFormat_PCM format_pcm = {SL_DATAFORMAT_PCM, 1, samplingRate,
  bitsPerSample, bitsPerSample,
  SL_SPEAKER_FRONT_CENTER, SL_BYTEORDER_LITTLEENDIAN};
  SLDataSource audioSrc = {&loc_bufq, &format_pcm};
  SLDataLocator_OutputMix loc_outmix = {SL_DATALOCATOR_OUTPUTMIX, outputMixObject};
  SLDataSink audioSnk = {&loc_outmix, NULL};

  result = (*engineEngine)->CreateAudioPlayer(engineEngine,
  &sound->audioPlayerObject, &audioSrc, &audioSnk,
  lBGMPlayerIIDCount, lBGMPlayerIIDs, lBGMPlayerReqs);
  if (result != SL_RESULT_SUCCESS)
  {

  }

  result = (*sound->audioPlayerObject)->Realize(sound->audioPlayerObject, SL_BOOLEAN_FALSE);
  if (result != SL_RESULT_SUCCESS)
  {

  }

  result = (*sound->audioPlayerObject)->GetInterface(sound->audioPlayerObject,
  SL_IID_PLAY, &sound->audioPlayer);
  if (result != SL_RESULT_SUCCESS)
  {

  }

  result = (*sound->audioPlayerObject)->GetInterface(sound->audioPlayerObject,
  SL_IID_SEEK, &sound->audioSeek);
  if (result != SL_RESULT_SUCCESS)
  {

  }*/

  // configure audio source
  SLDataLocator_AndroidSimpleBufferQueue loc_bufq = {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, 2};
  SLDataFormat_PCM format_pcm = {SL_DATAFORMAT_PCM, 1, samplingRate,
                                 bitsPerSample, bitsPerSample,
                                 SL_SPEAKER_FRONT_CENTER, SL_BYTEORDER_LITTLEENDIAN};
  SLDataSource audioSrc = {&loc_bufq, &format_pcm};

  // configure audio sink
  SLDataLocator_OutputMix loc_outmix = {SL_DATALOCATOR_OUTPUTMIX, outputMixObject};
  SLDataSink audioSnk = {&loc_outmix, NULL};

  // create audio player
  const SLInterfaceID ids_1[3] = {SL_IID_BUFFERQUEUE, SL_IID_EFFECTSEND, SL_IID_VOLUME};
  const SLboolean req_1[3] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE,  SL_BOOLEAN_TRUE};

#ifdef DEBUG
  kgm_log() << "OSL init audioPlayerObject \n";
#endif

  result = (*engineEngine)->CreateAudioPlayer(engineEngine, &sound->audioPlayerObject, &audioSrc, &audioSnk,
                                              3, ids_1, req_1);

  if(result != SL_RESULT_SUCCESS)
  {
    delete sound;

    return  null;
  }

  // realize the player
#ifdef DEBUG
  kgm_log() << "OSL Realize audioPlayerObject \n";
#endif

  result = (*sound->audioPlayerObject)->Realize(sound->audioPlayerObject, SL_BOOLEAN_FALSE);

  if(result != SL_RESULT_SUCCESS)
  {
    delete sound;

    return  null;
  }

#ifdef DEBUG
  kgm_log() << "OSL init audioPlayerQueue \n";
#endif

  result = (*sound->audioPlayerObject)->GetInterface(sound->audioPlayerObject, SL_IID_BUFFERQUEUE, &sound->audioPlayerQueue);

  if(result != SL_RESULT_SUCCESS)
  {
    delete sound;

    return  null;
  }

  //OSL_sound_bufferQueue_callback
  (*sound->audioPlayerQueue)->RegisterCallback(sound->audioPlayerQueue, OSL_sound_bufferQueue_callback, sound);

#ifdef DEBUG
  kgm_log() << "OSL init audioPlayer \n";
#endif

  result = (*sound->audioPlayerObject)->GetInterface(sound->audioPlayerObject, SL_IID_PLAY,  &sound->audioPlayer);

  if(result != SL_RESULT_SUCCESS)
  {
    delete sound;

    return  null;
  }

#ifdef DEBUG
  kgm_log() << "OSL init audioVolume \n";
#endif

  result = (*sound->audioPlayerObject)->GetInterface(sound->audioPlayerObject, SL_IID_VOLUME,  &sound->audioVolume);

  if(result != SL_RESULT_SUCCESS)
  {
    delete sound;

    return  null;
  }

#ifdef DEBUG
  kgm_log() << "OSL init audioSeek \n";
#endif

  result = (*sound->audioPlayerObject)->GetInterface(sound->audioPlayerObject, SL_IID_SEEK,  &sound->audioSeek);

  if(result != SL_RESULT_SUCCESS)
  {
#ifdef DEBUG
    kgm_log() << "OSL failed audioSeek\n";
#endif

    //delete sound;

    //return  null;
  }

#ifdef DEBUG
  kgm_log() << "OSL SetPlayState audioPlayer \n";
#endif

  result = (*sound->audioPlayer)->SetPlayState(sound->audioPlayer, SL_PLAYSTATE_PLAYING);

  if(result != SL_RESULT_SUCCESS)
  {
    delete sound;

    return  null;
  }

  if(listenerObject)
  {

  }

  sounds.add(sound);

  return sound;
}

void kgmOSL::listener(vec3& pos, vec3& vel, vec3& ort)
{
  float l = vel.length();
  float dirort[6] = {vel.x, vel.y, vel.z, ort.x, ort.y, ort.z};

  position = pos;
  velocity = vel;
  orient   = ort;
}

void kgmOSL::clear()
{
}

void kgmOSL::OSL_sound_bufferQueue_callback(SLBufferQueueItf caller,void *pContext)
{
  _Sound* s = (_Sound*)pContext;

#ifdef DEBUG
  kgm_log() << "OSL queue callback\n";
#endif

  if(!s)
    return;

#ifdef DEBUG
  kgm_log() << "OSL context is valid\n";
#endif

  if(s->loop && s->audioPlayerQueue)
  {
    (*s->audioPlayerQueue)->Clear(s->audioPlayerQueue);
    s->play(s->loop);
    s->volume(s->vol);
  }
}

void kgmOSL::run()
{
  active = true;

#ifdef DEBUG
  kgm_log() << "OSL Gues whooo.....\n";
  kgm_log() << "osl thread id " << (s32)gettid();
#endif

  while(active)
  {
    lock(mux);

    for(int i = sounds.length(); i > 0; --i)
    {
      _Sound* s = sounds[i - 1];
      SLresult result;

#ifdef DEBUG
      kgm_log() << "OSL sound " << i << "\n";
#endif

      if(s->remove)
      {
        delete s;

        sounds.erase(i - 1);

        continue;
      }

      float dst = s->position.distance(position);
      float vol = (700 - dst) / 750;

      if(vol < 0)
        vol = 0;

      s->volume(vol);
    }

    unlock(mux);

    kgmSystem::sleep(500);
  }
}
#endif
