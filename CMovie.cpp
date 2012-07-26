/*
 * CMovie.cpp
 *
 *  Created on: Mar 7, 2011
 *      Author: enekochan
 */

#include <iostream>
#include <SDL/SDL.h>
#include "SDLAudioSink.h"
#include "CMovie.h"
/*
class SDLAudioSink : public osg::AudioSink
{
    public:

        SDLAudioSink(osg::AudioStream* audioStream):
            _playing(false),
            _audioStream(audioStream) {}

        ~SDLAudioSink();

        virtual void startPlaying();
        virtual bool playing() const { return _playing; }


        bool                                _playing;
        osg::observer_ptr<osg::AudioStream> _audioStream;
};

SDLAudioSink::~SDLAudioSink()
{
    if (_playing)
    {

        SDL_PauseAudio(1);
        SDL_CloseAudio();

        osg::notify(osg::NOTICE)<<"~SDLAudioSink() destructor, but still playing"<<std::endl;
    }
}

void SDLAudioSink::startPlaying()
{
    _playing = true;
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
*/
CMovie::CMovie(std::string filename, bool bLoop) : osg::Group(), loop(bLoop), accelereationRatio(0.1) {

	osg::Image* img = osgDB::readImageFile(filename);
	mImageStream = dynamic_cast<osg::ImageStream*>(img);

	if (mImageStream.valid()) {
		std::cout << "Got movie" << std::endl;

		if (mImageStream) {
			osg::ImageStream::AudioStreams& audioStreams = mImageStream->getAudioStreams();
			audioStream = audioStreams[0].get();
			audioStream->setAudioSink(new SDLAudioSink(audioStream));
		}

		std::cout << "libraryName: " << mImageStream->libraryName() << std::endl;
		mImageStream->play(); // TODO: Fix -> Video does not correctly load if play-pause is not done
		mImageStream->pause();
		/*
		mImageStream->setVolume(0.0);
		mImageStream->play();
		mImageStream->pause();
		mImageStream->rewind();
		mImageStream->setVolume(1.0);
*/
	} else {
		std::cout << "No movie!" << std::endl;
	}

	// Makes video playback infinitely or not
	if (loop)
		mImageStream->setLoopingMode(osg::ImageStream::LOOPING);
	else
		mImageStream->setLoopingMode(osg::ImageStream::NO_LOOPING);

	float aspectRatio = (float)img->t() / (float)img->s();
	float videoWidth = 180;
	float videoHeight = videoWidth * aspectRatio;

	mVideoGeode = new osg::Geode();

	bool texRect = true;

	if (texRect) {
		mVideoTexture = new osg::TextureRectangle(img);
		mVideoGeode->addDrawable(osg::createTexturedQuadGeometry(
			osg::Vec3(-videoWidth / 2, -videoHeight / 2, 0),
			osg::Vec3(videoWidth, 0, 0),
			osg::Vec3(0, videoHeight, 0),
			0,  img->t(), img->s(), 0));

	} else {
		mVideoTexture = new osg::Texture2D(img);
		mVideoGeode->addDrawable(osg::createTexturedQuadGeometry(
			osg::Vec3(-videoWidth / 2, -videoHeight / 2, 0),
			osg::Vec3(videoWidth, 0, 0),
			osg::Vec3(0, videoHeight, 0),
			0, 1, 1, 0));
	}

	mVideoGeode->getOrCreateStateSet()->setTextureAttributeAndModes(0, mVideoTexture.get());
	this->addChild(mVideoGeode.get());

/*
    osg::ref_ptr<osg::Texture> pPictureTexture;
    osg::ref_ptr<osg::Geode> pPictureGeode;
	osg::Image* img2 = osgDB::readImageFile("data/images/tv_frame_1.jpg");
	//img->flipHorizontal();
	img->flipVertical();

	aspectRatio = (float)img2->t() / (float)img2->s();
	float pictureWidth = 230;
	float pictureHeight = pictureWidth * aspectRatio;

	pPictureGeode = new osg::Geode();

	if (texRect) {
		pPictureTexture = new osg::TextureRectangle(img2);
		pPictureGeode->addDrawable(osg::createTexturedQuadGeometry(
			osg::Vec3(-pictureWidth / 2, -pictureHeight / 2, 0),
			osg::Vec3(pictureWidth, 0, 0),
			osg::Vec3(0, pictureHeight, 0),
			0,  img->t(), img->s(), 0));

	} else {
		pPictureTexture = new osg::Texture2D(img2);
		pPictureGeode->addDrawable(osg::createTexturedQuadGeometry(
			osg::Vec3(-pictureWidth / 2, -pictureHeight / 2, 0),
			osg::Vec3(pictureWidth, 0, 0),
			osg::Vec3(0, pictureHeight, 0),
			0,  1,1, 0));
	}

	pPictureGeode->getOrCreateStateSet()->setTextureAttributeAndModes(0, pPictureTexture.get());
	this->addChild(pPictureGeode.get());
*/
}

void CMovie::play() {
	if (mImageStream.valid()) {
		if (mImageStream->getStatus() == osg::ImageStream::PAUSED) {
			mImageStream->play();
		}
	}
}

void CMovie::pause() {
	if (mImageStream.valid()) {
		if (mImageStream->getStatus() == osg::ImageStream::PLAYING) {
			mImageStream->pause();
		} else if (mImageStream->getStatus() == osg::ImageStream::PAUSED) {
			mImageStream->play();
		}
	}
}

void CMovie::rewind() {
	if (mImageStream.valid()) {
		mImageStream->rewind();
	}
}

void CMovie::setLooping(bool bLoop) {
	loop = bLoop;
	mImageStream->setLoopingMode(loop ? osg::ImageStream::LOOPING: osg::ImageStream::NO_LOOPING);
}

bool CMovie::getLooping() {
	return loop;
}

void CMovie::accelerate() {
    this->accelerate(accelereationRatio);
}

void CMovie::accelerate(double dRatio) {
    double tm = mImageStream->getTimeMultiplier();
    tm += dRatio;
    this->setSpeed(tm);
}

void CMovie::decelerate() {
    this->decelerate(accelereationRatio);
}

void CMovie::decelerate(double dRatio) {
    double tm = mImageStream->getTimeMultiplier();
    tm += dRatio;
    this->setSpeed(tm);
}

void CMovie::setSpeed(double dSpeed) {
	mImageStream->setTimeMultiplier(dSpeed);
}

void CMovie::seekForward() {

}

void CMovie::seekForward(double dSeek) {

}

void CMovie::seekBackwards() {

}

void CMovie::seekBackwards(double dSeek) {

}

void CMovie::seek(double dSeek) {
	if(dSeek > 3) dSeek = 0;
	double length = mImageStream->getLength();
	double t_pos = (length/4.0f)*dSeek;
	//mImageStream->rewind();
	mImageStream->seek(t_pos);
	mImageStream->play();
	//_seekIncr++;
}

CMovie::~CMovie() {
	//mImageStream
	//mVideoTexture
	//mVideoGeode->
}

static void soundReadCallback(void * user_data, uint8_t * data, int datalen)
{
    SDLAudioSink * sink = reinterpret_cast<SDLAudioSink*>(user_data);
    osg::ref_ptr<osg::AudioStream> as = sink->_audioStream.get();
    if (as.valid())
    {
        as->consumeAudioBuffer(data, datalen);
    }
}
