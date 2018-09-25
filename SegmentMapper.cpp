#include "provided.h"
#include "MyMap.h"
#include "Support.h"
#include <vector>
#include <iostream>
using namespace std;

class SegmentMapperImpl
{
public:
	SegmentMapperImpl();
	~SegmentMapperImpl();
	void init(const MapLoader& ml);
	vector<StreetSegment> getSegments(const GeoCoord& gc) const;
private:
	MyMap < GeoCoord, vector<StreetSegment>> m_map;
	// Removes smelly code
	void initPush(GeoCoord gc, StreetSegment segment)
	{
		// If the coordinate has no entry, create one
		if (m_map.find(gc) == nullptr)
		{
			vector<StreetSegment> myVec;
			myVec.push_back(segment);
			m_map.associate(gc, myVec);
		}		
		
		// If the coordinate already has an entry, add on to it
		else
		{
			vector<StreetSegment> *myVecPtr = (m_map.find(gc));
			(*myVecPtr).push_back(segment);
		}
		//cerr << m_map.find(gc)->size();
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
	//cerr << "init: " << endl;
	// Iterate through each StreetSegment
	for (int i = 0; i < ml.getNumSegments(); i++)
	{
		StreetSegment seg;
		ml.getSegment(i, seg);
		// Push the current street Segment's start
		initPush(seg.segment.start, seg);
		// Push the current street Segment's end
		initPush(seg.segment.end, seg);
		// Iterate through each of the streetSegment's attractions and push them
		for (int j = 0; j < seg.attractions.size(); j++)
			initPush(seg.attractions[j].geocoordinates, seg);
		
	}
	
}

//allows the getSegments() method to
//quickly find all StreetSegments that are associated with the specified geocoordinate.The
//getSegments() method returns a vector containing all those StreetSegment; if there are no
//StreetSegments associated with that geocoordinate, the returned vector will be empty.
vector<StreetSegment> SegmentMapperImpl::getSegments(const GeoCoord& gc) const
{
	vector<StreetSegment> segments;

	// If coordinate is found, set segments equal to GC's vector
	// If not found, leave segments empty
	if (m_map.find(gc) != nullptr)
	{
		segments = *(m_map.find(gc));
	}
	return segments;  // This compiles, but may not be correct
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
