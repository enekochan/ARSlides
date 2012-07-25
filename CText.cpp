/*
 * CText.cpp
 *
 *  Created on: Mar 7, 2011
 *      Author: enekochan
 */

#include "CText.h"

osg::Geode* CText::createTextGeode(std::string textString, float r, float g, float b, float a) {
	osg::Geode* textGeode = new osg::Geode();
	text = new osgText::Text();
	text->setFont("/usr/share/fonts/truetype/ttf-japanese-gothic.ttf");
	text->setCharacterSize(16.0f);
	text->setPosition(osg::Vec3(4, 4, 0));
	text->setAlignment(osgText::Text::LEFT_BASE_LINE);
	text->setColor(osg::Vec4(r, g, b, a));
	text->setText(textString);
	textGeode->addDrawable(text);
	return textGeode;
}

CText::CText(std::string textString) : osg::Group() {
	this->addChild(createTextGeode(textString, 1.0f, 1.0f, 1.0f, 1.0f));
}

void CText::setText(std::string textString) {
	/*
	osg::Geode* geode = dynamic_cast<osg::Geode*>(this->getChild(0));
	osgText::Text* text = dynamic_cast<osgText::Text*>(geode->getDrawable(0));
	*/
	text->setText(textString);
	/*
	this->removeChild(0, 1);
	this->addChild(createTextGeode(textString, 1.0f, 0.0f, 0.0f, 1.0f));
	*/
}

void CText::setColor(float r, float g, float b, float a) {
	/*
	osg::Geode* geode = dynamic_cast<osg::Geode*>(this->getChild(0));
	osgText::Text* text = dynamic_cast<osgText::Text*>(geode->getDrawable(0));
	*/
	text->setColor(osg::Vec4(r, g, b, a));
	/*
	this->removeChild(0, 1);
	this->addChild(createTextGeode(this->originalText, r, g, b, a));
	*/
}

void CText::setPosition(osg::Vec3 pos) {
	text->setPosition(pos);
}

CText::~CText()
{
}
