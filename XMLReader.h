/*
 * XMLReader.h
 *
 *  Created on: May 9, 2011
 *      Author: enekochan
 */

#ifndef XMLREADER_H_
#define XMLREADER_H_

#include <map>
#include <string>
#include <tinyxml.h> // tinyxml must be installed "sudo apt-get install libtinyxml-dev"

class XMLReader {

private:
	std::string videoTag;
	std::string deviceTag;
	std::string sourceTag;
	std::string widthTag;
	std::string heightTag;
	std::string fullscreenTag;
	std::string monitorTag;
	std::string debugTag;
	std::string markersTag;
	std::string markerTag;
	std::string slidesTag;
	std::string slideTag;
	std::string device, source, width, height, monitor;
	bool fullscreen, debug;
	std::map<int, std::string> markers;
	std::map<int, std::string> slides;
public:

	XMLReader(std::string path);
	std::map<int, std::string> getMarkers(void);
	std::map<int, std::string> getSlides(void);
	std::string getDevice();
	std::string getSource();
	std::string getWidth();
	std::string getHeight();
	bool getFullscreen();
	std::string getMonitor();
	bool getDebug();
	bool hasWidth();
	bool hasHeight();
};

#endif /* XMLREADER_H_ */
