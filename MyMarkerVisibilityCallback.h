/*
 * MyMarkerVisibilityCallback.h
 *
 *  Created on: May 8, 2011
 *      Author: enekochan
 */

#ifndef MYMARKERVISIBILITYCALLBACK_H_
#define MYMARKERVISIBILITYCALLBACK_H_

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

#include "osgText/Text"

class MyMarkerVisibilityCallback : public osgART::MarkerVisibilityCallback
{
private:
	osg::MatrixTransform* mt;
	bool visible;
	int delay;
	int timer;
	float angle;
	osg::ref_ptr<osgText::Text> text;

public:
	MyMarkerVisibilityCallback(osgART::Marker* marker);
	void operator()(osg::Node* node, osg::NodeVisitor* nv);
	void setText(osg::ref_ptr<osgText::Text>);
};

#endif /* MYMARKERVISIBILITYCALLBACK_H_ */
