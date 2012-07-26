/*
 * CMovie.h
 *
 *  Created on: Mar 7, 2011
 *      Author: enekochan
 */

#ifndef CMOVIE_H_
#define CMOVIE_H_

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgDB/FileUtils>
#include <osgDB/ReadFile>
#include <osg/ImageStream>
#include <osg/TextureRectangle>

class CMovie : public osg::Group {
    public:

        CMovie(std::string filename, bool loop);
        void play();
        void pause();
        void rewind();
        void setLooping(bool bLoop);
        bool getLooping();
        void accelerate();
        void accelerate(double dRatio);
        void decelerate();
        void decelerate(double dRatio);
        void setSpeed(double dSpeed);
        void seekForward();
        void seekForward(double dSeek);
        void seekBackwards();
        void seekBackwards(double dSeek);
        void seek(double dSeek);

    protected:

        ~CMovie();

    private:

        bool loop;
        double accelereationRatio;
        osg::ref_ptr<osg::ImageStream> mImageStream;
        osg::ref_ptr<osg::Texture> mVideoTexture;
        osg::ref_ptr<osg::Geode> mVideoGeode;

        //osg::ImageStream::AudioStreams& audioStreams;
        osg::AudioStream* audioStream;
};


#endif /* CMOVIE_H_ */
