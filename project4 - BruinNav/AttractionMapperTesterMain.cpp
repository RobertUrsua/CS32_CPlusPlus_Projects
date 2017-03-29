#include "provided.h"
#include "MyMap.h"
#include <iostream>
#include <string>
#include <cassert>
using namespace std;

int main()
{
	MapLoader mapLoad;

	// Test if loading mapdata.txt succeeds
	// This should succeed
	cout << "Attempting to load mapdata.txt...\n";
	assert(mapLoad.load("mapdata.txt"));
	cout << "Loading Successful!\n";
	cout << "Adding attractions to map \n";

	AttractionMapper attMap;
	attMap.init(mapLoad);

	GeoCoord result;

	// Try finding a known Attraction
	cerr << "Trying to find Capriotti's\n";
	assert(attMap.getGeoCoord("Capriotti's", result));
	cerr << result.latitudeText << " " << result.longitudeText << endl;
	assert(result.latitude == 34.0672744);
	assert(result.longitude == -118.4053818);


	// Try finding a known Attraction
	// Oppenheimer Tower|34.0583555, -118.4432201
	cerr << "Trying to find Oppenheimer Tower\n";
	assert(attMap.getGeoCoord("Oppenheimer Tower", result));
	cerr << result.latitudeText << " " << result.longitudeText << endl;
	assert(result.latitude == 34.0583555);
	assert(result.longitude == -118.4432201);



	// Try finding a known Attraction
	// The Coffee Bean & Tea Leaf_3 | 34.0627171, -118.4449675
	cerr << "Trying to find The Coffee Bean & Tea Leaf_3\n";
	assert(attMap.getGeoCoord("The Coffee Bean & Tea Leaf_3", result));
	cerr << result.latitudeText << " " << result.longitudeText << endl;
	assert(result.latitude == 34.0627171);
	assert(result.longitude == -118.4449675);

	// Try finding another known Attraction (with different capitalization)
	// Yves Saint Laurent | 34.0684106 -118.4019843
	cerr << "Trying to find YVeS SaiNt LaURent\n";
	assert(attMap.getGeoCoord("YVeS SaiNt LaURent", result));
	cerr << result.latitudeText << " " << result.longitudeText << endl;
	assert(result.latitude == 34.0684106);
	assert(result.longitude == -118.4019843);

	// Try finding an unknown Attraction
	// The Krusty Krab | DNE
	cerr << "Trying to find The Krusty Krab\n";
	assert(!attMap.getGeoCoord("The Krusty Krab", result));

	// Try finding an unknown Attraction
	// Yves Saint Laurent2 | DNE
	cerr << "Yves Saint Laurent2\n";
	assert(!attMap.getGeoCoord("Yves Saint Laurent2", result));
	
	
	cerr << "Passed all Tests" << endl;
	cerr << "Freeing data allocated to Maps" << endl;
}