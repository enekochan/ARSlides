/*
 * CPicture.cpp
 *
 *  Created on: Mar 7, 2011
 *      Author: enekochan
 */

#include <algorithm> // for lowercase
#include <osgText/Text>
#include <osg/Geometry>
#include <osg/BlendFunc>
#include <osg/Material>
#include "CPicture.h"

CPicture::CPicture(std::string filename, float width) : osg::Group() {
	// Take file extension and lowercase it
	std::string extension = filename.substr(filename.length()-3, 3);
	std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
	// If it is a png or tga file it's going to be transparent
	if (extension == "png" || extension == "tga") {
		this->addChild(readTransparentImageFile(filename, width));
	} else {
		osg::Image* img = osgDB::readImageFile(filename);
		//img->flipHorizontal();
		img->flipVertical();

		float aspectRatio = (float)img->t() / (float)img->s();
		float pictureWidth = width;
		float pictureHeight = pictureWidth * aspectRatio;

		pPictureGeode = new osg::Geode();

		bool texRect = true;

		if (texRect) {
			pPictureTexture = new osg::TextureRectangle(img);
			pPictureGeode->addDrawable(osg::createTexturedQuadGeometry(
				osg::Vec3(-pictureWidth / 2, -pictureHeight / 2, 0),
				osg::Vec3(pictureWidth, 0, 0),
				osg::Vec3(0, pictureHeight, 0),
				0,  img->t(), img->s(), 0));
		} else {
			pPictureTexture = new osg::Texture2D(img);
			pPictureGeode->addDrawable(osg::createTexturedQuadGeometry(
				osg::Vec3(-pictureWidth / 2, -pictureHeight / 2, 0),
				osg::Vec3(pictureWidth, 0, 0),
				osg::Vec3(0, pictureHeight, 0),
				0,  1,1, 0));
		}

		pPictureGeode->getOrCreateStateSet()->setTextureAttributeAndModes(0, pPictureTexture.get());
		this->addChild(pPictureGeode.get());
	}
}

CPicture::~CPicture()
{
}

osg::Node* CPicture::readTransparentImageFile(std::string filename, float width) {
	// Picture center aligned with marker center or lower left corner beginning on center of the marker
	bool centered = true;
	// Picture perpendicular or parallel to the marker
	bool perpendicular = false;

	// Read the image
	osg::Image* pImage = osgDB::readImageFile( filename );

	// Proportion and size of the image
	float aspectRatio = (float)pImage->t() / (float)pImage->s();
	float pictureWidth = width;
	float pictureHeight = pictureWidth * aspectRatio;

	printf("aspectRatio: %f", aspectRatio);

	// create drawable geometry object
	osg::Geometry* pGeo = new osg::Geometry;

	// add 4 vertices creating a quad
	osg::Vec3Array* pVerts = new osg::Vec3Array;
	if (perpendicular) {
		if (centered) {
			pVerts->push_back( osg::Vec3( -pictureWidth/2, 0, -pictureHeight/2 ) );
			pVerts->push_back( osg::Vec3( pictureWidth/2, 0, -pictureHeight/2 ) );
			pVerts->push_back( osg::Vec3( pictureWidth/2, 0, pictureHeight/2 ) );
			pVerts->push_back( osg::Vec3( -pictureWidth/2, 0, pictureHeight/2 ) );
		} else {
			pVerts->push_back( osg::Vec3( 0, 0, 0 ) );
			pVerts->push_back( osg::Vec3( pictureWidth, 0, 0 ) );
			pVerts->push_back( osg::Vec3( pictureWidth, 0, pictureHeight ) );
			pVerts->push_back( osg::Vec3( 0, 0, pictureHeight ) );
		}
	} else {
		if (centered) {
			pVerts->push_back( osg::Vec3( -pictureWidth/2, -pictureHeight/2, 0 ) );
			pVerts->push_back( osg::Vec3( pictureWidth/2, -pictureHeight/2, 0 ) );
			pVerts->push_back( osg::Vec3( pictureWidth/2, pictureHeight/2, 0 ) );
			pVerts->push_back( osg::Vec3( -pictureWidth/2, pictureHeight/2, 0 ) );
		} else {
			pVerts->push_back( osg::Vec3( 0, 0, 0 ) );
			pVerts->push_back( osg::Vec3( pictureWidth, 0, 0 ) );
			pVerts->push_back( osg::Vec3( pictureWidth, pictureHeight, 0 ) );
			pVerts->push_back( osg::Vec3( 0, pictureHeight, 0 ) );
		}
	}
	pGeo->setVertexArray( pVerts );

	// create a primitive set
	osg::DrawElementsUInt* pPrimitiveSet = new osg::DrawElementsUInt( osg::PrimitiveSet::QUADS, 0 );
	pPrimitiveSet->push_back( 3 );
	pPrimitiveSet->push_back( 2 );
	pPrimitiveSet->push_back( 1 );
	pPrimitiveSet->push_back( 0 );
	pGeo->addPrimitiveSet( pPrimitiveSet );

	// create an arraw for texture coordinates
	osg::Vec2Array* pTexCoords = new osg::Vec2Array( 4 );
	(*pTexCoords)[0].set( 0.0f, 0.0f );
	(*pTexCoords)[1].set( 1.0f, 0.0f );
	(*pTexCoords)[2].set( 1.0f, 1.0f );
	(*pTexCoords)[3].set( 0.0f, 1.0f );
	pGeo->setTexCoordArray( 0, pTexCoords );

	// create geometry node that will contain all our drawables
	osg::Geode* pGeode = new osg::Geode;
	osg::StateSet* pStateSet = pGeode->getOrCreateStateSet();
	pStateSet->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
	pGeode->addDrawable( pGeo );

	// load and set texture attribute and mode
	osg::Texture2D* pTex = new osg::Texture2D;
	pTex->setImage( pImage );
	pTex->setResizeNonPowerOfTwoHint(false);
	pStateSet->setTextureAttributeAndModes( 0, pTex, osg::StateAttribute::ON );

	//	Create a material for the geometry.
	osg::Material* material = new osg::Material;
	material->setAmbient(osg::Material::FRONT_AND_BACK,osg::Vec4(1.0f,1.0f,1.0f,1.0f));
	material->setDiffuse(osg::Material::FRONT_AND_BACK,osg::Vec4(1.0f,1.0f,1.0f,1.0f));
	material->setAlpha(osg::Material::FRONT_AND_BACK, 0.5);
 	//pStateSet->setAttribute(material,osg::StateAttribute::ON);

	pStateSet->setMode(GL_BLEND, osg::StateAttribute::ON);
	osg::BlendFunc* blend = new osg::BlendFunc;
	blend->setFunction(osg::BlendFunc::SRC_ALPHA, osg::BlendFunc::ONE_MINUS_DST_ALPHA);
 	//pStateSet->setAttributeAndModes(blend, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
	return pGeode;
}
