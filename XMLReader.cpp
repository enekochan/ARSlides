#include <stdio.h>
#include "XMLReader.h"

XMLReader::XMLReader(std::string path) {

	videoTag = "video";
	deviceTag = "device";
	sourceTag = "source";
	widthTag = "width";
	heightTag = "height";
	fullscreenTag = "fullscreen";
	monitorTag = "monitor";
	debugTag = "debug";
	markersTag = "markers";
	markerTag = "marker";
	slidesTag = "slides";
	slideTag = "slide";

	TiXmlDocument doc(path);
	bool loadOkay = doc.LoadFile();

	if (loadOkay) {
		TiXmlNode* pParent = &doc;
		TiXmlElement* pElement;

		TiXmlNode* pVideo = pParent->FirstChildElement(videoTag);
		device = pVideo->FirstChildElement(deviceTag)->FirstChild()->Value(); // TODO: Validate the fields
		source = pVideo->FirstChildElement(sourceTag)->FirstChild()->Value();
		if ((pElement = pVideo->FirstChildElement(widthTag)))
			width = pElement->FirstChild()->Value();
		if ((pElement = pVideo->FirstChildElement(heightTag)))
			height = pElement->FirstChild()->Value();
		fullscreen = std::string("true").compare(pVideo->FirstChildElement(fullscreenTag)->FirstChild()->Value()) == 0;
		monitor = pVideo->FirstChildElement(monitorTag)->FirstChild()->Value();
		debug = std::string("true").compare(pVideo->FirstChildElement(debugTag)->FirstChild()->Value()) == 0;

		TiXmlNode* pMarkers = pParent->FirstChildElement(markersTag)->FirstChildElement(markerTag);
		for ( int i = 0 ; pMarkers != 0; pMarkers = pMarkers->NextSibling())
		{
			if (pMarkers->Type() == TiXmlNode::TINYXML_ELEMENT) // If it is of type ELEMENT (used to bypass commented nodes)
				markers.insert(std::pair<int, std::string>(i++, pMarkers->FirstChild()->Value()));
		}

		TiXmlNode* pSlides = pParent->FirstChildElement(slidesTag)->FirstChildElement(slideTag);
		for ( int i = 0 ; pSlides != 0; pSlides = pSlides->NextSibling())
		{
			if (pSlides->Type() == TiXmlNode::TINYXML_ELEMENT) // If it is of type ELEMENT (used to bypass commented nodes)
				slides.insert(std::pair<int, std::string>(i++, pSlides->FirstChild()->Value()));
		}
	}
}

std::map<int, std::string> XMLReader::getMarkers(void) {
	return markers;
}

std::map<int, std::string> XMLReader::getSlides(void) {
	return slides;
}

std::string XMLReader::getDevice(void) {
	return device;
}

std::string XMLReader::getSource(void) {
	return source;
}

std::string XMLReader::getWidth(void) {
	return width;
}

std::string XMLReader::getHeight(void) {
	return height;
}

bool XMLReader::getFullscreen(void) {
	return fullscreen;
}

std::string XMLReader::getMonitor(void) {
	return monitor;
}

bool XMLReader::getDebug(void) {
	return debug;
}

bool XMLReader::hasWidth(void) {
	return std::string("").compare(width) != 0;
}

bool XMLReader::hasHeight(void) {
	return std::string("").compare(height) != 0;
}
