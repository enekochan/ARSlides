#include <stdio.h>
#include "MyMarkerVisibilityCallback.h"
//#include "CText.h"
#include <sstream>

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgDB/FileUtils>
#include <osgDB/ReadFile>
#include <osg/ImageStream>
#include <osg/TextureRectangle>

MyMarkerVisibilityCallback::MyMarkerVisibilityCallback(osgART::Marker* marker) : MarkerVisibilityCallback(marker)
{
	visible = false;
	timer = 0;
	delay = 16;
	angle = 0.0;
}

void MyMarkerVisibilityCallback::operator()(osg::Node* node, osg::NodeVisitor* nv)
{
	nv->setNodeMaskOverride(0xFFFFFFFF);

	mt = dynamic_cast<osg::MatrixTransform*>(node);

	//angle += 1.0f; // Uncomment this to make things rotate
	mt->preMult(osg::Matrix::rotate(osg::inDegrees(angle),osg::Z_AXIS));
	//mt->preMult(osg::Matrix::scale(osg::Vec3(10.0,10.0,10.0)));
	//mt->preMult(osg::Matrix::translate(200,200,200));

	//mt->addUpdateCallback(new osg::AnimationPathCallback(osg::Vec3(-10.0,-10.0,-10.0),osg::X_AXIS, osg::inDegrees(180.0f)));

	osg::Vec3 pos = mt->getMatrix().getTrans();
//	std::cout << "x: " << (int)pos.x() << " y: " << (int)pos.y() << " z: " << (int)pos.z() << std::endl;

	osg::Quat rotacion = mt->getMatrix().getRotate();
	double r,x,y,z;
	rotacion.getRotate(r, x, y, z);
//	std::cout << "x: " << (int)(x*1000) << " y: " << (int)(y*1000) << " z: " << (int)(z*1000) << std::endl;
//	std::cout << "r: " << r << std::endl;

	if (text) {
		std::stringstream out;
		out << " x: " << (int)pos.x() << " y: " << (int)pos.y() << " z: " << (int)pos.z();
		std::string s = out.str();
		text->setText(s);
	}

	/*
	CText* myText = dynamic_cast<CText*>(mt->getChild(1));
	myText->setText("hi");
	//myText->setColor(0.0,0.0,0.0,1.0);
	myText->setPosition(osg::Vec3(4, 4, 1));
	*/

	if(m_marker->valid())
	{
		mt->setNodeMask(0xFFFFFFFF);
		if (!visible) {
			timer += 1;
		}
		if (!visible && timer > delay) {
			visible = !visible;
			timer = 0;
			std::cout << "marker is visible" << std::endl;
		}
	}
	else
	{
		//mt->setNodeMask(0x0);
		//std::cout << "marker is not visible" << std::endl;
		if (visible) timer += 1;
		if (visible && timer > delay) {
			visible = !visible;
			timer = 0;
			mt->setNodeMask(0x0);
			std::cout << "marker is not visible" << std::endl;
		}
	}

	traverse(node, nv);
}

void MyMarkerVisibilityCallback::setText(osg::ref_ptr<osgText::Text> t) {
	text = t;
}
