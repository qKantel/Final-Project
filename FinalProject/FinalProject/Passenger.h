#ifndef PASSENGER_H
#define PASSENGER_H

#include <string>
#include <vector>

using namespace std;

class Passenger
{
	friend ostream &operator<<(ostream &, const Passenger &);
	friend istream &operator>>(istream&, Passenger& );
private:
	string firstName;
	string lastName;
	string Membership;
	size_t reservationNumber;
	int flightNumber;
public:
	
	Passenger();
	Passenger(string afirstName, string alastName, string aMembership, size_t areservationNumber, int aflightNumber);

	string getFirstName() const { return firstName; }
	string getLastName() const { return lastName; }
	string getMembership() const { return Membership; }
	size_t getReservationNumber() const { return reservationNumber; }
	int getFlightNumber() const { return flightNumber; }

	void setFirstName(string afirstName) { firstName = afirstName; }
	void setLastName(string alastName) { lastName = alastName; }
	void setMembership(string aMembership) { Membership = aMembership; }
	void setReservationNumber(int areservationNumber) { reservationNumber = areservationNumber; }
	void setflightNumber(int aflightNumber) { flightNumber = aflightNumber; }
};

#endif