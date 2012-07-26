/*
 * Media.h
 *
 *  Created on: Apr 7, 2011
 *      Author: enekochan
 */

#ifndef MEDIA_H_
#define MEDIA_H_

#include <osgART/Foundation>
#include <osgART/VideoLayer>
#include <osgART/PluginManager>
#include <osgART/VideoGeode>
#include <osgART/Utils>
#include <osgART/MarkerCallback>
#include <osgART/TransformFilterCallback>

#include <osg/Geometry>
#include <osg/ShapeDrawable>
#include <osg/AutoTransform>

class Media : public osg::Group
{
private:
    osg::ref_ptr<osg::ImageStream> m_MediaStream;
    osg::ref_ptr<osg::Texture> m_MediaTexture;
    osg::ref_ptr<osg::Geode> m_MediaGeode;

public:
	Media(std::string file, int size, bool isVideo, osg::Image* md);

	void stop();

	void start();
};

#endif /* MEDIA_H_ */
