#include "provided.h"
#include "MyMap.h"
#include "support.h"
#include <vector>
using namespace std;

class SegmentMapperImpl
{
public:
	SegmentMapperImpl();
	~SegmentMapperImpl();
	void init(const MapLoader& ml);
	vector<StreetSegment> getSegments(const GeoCoord& gc) const;

private:
	// Store a vector which contains all the loaded StreetSegments
	// so even if the mapLoader object is change, we still have a complete copy of Segments
	vector<StreetSegment> m_segVec;

	// This Map will contain stringified start point GeoCoords, end point GeoCoords, and attraction GeoCoords
	// as keys. The values associated to each key wilL be a vector.
	// Each vector will contain pointers corresponding to
	// StreetSegments with matching start point GeoCoords or end point GeoCoords.
	// The vectors will also include pointers to StreetSegments containing Attractions with
	// matching GeoCoords
	MyMap<string, vector<StreetSegment*>> m_segMap;

	// This function will associate the StreetSegment pointer with the indicated geoCoordinate
	// in the m_segMap
	void associateThisSegment(StreetSegment* addMe, GeoCoord coordToAssociate);

	// Turn a GeoCoordinate to a string 
	// which is a concatination of latitudeText and longitudeText
	// this string will be used as the key for the m_segMap
	string giveSegMapKey(GeoCoord hashMe) const {
		return hashMe.latitudeText + hashMe.longitudeText;
	}
};

SegmentMapperImpl::SegmentMapperImpl()
{
}

SegmentMapperImpl::~SegmentMapperImpl()
{
}

void SegmentMapperImpl::init(const MapLoader& ml)
{
	// Load all segments to the private member vector
	for (unsigned int ithSegment = 0; ithSegment < ml.getNumSegments(); ithSegment++) {
		StreetSegment tempSeg;
		ml.getSegment(ithSegment, tempSeg);
		m_segVec.push_back(tempSeg);
	}

	// Populate the Map of Vector of Segment pointers.
	for (size_t ithSegment = 0; ithSegment < m_segVec.size(); ithSegment++) {
		StreetSegment *tempSeg = &m_segVec[ithSegment];

		// associate this segment using its start GeoCoord
		associateThisSegment(tempSeg, tempSeg->segment.start);
		// associate this segment using its end GeoCoord
		associateThisSegment(tempSeg, tempSeg->segment.end);
		// associate this segment using its attractions' GeoCoords
		for (size_t ithAttraction = 0; ithAttraction < tempSeg->attractions.size(); ithAttraction++) {
			associateThisSegment(tempSeg, tempSeg->attractions[ithAttraction].geocoordinates);
		}

	}
}

void SegmentMapperImpl::associateThisSegment(StreetSegment* addMe, GeoCoord coordToAssociate) {

	auto latitudeSearchResult_streetVec = m_segMap.find(giveSegMapKey(coordToAssociate));

	// If no vector is associated with this StreetSegment's key, create a new one and add this segment
	if (latitudeSearchResult_streetVec == nullptr) {
		vector<StreetSegment*> tempVec;
		tempVec.push_back(addMe);
		m_segMap.associate(giveSegMapKey(coordToAssociate), tempVec);
	}
	// If it exists, just add this new StreetSegment
	else {
		latitudeSearchResult_streetVec->push_back(addMe);
	}
}

vector<StreetSegment> SegmentMapperImpl::getSegments(const GeoCoord& gc) const
{
	vector<StreetSegment> segmentsMatchingQuery;

	// Try to see if we can find a vector of StreetSegments associated
	// with the GeoCoord query
	auto vecSearchRes = m_segMap.find(giveSegMapKey(gc));

	// If we found a vector, linearly search that vector for Segments with
	// matching start/end GeoCoords, or segments which contain Attractions
	// with matching GeoCoords. The number of Segments associated
	// with exactly the same GeoCoord must be small enough to justify
	// using linear search
	if (vecSearchRes != nullptr) {
		auto currentStreetSeg = vecSearchRes->begin();

		while (currentStreetSeg != vecSearchRes->end()) {

			// First check if current Street Segment has matchin start coordinates
			if (areEqualCoords((*currentStreetSeg)->segment.start, gc))
				segmentsMatchingQuery.push_back(**currentStreetSeg);

			// Then check if current Street Segment has matchin end coordinates
			else if (areEqualCoords((*currentStreetSeg)->segment.end, gc))
				segmentsMatchingQuery.push_back(**currentStreetSeg);

			// Then check if current Street Segment contains Attractions with matchin coordinates
			else {
				for (size_t attsInThisSeg = 0; attsInThisSeg < (*currentStreetSeg)->attractions.size(); attsInThisSeg++) {
					if (areEqualCoords((*currentStreetSeg)->attractions[attsInThisSeg].geocoordinates, gc)) {
						segmentsMatchingQuery.push_back(**currentStreetSeg);
						break;
					}
				}
			}

			currentStreetSeg++;
		}

	}


	return segmentsMatchingQuery;  // This compiles, but may not be correct
}

//******************** SegmentMapper functions ********************************

// These functions simply delegate to SegmentMapperImpl's functions.
// You probably don't want to change any of this code.

SegmentMapper::SegmentMapper()
{
	m_impl = new SegmentMapperImpl;
}

SegmentMapper::~SegmentMapper()
{
	delete m_impl;
}

void SegmentMapper::init(const MapLoader& ml)
{
	m_impl->init(ml);
}

vector<StreetSegment> SegmentMapper::getSegments(const GeoCoord& gc) const
{
	return m_impl->getSegments(gc);
}
