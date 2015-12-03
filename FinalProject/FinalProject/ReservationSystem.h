
#ifndef RS_H
#define RS_H

#include <fstream>
#include <iostream>
#include <map>

#include "Flight.h"
#include "Graph.h"
#include "Passenger.h"

typedef map <size_t, Passenger> p_map;
typedef map <size_t, Flight> f_map;

class ReservationSystem
{
public:
	static ReservationSystem& getInstance()
	{
		if (!p_rsvs_instance_)
		{
			p_rsvs_instance_ = new ReservationSystem();
			p_rsvs_instance_->initialize();
		}
		return *p_rsvs_instance_;
	}

	bool runMenu();	// Returns false when user is done with the menu
	void closeMenu(); // Destructor is called, cleans up data containers

private:
	// Singleton definition
	static ReservationSystem* p_rsvs_instance_;

	ReservationSystem() {}
	~ReservationSystem() {}
	ReservationSystem(const ReservationSystem &)	= delete; // Copy constructor function deleted
	void operator= (const ReservationSystem &)		= delete; // Assignment operator function deleted

	// Class Data
	p_map passengers;
	f_map flights;

	// Initialization
	void initialize();

	// Utility Functions
	bool openFlightData(const string& fileName);
	bool openPassengerData(const string& fileName);
	void createGraph();

	// Menu Functions
	void endProgram(const int returnValue);
	void pauseProgram();
	void mainMenu();
	char getMainMenuChoice();
	char getSubMenuChoice(const int numChoices);
	void passengerDisplayMenu();
	void flightDisplayMenu();
	// Menu Options
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