#include <stdio.h>
#include "MarkerProximityUpdateCallback.h"

MarkerProximityUpdateCallback::MarkerProximityUpdateCallback(osgART::Marker* mkA, osgART::Marker* mkB, osg::MatrixTransform* mA, osg::MatrixTransform* mB, osg::Switch* switchA, float threshold, int* currentSlide, int position) :
	  osg::NodeCallback(),
		  markerA(mkA),markerB(mkB),
		  mtA(mA), mtB(mB),
		  mSwitchA(switchA), mThreshold(threshold), mCurrentSlide(currentSlide), mPosition(position) { }


void MarkerProximityUpdateCallback::operator()(osg::Node* node, osg::NodeVisitor* nv) {
	/** CALCULATE INTER-MARKER PROXIMITY:
	Here we obtain the current position of each marker, and the
	distance between them by examining
	the translation components of their parent transformation
	matrices **/
	float distance = -1;
	if (markerA->valid() && markerB->valid()) { // Only if both markers are on screen...
		// Markers already come with their trans. TODO: Should we just use markers trans and not instantiate 2 new ones?
		osg::Vec3 posA = mtA->getMatrix().getTrans();
		osg::Vec3 posB = mtB->getMatrix().getTrans();

		osg::Vec3 offset = posA - posB;
		distance = offset.length();
	}
	//printf("distance: %f, if: %s\r\n", distance, (distance < mThreshold)?"true":"false");
	//printf("mPosition: %i\r\n", mPosition);
	//printf("mCurrentSlide: %i\r\n", *mCurrentSlide);

	if (distance != -1 && distance < mThreshold && (mPosition-1) == *mCurrentSlide) {
		*mCurrentSlide = mPosition;
		mSwitchA->setSingleChildOn(*mCurrentSlide);
	} else if (distance != -1 && distance >= mThreshold && (mPosition) == *mCurrentSlide) {
		*mCurrentSlide = mPosition - 1;
		mSwitchA->setSingleChildOn(*mCurrentSlide);
	}

	//scene->setUpdateCallback(new MarkerProximityUpdateCallback(mtA, mtB, switchA.get(), switchB.get(), 200.0f));

	/** LOAD APPROPRIATE MODELS:
	Here we use each marker's OSG Switch node to swap between
	models, depending on the inter-marker distance we have just
	calculated. **/
	/*
	if (distance <= mThreshold) {
		//if (mSwitchA->getNumChildren() > 1) mSwitchA->setSingleChildOn(1);
		mSwitchA->setSingleChildOn(1);
	} else {
		//if (mSwitchA->getNumChildren() > 0) mSwitchA->setSingleChildOn(0);
		mSwitchA->setSingleChildOn(0);
	}
	*/
	traverse(node,nv);
}
