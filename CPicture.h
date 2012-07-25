/*
 * CPicture.h
 *
 *  Created on: Mar 7, 2011
 *      Author: enekochan
 */

#ifndef CPICTURE_H_
#define CPICTURE_H_

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgDB/FileUtils>
#include <osgDB/ReadFile>
#include <osg/ImageStream>
#include <osg/TextureRectangle>

class CPicture : public osg::Group {

	private:

		osg::ref_ptr<osg::Texture> pPictureTexture;
		osg::ref_ptr<osg::Geode> pPictureGeode;

		osg::Node* readTransparentImageFile(std::string filename, float width);

	public:

        CPicture(std::string filename, float width);

    protected:

        ~CPicture();

};

#endif /* CPICTURE_H_ */
