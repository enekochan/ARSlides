/*
 * MediaMarkerVisibilityCallback.h
 *
 *  Created on: Apr 7, 2011
 *      Author: enekochan
 */

#ifndef MEDIAMARKERVISIBILITYCALLBACK_H_
#define MEDIAMARKERVISIBILITYCALLBACK_H_

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

#include "Media.h"

class MediaMarkerVisibilityCallback : public osgART::MarkerVisibilityCallback
{
private:
	osg::MatrixTransform* mt;
	osg::ref_ptr<Media> mv;

public:
	MediaMarkerVisibilityCallback(osgART::Marker* marker);
	void operator()(osg::Node* node, osg::NodeVisitor* nv);
};

#endif /* MEDIAMARKERVISIBILITYCALLBACK_H_ */
