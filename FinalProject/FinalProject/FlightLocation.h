/*
*	file		FlightLocation.h
*	status		Complete
*/

#include <vector>
#include <map>

/*
*	Resembles a city for use in FlightMap.
*	The city has a name and other cities that an airplane can fly to.
*	The class also holds the distance to the adjacent cities.
*/
class FlightLocation
{
private:
	static const char UNKNOWN = '2';
	//The identifier for this location
	char identifier;

	//a vector holding all the adjacent locations and the distance to them
	//std::vector<Pair<FlightLocation, int>> adjacencyMap;
	std::vector<char> adjacentLocations;
	std::map<char,int> distanceToLocation;


public:
	// Constructor
	FlightLocation();

	/*
	*	parameters
	*	- identifier		Name of the city
	*	info
	*	- initialize constructor
	*/
	FlightLocation(char identifer); //this should initially set the fscore to -1 (which we are using to represent infinity)
	
	/*
	*	parameters
	*	- location		The FlightLocation of an adjacent city
	*	- distance		Distance between the calling city and location
	*	return
	*	- FlightLocation&		Reference to the flight location that was just added
	*/
	FlightLocation& addAdjacentLocation(FlightLocation& location, int distance);

	//gets the start of the iterator for iterating through all locations
	std::vector<char>::iterator getAdjacencyIterator();
	std::vector<char>::iterator getIteratorEnd();

	/*
	*	parameters
	*	- location		end location
	*	return
	*	- int			distance between calling object and end location
	*/
	int getDistanceToLocation(char location);

	//returns the identifer of this location
	char getIdentifier() const;

	// Assignment Operator
	FlightLocation& operator=(const FlightLocation& other);

	// Equivalence operators
	bool operator==(const FlightLocation& other);
	bool operator==(const char& other);
	bool operator!=(const FlightLocation& other);
	bool operator!=(const char& other);
};