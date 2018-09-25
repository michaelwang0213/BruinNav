#ifndef SUPPORT_H
#define SUPPORT_H

inline
bool operator== (const GeoCoord &gc, const GeoCoord &gc2)
{
	return (gc.latitude == gc2.latitude && gc.longitude == gc2.longitude);
}

inline
bool operator< (const GeoCoord &gc, const GeoCoord &gc2)
{
	if (gc.latitude < gc2.latitude)
		return true;
	else
		return (gc.longitude < gc2.longitude);
}

inline
bool operator> (const GeoCoord &gc, const GeoCoord &gc2)
{
	if (gc.latitude > gc2.latitude)
		return true;
	else
		return (gc.longitude > gc2.longitude);
}

struct prevPointAndStreet
{
	prevPointAndStreet()
	{

	}
	prevPointAndStreet(GeoCoord gc, StreetSegment ss)
	{
		prevgc = gc;
		prevss = ss;
	}

	GeoCoord prevgc;
	StreetSegment prevss;
};

/*
inline
bool operator== (const prevPointAndStreet &gc, const prevPointAndStreet &gc2)
{
	return (gc.latitude == gc2.latitude && gc.longitude == gc2.longitude);
}

inline
bool operator< (const prevPointAndStreet &gc, const prevPointAndStreet &gc2)
{
	if (gc.latitude < gc2.latitude)
		return true;
	else
		return (gc.longitude < gc2.longitude);
}

inline
bool operator> (const prevPointAndStreet &gc, const prevPointAndStreet &gc2)
{
	if (gc.latitude > gc2.latitude)
		return true;
	else
		return (gc.longitude > gc2.longitude);
}
*/
inline
bool operator== (const StreetSegment &ss, const StreetSegment &ss2)
{
	return (ss.streetName == ss2.streetName);
}
#endif