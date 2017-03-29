#include "provided.h"
#include "MyMap.h"
#include <string>
#include <cctype>
#include <iostream>
using namespace std;

class AttractionMapperImpl
{
public:
	AttractionMapperImpl();
	~AttractionMapperImpl();
	void init(const MapLoader& ml);
	bool getGeoCoord(string attraction, GeoCoord& gc) const;

private:
	MyMap<string, Attraction> m_attMap;

	// returns a purely lower case version of an input string
	string thisStringInLowerCase(string input) const;
};

AttractionMapperImpl::AttractionMapperImpl()
{
}

AttractionMapperImpl::~AttractionMapperImpl()
{
}

void AttractionMapperImpl::init(const MapLoader& ml)
{
	// Look at all segments and load the attractions inside each segment (if they exist)
	for (unsigned int ithSegment = 0; ithSegment < ml.getNumSegments(); ithSegment++) {
		StreetSegment tempSeg;
		ml.getSegment(ithSegment, tempSeg);

		for (unsigned int jthAttraction = 0; jthAttraction < tempSeg.attractions.size(); jthAttraction++) {
			// Adds an Attraction to the Attraction map
			// using its lower case name as a key and itself as the value
			m_attMap.associate(thisStringInLowerCase(tempSeg.attractions[jthAttraction].name), tempSeg.attractions[jthAttraction]);
		}
	}
	// cerr << "Size of map is: " << m_attMap.size() << endl;
}

bool AttractionMapperImpl::getGeoCoord(string attraction, GeoCoord& gc) const
{
	const Attraction* foundAttraction = m_attMap.find(thisStringInLowerCase(attraction));

	// If the attraction was found
	if (foundAttraction != nullptr) {
		gc = foundAttraction->geocoordinates;
		return true;
	}

	return false;  // This compiles, but may not be correct
}

string AttractionMapperImpl::thisStringInLowerCase(string input) const {
	int stringLength = input.length();
	string result = input;
	for (int i = 0; i < stringLength; i++) {
		result[i] = tolower(input[i]);
	}
	return result;
}


//******************** AttractionMapper functions *****************************

// These functions simply delegate to AttractionMapperImpl's functions.
// You probably don't want to change any of this code.

AttractionMapper::AttractionMapper()
{
	m_impl = new AttractionMapperImpl;
}

AttractionMapper::~AttractionMapper()
{
	delete m_impl;
}

void AttractionMapper::init(const MapLoader& ml)
{
	m_impl->init(ml);
}

bool AttractionMapper::getGeoCoord(string attraction, GeoCoord& gc) const
{
	return m_impl->getGeoCoord(attraction, gc);
}
