#include "kgmOSL.h"

#ifdef OSL

kgmOSL::_Sound::_Sound(SLBufferQueueItf q, void* buf, u32 len){
    audioPlayerBufferQueue = q;
    buffer = malloc(len);
    memcpy(buffer, buf, len);
    length = len;
}

kgmOSL::_Sound::~_Sound(){
    if(buffer)
    {
        free(buffer);
    }
    //alDeleteSources(1, &source);
    //alDeleteBuffers(1, &buffer);
}

void kgmOSL::_Sound::stop(){
    //alSourceStop(source);
}

void kgmOSL::_Sound::play(bool loop){
    SLresult result;

    result = (*audioPlayerQueue)->Enqueue(audioPlayerQueue, buffer, length);

    if(result != SL_RESULT_SUCCESS)
        return;
}

void kgmOSL::_Sound::volume(float vol){
}

void kgmOSL::_Sound::pause(){
}

void kgmOSL::_Sound::emit(vec3& pos, vec3& vel){
    //alSource3f(source, AL_POSITION, pos.x, pos.y, pos.z);
    //alSource3f(source, AL_VELOCITY, vel.x, vel.y, vel.z);
}

//

kgmOSL::kgmOSL()
{
    SLresult result;

    engineObject = null;
    engineEngine = null;
    outputMixObject = null;

    result = slCreateEngine(&engineObject, 0, NULL, 0, NULL, NULL);
    result = (*engineObject)->Realize(engineObject, SL_BOOLEAN_FALSE);

    if(result != SL_RESULT_SUCCESS)
        return;

    // get the engine interface, which is needed in order to create other objects
    result = (*engineObject)->GetInterface(engineObject, SL_IID_ENGINE, &engineEngine);

    if(result != SL_RESULT_SUCCESS)
        return;

    const SLInterfaceID ids[1] = {SL_IID_ENVIRONMENTALREVERB};
    const SLboolean req[1] = {SL_BOOLEAN_FALSE};
    result = (*engineEngine)->CreateOutputMix(engineEngine, &outputMixObject, 1, ids, req);

    if(result != SL_RESULT_SUCCESS)
        return;

    // realize the output mix
    result = (*outputMixObject)->Realize(outputMixObject, SL_BOOLEAN_FALSE);

    if(result != SL_RESULT_SUCCESS)
        return;

    // configure audio source
    SLDataLocator_AndroidSimpleBufferQueue loc_bufq = {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, 2};
    SLDataFormat_PCM format_pcm = {SL_DATAFORMAT_PCM, 1, SL_SAMPLINGRATE_8,
                                   SL_PCMSAMPLEFORMAT_FIXED_16, SL_PCMSAMPLEFORMAT_FIXED_16,
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
    result = (*engineEngine)->CreateAudioPlayer(engineEngine, &audioPlayerObject, &audioSrc, &audioSnk,
                                                3, ids_1, req_1);

    if(result != SL_RESULT_SUCCESS)
        return;

    // realize the player
    result = (*audioPlayerObject)->Realize(audioPlayerObject, SL_BOOLEAN_FALSE);

    if(result != SL_RESULT_SUCCESS)
        return;

    result = (*audioPlayerObject)->GetInterface(audioPlayerObject, SL_IID_BUFFERQUEUE,
                                                &audioPlayerBufferQueue);

    if(result != SL_RESULT_SUCCESS)
        return;
    \
    result = (*audioPlayerObject)->GetInterface(audioPlayerObject, SL_IID_PLAY,  &audioPlayer);

    if(result != SL_RESULT_SUCCESS)
        return;

    result = (*audioPlayer)->SetPlayState(audioPlayer, SL_PLAYSTATE_PLAYING);

    if(result != SL_RESULT_SUCCESS)
        return;

}

kgmOSL::~kgmOSL()
{
    if(audioPlayerObject)
    {
        (*audioPlayerObject)->Destroy(audioPlayerObject);
    }

    if(outputMixObject)
    {
        (*outputMixObject)->Destroy(outputMixObject);
    }

    if(engineObject)
    {
        (*engineObject)->Destroy(engineObject);
    }
}

//kgmSound* kgmOAL::generic(kgmWave* wav)
kgmIAudio::Sound* kgmOSL::create(FMT fmt, u16 freq, u32 size, void* data)
{
    //ALenum _fmt;

    if(!engineObject)
        return null;

    switch(fmt){
    case FMT_MONO8:
        //_fmt = AL_FORMAT_MONO8;
        break;
    case FMT_MONO16:
        //_fmt = AL_FORMAT_MONO16;
        break;
    case FMT_STEREO8:
        //_fmt = AL_FORMAT_STEREO8;
        break;
    case FMT_STEREO16:
        //_fmt = AL_FORMAT_STEREO16;
        break;
    }

    u32 buf = 0, src = 0;
    //alGenBuffers(1, &buf);
    //alGenSources(1, &src);
    //alBufferData(buf, _fmt, data, size, freq);
    //alSourceQueueBuffers(src, 1, &buf);

    //if((error = alGetError()) != AL_NO_ERROR)
    //{
    //  int k = 0;
    //}

    if(!buf || !src)
        return 0;

    return new _Sound(buf, src);
}

void kgmOSL::listener(vec3& pos, vec3& vel, vec3& ort)
{
    float l = vel.length();
    float dirort[6] = {vel.x, vel.y, vel.z, ort.x, ort.y, ort.z};
    //alListener3f(AL_POSITION, pos.x, pos.y, pos.z);
    //alListener3f(AL_VELOCITY, vel.x, vel.y, vel.z);
    //alListenerfv(AL_ORIENTATION, dirort);
}

#endif
