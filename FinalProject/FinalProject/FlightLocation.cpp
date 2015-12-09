/*
*	file		FlightLocation.cpp
*	status		Complete
*/

#include "FlightLocation.h"

FlightLocation::FlightLocation() : identifier(UNKNOWN)
{
	
} // end constructor

FlightLocation::FlightLocation(char newIdentifer) : identifier(newIdentifer)
{

} // end initializer constructor

FlightLocation& FlightLocation::addAdjacentLocation(FlightLocation& location, int distance)
{
	//Add the location to our adjacentLocations
	adjacentLocations.push_back(location.getIdentifier());
	distanceToLocation[location.getIdentifier()] = distance;

	//Add us to the other locations adjacentLocations
	location.adjacentLocations.push_back(this->getIdentifier());
	location.distanceToLocation[this->getIdentifier()] = distance;

	//return the location we added to allow for chaining
	return location;
}

int FlightLocation::getDistanceToLocation(char location)
{
	//this will probably throw an exception if that node is not an adjacent one
	return distanceToLocation.find(location)->second;
}


std::vector<char>::iterator FlightLocation::getAdjacencyIterator()
{
	return adjacentLocations.begin();
}

std::vector<char>::iterator FlightLocation::getIteratorEnd()
{
	return adjacentLocations.end();
}

char FlightLocation::getIdentifier() const
{
	return identifier;
}

FlightLocation& FlightLocation::operator=(const FlightLocation& other)
{
	identifier = other.identifier;

	adjacentLocations = other.adjacentLocations;
	distanceToLocation = other.distanceToLocation;

	return *this;
}

bool FlightLocation::operator==(const FlightLocation& other)
{
	return identifier == other.identifier;
}

bool FlightLocation::operator==(const char& other)
{
	return identifier == other;
}

bool FlightLocation::operator!=(const FlightLocation& other)
{
	return !((*this) == other);
}

bool FlightLocation::operator!=(const char& other)
{
	return !(identifier == other);
}
