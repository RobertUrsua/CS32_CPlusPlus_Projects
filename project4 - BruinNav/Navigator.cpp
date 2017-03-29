#include "provided.h"
#include "MyMap.h"
#include "support.h"
#include <string>
#include <vector>
#include <list>
#include <queue>
using namespace std;

class NavigatorImpl
{
public:
	NavigatorImpl();
	~NavigatorImpl();
	bool loadMapData(string mapFile);
	NavResult navigate(string start, string end, vector<NavSegment>& directions) const;

private:
	MapLoader m_mapLoad;
	AttractionMapper m_attMap;
	SegmentMapper m_segMap;

	// Perfomrs A* path finding algorithm to find the shortest path
	// between the locations pointed to be the start and end GeoCoord parameters
	// Also modifies a passed shortestPath vector so that if a route was found,
	// the vector will contain a string of GeoCoordss that need to be
	// travered to get from 'start' to 'end' (it doesn't include start/end)
	// This function will return true if a path is found, and false otherwise
	bool performAStar(GeoCoord start, GeoCoord end, list<GeoCoord> &shortestPath) const;


	// Helper class for A* algorithm
	// These nodes will represent geo coordinates for the A star to work on
	// it has the proper A star values (G H and F values)
	// and has a parent member function as indicated in the algorithm
	class aStarNode {
	public:
		aStarNode() {};
		// constructor, sets g h and f values
		aStarNode(double g, double h, GeoCoord myGeoCoord, GeoCoord parentCoord)
			:_g(g), _h(h), _f(g + h), _me(myGeoCoord), _parent(parentCoord) {}

		// re initialization function, assigns new g h and f values
		void reInit(double g) {
			_g = g;
			_f = g + _h;
		}

		double g() { return _g; }
		double h() { return _h; }
		double f() { return _f; }

		double getF() const { return _f; }
		double getG() const { return _g; }
		double getH() const { return _h; }
		void setParent(GeoCoord newP) { _parent = newP; }
		GeoCoord getParentsCoord() const { return _parent; }
		GeoCoord getMyCoord() const { return _me; }

	private:
		double _g;	// G value (movement cost or distance from start point)
		double _h;	// H value (distance from end point)
		double _f;	// F value (G+H)
		GeoCoord _me; // GeoCoord corresponding to this node
		GeoCoord _parent;	// GeoCoord from which this GeoCoord was reached
	};

	// Class for comparing a star nodes, used for keeping the priority queue used in A star
	// a node has higher priority if it has lower F value
	class compAStar {
	public:
		bool operator()(const aStarNode l, const aStarNode r) {
			return l.getF() > r.getF();
		}
	};



	// helper functions for A* algorithm
	// the a star algorithm uses maps to keep track of nodes
	// this function return the key in the desired format
	string getAStarMapKey(aStarNode x) const {
		return x.getMyCoord().latitudeText + x.getMyCoord().longitudeText;
	}
	string getAStarMapKey(GeoCoord x) const {
		return x.latitudeText + x.longitudeText;
	}

	// Given a string (list) of geoCoords representing a valid path from start to end,
	// generate the directions required to get from start to end
	void generateNavSegs(vector<NavSegment> &directions, list<GeoCoord> shortestPath) const;

	// returns the StreetSegment related to BOTH GeoCoords one and two
	// should be unique.
	StreetSegment getSpecificSeg(const GeoCoord& one, const  GeoCoord& two) const;

	// Given a geosegment with start and end points,
	// return the direction to travel to get from start to end
	string directionOfTravel(const GeoSegment& travelThroughMe) const;


};

NavigatorImpl::NavigatorImpl()
{
}

NavigatorImpl::~NavigatorImpl()
{
}

bool NavigatorImpl::loadMapData(string mapFile)
{
	// Try loading the mapFile using mapLoader private member
	// cerr << "Attempting to load " << mapFile << " ...\n";

	if (!m_mapLoad.load(mapFile))
		return false;

	// cerr << "Successfully loaded " << mapFile << endl;
	// cerr << "Attempting to load Attractions to AttractionMapper ... \n";

	m_attMap.init(m_mapLoad);

	// cerr << "Success\n";
	// cerr << "Attempting to load StreetSegments to SegmentMapper ... \n";

	m_segMap.init(m_mapLoad);
	// cerr << "Success\n";



	return true;  // This compiles, but may not be correct
}

