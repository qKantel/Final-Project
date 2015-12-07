#include <ctime>
#include <deque>
#include <iostream>
#include <map>
#include <string>
#include <vector>
using namespace std;

#include "NotFoundException.h"

#ifndef _FLIGHT
#define _FLIGHT


class Flight
{
	friend ostream& operator<<(ostream& out, const Flight& flights);

private:
	deque<size_t> pilotClub,		// Holds passenger according to their membership
		firstClass,
		businessClass,
		economy,
		waiting;		// Holds overbooked passengers

	map<size_t, string> manifest;  // Holds total passengers, including those waiting to board

	size_t flightNumber;
	size_t mileage;

	string fromCity;
	string toCity;

	time_t currentTime;  // Used to get the current time
	tm departureTime;
	tm arrivalTime;

	bool addPilotClub(const size_t& reservation, bool& ableToAdd, size_t& reassign);
	bool addFirstClass(const size_t& reservation, bool& ableToAdd, size_t& reassign);
	bool addBusinessClass(const size_t& reservation, bool& ableToAdd, size_t& reassign);
	bool addEconomy(const size_t& reservation, bool& ableToAdd, size_t& reassign);


public:
	Flight();
	Flight(const size_t& flightNumber, const size_t& departTime, const size_t& arriveTime, const size_t& mileage, const string& fromCity, const string& toCity);

	/** Add flight number, departure & arrival times, origin & destination cities. */
	void addFlightNumber(const size_t& newFlightNumber) { flightNumber = newFlightNumber; }
	void addOrigin(const string& from) { fromCity = from; }
	void addDestination(const string& to) { toCity = to; }
	void addDepartTime(const size_t& departureTime);
	void addArriveTime(const size_t& arrivalTime);

	/** Total distance between departure and arrival city. */
	void addMileage(const size_t& mileage) { this->mileage = mileage; };

	/** Adds passenger to manifest using reservation number.
	@pre  Max capacity of 40 passengers.  Up to 10 seats can be used for higher ranking passengers
	(while reassiging lower ranked passengers).
	@return true  If passengers (seated + waiting) <= 50
	false When waitlist >= 10  (can only displace 10 passengers)	*/
	bool addPassenger(const size_t& reservation, const string& membership);

	/** Retrieves flight data. */
	size_t getFlightNumber() const { return flightNumber; }
	tm getDepartTime() const { return departureTime; }
	tm getArriveTime() const { return arrivalTime; }
	size_t getMileage() const { return mileage; }
	string getOrigin() const { return fromCity; }
	string getDestination() const { return toCity; }

	/** Returns a list of all passengers (reservation number) aboard flight */
	vector<size_t> getManifest() const;

	/** Returns passenger reservation number if on flight.
	@throw NotFoundException if the passenger is not on the flight. */
	void findPassenger(const size_t& reservation) const;

	/** Removes passenger from manifest.  */
	void removePassenger(const size_t& reservation);
};

#endif