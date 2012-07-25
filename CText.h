/*
 * CText.h
 *
 *  Created on: Mar 7, 2011
 *      Author: enekochan
 */

#ifndef CTEXT_H_
#define CTEXT_H_

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgDB/FileUtils>
#include <osgDB/ReadFile>
#include <osg/ImageStream>
#include <osg/TextureRectangle>

class CText : public osg::Group {
    public:

		CText(std::string textString);
		void setText(std::string textString);
		void setColor(float r, float g, float b, float a);
		void setPosition(osg::Vec3 pos);

    protected:

        ~CText();

    private:

        osgText::Text* text;
        osg::Geode* createTextGeode(std::string textString, float r, float g, float b, float a);
};

#endif /* CTEXT_H_ */
