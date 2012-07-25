#include <osgART/Foundation>
#include <osgART/VideoLayer>
#include <osgART/PluginManager>
#include <osgART/VideoGeode>
#include <osgART/Utils>
#include <osgART/GeometryUtils>
#include <osgART/MarkerCallback>
#include <osgART/TransformFilterCallback>

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>

#include <osgDB/FileUtils>
#include <osgDB/ReadFile>

#include <sys/stat.h> // Used to know if files exist or not

#include "CText.h"
#include "CPicture.h"
#include "HUD.h"
#include "MyMarkerVisibilityCallback.h"
#include "MarkerProximityUpdateCallback.h"
#include "XMLReader.h"

// Used to configure the geode mode of the video background
// You should ALWAYS use TEXTURE_RECTANGLE
enum VideoGeodeMode
{
	TEXTURE_2D,
	TEXTURE_RECTANGLE,
};

#define VIDEO_GEODE_MODE TEXTURE_RECTANGLE


int currentSlide = 0;
int totalSlides = 0;
int position = 0;
std::map<int, std::string> slides;
osg::ref_ptr<HUD> hud; // heads-up display for on screen debug data


bool FileExists(std::string strFilename) {
	struct stat stFileInfo;
	bool blnReturn;
	int intStat;

	// Attempt to get the file attributes
	intStat = stat(strFilename.c_str(),&stFileInfo);
	if(intStat == 0) {
	  blnReturn = true;
	} else {
	  blnReturn = false;
	}
	return(blnReturn);
}

osg::ref_ptr<osg::Node> loadNode(std::string file) {
	std::string extension = file.substr(file.length()-3, 3);
	if (extension == "osg" || extension == "3ds" || extension == "obj" || extension == "dae" || extension == "fbx") {
		osg::ref_ptr<osg::Node> n = osgDB::readNodeFile(file);
		n->setName(extension);
		return n;
	} else if (extension == "png" || extension == "tga" || extension == "jpg" || extension == "jpeg" || extension == "gif") {
		osg::ref_ptr<CPicture> p = new CPicture(file, 180);
		p->setName(extension);
		return p;
	} else {
		// It should never reach here, but if it does a simple cube is shown (this also prevents a warning)
		return osgART::testCube();
	}
}

void loadNextObject(osg::ref_ptr<osg::Switch> arSwitch) {
	if (++currentSlide >= totalSlides)
		currentSlide = 0;
	if(slides[currentSlide] == "data/models/flores/flor2.osg") {
		if (++currentSlide >= totalSlides)
			currentSlide = 0;
	}
	arSwitch->setSingleChildOn(currentSlide);
}

void loadPreviousObject(osg::ref_ptr<osg::Switch> arSwitch) {
	if (--currentSlide < 0)
		currentSlide = totalSlides - 1;
	if(slides[currentSlide] == "data/models/flores/flor2.osg") {
		if (--currentSlide < 0)
			currentSlide = totalSlides - 1;
	}
	arSwitch->setSingleChildOn(currentSlide);
}

class MyKeyboardEventHandler : public osgGA::GUIEventHandler {

private:
	osg::ref_ptr<osg::MatrixTransform> arTransform;
	osg::ref_ptr<osg::Switch> arSwitch;
	osg::ref_ptr<osg::Group> object;
	osg::ref_ptr<osg::Group> currentObject;
	osg::ref_ptr<osg::Group> oldObject;

public:
    MyKeyboardEventHandler() : osgGA::GUIEventHandler() { }

    void setMatrixTransform(osg::ref_ptr<osg::MatrixTransform> mt) {
    	arTransform = mt;
    }

    void setSwitch(osg::ref_ptr<osg::Switch> sw) {
    	arSwitch = sw;
    }

    void setObject(osg::ref_ptr<osg::Group> g) {
    	object = g;
    }

    void setOldObject() {
       	currentObject = oldObject;
      	this->setCurrentObject();
    }

    void setNewObject(osg::ref_ptr<osg::Group> g) {
    	oldObject = currentObject;
    	currentObject = g;
    	this->setCurrentObject();
    }

