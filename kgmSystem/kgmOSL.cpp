#include "kgmOSL.h"
#include "../kgmBase/kgmLog.h"

#ifdef OSL

kgmOSL::_Sound::_Sound(kgmOSL* sl){
  audioPlayerObject = null;
  audioPlayer = null;
  audioVolume = null;

  osl = sl;
}

kgmOSL::_Sound::~_Sound(){
  if(audioPlayerObject)
  {
    stop();
    (*audioPlayerObject)->Destroy(audioPlayerObject);
  }

  if(buffer)
  {
    free(buffer);
  }

  if(osl)
  {
      osl->remove(this);
  }
}

void kgmOSL::_Sound::stop(){
  SLresult result;

  result = (*audioPlayer)->SetPlayState(audioPlayer, SL_PLAYSTATE_STOPPED);
}

void kgmOSL::_Sound::play(bool loop){
  SLresult result;

  result = (*audioPlayerQueue)->Enqueue(audioPlayerQueue, buffer, length);

  if(result != SL_RESULT_SUCCESS)
    return;
}

void kgmOSL::_Sound::volume(float vol)
{
  SLresult result;

  if(audioVolume)
  {
    result = (*audioVolume)->SetVolumeLevel(audioVolume, vol);
  }
}

void kgmOSL::_Sound::pause(){
  SLresult result;

  result = (*audioPlayer)->SetPlayState(audioPlayer, SL_PLAYSTATE_PAUSED);
}

void kgmOSL::_Sound::emit(vec3& pos, vec3& vel){
  position = pos;
  velosity = vel;
}

//
kgmOSL::kgmOSL()
{
  SLresult result;

  engineObject = null;
  engineEngine = null;
  outputMixObject = null;
  listenerObject = null;

  kgm_log() << "OSL init engineObject \n";
  result = slCreateEngine(&engineObject, 0, NULL, 0, NULL, NULL);
  result = (*engineObject)->Realize(engineObject, SL_BOOLEAN_FALSE);

  if(result != SL_RESULT_SUCCESS)
    return;

  // get the engine interface, which is needed in order to create other objects
  kgm_log() << "OSL init engineEngine \n";
  result = (*engineObject)->GetInterface(engineObject, SL_IID_ENGINE, &engineEngine);

  if(result != SL_RESULT_SUCCESS)
    return;

  const SLInterfaceID ids[1] = {SL_IID_NULL}; //{SL_IID_ENVIRONMENTALREVERB};
  const SLboolean req[1] = {SL_BOOLEAN_FALSE};
  kgm_log() << "OSL init outputMixObject \n";
  result = (*engineEngine)->CreateOutputMix(engineEngine, &outputMixObject, 1, ids, req);

  if(result != SL_RESULT_SUCCESS)
    return;

  // realize the output mix
  kgm_log() << "OSL realize outputMixObject \n";
  result = (*outputMixObject)->Realize(outputMixObject, SL_BOOLEAN_FALSE);

  if(result != SL_RESULT_SUCCESS)
    return;

  const SLInterfaceID listener_ids[] = {SL_IID_3DLOCATION, SL_IID_3DSOURCE};
  const SLboolean     listener_req[] = {SL_BOOLEAN_TRUE,  SL_BOOLEAN_TRUE};
  result = (*mEngine)->CreateListener(engineEngine, &listenerObject, 1, listener_ids, listener_req);

  if(result != SL_RESULT_SUCCESS)
    return;
}

kgmOSL::~kgmOSL()
{
}

