#include "Media.h"

Media::Media(std::string file, int size, bool isVideo, osg::Image* md) : osg::Group()
{
	//osg::Image* md = osgDB::readImageFile(file);
	//m_MediaStream = dynamic_cast<osg::ImageStream*>(md);
	//osg::ref_ptr<osg::Image> md = osgDB::readImageFile(file);
	//m_MediaStream = dynamic_cast<osg::ImageStream*>(md.get());
	m_MediaStream = dynamic_cast<osg::ImageStream*>(md);

	if(!m_MediaStream.valid() && isVideo)
	{
		std::cout << "Could not get media!" << std::endl;
		return;
	}

	float aspectRatio = (float)md->t() / (float)md->s();
	float videoWidth = size;
	float videoHeight = videoWidth * aspectRatio;

	m_MediaGeode = new osg::Geode();

	if(isVideo)
	{
		m_MediaTexture = new osg::TextureRectangle(md);
		m_MediaGeode->addDrawable(
			osg::createTexturedQuadGeometry(
				osg::Vec3(-videoWidth / 2, -videoHeight / 2, 0),
				osg::Vec3(videoWidth, 0, 0),
				osg::Vec3(0, videoHeight, 0),
				0,  md->t(), md->s(), 0
			)
		);
	}
	else
	{
		m_MediaTexture = new osg::Texture2D(md);
		m_MediaGeode->addDrawable(
			osg::createTexturedQuadGeometry(
				osg::Vec3(-videoWidth / 2, -videoHeight / 2, 0),
				osg::Vec3(videoWidth, 0, 0),
				osg::Vec3(0, videoHeight, 0),
				0, 1, 1, 0
			)
		);
	}

	m_MediaGeode->getOrCreateStateSet()->setTextureAttributeAndModes(0, m_MediaTexture.get());
	this->addChild(m_MediaGeode.get());
}

void Media::stop()
{
	m_MediaStream->pause();
}

void Media::start()
{
	m_MediaStream->play();
}
