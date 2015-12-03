#include <string>
#include <deque>
#include <map>
#include <iostream>
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
	size_t departTime;
	size_t arriveTime;
	size_t mileage;

	string fromCity;
	string toCity;

	bool addPilotClub(const size_t& reservation, bool& ableToAdd, size_t& reassign);
	bool addFirstClass(const size_t& reservation, bool& ableToAdd, size_t& reassign);
	bool addBusinessClass(const size_t& reservation, bool& ableToAdd, size_t& reassign);
	bool addEconomy(const size_t& reservation, bool& ableToAdd, size_t& reassign);


public:
	Flight() : flightNumber(0), departTime(0), arriveTime(0), mileage(0), fromCity(""), toCity("") {}
	Flight(const size_t& flightNumber, const size_t& departTime, const size_t& arriveTime, const size_t& mileage, const string& fromCity, const string& toCity);

	/** Add flight number, departure & arrival times, origin & destination cities. */
	void addFlightNumber(const size_t& newFlightNumber) { flightNumber = newFlightNumber; }
	void addDepartTime(const size_t& departureTime) { departTime = departureTime; }
	void addArriveTime(const size_t& arrivalTime) { arriveTime = arrivalTime; }
	void addOrigin(const string& from) { fromCity = from; }
	void addDestination(const string& to) { toCity = to; }

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
	size_t getDepartTime() const { return departTime; }
	size_t getArriveTime() const { return arriveTime; }
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


//==============================
//		Implementation
//==============================

//==============================
//		Friend Methods
//==============================

ostream& operator<<(ostream& out, const Flight& flights)
{
	out << "Flight Number: " << flights.getFlightNumber();
	out << "\nDeparture Time: " << flights.getDepartTime();
	out << "\nArrival Time: " << flights.getArriveTime();
	out << "\nTotal Mileage: " << flights.getMileage();
	out << "\nDeparting From: " << flights.getOrigin();
	out << "\nDestination: " << flights.getDestination();
	out << endl;

	return out;
}  // end overloaded << operator


   //==============================
   //		Private Methods
   //==============================

bool Flight::addPilotClub(const size_t& reservation, bool& ableToAdd, size_t& reassign)
{
	if (pilotClub.size() >= 3)  // Pilot Club full, need to reassign passengers
	{
		if (firstClass.size() >= 5)  // First Class full, need to reassign passengers
		{
			if (businessClass.size() >= 5)  // Business Class full, need to reassign passengers
			{
				if (economy.size() >= 27)  // Economy Class full, need to reassign passengers
				{
					// Move Economy to waitlist
					reassign = economy.back();
					economy.pop_back();
					waiting.push_front(reassign);

					// Move Business to Economy
					reassign = businessClass.back();
					businessClass.pop_back();
					economy.push_front(reassign);

					// Move First to Business
					reassign = firstClass.back();
					firstClass.pop_back();
					businessClass.push_front(reassign);

					// Add Pilot to First
					firstClass.push_front(reservation);
				}
				else  // Economy isn't full, but Pilot, First, and Business are
				{
					// Move Business to Economy
					reassign = businessClass.back();
					businessClass.pop_back();
					economy.push_front(reassign);

					// Move First to Business
					reassign = firstClass.back();
					firstClass.pop_back();
					businessClass.push_front(reassign);

					// Add Pilot to First
					firstClass.push_front(reservation);
				}
			}
			else  // Business Class isn't full, but Pilot and First are
			{
				// Move First to Business
				reassign = firstClass.back();
				firstClass.pop_back();
				businessClass.push_front(reassign);

				// Add Pilot to First
				firstClass.push_front(reservation);
			}
		}
		else  // First Class isn't full, add without reassigning passengers
			firstClass.push_front(reservation);
	}
	else  // Pilot Club isn't full, add without reassigning passengers
		pilotClub.push_front(reservation);

	return ableToAdd;
}  // end addPilotclub

