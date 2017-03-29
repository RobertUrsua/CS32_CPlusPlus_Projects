#include "provided.h"
#include "MyMap.h"
#include <iostream>
#include <string>
#include <vector>
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
	cout << "Adding Segments to map\n";

	SegmentMapper segMap;
	segMap.init(mapLoad);


	vector<StreetSegment> results;

	// Attempt to find known geo coords
	// Gayley Avenue
	results = segMap.getSegments(GeoCoord("34.0597400", "-118.4460477"));
	cout << results.size() << endl;

	for (auto i : results) {
		cout << i.streetName << endl;
	}


	// Attempt to find known geo coords
	// South Gretna Green Way
	results = segMap.getSegments(GeoCoord("34.0526400","-118.4759830"));
	cout << results.size() << endl;

	for (auto i : results) {
		cout << i.streetName << endl;
	}


	// Attempt to find known geo coords
	// 14th Helena Drive
	results = segMap.getSegments(GeoCoord("34.0558289", "-118.4798296"));
	cout << results.size() << endl;

	for (auto i : results) {
		cout << i.streetName << endl;
	}


	// Attempt to find known geo coords
	// 3 x 220 kV + Electrode Line of HVDC Pacific DC Intertie
	//	34.1019000, -118.4960442 34.0989151, -118.4966827
	results = segMap.getSegments(GeoCoord("34.1019000", "-118.4960442"));
	cout << results.size() << endl;

	for (auto i : results) {
		cout << i.streetName << endl;
	}


	// Attempt to find known geo coords
	// Thai House | 34.0610306, -118.4473883 in Gayley Avenue
	results = segMap.getSegments(GeoCoord("34.0610306", "-118.4473883"));
	cout << results.size() << endl;

	for (auto i : results) {
		cout << i.streetName << endl;
	}


	// Attempt to find unknown geo coords
	// TopSecretLocation | 55.0610306, -118.4473883
	results = segMap.getSegments(GeoCoord("55.0610306", "-118.4473883"));
	cout << results.size() << endl;

	for (auto i : results) {
		cout << i.streetName << endl;
	}


	// Attempt to find known geo coords
	//Whole Foods Market | 34.0612088 -118.4470689
	//At Gayley Avenue 34.0619693, -118.4479670 34.0607070,-118.4469116
	results = segMap.getSegments(GeoCoord("34.0612088", "-118.4470689"));
	cout << results.size() << endl;

	for (auto i : results) {
		cout << i.streetName << endl;
		cout << i.segment.start.latitudeText << " " << i.segment.start.longitudeText << " ";
		cout << i.segment.end.latitudeText << " " << i.segment.end.longitudeText << " " << endl;
	}

	cerr << "Passed all Tests" << endl;
	cerr << "Freeing data allocated to Maps" << endl;
}