    void setCurrentObject() {

    }

    /**
        OVERRIDE THE HANDLE METHOD:
        The handle() method should return true if the event has been dealt with
        and we do not wish it to be handled by any other handler we may also have
        defined. Whether you return true or false depends on the behaviour you
        want - here we have no other handlers defined so return true.
    **/
    virtual bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa,
                        osg::Object* obj, osg::NodeVisitor* nv) {

        switch (ea.getEventType()) {


            /** KEY EVENTS:
                Key events have an associated key and event names.
                In this example, we are interested in keys up/down/right/left arrow
                and space bar.
                If we detect a press then we modify the transformation matrix
                of the local transform node. **/
            case osgGA::GUIEventAdapter::KEYDOWN: {

                switch (ea.getKey()) {

                    case osgGA::GUIEventAdapter::KEY_Up:
                    	std::cout << "+ UP key pressed" << std::endl;
                    	return true;
                    case osgGA::GUIEventAdapter::KEY_Down:
                    	std::cout << "+ DOWN key pressed" << std::endl;
                        return true;
                    case osgGA::GUIEventAdapter::KEY_Right:
                    	std::cout << "+ RIGHT key pressed" << std::endl;
                    	loadNextObject(arSwitch);
                    	return true;
                    case osgGA::GUIEventAdapter::KEY_Left:
                        std::cout << "+ LEFT key pressed" << std::endl;
                        loadPreviousObject(arSwitch);
                        return true;
                }
                default: return false;
            }
        }
    }
};

osg::Group* createImageBackground(osg::Image* video) {
	osgART::VideoLayer* _layer = new osgART::VideoLayer();
	_layer->setSize(*video);
	osgART::VideoGeode* _geode;
	if (VIDEO_GEODE_MODE == TEXTURE_RECTANGLE)
		_geode = new osgART::VideoGeode(osgART::VideoGeode::USE_TEXTURE_RECTANGLE, video);
	else if (VIDEO_GEODE_MODE == TEXTURE_2D)
		_geode = new osgART::VideoGeode(osgART::VideoGeode::USE_TEXTURE_2D, video); // This can't be used to view video data on slides
	else {
		std::cout << "ERROR: Video Geode Mode not defined!!!" << std::endl;
		exit (-1);
	}

	addTexturedQuad(*_geode,video->s(),video->t());
	_layer->addChild(_geode);
	return _layer;
}