bool Flight::addFirstClass(const size_t& reservation, bool& ableToAdd, size_t& reassign)
{
	if (firstClass.size() >= 5)  // First Class full, need to reassign passengers
	{
		if (businessClass.size() >= 5)  // Business Class full, need to reassign passengers
		{
			if (economy.size() >= 27)  // Economy Class full, need to reassign passengers
			{
				// Move Economy to waitlist
				reassign = economy.back();
				economy.pop_back();
				waiting.push_front(reassign);

				// Move Business to Economy
				reassign = businessClass.back();
				businessClass.pop_back();
				economy.push_front(reassign);

				// Add First to Business
				businessClass.push_front(reservation);
			}
			else  // Economy Class isn't full, but First and Business Class are
			{
				// Move Business to Economy
				reassign = businessClass.back();
				businessClass.pop_back();
				economy.push_front(reassign);

				// Add First to Business
				businessClass.push_front(reservation);
			}
		}
		else  // Business Class isn't full, add without reassigning passengers
			businessClass.push_front(reservation);
	}
	else  // First Class isn't full, add without reassigning passengers
		firstClass.push_front(reservation);

	return ableToAdd;
}  // end addFirstClass

bool Flight::addBusinessClass(const size_t& reservation, bool& ableToAdd, size_t& reassign)
{
	if (businessClass.size() >= 5)  // Business Class is full, need to reassign passengers
	{
		if (economy.size() >= 27)  // Economy Class is full, need to reassign passengers
		{
			// Move Economy to waitlist
			reassign = economy.back();
			economy.pop_back();
			waiting.push_front(reassign);

			// Add Business to Economy
			economy.push_front(reservation);

		}
		else  // Economy isn't full, add without reassigning passengers
			economy.push_front(reservation);
	}
	else  // Business Class isn't full, add without reassigning passengers
		businessClass.push_front(reservation);

	return ableToAdd;
}  // end addBusinessClass

bool Flight::addEconomy(const size_t& reservation, bool& ableToAdd, size_t& reassign)
{
	if (economy.size() >= 27)  // Economy full, add to waitlist
			waiting.push_front(reservation);
	else  // Economy Class isn't full yet, add without reassigning passengers
		economy.push_front(reservation);

	return ableToAdd;
}  // end addEconomy


   //==============================
   //		Public Methods
   //==============================


Flight::Flight(const size_t& flightNumber, const size_t& departTime, const size_t& arriveTime, const size_t& mileage, const string& fromCity, const string& toCity)
{
	this->flightNumber = flightNumber;
	this->departTime = departTime;
	this->arriveTime = arriveTime;
	this->mileage = mileage;
	this->fromCity = fromCity;
	this->toCity = toCity;
}  // end constructor

bool Flight::addPassenger(const size_t& reservation, const string& membership)
{
	bool ableToAdd = true;  // Only false if 10 passengers have been reassigned
	size_t reassign;  // For assigning passengers to different seats

	if (waiting.size() >= 10)  // Can only displace 10 passengers, so any new passengers are added to waitlist
	{
		waiting.push_front(reservation);
		ableToAdd = false;  // Unable to board, add to waitlist
	}
	// 10 passengers have not been reassigned/displaced
	else if (membership == "Pilot Club")
		ableToAdd = addPilotClub(reservation, ableToAdd, reassign);
	else if (membership == "First Class")
		ableToAdd = addFirstClass(reservation, ableToAdd, reassign);
	else if (membership == "Business Class")
		ableToAdd = addBusinessClass(reservation, ableToAdd, reassign);
	else if (membership == "Economy")  // Add economy class passengers up to max capacity
		ableToAdd = addEconomy(reservation, ableToAdd, reassign);

	manifest.emplace(reservation, membership);  // Keep track of total passengers
	
	return ableToAdd;
}  // end addPassenger

void Flight::removePassenger(const size_t& reservation)
{
	manifest.erase(reservation);
}  // end removePassenger

void Flight::findPassenger(const size_t& reservation) const
{
	try
	{
		manifest.find(reservation);
	}
	catch (const NotFoundException&)
	{
		cout << "Unable to find passenger aboard flight " << flightNumber << endl;
	}
}  // end findPassenger

vector<size_t> Flight::getManifest() const
{
	vector<size_t> manifestToReturn;
	for (auto const& iMap : manifest)
		manifestToReturn.push_back(iMap.first);

	return manifestToReturn;
}  // end getManifest

#endif