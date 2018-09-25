#include "provided.h"
#include "MyMap.h"
#include "Support.h"
#include <string>
#include <vector>
#include <queue>
#include <iostream>
using namespace std;

string geoSegToDirection(const GeoSegment& line)
{
	string direction;
	// get the angle
	double angle = angleOfLine(line);
	// and translate
	if (angle >= 0 && angle <= 22.5)
		direction = "east";
	else if (angle > 22.5 && angle <= 67.5)
		direction = "northeast";
	else if (angle > 67.5 && angle <= 112.5)
		direction = "north";
	else if (angle > 112.5 && angle <= 157.5)
		direction = "northwest";
	else if (angle > 157.5 && angle <= 202.5)
		direction = "west";
	else if (angle > 202.5 && angle <= 247.5)
		direction = "southwest";
	else if (angle > 247.5 && angle <= 292.5)
		direction = "south";
	else if (angle > 292.5 && angle <= 337.5)
		direction = "southeast";
	else if (angle > 337.5 && angle <= 360)
		direction = "east";

	return direction;
}
string geoSegsToDirection(const GeoSegment& line1, const GeoSegment& line2)
{
	string direction;
	// get the angle
	double angle = angleBetween2Lines(line1, line2);
	// and translate
	if (angle < 180)
		direction = "left";
	else if (angle >= 180)
		direction = "right";

	return direction;
}

class NavigatorImpl
{
public:
	NavigatorImpl();
	~NavigatorImpl();
	bool loadMapData(string mapFile);
	NavResult navigate(string start, string end, vector<NavSegment>& directions) const;
	
private:
	MapLoader m_ml;
	AttractionMapper m_am;
	SegmentMapper m_sm;
};

NavigatorImpl::NavigatorImpl()
{
}

NavigatorImpl::~NavigatorImpl()
{
}

bool NavigatorImpl::loadMapData(string mapFile)
{
	// Load the maploader. If that fails, return false
	if (m_ml.load(mapFile) == false)
		return false;
	m_am.init(m_ml);  // Initialize the AttractionMapper with the loaded MapLoader
	m_sm.init(m_ml);  // Initialize the SegmentMapper with the loaded MapLoader

	//cerr << "load complete : " << endl << endl;
	return true;  // This compiles, but may not be correct
}

