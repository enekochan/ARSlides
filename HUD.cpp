/*
 * HUD.cpp
 *
 *  Created on: Jun 1, 2011
 *      Author: enekochan
 */

#include "HUD.h"

HUD::HUD(int width, int height, int thickness) {

	this->setMatrix(osg::Matrix::ortho2D(0, width, 0, height));

	// A geometry node for our HUD:
	osg::ref_ptr<osg::Geode> HUDGeode = new osg::Geode();
	// Text instance that will be shown up in the HUD:
	textOne = new osgText::Text();

	osg::ref_ptr<osg::MatrixTransform> HUDModelViewMatrix = new osg::MatrixTransform;
	HUDModelViewMatrix->setMatrix(osg::Matrix::identity());
	HUDModelViewMatrix->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
	HUDModelViewMatrix->addChild(HUDGeode);
	this->addChild(HUDModelViewMatrix);

	// Set up geometry for the HUD and add it to the HUD
	HUDBackgroundGeometry = new osg::Geometry();

	osg::ref_ptr<osg::Vec3Array> HUDBackgroundVertices = new osg::Vec3Array;
	HUDBackgroundVertices->push_back( osg::Vec3( 0,           0,-1) );
	HUDBackgroundVertices->push_back( osg::Vec3(width,        0,-1) );
	HUDBackgroundVertices->push_back( osg::Vec3(width,thickness,-1) );
	HUDBackgroundVertices->push_back( osg::Vec3(    0,thickness,-1) );

	osg::ref_ptr<osg::DrawElementsUInt> HUDBackgroundIndices = new osg::DrawElementsUInt(osg::PrimitiveSet::POLYGON, 0);
	HUDBackgroundIndices->push_back(0);
	HUDBackgroundIndices->push_back(1);
	HUDBackgroundIndices->push_back(2);
	HUDBackgroundIndices->push_back(3);

	osg::ref_ptr<osg::Vec4Array> HUDcolors = new osg::Vec4Array;
	HUDcolors->push_back(osg::Vec4(0.8f, 0.8f, 0.8f, 0.8f));

	osg::ref_ptr<osg::Vec2Array> texcoords = new osg::Vec2Array(4);
	(*texcoords)[0].set(0.0f, 0.0f);
	(*texcoords)[1].set(1.0f, 0.0f);
	(*texcoords)[2].set(1.0f, 1.0f);
	(*texcoords)[3].set(0.0f, 1.0f);

	HUDBackgroundGeometry->setTexCoordArray(0,texcoords);
	osg::ref_ptr<osg::Texture2D> HUDTexture = new osg::Texture2D;
	HUDTexture->setDataVariance(osg::Object::DYNAMIC);
	osg::ref_ptr<osg::Image> hudImage;
	hudImage = osgDB::readImageFile("HUDBack2.tga");
	HUDTexture->setImage(hudImage);
	osg::ref_ptr<osg::Vec3Array> HUDnormals = new osg::Vec3Array;
	HUDnormals->push_back(osg::Vec3(0.0f, 0.0f, 1.0f));
	HUDBackgroundGeometry->setNormalArray(HUDnormals);
	HUDBackgroundGeometry->setNormalBinding(osg::Geometry::BIND_OVERALL);
	HUDBackgroundGeometry->addPrimitiveSet(HUDBackgroundIndices);
	HUDBackgroundGeometry->setVertexArray(HUDBackgroundVertices);
	HUDBackgroundGeometry->setColorArray(HUDcolors);
	HUDBackgroundGeometry->setColorBinding(osg::Geometry::BIND_OVERALL);

	HUDGeode->addDrawable(HUDBackgroundGeometry);

	// Create and set up a state set using the texture from above:
	osg::ref_ptr<osg::StateSet> HUDStateSet = new osg::StateSet();
	HUDGeode->setStateSet(HUDStateSet);
	HUDStateSet->setTextureAttributeAndModes(0, HUDTexture, osg::StateAttribute::ON);

	// For this state set, turn blending on (so alpha texture looks right)
	HUDStateSet->setMode(GL_BLEND, osg::StateAttribute::ON);

	// Disable depth testing so geometry is draw regardless of depth values
	// of geometry already draw.
	HUDStateSet->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);
	HUDStateSet->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);

	// Need to make sure this geometry is draw last. RenderBins are handled
	// in numerical order so set bin number to 11
	HUDStateSet->setRenderBinDetails(11, "RenderBin");

	// Add the text (Text class is derived from drawable) to the geode:
	HUDGeode->addDrawable(textOne);

	// Set up the parameters for the text we'll add to the HUD:
	textOne->setCharacterSize(25);
	//textOne->setFont("data/fonts/arial.ttf");
	textOne->setFont("/usr/share/fonts/truetype/freefont/FreeMono.ttf");
	textOne->setText("");
	textOne->setAxisAlignment(osgText::Text::SCREEN);
	textOne->setPosition(osg::Vec3(5, 75, -1.5));
	textOne->setColor(osg::Vec4(199, 77, 15, 1));
}

HUD::~HUD() {
	// TODO Auto-generated destructor stub
}

void HUD::setText(std::string text) {
	textOne->setText(text);
}

void HUD::setTextPosition(int x, int y) {
	textOne->setPosition(osg::Vec3(x, y, -1.5));
}

void HUD::setTextColor(int r, int g, int b) {
	textOne->setColor(osg::Vec4(r, g, b, 1));
}

void HUD::setTextSize(int s) {
	textOne->setCharacterSize(s);
}

void HUD::setHUDColor(float r, float g, float b, float a) {
	osg::ref_ptr<osg::Vec4Array> HUDcolors = new osg::Vec4Array;
	HUDcolors->push_back(osg::Vec4(r, g, b, a));
	HUDBackgroundGeometry->setColorArray(HUDcolors);
}

osg::ref_ptr<osgText::Text> HUD::getTextPointer() {
	return textOne;
}