//kgmSound* kgmOAL::generic(kgmWave* wav)
kgmIAudio::Sound* kgmOSL::create(FMT fmt, u16 freq, u32 size, void* data)
{
  //ALenum _fmt;

  if(!engineObject)
    return null;

  SLuint32 bitsPerSample = 0xffffffff;
  kgm_log() << "OSL create buffer: " << (s32)fmt << " " << (s32)freq << "\n";
  switch(fmt){
  case FMT_MONO8:
    //_fmt = AL_FORMAT_MONO8;
    bitsPerSample = SL_PCMSAMPLEFORMAT_FIXED_8;
    break;
  case FMT_MONO16:
    //_fmt = AL_FORMAT_MONO16;
    bitsPerSample = SL_PCMSAMPLEFORMAT_FIXED_16;
    break;
  case FMT_STEREO8:
    //_fmt = AL_FORMAT_STEREO8;
    bitsPerSample = SL_PCMSAMPLEFORMAT_FIXED_8;
    break;
  case FMT_STEREO16:
    //_fmt = AL_FORMAT_STEREO16;
    bitsPerSample = SL_PCMSAMPLEFORMAT_FIXED_16;
    break;
  }

  SLuint32 samplingRate = freq * 1000;
  /*switch(freq)
  {
  case 8000:
    samplingRate = SL_SAMPLINGRATE_8;
    break;
  case 11025:
    samplingRate = SL_SAMPLINGRATE_11_025;
    break;
  case 12000:
    samplingRate = SL_SAMPLINGRATE_12;
    break;
  case 16000:
    samplingRate = SL_SAMPLINGRATE_16;
    break;
  case 22050:
    samplingRate = SL_SAMPLINGRATE_22_05;
    break;
  case 24000:
    samplingRate = SL_SAMPLINGRATE_24;
    break;
  case 32000:
    samplingRate = SL_SAMPLINGRATE_32;
    break;
  case 44100:
    samplingRate = SL_SAMPLINGRATE_44_1;
    break;
  case 48000:
    samplingRate = SL_SAMPLINGRATE_48;
    break;
  case 64000:
    samplingRate = SL_SAMPLINGRATE_64;
    break;
  case 88200:
    samplingRate = SL_SAMPLINGRATE_88_2;
    break;
  case 96000:
    samplingRate = SL_SAMPLINGRATE_96;
    break;
  case 192000:
    samplingRate = SL_SAMPLINGRATE_192;
    break;
  }*/

  if(!data || !size ||
      (samplingRate == 0xffffffff) ||
        (bitsPerSample == 0xffffffff))
    return null;

  _Sound*  sound = new _Sound();
  SLresult result;

  sound->buffer = malloc(size);
  memcpy(sound->buffer, data, size);
  sound->length = size;

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
  const SLInterfaceID ids_1[3] = {SL_IID_BUFFERQUEUE, SL_IID_EFFECTSEND,
  /*SL_IID_MUTESOLO,*/ SL_IID_VOLUME};
  const SLboolean req_1[3] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE,
  /*SL_BOOLEAN_TRUE,*/ SL_BOOLEAN_TRUE};
  kgm_log() << "OSL init audioPlayerObject \n";
  result = (*engineEngine)->CreateAudioPlayer(engineEngine, &sound->audioPlayerObject, &audioSrc, &audioSnk,
  3, ids_1, req_1);

  if(result != SL_RESULT_SUCCESS)
  {
    delete sound;
    return  null;
  }

  // realize the player
  kgm_log() << "OSL Realize audioPlayerObject \n";
  result = (*sound->audioPlayerObject)->Realize(sound->audioPlayerObject, SL_BOOLEAN_FALSE);

  if(result != SL_RESULT_SUCCESS)
  {
    delete sound;
    return  null;
  }

  kgm_log() << "OSL init audioPlayerQueue \n";
  result = (*sound->audioPlayerObject)->GetInterface(sound->audioPlayerObject, SL_IID_BUFFERQUEUE, &sound->audioPlayerQueue);

  if(result != SL_RESULT_SUCCESS)
  {
    delete sound;
    return  null;
  }
  \
  kgm_log() << "OSL init audioPlayer \n";
  result = (*sound->audioPlayerObject)->GetInterface(sound->audioPlayerObject, SL_IID_PLAY,  &sound->audioPlayer);

  if(result != SL_RESULT_SUCCESS)
  {
    delete sound;

    return  null;
  }

  kgm_log() << "OSL init audioVolume \n";
  result = (*sound->audioPlayerObject)->GetInterface(sound->audioPlayerObject, SL_IID_VOLUME,  &sound->audioVolume);

  if(result != SL_RESULT_SUCCESS)
  {
    delete sound;

    return  null;
  }

  kgm_log() << "OSL SetPlayState audioPlayer \n";
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
}

void kgmOSL::release()
{
    if(outputMixObject)
    {
      kgm_log() << "OSL delete outputMixObject \n";
      (*outputMixObject)->Destroy(outputMixObject);
    }

    if(engineObject)
    {
      kgm_log() << "OSL delete engineObject \n";
      (*engineObject)->Destroy(engineObject);
    }
}

void kgmOSL::remove(_Sound* s)
{
    for(int i = 0; i < sounds.length(); i++)
    {
        if(sounds[i] == s)
        {
            sounds.erase(i);

            return;
        }
    }
}

#endif
