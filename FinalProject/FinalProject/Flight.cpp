/*
*	file	Flight.cpp
*	status	Complete
*/

#include "Flight.h"

//==============================
//		Implementation
//==============================

//==============================
//		Friend Methods
//==============================

ostream& operator<<(ostream& out, const Flight& flights)
{
	char departure[10], arrival[10];
	strftime(departure, sizeof(departure), "%I:%M %p", &flights.getDepartTime());
	strftime(arrival, sizeof(arrival), "%I:%M %p", &flights.getArriveTime());

	out << "Flight Number: " << flights.getFlightNumber();
	out << "\nDeparture Time: " << departure;
	out << "\nArrival Time: " << arrival;
	out << "\nTotal Mileage: " << flights.getMileage();
	out << "\nDeparting From: " << flights.getOrigin();
	out << "\nDestination: " << flights.getDestination();

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

					// Need to adjust membership from Business to Economy just for this flight
					manifest.erase(reassign);
					manifest.emplace(reassign, "Economy");

					// Move First to Business
					reassign = firstClass.back();
					firstClass.pop_back();
					businessClass.push_front(reassign);

					// Adjust membership from First to Business
					manifest.erase(reassign);
					manifest.emplace(reassign, "Business Class");

					// Add Pilot to First & adjust membership to First Class
					firstClass.push_front(reservation);
					manifest.emplace(reservation, "First Class");
				}
				else  // Economy isn't full, but Pilot, First, and Business are
				{
					// Move Business to Economy
					reassign = businessClass.back();
					businessClass.pop_back();
					economy.push_front(reassign);

					// Adjust membership from Business to Economy
					manifest.erase(reassign);
					manifest.emplace(reassign, "Economy");

					// Move First to Business
					reassign = firstClass.back();
					firstClass.pop_back();
					businessClass.push_front(reassign);

					// Adjust membership from First to Business
					manifest.erase(reassign);
					manifest.emplace(reassign, "Business Class");

					// Add Pilot to First & adjust membership to First Class
					firstClass.push_front(reservation);
					manifest.emplace(reservation, "First Class");
				}
			}
			else  // Business Class isn't full, but Pilot and First are
			{
				// Move First to Business
				reassign = firstClass.back();
				firstClass.pop_back();
				businessClass.push_front(reassign);

				// Adjust membership from First to Business
				manifest.erase(reassign);
				manifest.emplace(reassign, "Business Class");

				// Add Pilot to First & adjust membership to First Class
				firstClass.push_front(reservation);
				manifest.emplace(reservation, "First Class");
			}
		}
		else  // First Class isn't full, add without reassigning passengers
		{
			firstClass.push_front(reservation);
			manifest.emplace(reservation, "First Class");  // Add as First Class member
		}
	}
	else  // Pilot Club isn't full, add without reassigning passengers
	{
		pilotClub.push_front(reservation);
		manifest.emplace(reservation, "Pilot Club");
	}

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

				// Adjust membership from Business to Economy
				manifest.erase(reassign);
				manifest.emplace(reassign, "Economy");

				// Add First to Business & adjust membership from First to Business
				businessClass.push_front(reservation);
				manifest.emplace(reservation, "Business Class");
			}
			else  // Economy Class isn't full, but First and Business Class are
			{
				// Move Business to Economy
				reassign = businessClass.back();
				businessClass.pop_back();
				economy.push_front(reassign);

				// Adjust membership from Business to Economy
				manifest.erase(reassign);
				manifest.emplace(reassign, "Economy");

				// Add First to Business & adjust membership to Business
				businessClass.push_front(reservation);
				manifest.emplace(reservation, "Business Class");
			}
		}
		else  // Business Class isn't full, add without reassigning passengers
		{
			businessClass.push_front(reservation);
			manifest.emplace(reservation, "Business Class");  // Add as a Business Class member
		}
	}
	else  // First Class isn't full, add without reassigning passengers
	{
		firstClass.push_front(reservation);
		manifest.emplace(reservation, "First Class");
	}

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

			// Add Business to Economy & adjust membership from Business to Economy
			economy.push_front(reservation);
			manifest.emplace(reservation, "Economy");
		}
		else  // Economy isn't full, add without reassigning passengers
		{
			economy.push_front(reservation);
			manifest.emplace(reservation, "Economy");  // Add as Economy member
		}
	}
	else  // Business Class isn't full, add without reassigning passengers
	{
		businessClass.push_front(reservation);
		manifest.emplace(reservation, "Business Class");
	}

	return ableToAdd;
}  // end addBusinessClass

