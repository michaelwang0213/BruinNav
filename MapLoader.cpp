#include "provided.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
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
	std::vector<StreetSegment> StreetSegments;
};

MapLoaderImpl::MapLoaderImpl()
{
}

MapLoaderImpl::~MapLoaderImpl()
{
	/*std::vector<StreetSegment>::iterator it;
	it = StreetSegments.begin();
	while (it != StreetSegments.end())
	{
		delete *it;
		it++;
	}*/
}

///////////////////////////////////////////////////////////////
// This function right now is a freakin joke. Unfortunate.
///////////////////////////////////////////////////////////////

//
// Make sure to return false in the correct situations.
//
bool MapLoaderImpl::load(string mapFile)
{
	
	ifstream infile(mapFile);    // infile is a name of our choosing
	if (!infile)		        // Did opening the file fail?
	{
		//cerr << "Error: Cannot open data.txt!" << endl;
		return false;
	}
	
	std::string streetname;
	int numSegments = 0;
	for (;;)
	{
		// The first line gives us the name of the StreetSegment.
		if (getline(infile, streetname))
		{
			StreetSegment s;
			StreetSegments.push_back(s);
			StreetSegments[numSegments].streetName = streetname;
		}
		else
			break;

		//cerr << "hi1: " << streetname << endl;
		//////////////////////////////////////////////////////////////////////
		// The following code looks really stupid but I don't know any better.
		// Viewer discretion is advised.
		//////////////////////////////////////////////////////////////////////

		// The next line gives us the start and end coordinates for the geoSegment
		string coordinates, startLat, startLon, endLat, endLon;
		getline(infile, coordinates);
		int pos = 0;
		while (coordinates[pos] != ',')
		{
			startLat += coordinates[pos];  // Define startLat
			pos++;
		}
		while (coordinates[pos] == ',' || coordinates[pos] == ' ')
			pos++;  // ignore characters until nect coordinate is ready to be initialized
		while (coordinates[pos] != ' ')
		{
			startLon += coordinates[pos];  // Define startLon
			pos++;
		}
		while (coordinates[pos] == ',' || coordinates[pos] == ' ')
			pos++;  // ignore characters until nect coordinate is ready to be initialized
		while (coordinates[pos] != ',')
		{
			endLat += coordinates[pos];  // Define endLat
			pos++;
		}
		while (coordinates[pos] == ',' || coordinates[pos] == ' ')
			pos++;  // ignore characters until nect coordinate is ready to be initialized
		while (pos != coordinates.length())
		{
			endLon += coordinates[pos];  // Define endLon
			pos++;
		}
		//cerr << "hi2: " << startLat << "   " << startLon << "   " << endLat << "   " << endLon << endl;

		// Create the GeoSegment from the acquired coordinates
		GeoCoord ssStart(startLat, startLon);
		GeoCoord ssEnd(endLat, endLon);
		GeoSegment ssgs(ssStart, ssEnd);
		StreetSegments[numSegments].segment = ssgs;
		
		
		// The next line tells us how many attractions there are
		int numAttractions;
		infile >> numAttractions;
		//cerr << "NumAttractions: " << numAttractions << endl;
		infile.ignore(10000, '\n');

		// Fill in StreetSegment's Attractions vector
		for (int i = 0; i < numAttractions; i++)
		{
			
			std::string attraction, aName(""), aLat(""), aLon("");
			getline(infile, attraction);
			//cerr << attraction << endl;
			
			int pos2 = 0;
			while (attraction[pos2] != '|')  // Form the attraction's name
			{
				aName += attraction[pos2];
				pos2++;
			}
			//cerr << aName << ":" << endl;
			pos2++;
			aLat = "";
			while (attraction[pos2] != ',') 
			{
				aLat += attraction[pos2];  // Define aLat
				pos2++;
			}
			//cerr << aLat << ":" << endl;
			while (attraction[pos2] == ',' || attraction[pos2] == ' ')
			{
				pos2++;
			}
			aLon = "";
			while (pos2 != attraction.length())
			{
				aLon += attraction[pos2];  // Define aLon
				pos2++;
			}
			//cerr << aLon << ":" << endl;
			//cerr << "hi3: " << endl;
			Attraction attract;  // Give the attraction an identy and add it
			attract.name = aName;
			//cerr << "hi3: " << aName << ":   :" << aLat << ":   :" << aLon << endl;
			GeoCoord aGc(aLat, aLon);
			attract.geocoordinates = aGc; 
			
			StreetSegments[numSegments].attractions.push_back(attract);
		}
		
		//cerr << "hi4 : Success!" << endl << endl;
		numSegments++;
		/*-118.4902543
			1
			Brentwood Country Mart | 34.0474215, -118.4901897
			26th Street
			34.0483083, -*/
	}
	//cerr << "Number of Segments is " << numSegments << endl;
	return true;
}

size_t MapLoaderImpl::getNumSegments() const
{
	return StreetSegments.size();
}

bool MapLoaderImpl::getSegment(size_t segNum, StreetSegment &seg) const
{
	// If segNum is out of bounds then failure
	if (segNum >= StreetSegments.size())
		return false;
	
	// Otherwise, success! Set the treet number and return
	seg = StreetSegments[segNum];
	return true;
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
