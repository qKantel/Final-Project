/*
*	file	FlightMap.h
*	status	Complete
*/

#include "FlightLocation.h"
#include <map>

/*
*	FlightMap contains a list of locations and creates a graph.
*	This graph is the possible routes an airplane can take.
*/
class FlightMap
{
private:
	std::map<char,FlightLocation> locations;

public:
	// Constructor sets up locations, distances, cities.
	FlightMap();

	/*
	*	parameters
	*	- startIdentifier		name of starting city to compute distance
	*	- endIdentifier			name of ending city for distance computing
	*	return
	*	- int					the minimum distance from city to city based on a graph
	*	info
	*	- Computes the minimum distance it takes for an airplane to reach from one city to another.
	*/
	int getMinDistance(char startIdentifier, char endIdentifier);
};