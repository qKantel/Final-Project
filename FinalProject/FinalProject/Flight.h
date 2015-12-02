#include <string>
using namespace std;

#include "HashTable.h"
#include "Passenger.h"

#ifndef _FLIGHT
#define _FLIGHT


class Flight
{
	friend ostream& operator<<(ostream& out, const Flight& flights);

private:
	vector<int> pilotClub,
		firstClass,
		businessClass,
		economy,
		waiting;

	size_t flightNumber;
	size_t departTime;
	size_t arriveTime;
	size_t mileage;

	string fromCity;
	string toCity;

	HashTable<size_t, string> manifest;

public:
	Flight() : pilotClub(0), firstClass(0), businessClass(0), economy(0), flightNumber(0), departTime(0), arriveTime(0), mileage(0), fromCity(""), toCity("") {}
	Flight(const size_t& flightNumber, const size_t& departTime, const size_t& arriveTime, const size_t& mileage, const string& fromCity, const string& toCity);

	/** Add flight number, departure & arrival times, origin & destination cities. */
	void addFlightNumber(const size_t& newFlightNumber) { flightNumber = newFlightNumber; }
	void addDepartTime  (const size_t& departureTime)	{ departTime = departureTime;	  }
	void addArriveTime  (const size_t& arrivalTime)		{ arriveTime = arrivalTime;		  }
	void addOrigin      (const string& from)		    { fromCity = from;				  }
	void addDestination (const string& to)				{ toCity = to;					  }

	/** Total distance between departure and arrival city. */
	void addMileage(const size_t& mileage) { this->mileage = mileage; };
	
	/** Adds passenger to manifest using reservation number. 
	@pre  Only have room for 27 economy passengers with a maximum total capacity of 40 passengers. */
	void addPassenger(const int& reservationNumber, const string& membership);

	/** Retrieves flight data. */
	size_t getFlightNumber() const { return flightNumber; }
	size_t getDepartTime() const   { return departTime;	  }
	size_t getArriveTime() const   { return arriveTime;   }
	size_t getMileage() const	   { return mileage;	  }
	string getOrigin() const	   { return fromCity;	  }
	string getDestination() const  { return toCity;		  }

	/** Returns a list of all passengers aboard flight */
	vector<int> getManifest() const;

	/** Returns passenger information if on flight.
	@throw NotFoundException if the passenger is not on the flight. */
	bool findPassenger(const int& reservationNumber) const;

	/** Removes passenger from manifest.  */
	void removePassenger(const int& reservationNumber);
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
//		Public Methods
//==============================

Flight::Flight(const size_t& flightNumber, const size_t& departTime, const size_t& arriveTime, const size_t& mileage, const string& fromCity, const string& toCity)
	: pilotClub(0), firstClass(0), businessClass(0), economy(0)
{
	this->flightNumber = flightNumber;
	this->departTime = departTime;
	this->arriveTime = arriveTime;
	this->mileage = mileage;
	this->fromCity = fromCity;
	this->toCity = toCity;
}  // end constructor

void Flight::addPassenger(const int& reservationNumber, const string& membership)
{
	if (membership == "Pilot Club")
		pilotClub.push_back(reservationNumber);
	else if (membership == "First Class")
		firstClass.push_back(reservationNumber);
	else if (membership == "Business Class")
		businessClass.push_back(reservationNumber);
	else
		economy.push_back(reservationNumber);
	
	if (manifest.getTotalEntries() <= 40)
	{
		// Only have room for 27 in economy
		if (membership == "Economy" && economy.size() > 27)
			throw exception();
		else
			manifest.add( reservationNumber, membership );
	}
}  // end addPassenger

void Flight::removePassenger(const int& reservationNumber)
{
	manifest.remove( reservationNumber );
}  // end removePassenger

bool Flight::findPassenger(const int& reservationNumber) const
{
	bool foundPassenger = false;
	try
	{
		manifest.find( reservationNumber );
	}
	catch (const NotFoundException&)
	{
		cout << "Unable to find passenger aboard flight " << flightNumber << endl;
	}

	return foundPassenger;
}  // end findPassenger

vector<int> Flight::getManifest() const
{
	return manifest.getAll();
}  // end getManifest

#endif