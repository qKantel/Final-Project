
#include <fstream>
#include <iostream>
#include <map>
#include <vector>

#include "Passenger.h"
#include "graph.h"
#include "AdjListNode.h"
#include "Edge.h"
#include "Flight.h"
#include "HashTable.h"
#include "NotFoundException.h"
#include "LinkedHashedEntry.h"

using namespace std;

typedef map <size_t, Passenger> p_map;
typedef map <size_t, Flight> f_map;

//////////////////////////////////////////////////////////////////////////////////
//						Function Prototypes
//////////////////////////////////////////////////////////////////////////////////

// Main Menu Functions
void pauseProgram();
void endProgram(const int returnValue);
void mainMenu();
char getMainMenuChoice();
char getSubMenuChoice(const int numChoices);
void passengerDisplayMenu();
void flightDisplayMenu();
// Menu calls
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

bool openFlightData(const string& fileName, f_map& allFlights);
bool openPassengerData(const string& fileName, p_map & aList, f_map & flight_list);

int main()
{
	Graph<char, int> flightMap(14);
	p_map passengers;
	f_map flights;

	flightMap.add(Edge<char, int> ('A', 'B', 142));
	flightMap.add(Edge<char, int> ('A', 'C', 170));
	flightMap.add(Edge<char, int> ('C', 'D', 114));
	flightMap.add(Edge<char, int> ('D', 'E', 93));
	flightMap.add(Edge<char, int> ('D', 'M', 209));
	flightMap.add(Edge<char, int> ('M', 'N', 208));
	flightMap.add(Edge<char, int> ('N', 'P', 134));
	flightMap.add(Edge<char, int> ('P', 'O', 193));
	flightMap.add(Edge<char, int> ('E', 'F', 155));
	flightMap.add(Edge<char, int> ('F', 'G', 184));
	flightMap.add(Edge<char, int> ('F', 'I', 160));
	flightMap.add(Edge<char, int> ('I', 'G', 83));
	flightMap.add(Edge<char, int> ('I', 'L', 88));
	flightMap.add(Edge<char, int> ('I', 'J', 73));

	// Create all flight objects
	string flights_filename = "Flights.txt";
	bool ableToOpen;
	do
	{
		ableToOpen = openFlightData(flights_filename, flights);
		if (!ableToOpen)
		{
			cout << flights_filename << " cannot be opened.  Enter another file name -->  ";
			getline(cin, flights_filename);
		}
	} while (!ableToOpen);

	// Create all passenger objects
	string passengers_filename = "Passengers.txt";
	do
	{
		ableToOpen = openPassengerData(passengers_filename, passengers, flights);
		if (!ableToOpen)
		{
			cout << passengers_filename << " cannot be opened.  Enter another file name -->  ";
			getline(cin, passengers_filename);
		}
	} while (!ableToOpen);


	//Initialize selection variable.
	char selection;
	//Main Menu loop.
	do
	{
		//Print menu.
		mainMenu();

		//Menu switch.
		selection = getMainMenuChoice();
		switch (selection)
		{
		case '1': passengerDisplayMenu();
			break;
		case '2': flightDisplayMenu();
			break;
		case '3': passengerSearch_Option();
			break;
		case '4': flightSearch_Option();
			break;
		}

	} while (selection != '0');


	endProgram(0);
	return 0;
}

bool openFlightData(const string& fileName, f_map& allFlights)
{
	ifstream flightData(fileName);
	string fromCity, toCity;
	int flightNumber, departTime, arriveTime;

	if (flightData.fail())
		return false;
	else  // Able to open file for processing
	{
		while (!flightData.eof())
		{
			// Create new Flight objects using data from file
			Flight* flights = new Flight();

			flightData >> flightNumber;
			flightData >> departTime;
			flightData >> arriveTime;
			flightData >> fromCity;
			flightData >> toCity;

			flights->addFlightNumber(flightNumber);
			flights->addDepartTime(departTime);
			flights->addArriveTime(arriveTime);
			flights->addOrigin(fromCity);
			flights->addDestination(toCity);

			// Add new Flight objects to vector
			allFlights.insert(f_map::value_type( flights->getFlightNumber(), *flights ));
			delete flights;
			flights = nullptr;
		}  // end while
		flightData.close();
	}  // end if
	return true;
}  // end openFlightData