NavResult NavigatorImpl::navigate(string start, string end, vector<NavSegment> &directions) const
{
	GeoCoord startCoord;
	GeoCoord endCoord;

	NavigatorImpl::aStarNode a;
	// Clear directions vector
	directions.clear();

	// If attraction mapper can't find starting attraction
	if (!m_attMap.getGeoCoord(start, startCoord))
		return NAV_BAD_SOURCE;

	// If attraction mapper can't find destination attraction
	if (!m_attMap.getGeoCoord(end, endCoord))
		return NAV_BAD_DESTINATION;

	list<GeoCoord> shortestPathGeoSegs;

	// Attempt to find a path using a star
	if (!performAStar(startCoord, endCoord, shortestPathGeoSegs))
		return NAV_NO_ROUTE;

	generateNavSegs(directions, shortestPathGeoSegs);

	return NAV_SUCCESS;  // This compiles, but may not be correct
}


bool NavigatorImpl::performAStar(GeoCoord start, GeoCoord end, list<GeoCoord> &shortestPath) const {

	// SETUP ACTIONS FOR A STAR

	// Create a priority queue to establish the order in which we're
	// going to inspecs the nodes 
	priority_queue<aStarNode, vector<aStarNode>, compAStar> nodesToVisit;

	// keeps track of already visited aStarNodes.
	// only one aStarNode must correspond to any unique geoCoord
	// key used will be latitude + longitude in string format in this order
	// value will be the aStarNode object
	MyMap<string, aStarNode> visitedNodes;

	// since there are could duplicate nodes in the priority queue
	// we need to keep track of how many unique GeoCoords / nodes
	// there are so we can know if we've visited all unique nodes
	// the stringified geoCoord will be used as key. The value doesn't really matter
	MyMap<string, int> uniqueGeoCoords;

	// Determine the street segments to which the start/end attractions belong to
	vector<StreetSegment> potentialHomeSegments = m_segMap.getSegments(start);
	vector<StreetSegment> potentialEndSegments = m_segMap.getSegments(end);

	// if multiple home / end segments are associated with an attraction, only pick the first
	StreetSegment homeSegment = potentialHomeSegments[0];
	StreetSegment endSegment = potentialEndSegments[0];

	// If home and end segment are the same, the starting and ending 
	// attractions are in the same streetsegment! just add the start and
	// end geocoords and return
	if (areEqualCoords(homeSegment.segment.start, endSegment.segment.start) &&
		areEqualCoords(homeSegment.segment.end, endSegment.segment.end)) {
		shortestPath.push_front(start);
		shortestPath.push_back(end);
		return true;
	}

	// Create a node for the two possible starting points
	// (Either start from the home segment's 'start' GeoCoord or from it's 'end' GeoCoord
	aStarNode homeSegmentStartCoord(
		distanceEarthMiles(start, homeSegment.segment.start),
		distanceEarthMiles(end, homeSegment.segment.start),
		homeSegment.segment.start, start
	);

	aStarNode homeSegmentEndCoord(
		distanceEarthMiles(start, homeSegment.segment.end),
		distanceEarthMiles(end, homeSegment.segment.end),
		homeSegment.segment.end, start
	);

	// Add the starting nodes to the open list and existing nodes map
	nodesToVisit.push(homeSegmentStartCoord);
	nodesToVisit.push(homeSegmentEndCoord);
	uniqueGeoCoords.associate(getAStarMapKey(homeSegmentStartCoord), 0);
	uniqueGeoCoords.associate(getAStarMapKey(homeSegmentEndCoord), 0);

	// "RECURSIVE" ACTIONS FOR A STAR

	while (visitedNodes.size() < uniqueGeoCoords.size()) {
		// Retrieve first unvisited node from priority queue and pop it
		aStarNode curNode = nodesToVisit.top();
		nodesToVisit.pop();

		// If this node has been visited already, go to the next node in priority queue
		if (visitedNodes.find(getAStarMapKey(curNode))!=nullptr)
			continue;

		// mark this node as visited
		visitedNodes.associate(getAStarMapKey(curNode), curNode);

		// If we've just visited the destination GeoCoord, we're done!
		if (areEqualCoords(curNode.getMyCoord(), end))
			break;

		// Find all connected Nodes/GeoCoords (connected via StreetSegs)
		vector<StreetSegment> cnnctedSegs = m_segMap.getSegments(curNode.getMyCoord());

		// Update F values for all neighbors
		// add unvisited neighbors to nodesToVisit / open set
		for (auto i = cnnctedSegs.begin(); i != cnnctedSegs.end(); i++)
		{
			// If we're currently in one point in a streetsegment,
			// Make sure we're checking the OPPOSITE end point
			// of that StreetSegment (the end point that's different from
			// the currect GeoCoord / aStarNode we're inspecting )
			GeoCoord otherEndPoint;

			if (areEqualCoords((i->segment.start), curNode.getMyCoord()))
				otherEndPoint = i->segment.end;
			else
				otherEndPoint = i->segment.start;


			// Check if this geocoord already has already been visited
			//		if yes, skip to next connected geoCoord
			// if it hasn't been visited, attempt to update its F value.
			// and create a new aStarNode for it and add it to the 
			// nodesToVisit priority queue (duplicate nodes are ok, because
			// the node with the lower F value will be visited first anyway)
			aStarNode* currentNeighbor = visitedNodes.find(getAStarMapKey(otherEndPoint));

			if (currentNeighbor == nullptr) {

				// create a new aStarNode for it and add it to the 
				// nodesToVisit priority queue
				aStarNode newNodeForThisEndPoint(
					// G value at this new node is G value at current node + distance between the two
					curNode.getG() + distanceEarthMiles(curNode.getMyCoord(), otherEndPoint),
					// H value is straight line distance to end point
					distanceEarthMiles(end, otherEndPoint),
					otherEndPoint, curNode.getMyCoord()
				);

				nodesToVisit.push(newNodeForThisEndPoint);

				// if this node is a new unique node, add it to the unique node book keeping map
				if (uniqueGeoCoords.find(getAStarMapKey(newNodeForThisEndPoint)) == nullptr)
					uniqueGeoCoords.associate(getAStarMapKey(newNodeForThisEndPoint), 0);
			}

		}

	}


	// NOW THAT WE PROCESSED ALL NODES:
	// Find the end node or the GeoSegment in which the end attraction is located
	// then trace back from that geosegment to reach the start point using 
	// the aStarNodes getParentsCoord function


	// Look at the end points of the geosegment in which the end ttraction is located
	// compare the G values to determine which path is optimal
	aStarNode* destinationSegmentEndPoint = visitedNodes.find(getAStarMapKey(endSegment.segment.end));
	aStarNode* destinationSegmentStartPoint = visitedNodes.find(getAStarMapKey(endSegment.segment.start));
	aStarNode* optimalEndPoint;

	// If the end attraction doesn't have an A star node created for it,
	// then it means it was never reached! so a path to it does not exist
	if (destinationSegmentEndPoint == nullptr
		&& destinationSegmentStartPoint == nullptr)
		return false;

	// If it was reached, we chose to start back tracking from the endpoint with lower g value
	if (destinationSegmentEndPoint == nullptr)
		optimalEndPoint = destinationSegmentStartPoint;
	if (destinationSegmentStartPoint == nullptr)
		optimalEndPoint = destinationSegmentEndPoint;
	if (destinationSegmentEndPoint->getG() < destinationSegmentStartPoint->getG())
		optimalEndPoint = destinationSegmentEndPoint;
	else
		optimalEndPoint = destinationSegmentStartPoint;

	while (optimalEndPoint != nullptr)
	{
		shortestPath.push_front(optimalEndPoint->getMyCoord());

		// if we found our way back into the start node we're done!
		if (areEqualCoords(optimalEndPoint->getMyCoord(), homeSegmentStartCoord.getMyCoord())
			|| areEqualCoords(optimalEndPoint->getMyCoord(), homeSegmentEndCoord.getMyCoord())) {

			// just add the start and  end GeoCoords,
			shortestPath.push_front(start);
			shortestPath.push_back(end);

			return true;
		}

		// if we haven't found our way back yet, we attempt tracing back towards the start point by
		// repeatedly going back to the "parent" of this current node we're processing
		aStarNode* potNextNode = visitedNodes.find(getAStarMapKey(optimalEndPoint->getParentsCoord()));
		*optimalEndPoint = *potNextNode;
	}

	// if we somehow reach this point just return false
	return false;
}


