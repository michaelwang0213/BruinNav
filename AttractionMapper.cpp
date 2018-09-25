#include "provided.h"
#include "MyMap.h"
#include <string>
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
	MyMap<std::string, GeoCoord> m_aTGc;
};

AttractionMapperImpl::AttractionMapperImpl()
{
}

AttractionMapperImpl::~AttractionMapperImpl()
{
}

void AttractionMapperImpl::init(const MapLoader& ml)
{
	// Iterate through each StreetSegment
	for (int i = 0; i < ml.getNumSegments(); i++)
	{
		StreetSegment seg;
		ml.getSegment(i, seg);
		//cerr << seg.streetName << endl;
		//cerr << seg.attractions.size() << endl;
		// Iterate through each attraction in current StreetSegment
		for (int j = 0; j < seg.attractions.size(); j++)
		{
			// Get attraction's name and to-lower it for convenience
			std::string s = seg.attractions[j].name;
			for (int k = 0; k < s.length(); k++)
				s[k] = tolower(s[k]);
			// Push the data onto MyMap
			//cerr << s;
			m_aTGc.associate(s, seg.attractions[j].geocoordinates);
			//cerr << seg.attractions[j].geocoordinates.latitudeText;
		}
	}
}

bool AttractionMapperImpl::getGeoCoord(string attraction, GeoCoord& gc) const
{
	// To lower the given search term for convenience
	string s = attraction;
	for (int i = 0; i < s.length(); i++)
		s[i] = tolower(s[i]);
	
	// Search MyMap for the search term. if found, return the GeoCoordinate. Declare success.
	if (m_aTGc.find(s) != nullptr)
	{
		gc = *(m_aTGc.find(s));
		return true;
	}

	return false;
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