int main(int argc, char* argv[])  {

	// create a root node
	osg::ref_ptr<osg::Group> root = new osg::Group;

	osgViewer::Viewer viewer;

	// attach root node to the viewer
	viewer.setSceneData(root.get());

	// add relevant handlers to the viewer
	viewer.addEventHandler(new osgViewer::StatsHandler);
	viewer.addEventHandler(new osgViewer::WindowSizeHandler);
	viewer.addEventHandler(new osgViewer::ThreadingHandler);
	viewer.addEventHandler(new osgViewer::HelpHandler);
	MyKeyboardEventHandler *myKeyboardEventHandler = new MyKeyboardEventHandler();
	viewer.addEventHandler(myKeyboardEventHandler);

	// preload the video and tracker
	int _video_id = osgART::PluginManager::instance()->load("osgart_video_artoolkit2");
	int _tracker_id = osgART::PluginManager::instance()->load("osgart_tracker_artoolkit2");

	// Load a video plugin.
	osg::ref_ptr<osgART::Video> video =
		dynamic_cast<osgART::Video*>(osgART::PluginManager::instance()->get(_video_id));

	// check if an instance of the video stream could be started
	if (!video.valid())
	{
		// Without video an AR application can not work. Quit if none found.
		osg::notify(osg::FATAL) << "Could not initialize video plugin!" << std::endl;
		exit(-1);
	}

	// Read configuration from XML file
	XMLReader* configXML = new XMLReader("data/config.xml");

	// found video - configure now
	osgART::VideoConfiguration* _config = video->getVideoConfiguration();
	// Use any existing configuration.
	if (_config)
	{
 		if (argc > 1) {
			_config->deviceconfig = argv[1];
		} else  {
			if (std::string("").compare(configXML->getSource()) == 0) {
				osg::notify(osg::FATAL) << "Video source not defined in configuration XML" << std::endl;
				exit(-1);
			}

			if (std::string("USB").compare(configXML->getDevice()) == 0) {
				// Height and Width are used only if both are defined
				std::string w = configXML->hasWidth()?",width=" + configXML->getWidth():"";
				std::string h = configXML->hasHeight()?",height=" + configXML->getHeight():"";
				std::string wh = (std::string("").compare(w+h) != 0) ? "" : w+h;
				_config->deviceconfig = "v4l2src device=" + configXML->getSource() + " use-fixed-fps=false ! ffmpegcolorspace ! capsfilter caps=video/x-raw-rgb,bpp=24" + wh + " ! identity name=artoolkit ! fakesink";
			} else if (std::string("file").compare(configXML->getDevice()) == 0) {
				_config->deviceconfig = "filesrc location=" + configXML->getSource() + " ! decodebin ! ffmpegcolorspace ! capsfilter caps=video/x-raw-rgb,bpp=24 ! identity name=artoolkit ! fakesink";
			} else {
				osg::notify(osg::FATAL) << "Incorrect video mode in XML file: " << configXML->getDevice() << std::endl;
				exit(-1);
			}
		}
	}

	// Open the video. This will not yet start the video stream but will
	// get information about the format of the video which is essential
	// for the connected tracker
	video->open();

	osg::ref_ptr<osgART::Tracker> tracker =
		dynamic_cast<osgART::Tracker*>(osgART::PluginManager::instance()->get(_tracker_id));

	if (!tracker.valid())
	{
		// Without tracker an AR application can not work. Quit if none found.
		osg::notify(osg::FATAL) << "Could not initialize tracker plugin!" << std::endl;
		exit(-1);
	}

	// get the tracker calibration object
	osg::ref_ptr<osgART::Calibration> calibration = tracker->getOrCreateCalibration();

	// load a calibration file
	if (!calibration->load(std::string("data/camera_para.dat")))
	{
		// the calibration file was non-existing or couldn't be loaded
		osg::notify(osg::FATAL) << "Non existing or incompatible calibration file" << std::endl;
		exit(-1);
	}

	// set the image source for the tracker
	tracker->setImage(video.get());

	osgART::TrackerCallback::addOrSet(root.get(), tracker.get());

	// Tracker parameters are read and written via a field mechanism.
	// Init access to a field within the tracker, in this case, the binarization threshhold.
	osg::ref_ptr< osgART::TypedField<int> > _threshold =
	    reinterpret_cast< osgART::TypedField<int>* >(tracker->get("threshold"));

	// Values are be accessed through a get()/set() mechanism on the field pointer.
	if (_threshold.valid())  {
	    // Set the threshold, and read back.
	    _threshold->set(100);
	    osg::notify(osg::WARN) << "Field 'threshold' = " << _threshold->get() << std::endl;
	} else {
	    osg::notify(osg::WARN) << "Field 'threshold' not supported for this tracker" << std::endl;
	}

	std::map<int, std::string> markers = configXML->getMarkers();
	int numMarkers = markers.size();

	if (numMarkers > 2)
	{
		osg::notify(osg::FATAL) << "You can only define 2 markers in XML configuration file" << std::endl;
		exit(-1);
	}

	osg::ref_ptr<osgART::Marker> marker = tracker->addMarker(markers[0]);
	osg::ref_ptr<osgART::Marker> marker2 = tracker->addMarker(markers[1]);

	if (!marker.valid() || !marker2.valid())
	{
		// Without marker an AR application can not work. Quit if none found.
		osg::notify(osg::FATAL) << "Could not add marker!" << std::endl;
		exit(-1);
	}

	marker->setActive(true);
	marker2->setActive(true);

	osg::ref_ptr<osg::MatrixTransform> arTransform = new osg::MatrixTransform();
	osg::ref_ptr<osg::MatrixTransform> arTransform2 = new osg::MatrixTransform();

	osgART::attachDefaultEventCallbacks(arTransform.get(), marker.get());
	osgART::attachDefaultEventCallbacks(arTransform2.get(), marker2.get());

	osg::ref_ptr<MyMarkerVisibilityCallback> myMarkerVisibilityCallback = new MyMarkerVisibilityCallback(marker);
	osgART::addEventCallback(arTransform.get(), myMarkerVisibilityCallback);

	bool debugOnHUD = configXML->getDebug();
	if (debugOnHUD) {
		hud = new HUD(640, 480, 100);
		hud->setText("");
		hud->setHUDColor(1.0, 0.0, 0.0, 0.5);
		myMarkerVisibilityCallback->setText(hud->getTextPointer());
	}

	currentSlide = 0;

	slides = configXML->getSlides();
	totalSlides = slides.size();

	osg::ref_ptr<osg::Switch> sw = new osg::Switch();
	for (int i = 0 ; i < totalSlides ; i++) {
		if (!FileExists(slides[i])) { // If the file does not exist
			osg::notify(osg::FATAL) << "The " << slides[i] << " file does not exist" << std::endl;
		} else {
			if (slides[i] == "data/models/flores/flor2.osg")
				position = i;
			if (i == 0) { // Only the first slide is visible at first
				sw->addChild(loadNode(slides[i]), true);
			} else {
				sw->addChild(loadNode(slides[i]), false);
			}
		}
	}

	osg::ref_ptr<osg::MatrixTransform> mTransform = new osg::MatrixTransform();
	/*
	osg::ref_ptr<osg::Matrix> mRotationMatrix;  // coefficient of matrix multiplication
	// Make a rotation matrix:
	mRotationMatrix->makeRotate( 5.0, osg::Vec3f( 1.0f, 0.0f, 0.0f ) );  // X axis
	// Rotate matrix every frame (matrix of OSG::MatrixTransform):
	osg::Matrix m = mTransform->getMatrix();
	m = mRotationMatrix->identity() * m;
	mTransform->setMatrix( m );
*/
	mTransform->addChild(sw.get());
	arTransform->addChild(sw.get());
	arTransform2->addChild(osgDB::readNodeFile("data/models/flores/flores.osg"));

	myKeyboardEventHandler->setSwitch(sw);

	arTransform->getOrCreateStateSet()->setRenderBinDetails(100, "RenderBin");
	arTransform2->getOrCreateStateSet()->setRenderBinDetails(100, "RenderBin");

	osg::ref_ptr<osg::Group> videoBackground = createImageBackground(video.get());
	videoBackground->getOrCreateStateSet()->setRenderBinDetails(0, "RenderBin");

	osg::ref_ptr<osg::Camera> cam = calibration->createCamera();

	cam->addChild(arTransform.get());
	cam->addChild(arTransform2.get());
	cam->addChild(videoBackground.get());

	root->addChild(cam.get());
	if (debugOnHUD)
		root->addChild(hud);

	// Used to know the distance between the 2 markers
	root->setUpdateCallback(new MarkerProximityUpdateCallback(marker, marker2, arTransform, arTransform2, sw.get(), 300, &currentSlide, position));

	video->start();

	if (!configXML->getFullscreen() && configXML->hasWidth() && configXML->hasHeight()) {
		viewer.setUpViewInWindow( 0, 0, atoi(configXML->getWidth().c_str()), atoi(configXML->getHeight().c_str()) ); // Hace que se ejecute en una ventana en vez de a pantalla completa (parametros: posición y tamaño de la ventana)
	}

	if (std::string("all").compare(configXML->getMonitor()) != 0) {
		int monitor = atoi(configXML->getMonitor().c_str()); // A non number string will generate a 0
		viewer.setUpViewOnSingleScreen(monitor); // Output to monitor number defined in XML configuration file
	}

	viewer.run();

	video->stop();
	video->close();

	return 0;
}