void NavigatorImpl::generateNavSegs(vector<NavSegment> &directions, list<GeoCoord> shortestPath) const
{


	string PreviousStreetName = "";

	// otherwise, take paris of geocoords at a time and make a nav segment
	// for each one
	// if possible, check if the previous navSegment has a different name
	// if so, add a TURN style navsegment first before adding this new nav segment
	auto tempStart = shortestPath.begin();
	auto tempEnd = shortestPath.begin();
	tempEnd++;
	for (; tempEnd != shortestPath.end(); tempStart++, tempEnd++) {

		StreetSegment containingStartAndEnd = getSpecificSeg(*tempStart, *tempEnd);
		GeoSegment fromStartToEnd(*tempStart, *tempEnd);

		NavSegment result(
			directionOfTravel(fromStartToEnd),
			containingStartAndEnd.streetName,
			distanceEarthMiles(*tempStart, *tempEnd),
			fromStartToEnd
		);

		// If we turned into a street, add a turn style segment first
		if (PreviousStreetName != "" && PreviousStreetName != result.m_streetName) {

			double angleOfSource = angleOfLine(directions.back().m_geoSegment);
			double angleOfDest = angleOfLine(fromStartToEnd);
			double difference;

			if (angleOfDest > angleOfSource) {
				difference = angleOfDest - angleOfSource;
				string turnDirection = difference < 180 ? "left" : "right";

				NavSegment turnSeg(turnDirection, result.m_streetName);
				directions.push_back(turnSeg);
			}
			else {
				difference = angleOfSource - angleOfDest;
				string turnDirection = difference < 180 ? "right" : "left";

				NavSegment turnSeg(turnDirection, result.m_streetName);
				directions.push_back(turnSeg);

			}

		}

		PreviousStreetName = containingStartAndEnd.streetName;

		directions.push_back(result);
	}
}


