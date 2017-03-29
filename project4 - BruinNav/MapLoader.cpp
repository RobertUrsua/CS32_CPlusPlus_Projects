#include "provided.h"
#include <string.h>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

class MapLoaderImpl
{
public:
	MapLoaderImpl();
	~MapLoaderImpl();
	bool load(string mapFile);
	size_t getNumSegments() const;
	bool getSegment(size_t segNum, StreetSegment& seg) const;

private:
	vector<StreetSegment> m_streetSegs;

};

MapLoaderImpl::MapLoaderImpl()
{
}

MapLoaderImpl::~MapLoaderImpl()
{
}

bool MapLoaderImpl::load(string mapFile)
{
	// create if stream for opening file
	ifstream ifs;
	ifs.open(mapFile);

	// if loading the file fails, return false
	if (!ifs) {
		return false;
	}

	// If loading succeeds, load the street segments

	string tempString;
	while (getline(ifs, tempString) && tempString != "") {

		//	FIRST ATTEMP TO LOAD SEGMENT NAME INSIDE THE WHILE LOOP CONDITIONAL

		// I tested to see if this getline includes the new line character. Seems like it doesn't

		// Assign street segment's name as indicated
		StreetSegment newSeg;
		newSeg.streetName = tempString;



		// THEN LOAD SEGMENT'S START AND END GEO COORDS
		getline(ifs, tempString);



		// Parse geo coordinate line
		string startLat, startLon, endLat, endLon;

		int delimiterIndex = tempString.find_first_of(",");		// variable to keep track of index of certain delimiters
																// used to help with parsing
		startLat = tempString.substr(0, delimiterIndex);
		tempString = tempString.substr(delimiterIndex);
		delimiterIndex = tempString.find_first_not_of(", ");
		tempString = tempString.substr(delimiterIndex);

		delimiterIndex = tempString.find_first_of(", ");
		startLon = tempString.substr(0, delimiterIndex);
		tempString = tempString.substr(delimiterIndex);
		delimiterIndex = tempString.find_first_not_of(", ");
		tempString = tempString.substr(delimiterIndex);

		delimiterIndex = tempString.find_first_of(", ");
		endLat = tempString.substr(0, delimiterIndex);
		tempString = tempString.substr(delimiterIndex);
		delimiterIndex = tempString.find_first_not_of(", ");

		endLon = tempString.substr(delimiterIndex);

		// create a new GeoSegment object and assign it to this StreetSegment's appropriate member variable
		newSeg.segment = GeoSegment(GeoCoord(startLat, startLon), GeoCoord(endLat, endLon));



		// LOAD ATTRACTIONS

		// First, retrive the number of attractions in this segment
		getline(ifs, tempString);
		int numAttract = stoi(tempString);

		for (int i = 0; i < numAttract; i++) {
			getline(ifs, tempString);
			Attraction newAtt;

			// Retrieve this attraction's name
			delimiterIndex = tempString.find("|");
			newAtt.name = tempString.substr(0, delimiterIndex);
			tempString = tempString.substr(delimiterIndex);
			delimiterIndex = tempString.find_first_not_of("|");
			tempString = tempString.substr(delimiterIndex);

			// Retrieve this attraction's Geo Coords
			string attLat, attLon;

			delimiterIndex = tempString.find_first_of(", ");
			attLat = tempString.substr(0, delimiterIndex);
			tempString = tempString.substr(delimiterIndex);
			delimiterIndex = tempString.find_first_not_of(", ");
			attLon = tempString.substr(delimiterIndex);

			// Assign the Geo Coordinates
			newAtt.geocoordinates = GeoCoord(attLat, attLon);

			// Attach this attraction to the street segment
			newSeg.attractions.push_back(newAtt);

			// cerr << i << ": " << newAtt.name << " | " << newAtt.geocoordinates.latitudeText << " " << newAtt.geocoordinates.longitudeText << endl;
		}

		// Add this fully loaded street segment to the street segment vector
		m_streetSegs.push_back(newSeg);

	}

	// Return true upon successful loading of map file
	return true;
}

size_t MapLoaderImpl::getNumSegments() const
{
	return m_streetSegs.size();
}

bool MapLoaderImpl::getSegment(size_t segNum, StreetSegment &seg) const
{
	if (segNum < m_streetSegs.size()) {
		seg = m_streetSegs[segNum];
		return true;
	}

	return false;  // This compiles, but may not be correct
}

//******************** MapLoader functions ************************************

// These functions simply delegate to MapLoaderImpl's functions.
// You probably don't want to change any of this code.

MapLoader::MapLoader()
{
	m_impl = new MapLoaderImpl;
}

MapLoader::~MapLoader()
{
	delete m_impl;
}

bool MapLoader::load(string mapFile)
{
	return m_impl->load(mapFile);
}

size_t MapLoader::getNumSegments() const
{
	return m_impl->getNumSegments();
}

bool MapLoader::getSegment(size_t segNum, StreetSegment& seg) const
{
	return m_impl->getSegment(segNum, seg);
}