bool Flight::addEconomy(const size_t& reservation, bool& ableToAdd, size_t& reassign)
{
	if (economy.size() >= 27)  // Economy full, add to waitlist
		waiting.push_front(reservation);
	else  // Economy Class isn't full yet, add without reassigning passengers
		economy.push_front(reservation);

	// Add as Economy member
	manifest.emplace(reservation, "Economy");

	return ableToAdd;
}  // end addEconomy

bool Flight::remove(deque<size_t>& passengers, const size_t& reservation)
{
	size_t passengerToBoard;
	string membership;

	for (size_t i = 0; i < passengers.size(); i++)
	{
		if (passengers[i] == reservation)  // Found the passenger
		{
			passengers.erase(passengers.begin() + i);  // Remove from flight

													   // If flight has passengers waiting to board,
													   // Get 'oldest' passenger who is waiting to board
													   // Add passenger to the flight
			if (waiting.size() > 0)
			{
				passengerToBoard = waiting.back();
				waiting.pop_back();
				membership = manifest.at(passengerToBoard);
				addPassenger(passengerToBoard, membership);
			}

			return true;
		}  // end if
	}  // end for
	return false;  // Did not find passenger
}  // end remove


   //==============================
   //		Public Methods
   //==============================


Flight::Flight() : flightNumber(0), mileage(0), fromCity(""), toCity("")
{
	// Get the current time
	time(&currentTime);
	localtime_s(&departureTime, &currentTime);
	localtime_s(&arrivalTime, &currentTime);

	departureTime.tm_sec = 0;
	arrivalTime.tm_sec = 0;
}  // end constructor

Flight::Flight(const size_t& flightNumber, const size_t& departTime, const size_t& arriveTime, const size_t& mileage, const string& fromCity, const string& toCity)
{
	this->flightNumber = flightNumber;
	this->mileage = mileage;
	this->fromCity = fromCity;
	this->toCity = toCity;

	departureTime.tm_hour = static_cast<int> (departTime) / 100;
	departureTime.tm_min = static_cast<int> (departTime) % 100;

	arrivalTime.tm_hour = static_cast<int> (arriveTime) / 100;
	arrivalTime.tm_min = static_cast<int> (arriveTime) % 100;
}  // end constructor


bool Flight::hasFlightLeft(tm atTime)
{
	// Check if flight has left [reference time - departure time]
	return (difftime(mktime(&atTime), mktime(&departureTime))) >= 0;
}

void Flight::addDepartTime(const size_t & departTime)
{
	departureTime.tm_hour = static_cast<int> (departTime) / 100;
	departureTime.tm_min = static_cast<int> (departTime) % 100;
}  // end addDepartTime

void Flight::addArriveTime(const size_t & arriveTime)
{
	arrivalTime.tm_hour = static_cast<int> (arriveTime) / 100;
	arrivalTime.tm_min = static_cast<int> (arriveTime) % 100;
}  // end addArriveTime

bool Flight::addPassenger(const size_t& reservation, const string& membership)
{
	bool ableToAdd = true;  // Only false if 10 passengers have been reassigned
	size_t reassign;  // For assigning passengers to different seats

	if (waiting.size() > 10)  // Can only displace 10 passengers, so any new passengers are added to waitlist
	{
		waiting.push_front(reservation);
		manifest.emplace(reservation, membership);  // Add to manifest
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

	return ableToAdd;
}  // end addPassenger

bool Flight::removePassenger(const size_t& reservation)
{
	bool ableToRemove = false;

	if (manifest.count(reservation))
	{
		// Remove from appropriate section
		string membership = manifest.at(reservation);

		if (membership == "Pilot Club")
			ableToRemove = (remove(pilotClub, reservation));
		else if (membership == "First Class")
			ableToRemove = (remove(firstClass, reservation));
		else if (membership == "Business Class")
			ableToRemove = (remove(businessClass, reservation));
		else if (membership == "Economy")
			ableToRemove = (remove(economy, reservation));

		if (!ableToRemove)
			ableToRemove = (remove(waiting, reservation));

		manifest.erase(reservation);  // Remove from manifest

		return ableToRemove;
	}  // end if
	else  // Not on manifest
		return ableToRemove;
}  // end removePassenger

vector<size_t> Flight::getManifest() const
{
	vector<size_t> manifestToReturn;
	for (auto const& iMap : manifest)
		manifestToReturn.push_back(iMap.first);

	return manifestToReturn;
}  // end getManifest

deque<size_t> Flight::getOverBooked() const
{
	return waiting;
}  // end getOverBooked