StreetSegment NavigatorImpl::getSpecificSeg(const GeoCoord& one, const  GeoCoord& two) const {
	// Determine the street segments to which the each GeoCoord belong to
	vector<StreetSegment> potentialSegsOne = m_segMap.getSegments(one);
	vector<StreetSegment> potentialSegsTwo = m_segMap.getSegments(two);

	// If a pair of segments from either vector is similar, then
	// that segment is related to both GeoCoord one and two and we return it!
	for (size_t i = 0; i < potentialSegsOne.size(); i++) {
		for (size_t j = 0; j < potentialSegsTwo.size(); j++) {
			if (areEqualCoords(potentialSegsOne[i].segment.start, potentialSegsTwo[j].segment.start) &&
				areEqualCoords(potentialSegsOne[i].segment.end, potentialSegsTwo[j].segment.end)) {
				return potentialSegsOne[i];
			}
		}
	}
}

string NavigatorImpl::directionOfTravel(const GeoSegment& travelThroughMe) const {
	double angle = angleOfLine(travelThroughMe);


	if (angle <= 22.5) return "east";
	if (angle <= 67.5) return "northeast";
	if (angle <= 112.5) return "north";
	if (angle <= 157.5) return "northwest";
	if (angle <= 202.5) return "west";
	if (angle <= 247.5) return "southwest";
	if (angle <= 292.5) return "south";
	if (angle <= 337.5) return "southeast";
	if (angle <= 360.5) return "east";
	// if we somehow got here, something's wrong
	return "ERROR";
}

//******************** Navigator functions ************************************

// These functions simply delegate to NavigatorImpl's functions.
// You probably don't want to change any of this code.

Navigator::Navigator()
{
	m_impl = new NavigatorImpl;
}

Navigator::~Navigator()
{
	delete m_impl;
}

bool Navigator::loadMapData(string mapFile)
{
	return m_impl->loadMapData(mapFile);
}

NavResult Navigator::navigate(string start, string end, vector<NavSegment>& directions) const
{
	return m_impl->navigate(start, end, directions);
}
