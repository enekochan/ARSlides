/*
 * HUD.h
 *
 *  Created on: Jun 1, 2011
 *      Author: enekochan
 */

#ifndef HUD_H_
#define HUD_H_

#include <osg/Projection>
#include <osg/Geode>
#include <osg/MatrixTransform>
#include <osg/Geometry>
#include <osg/Texture2D>

#include <osgDB/ReadFile>

#include <osgText/Text>

class HUD : public osg::Projection {
public:
	HUD(int width, int height, int thickness);
	virtual ~HUD();
	void setText(std::string text);
	void setTextPosition(int x, int y);
	void setTextColor(int r, int g, int b);
	void setTextSize(int s);
	void setHUDColor(float r, float g, float b, float a);
	osg::ref_ptr<osgText::Text> getTextPointer();
private:
	osg::ref_ptr<osgText::Text> textOne;
	osg::ref_ptr<osg::Geometry> HUDBackgroundGeometry;
};

#endif /* HUD_H_ */
