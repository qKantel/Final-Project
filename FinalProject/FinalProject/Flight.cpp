#include "Flight.h"

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
