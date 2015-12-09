/*
*	file	Flight.h
*	status	Complete
*/

#include <ctime>
#include <deque>
#include <iostream>
#include <map>
#include <string>
#include <vector>
using namespace std;

#ifndef _FLIGHT
#define _FLIGHT

/*
*	A Flight object in use with the reservation system.
*	Contains information pertinent to the ReservationSystem.
*/
class Flight
{
	friend ostream& operator<<(ostream& out, const Flight& flights);

private:
	deque<size_t> pilotClub,		// Holds passenger (reservation #) according to their membership
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

	bool remove(deque<size_t>& passengers, const size_t& reservation);

public:
	Flight();
	Flight(const size_t& flightNumber, const size_t& departTime, const size_t& arriveTime, const size_t& mileage, const string& fromCity, const string& toCity);

	/** Add flight number, departure & arrival times, origin & destination cities. */
	void addFlightNumber(const size_t& newFlightNumber) { flightNumber = newFlightNumber; }
	void addOrigin(const string& from) { fromCity = from; }
	void addDestination(const string& to) { toCity = to; }
	void addDepartTime(const size_t& departureTime);
	void addArriveTime(const size_t& arrivalTime);

	bool hasFlightLeft(tm atTime);

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
	
	/** Returns a list of overbooked passenger's reservation numbers */
	deque<size_t> getOverBooked() const;

	/** Removes passenger from manifest. 
	@pre  Flight has not yet departed  
	@return true  If passenger is on flight
			false If passenger is not on flight */
	bool removePassenger(const size_t& reservation);
};

#endif