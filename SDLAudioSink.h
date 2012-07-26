#include <osg/observer_ptr>
#include <osg/AudioStream>
#include <SDL/SDL.h> // libsdl must be installed "sudo apt-get install libsdl1.2-dev"

class SDLAudioSink : public osg::AudioSink
{
    public:

        SDLAudioSink(osg::AudioStream* audioStream):
            _started(false),
            _paused(false),
            _audioStream(audioStream) {}

        ~SDLAudioSink();

        virtual void play();
        virtual void pause();
        virtual void stop();

        virtual bool playing() const { return _started && !_paused; }


        bool                                _started;
        bool                                _paused;
        osg::observer_ptr<osg::AudioStream> _audioStream;
};