NavResult NavigatorImpl::navigate(string start, string end, vector<NavSegment> &directions) const
{
	
	GeoCoord startgc;
	GeoCoord endgc;
	// Check if the starting location exists
	if (m_am.getGeoCoord(start, startgc) == false)
	{
		//cerr << "Nav_BAD_SOURCE" << endl;
		return NAV_BAD_SOURCE;
	}
	//cerr << "startgc: " << startgc.latitude << " ";
	// Check if the ending location exists
	if (m_am.getGeoCoord(end, endgc) == false)
	{
		//cerr << "NAV_BAD_DESTINATION" << endl;
		return NAV_BAD_DESTINATION;
	}
	//cerr << "endgc: " << endgc.latitude << endl << endl;
	StreetSegment startStreet = m_sm.getSegments(startgc)[0];
	StreetSegment endStreet = m_sm.getSegments(endgc)[0];
	

	
	// Find the path
	queue<GeoCoord> gcQueue;
	MyMap<GeoCoord, prevPointAndStreet> prevPoints;
	// Enqueue the start and end coordinates of the start location's StreetSegment
	GeoCoord gc;
	m_am.getGeoCoord(start, gc);
	
	gcQueue.push(m_sm.getSegments(gc)[0].segment.start); // I'm assuming that each attraction
	gcQueue.push(m_sm.getSegments(gc)[0].segment.end);  // is only assoicated with one Street Segment
	// Push the aforementioned coordinates on our prevPoints map
	prevPointAndStreet pointAndStreet(gc, startStreet);
	prevPoints.associate(m_sm.getSegments(gc)[0].segment.start, pointAndStreet);
	prevPoints.associate(m_sm.getSegments(gc)[0].segment.end, pointAndStreet);
	// Process the points
	for(;;)
	{
		// If queue is empty, then we are out of ideas
		if (gcQueue.empty())
		{
			//cerr << "Nav_NO_ROUTE" << endl;
			return NAV_NO_ROUTE;  // This compiles, but may not be correct
		}

		// Acquire the vector of street segments associated with the top g-Coordinate
		vector<StreetSegment> tempv;
		GeoCoord frontgc = gcQueue.front();
		tempv = m_sm.getSegments(frontgc);
		gcQueue.pop();  // Then pop the top top g-Coordinate

		if (frontgc == endgc)  // If popped gc equals the destination, Success!
		{
			break;
		}
		////////////////////////////////////////////////////////////////////////////////
		// Create a priority queue that tracks which point is closest to the destination
		////////////////////////////////////////////////////////////////////////////////
		// Search vector for coordinates to place in the priority queue
		for (int i = 0; i < tempv.size(); i++)
		{
			// If streetSegment contains destination, push the destination
			if (tempv[i] == endStreet)
			{
				gcQueue.push(endgc);
				prevPointAndStreet lastPointAndStreet(frontgc, endStreet);
				prevPoints.associate(endgc, lastPointAndStreet);
				break;
			}

			// 
			if (prevPoints.find(tempv[i].segment.start) == nullptr)
			{
				gcQueue.push(tempv[i].segment.start);
				prevPointAndStreet pass(frontgc, tempv[i]);
				prevPoints.associate(tempv[i].segment.start, pass);
				//cerr << "pushed start: " << tempv[i].segment.start.latitude << endl;
			}

			if (prevPoints.find(tempv[i].segment.end) == nullptr)
			{
				gcQueue.push(tempv[i].segment.end);
				prevPointAndStreet pass(frontgc, tempv[i]);
				prevPoints.associate(tempv[i].segment.end, pass);
				//cerr << "pushed end: " << tempv[i].segment.end.latitude << endl;
			}
		}
	}
	
	GeoCoord hi = endgc;
	for (;;)
	{
		hi = (*(prevPoints.find(hi))).prevgc;

		if (hi == startgc)
			break;
	}

	// The path has been found. Now it is time to place it in our NavSegment vector
	vector<NavSegment> navi;
	std::string navssname = endStreet.streetName;
	GeoCoord curPoint = endgc;
	GeoCoord prevPoint = (*(prevPoints.find(endgc))).prevgc;
	GeoSegment navgs(prevPoint, curPoint);
	double navdistance = distanceEarthKM(endgc, prevPoint);
	string navdirection = geoSegToDirection(navgs);
	NavSegment intruction(navdirection, navssname, navdistance, navgs);
	navi.push_back(intruction);
	for (;;)
	{
		navssname = (*(prevPoints.find(curPoint))).prevss.streetName;
		curPoint = prevPoint;
		prevPoint = (*(prevPoints.find(curPoint))).prevgc;
		GeoSegment navgs2(prevPoint, curPoint);
		navdistance = distanceEarthKM(curPoint, prevPoint);
		navdirection = geoSegToDirection(navgs2);
		NavSegment instruction2(navdirection, navssname, navdistance, navgs2);

		// If current street is not the same name as previous street, first insert turn instruction
		if (navssname != navi.back().m_streetName)
		{
			string navssname2 = navi.back().m_streetName;
			string direction2 = geoSegsToDirection(navgs2, navi.back().m_geoSegment);

			NavSegment turnInstruction(navssname2, direction2);
			navi.push_back(turnInstruction);
		}

		navi.push_back(instruction2);
		if (prevPoint == startgc)
			break;
	}

	if (startStreet == endStreet)
	{
		navssname = startStreet.streetName;
		GeoSegment navgs2(startgc, endgc);
		navdistance = distanceEarthKM(startgc, endgc);
		navdirection = geoSegToDirection(navgs2);
		NavSegment hilo(navdirection, navssname, navdistance, navgs2);
		directions.push_back(hilo);
	}
	else
	{
		for (int i = navi.size() - 1; i >= 0; i--)
			directions.push_back(navi[i]);
	}


	//cerr << "Num Instructions: " << directions.size() << endl;

	/*for (int i = 0; i < directions.size(); i++)
	{
		cerr << i << ": " << directions[i].m_direction << ", ";
		cerr << directions[i].m_streetName << ", " << directions[i].m_distance << "km" << endl;
	}*/

	//cerr << "NAV_SUCCESS" << endl;
	return NAV_SUCCESS;
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
