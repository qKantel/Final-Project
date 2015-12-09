/*
*	file	Passenger.h
*	status	Complete
*/

#ifndef PASSENGER_H
#define PASSENGER_H

#include <string>
#include <vector>

using namespace std;

/*
*	This class is a container of a passenger in an airline reservation system.
*	Contains data pertinent for ReservationSystem
*/
class Passenger
{
	friend ostream &operator<<(ostream &, const Passenger &);

private:
	string firstName;
	string lastName;
	string Membership;
	size_t reservationNumber;
	int flightNumber;

public:
	// empty constructor
	Passenger();
	/*
	*	parameters
	*	- afirstName		first name of passenger
	*	- alastName			last name of passenger
	*	- aMembership		flight membership of passenger
	*	- areservationNumber	unique reservation number of passenger
	*	- aflightNumber		specific flight passenger is on
	*	info
	*	- initialize constructor
	*/
	Passenger(string afirstName, string alastName, string aMembership, size_t areservationNumber, int aflightNumber);

	////////////////////////////////////////////////////////////////////////////////////////////////
	//								Accessor functions
	////////////////////////////////////////////////////////////////////////////////////////////////
	/*
	*	return
	*	- string		first name of passenger
	*/
	string getFirstName() const { return firstName; }
	/*
	*	return
	*	- string		last name of passenger
	*/
	string getLastName() const { return lastName; }
	/*
	*	return
	*	- string		flight membership of passenger
	*/
	string getMembership() const { return Membership; }
	/*
	*	return
	*	- size_t		unique reservation number of passenger
	*/
	size_t getReservationNumber() const { return reservationNumber; }
	/*
	*	return
	*	- int			specific flight passenger is on
	*/
	int getFlightNumber() const { return flightNumber; }

	////////////////////////////////////////////////////////////////////////////////////////////////
	//								Modifier functions
	////////////////////////////////////////////////////////////////////////////////////////////////
	/*
	*	parameters
	*	- afirstName		first name of passenger
	*/
	void setFirstName(string afirstName) { firstName = afirstName; }
	/*
	*	parameters
	*	- alastName			last name of passenger
	*/
	void setLastName(string alastName) { lastName = alastName; }
	/*
	*	parameters
	*	- aMembership		flight membership of passenger
	*/
	void setMembership(string aMembership) { Membership = aMembership; }
	/*
	*	parameters
	*	- areservationNumber		unique reservation number of passenger
	*/
	void setReservationNumber(size_t areservationNumber) { reservationNumber = areservationNumber; }
	/*
	*	parameters
	*	- aflightNumber		specific flight passenger is on
	*/
	void setflightNumber(int aflightNumber) { flightNumber = aflightNumber; }
};

#endif