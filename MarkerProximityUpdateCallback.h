/*
 * MediaMarkerVisibilityCallback.h
 *
 *  Created on: May 8, 2011
 *      Author: enekochan
 */

#ifndef MARKERPROXIMITYUPDATECALLBACK_H_
#define MARKERPROXIMITYUPDATECALLBACK_H_

#include <osgART/Marker>

#include <osg/NodeCallback>
#include <osg/MatrixTransform>
#include <osg/Switch>

class MarkerProximityUpdateCallback : public osg::NodeCallback {

private:
	osgART::Marker* markerA;
	osgART::Marker* markerB;

	osg::MatrixTransform* mtA;
	osg::MatrixTransform* mtB;

	osg::Switch* mSwitchA;

	float mThreshold;

	int mPosition;
	int* mCurrentSlide;

public:

	MarkerProximityUpdateCallback(osgART::Marker* mkA, osgART::Marker* mkB, osg::MatrixTransform* mA, osg::MatrixTransform* mB, osg::Switch* switchA, float threshold, int* currentSlide, int position);
	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv);
	void setPosition(int pos);
	void setCurrentSlide(int *slide);
};

#endif /* MARKERPROXIMITYUPDATECALLBACK_H_ */
