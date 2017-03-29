#include "provided.h"
#include "MyMap.h"
#include <iostream>
#include <cassert>

using namespace std;

int main()
{
	MapLoader m1;

	// Test if load() returns correct value

	// This should fail
	MapLoader m2;
	assert(!m2.load("ThisFileMustNotBeFound.txt"));

	// This should succeed
	cout << "Attempting to load mapdata.txt...\n";
	assert(m1.load("mapdata.txt"));
	cout << "Loading Successful!\n";

	cout << m1.getNumSegments() << endl;
	StreetSegment temp;

	// Test getSegment for indices within bound
	assert(m1.getSegment(0, temp));
	cout << temp.streetName << " " << temp.segment.start.latitudeText << " " << temp.segment.start.longitudeText;
	cout << " " << temp.segment.end.latitudeText << " " << temp.segment.end.longitudeText << endl;

	assert(temp.streetName == "10th Helena Drive");
	assert(temp.segment.start.latitudeText == "34.0547000");
	assert(temp.segment.start.longitudeText == "-118.4794734");
	assert(temp.segment.end.latitudeText == "34.0544590");
	assert(temp.segment.end.longitudeText == "-118.4801137");
	
	assert(m1.getSegment(19640, temp));
	cout << temp.streetName << " " << temp.segment.start.latitudeText << " " << temp.segment.start.longitudeText;
	cout << " " << temp.segment.end.latitudeText << " " << temp.segment.end.longitudeText << endl;

	assert(temp.streetName == "access road for water tank");
	assert(temp.segment.start.latitudeText == "34.0853742");
	assert(temp.segment.start.longitudeText == "-118.4955413");
	assert(temp.segment.end.latitudeText == "34.0852898");
	assert(temp.segment.end.longitudeText == "-118.4954341");

	// test getSegment for indices OUT of bounds
	assert(!m1.getSegment(-1, temp));
	assert(!m1.getSegment(20000, temp));

	cout << "All test cases passed" << endl;

	int x;
	cin >> x;
}
