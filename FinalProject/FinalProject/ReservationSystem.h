/*
*	file	ReservationSystem.h
*	status	Complete
*/
#ifndef RS_H
#define RS_H

// clibrary
#include <ctime>

// containers
#include <map>
#include <set>

// IO
#include <fstream>
#include <iostream>

// local includes
#include "Flight.h"
#include "Passenger.h"
#include "FlightMap.h"

/* Typedefs */
typedef map <size_t, Passenger> p_map;
typedef map <size_t, Flight> f_map;

/*
*	Displays a user interface. Allows the user to perform a variety of functions to run an airline system.
*/
class ReservationSystem
{
public:
	// Constructor
	/*
	*	parameters
	*	- baseTime		time the user is running the program in relation to the day
	*/
	ReservationSystem(tm baseTime);

	/*
	*	return
	*	- bool		returns false when the user is done with the menu
	*	info
	*	- Continually runs the menu system as long as the user does not ask to exit.
	*/
	bool runMenu();

private:
	ReservationSystem(const ReservationSystem &) {} // Copy constructor function deleted
	void operator= (const ReservationSystem &) {} // Assignment operator function deleted

	// Class Data
	p_map passengers;
	f_map flights;
	set<string> cities;
	FlightMap flightMap;
	tm input_time;

	// Initialization
	void initialize();

	// Utility Functions
	bool openFlightData(const string& fileName);
	bool openPassengerData(const string& fileName);

	void uppercaseString(std::string& s);
	bool caseInsensitiveStringEquals(const std::string& a, const std::string& b);

	void parseFlightNumber(size_t& flightNumber);
	void parsePassengerName(string& firstName, string& lastName);


	// Menu Functions
	void endProgram(const int returnValue);
	void pauseProgram();
	void mainMenu();
	char getMainMenuChoice();
	char getSubMenuChoice(const int numChoices);
	void passengerDisplayMenu();
	void flightDisplayMenu();
	// Menu Options
	void deletePassenger_Option();
	void passengerSearch_Option();
	void flightSearch_Option();
	void allPassengers_Option();
	void pasByDestination_Option();
	void pasByDeparture_Option();
	void pasByFlight_Option();
	void pasOverbook_Option();
	void pasWaiting_Option();
	void allFlights_Option();
	void flightsAndPas_Option();
};

#endif