bool openPassengerData(const string& fileName, p_map & aList, f_map & flight_list)
{
	fstream in(fileName);

	if (in.fail())
		return false;

	Passenger aPassenger;
	int iBuf;
	string buffer, temp;
	string delimiter = " ";  // Remove trailing whitespace


	while (!in.eof())
	{
		getline(in, buffer, '\n');

		// 0 - 20
		temp = buffer.substr(0, 20);
		string firstname = temp.substr(0, temp.find_last_not_of(delimiter) + 1);
		aPassenger.setFirstName(firstname);

		// 20-40
		temp = buffer.substr(20, 20);
		string lastname = temp.substr(0, temp.find_last_not_of(delimiter) + 1);
		aPassenger.setLastName(lastname);

		// 40 - 60
		temp = buffer.substr(40, 20);
		string membership = temp.substr(0, temp.find_last_not_of(delimiter) + 1);
		aPassenger.setMembership(membership);

		// 60 - 80
		iBuf = stoi(buffer.substr(60, 4));
		aPassenger.setReservationNumber(iBuf);

		// 80 - 100
		iBuf = stoi(buffer.substr(80, 4));
		aPassenger.setflightNumber(iBuf);

		aList.insert(p_map::value_type(aPassenger.getReservationNumber(), aPassenger));
		flight_list[aPassenger.getFlightNumber()].addPassenger(aPassenger.getReservationNumber(), aPassenger.getMembership());
	}

	in.close();
	return true;
}

//////////////////////////////////////////////////////////////////////////////////
//							Menu Functions
//////////////////////////////////////////////////////////////////////////////////

void endProgram(const int returnValue)
{
	//Indicates that program finished successfully.
	cout << ". . . Program Ending ";
	cout << ((returnValue == 0) ? "Successfully" : "With a Problem") << endl;
	cout << "\nPress Enter to end";
	cin.ignore();
	exit(returnValue);
}

void pauseProgram()
{
	//Pauses program. Continue on enter.
	cout << "Press enter to continue . . .";
	cin.get();
	cout << endl;
}

//Menu functions

void mainMenu()
{
	//Display main menu.
	cout << endl << endl << endl << endl;
	cout << "Airline Reservation Main Menu" << endl;
	cout << "-----------------------------" << endl;
	cout << "1. Display a list of Passengers" << endl;
	cout << "2. Display a list of Flights" << endl;
	cout << "3. Search for a Passenger" << endl;
	cout << "4. Search for a Flight" << endl;
	cout << "0. [Q]uit" << endl;
	cout << endl;

}

char getMainMenuChoice()
{
	//Initialize variable.
	char choice;

	//Prompt user.
	cout << "Select from options 1 to 4, 0 or Q to quit: ";

	//Get input.
	cin >> choice;
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	cout << endl;

	//Validate user input.
	if (choice == '1' || choice == '2' || choice == '3' || choice == '4')
		return choice;
	else if (choice == '0' || choice == 'Q' || choice == 'q')
	{
		choice = '0';
		return choice;
	}
	else
	{
		//Invalid selection.
		cout << choice << " is not a valid choice." << endl << endl;

		//Recursive call.
		return getMainMenuChoice();
	}

}

char getSubMenuChoice(const int numChoices = 0)
{
	//A generic input validation func to be used with different sub-menus.
	//Input: numChoices is the number of selections the user can choose from.
	//Outputs: a valid choice represented as an integer.

	//Initialize.
	char choice;

	//Pompt user.
	cout << "Enter the number for the desired operation, 0 or B to go back: ";

	//Get input.
	cin >> choice;
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	cout << endl;

	//Validate. Relates ascII to decimal equivalent.
	if (choice >= 49 && choice < 49 + numChoices)
		return choice;
	else if (choice == 48 || choice == 'B' || choice == 'b')
		return '0';
	else
	{
		//Bad input. 
		cout << "Not a valid selection." << endl << endl;

		//Recursive call.
		return getSubMenuChoice(numChoices);
	}


}

