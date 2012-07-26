#include "MediaMarkerVisibilityCallback.h"

MediaMarkerVisibilityCallback::MediaMarkerVisibilityCallback(osgART::Marker* marker) : MarkerVisibilityCallback(marker)
{
}

void MediaMarkerVisibilityCallback::operator()(osg::Node* node, osg::NodeVisitor* nv)
{
	nv->setNodeMaskOverride(0xFFFFFFFF);

	mt = dynamic_cast<osg::MatrixTransform*>(node);
	mv = (Media*)mt->getChild(0);

	if(m_marker->valid())
	{
		mt->setNodeMask(0xFFFFFFFF);
		mv->start();
	}
	else
	{
		mt->setNodeMask(0x0);
		mv->stop();
	}

	traverse(node, nv);
}
