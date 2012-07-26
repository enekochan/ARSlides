#include "SDLAudioSink.h"

static void soundReadCallback(void * user_data, uint8_t * data, int datalen)
{
    SDLAudioSink * sink = reinterpret_cast<SDLAudioSink*>(user_data);
    osg::ref_ptr<osg::AudioStream> as = sink->_audioStream.get();
    if (as.valid())
    {
        as->consumeAudioBuffer(data, datalen);
    }
}

SDLAudioSink::~SDLAudioSink()
{
    stop();
}

void SDLAudioSink::play()
{
    if (_started)
    {
        if (_paused)
        {
            SDL_PauseAudio(0);
            _paused = false;
        }
        return;
    }

    _started = true;
    _paused = false;

    osg::notify(osg::NOTICE)<<"SDLAudioSink()::startPlaying()"<<std::endl;

    osg::notify(osg::NOTICE)<<"  audioFrequency()="<<_audioStream->audioFrequency()<<std::endl;
    osg::notify(osg::NOTICE)<<"  audioNbChannels()="<<_audioStream->audioNbChannels()<<std::endl;
    osg::notify(osg::NOTICE)<<"  audioSampleFormat()="<<_audioStream->audioSampleFormat()<<std::endl;

    SDL_AudioSpec specs = { 0 };
    SDL_AudioSpec wanted_specs = { 0 };

    wanted_specs.freq = _audioStream->audioFrequency();
    wanted_specs.format = AUDIO_S16SYS;
    wanted_specs.channels = _audioStream->audioNbChannels();
    wanted_specs.silence = 0;
    wanted_specs.samples = 1024;
    wanted_specs.callback = soundReadCallback;
    wanted_specs.userdata = this;

    if (SDL_OpenAudio(&wanted_specs, &specs) < 0)
        throw "SDL_OpenAudio() failed (" + std::string(SDL_GetError()) + ")";

    SDL_PauseAudio(0);

}

void SDLAudioSink::pause()
{
    if (_started)
    {
        SDL_PauseAudio(1);
        _paused = true;
    }
}

void SDLAudioSink::stop()
{
    if (_started)
    {
        if (!_paused) SDL_PauseAudio(1);
        SDL_CloseAudio();

        osg::notify(osg::NOTICE)<<"~SDLAudioSink() destructor, but still playing"<<std::endl;
    }
}