void passengerDisplayMenu()
{
	//Initialize.
	char selection;

	//Menu loop.
	do
	{
		//Passenger lists menu.
		cout << endl << endl << endl << endl;
		cout << "List Passengers - Options" << endl;
		cout << "-------------------------" << endl;
		cout << "1. List all passengers and data" << endl;
		cout << "2. List passengers by destination" << endl;
		cout << "3. List passengers by city of departure" << endl;
		cout << "4. List passengers by flight" << endl;
		cout << "5. List overbooked passengers" << endl;
		cout << "6. List passengers waiting for a flight" << endl;
		cout << "0. [B]ack" << endl;
		cout << endl;

		//Get selection.
		selection = getSubMenuChoice(6);

		//Switch.
		switch (selection)
		{
		case '1': allPassengers_Option();
			break;
		case '2': pasByDestination_Option();
			break;
		case '3': pasByDeparture_Option();
			break;
		case '4': pasByFlight_Option();
			break;
		case '5': pasOverbook_Option();
			break;
		case '6': pasWaiting_Option();
		}

	} while (selection != '0');

}

void flightDisplayMenu()
{
	//Initialize.
	char selection;

	do
	{
		//List flights menu.
		cout << endl << endl << endl << endl;
		cout << "List Flights - Options" << endl;
		cout << "----------------------" << endl;
		cout << "1. Display all flights" << endl;
		cout << "2. Display all flights with passengers" << endl;
		cout << "0. [B]ack" << endl;
		cout << endl;

		//Get selection.
		selection = getSubMenuChoice(2);

		//switch.
		switch (selection)
		{
		case '1': allFlights_Option();
			break;
		case '2': flightsAndPas_Option();
			break;
		}

	} while (selection != '0');
}


void passengerSearch_Option()
{
	//Pause Program
	pauseProgram();
}

void flightSearch_Option(f_map & flights)
{
	char selection;
	// TODO
	// Add search flight	by Departure City
	//						by Destination City
	do
	{
		cout << "Search Flight - Options" << endl;
		cout << "-----------------------" << endl;
		cout << "1. Search flight by city to city" << endl;
		cout << "2. Search flight by flight number" << endl;
		cout << "0. [B]ack" << endl;

		//Get selection 
		selection = getSubMenuChoice(2);
		string city1;
		string city2;

		switch (selection)
		{
		case '1':
			// TODO
			// Input check city1, city2
			cout << "What is the departure city?" << endl;
			cin >> city1;
			cout << "What is the destination city?" << endl;
			cin >> city2;
			for (f_map::iterator iter = flights.begin(); iter != flights.end(); iter++)
			{
				cout << "List of flights with the following requirements:" << endl;
				if (iter->second.getOrigin() == city1 && iter->second.getDestination() == city2)
					cout << iter->second << endl;
				cout << endl;
			}
			break;
		
		case '2':
			// TODO
			// Input check flight number
			size_t flightNum;
			cout << "Enter flight number: ";
			cin >> flightNum;
			Flight fOb = flights[flightNum];
			cout << fOb;
			break;
		}
	} while (selection != '0');
}


void allPassengers_Option()
{

	//Pause program.
	pauseProgram();
}

void pasByDestination_Option()
{

	//Pause program.
	pauseProgram();
}

void pasByDeparture_Option()
{

	//Pause program.
	pauseProgram();
}

// #4
void pasByFlight_Option(p_map &pasMap, int &flightNum)
{
	typedef p_map::iterator it_type;
	for (it_type iterator = pasMap.begin(); iterator != pasMap.end(); iterator++)
	{
		if (iterator->second.getFlightNumber() == flightNum)
		{
			cout << iterator->second;
		}
		//else
		//Not On This Flight
	}
}

void pasOverbook_Option()
{

	//Pause program.
	pauseProgram();
}

void pasWaiting_Option()
{

	//Pause program.
	pauseProgram();
}

void allFlights_Option()
{

	//Pause program.
	pauseProgram();
}

void flightsAndPas_Option()
{

	//Pause program.
	pauseProgram();
}