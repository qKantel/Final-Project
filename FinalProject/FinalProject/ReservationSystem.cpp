/*
*	file	ReservationSystem.cpp
*	status	Complete
*/

#include "ReservationSystem.h"
#include <algorithm>


ReservationSystem::ReservationSystem(tm baseTime)
{
	input_time = baseTime;
	initialize();
}

bool ReservationSystem::runMenu()
{
	//Initialize selection variable.
	char selection;

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
	case '5': deletePassenger_Option();
		break;
	}

	if (selection == '0')
		return false;

	return true;
}

void ReservationSystem	::deletePassenger_Option()
{
	char selection;

	//Menu loop.
	do
	{
		//Passenger lists menu.
		cout << endl << endl << endl << endl;
		cout << "Delete a Passenger - Options" << endl;
		cout << "-------------------------" << endl;
		cout << "1. Delete a Passenger using Reservation Number" << endl;
		cout << "2. Delete a Passenger using a Flight Number" << endl;
		cout << "0. [B]ack" << endl;
		cout << endl << endl;

		//Get selection.
		selection = getSubMenuChoice(6);

		//REMEMBER: make sure that the flight hasnt left before we delete someone off of it

		switch(selection)
		{
			case '1':
			{
				size_t resNumber;
				cout << "Enter the reservation number of the passenger you want to delete: ";
				cin >> resNumber;
				cout << endl;

				if(passengers.find(resNumber) != passengers.end())
				{
					Passenger pass = passengers[resNumber];
					if(passengers[resNumber].getFlightNumber() != 0)
					{
						if(!flights[pass.getFlightNumber()].hasFlightLeft(input_time))
						{
							bool removed = flights[pass.getFlightNumber()].removePassenger(pass.getReservationNumber());
							if(removed)
							{
								cout << "Successfully removed the passenger from flight " << pass.getFlightNumber() << endl; 
								passengers[resNumber].setflightNumber(0);
							}
							else cout << "The passenger was NOT removed from flight " << pass.getFlightNumber() << endl;
						}
						else cout << "The flight this passenger is on has already left. You can not delete them from it." << endl;
					}
					else cout << "The passenger with reservation number " << resNumber << " has already been removed from a flight." << endl;
				}
				else cout << "Could not find a passenger with reservation number " << resNumber << endl;
			}
			break;

			case '2':
			{
				size_t flightNumber;
				parseFlightNumber(flightNumber);
				cout << endl;

				Flight flight = flights[flightNumber];
				if(!flight.hasFlightLeft(input_time))
				{
					cout << flight << endl << "Passengers: " << endl;
				
					vector<size_t> manifest = flight.getManifest();
					for(int i = 0; i < manifest.size(); i++)
					{
						cout << passengers[manifest[i]] << endl;
					}
					cout << endl;
					cout << "Select a person from this flight to delete" << endl;
					string firstName,lastName;
					parsePassengerName(firstName,lastName);
					bool foundPassenger = false;

					for(int i = 0; i < manifest.size(); i++)
					{
						Passenger p = passengers[manifest[i]];
						if(caseInsensitiveStringEquals(firstName,p.getFirstName()) 
							&& caseInsensitiveStringEquals(lastName,p.getLastName()))
						{
							foundPassenger = true;
							bool success = flights[flightNumber].removePassenger(p.getReservationNumber());
							if(success)
							{
								cout << p.getFirstName() << " " << p.getLastName() << " has been successfully removed from flight " << flight.getFlightNumber() << endl;
								passengers[manifest[i]].setflightNumber(0);
							}
							else 
								cout << p.getFirstName() << " " << p.getLastName() << " has NOT been removed from flight " << flight.getFlightNumber() << endl;
							break;
						}
					}

					if(!foundPassenger)
						cout << "No passenger with the name " << firstName << " " << lastName << " was found on fight " << flight.getFlightNumber() << endl;
				}
				else cout << "Flight " << flightNumber << " has already left. You cannot delete a passenger from it." << endl;
			}
			break;
		}
	}
	while(selection != '0');
}

void ReservationSystem::parseFlightNumber(size_t& flightNumber)
{
	cout << "Enter flight number: ";
	cin >> flightNumber;
	cout << endl;

	while(flightNumber > flights.size() || flightNumber < 0)
	{
		cout << "Invalid flight number entered." << endl;
		cout << "Enter flight number: ";
		cin >> flightNumber;
		cout << endl;
	}
}

void ReservationSystem::parsePassengerName(string& firstName, string& lastName)
{
	cout << "Enter the passenger's firsts name: ";
	cin >> firstName;
	cout << endl << "Enter the passenger's last name: ";
	cin >> lastName;
	cout << endl;
}

void ReservationSystem::initialize()
{
	cities.insert("A");
	cities.insert("B");
	cities.insert("C");
	cities.insert("D");
	cities.insert("E");
	cities.insert("F");
	cities.insert("G");
	cities.insert("I");
	cities.insert("J");
	cities.insert("L");
	cities.insert("M");
	cities.insert("N");
	cities.insert("O");
	cities.insert("P");

	// Create all flight objects
	string flights_filename = "Flights.txt";
	bool ableToOpen;
	do
	{
		ableToOpen = openFlightData(flights_filename);
		if (!ableToOpen)
		{
			cout << flights_filename << " cannot be opened.  Enter another file name -->  ";
			getline(cin, flights_filename);
		}
	} while (!ableToOpen);

	// Create all passenger objects
	string passengers_filename = "Passenger.txt";
	do
	{
		ableToOpen = openPassengerData(passengers_filename);
		if (!ableToOpen)
		{
			cout << passengers_filename << " cannot be opened.  Enter another file name -->  ";
			getline(cin, passengers_filename);
		}
	} while (!ableToOpen);
}

bool ReservationSystem::openFlightData(const string & fileName)
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
			Flight* temp_flight = new Flight();

			flightData >> flightNumber;
			flightData >> departTime;
			flightData >> arriveTime;
			flightData >> fromCity;
			flightData >> toCity;

			temp_flight->addFlightNumber(flightNumber);
			temp_flight->addDepartTime(departTime);
			temp_flight->addArriveTime(arriveTime);
			temp_flight->addOrigin(fromCity);
			temp_flight->addDestination(toCity);
			temp_flight->addMileage(flightMap.getMinDistance(fromCity[0],toCity[0]));

			// Add new Flight objects to vector
			flights.insert(f_map::value_type(temp_flight->getFlightNumber(), *temp_flight));
			delete temp_flight;
			temp_flight = nullptr;
		}  // end while
		flightData.close();
	}  // end if
	return true;
}

bool ReservationSystem::openPassengerData(const string & fileName)
{
	ifstream in(fileName);

	if (in.fail())
		return false;

	Passenger aPassenger;
	int iBuf;
	string buffer, temp;
	string delimiter = " ";  // Remove trailing whitespace

	getline(in, buffer, '\n');
	while (!in.eof())
	{
		getline(in, buffer, '\n');

		// 0 - 20
		temp = buffer.substr(0, 20);
		string firstname = temp.substr(0, temp.find_last_not_of(delimiter) + 1);
		aPassenger.setFirstName(firstname);

		// 20-40
		temp = buffer.substr(20, 19);
		string lastname = temp.substr(0, temp.find_last_not_of(delimiter) + 1);
		aPassenger.setLastName(lastname);

		// 40 - 60
		temp = buffer.substr(40, 19);
		string membership = temp.substr(0, temp.find_last_not_of(delimiter) + 1);
		aPassenger.setMembership(membership);

		// 60 - 80
		temp = buffer.substr(60, 4);
		temp = temp.substr(0, temp.find_last_not_of(delimiter) + 1);
		iBuf = stoi(temp);
		aPassenger.setReservationNumber(iBuf);

		// 80 - 100
		temp = buffer.substr(81);
		temp = temp.substr(0, temp.find_last_not_of(delimiter) + 1);
		iBuf = stoi(temp);
		aPassenger.setflightNumber(iBuf);

		passengers.insert(p_map::value_type(aPassenger.getReservationNumber(), aPassenger));
		flights[aPassenger.getFlightNumber()].addPassenger(
			aPassenger.getReservationNumber(), aPassenger.getMembership());
	}

	in.close();
	return true;
}


//////////////////////////////////////////////////////////////////////////////////
//							Menu Functions
//////////////////////////////////////////////////////////////////////////////////

void ReservationSystem::endProgram(const int returnValue)
{
	//Indicates that program finished successfully.
	cout << ". . . Program Ending ";
	cout << ((returnValue == 0) ? "Successfully" : "With a Problem") << endl;
	cout << "\nPress Enter to end";
	cin.ignore();
	exit(returnValue);
}

void ReservationSystem::pauseProgram()
{
	//Pauses program. Continue on enter.
	cout << "Press enter to continue . . .";
	cin.get();
	cout << endl;
}

//Menu functions

void ReservationSystem::mainMenu()
{
	//Display main menu.
	cout << endl << endl << endl << endl;
	cout << "Airline Reservation Main Menu" << endl;
	cout << "-----------------------------" << endl;
	cout << "1. Display a list of Passengers" << endl;
	cout << "2. Display a list of Flights" << endl;
	cout << "3. Search for a Passenger" << endl;
	cout << "4. Search for a Flight" << endl;
	cout << "5. Delete a Passenger from a Flight" << endl;
	cout << "0. [Q]uit" << endl;
	cout << endl;

}

char ReservationSystem::getMainMenuChoice()
{
	//Initialize variable.
	char choice;

	//Prompt user.
	cout << "Select from options 1 to 5, 0 or Q to quit: ";

	//Get input.
	cin >> choice;
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	cout << endl;

	//Validate user input.
	if (choice == '1' || choice == '2' || choice == '3' || choice == '4' || choice == '5')
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

char ReservationSystem::getSubMenuChoice(const int numChoices = 0)
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

void ReservationSystem::passengerDisplayMenu()
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
		cout << "5. List passengers not able to fly due to space limitations" << endl;
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

void ReservationSystem::flightDisplayMenu()
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


void ReservationSystem::passengerSearch_Option()
{
	char selection;
	do
	{
		cout << "Search Passengers - Options" << endl;
		cout << "-----------------------" << endl;
		cout << "1. Search for a passenger by reservation number" << endl;
		cout << "2. Search for a passenger by full name" << endl;
		cout << "3. Search for passengers with a first name" << endl;
		cout << "4. Search for passengers with a last name" << endl;
		cout << "0. [B]ack" << endl;

		selection = getSubMenuChoice(4);
		switch(selection)
		{
			//searching by reservation number
			case '1':
			{
				size_t reservationNumber;
				cout << "Enter a reservation number: ";
				cin >> reservationNumber;
				cout << endl;

				std::map<size_t,Passenger>::iterator itt = passengers.find(reservationNumber);

				//This means that there wasnt a passenger with that reservationNumber
				if(itt == passengers.end())
				{
					cout << "No passenger found with reservation number " << reservationNumber << endl << endl;
					break;
				}

				cout << "Found passenger with reservation number " << reservationNumber << endl << endl;
				cout << "----" << endl;
				cout << itt->second << endl;
				cout << "----" << endl << endl;
			}
			break;
			
			//searching by passenger name
			case '2':
			{
				string firstName;
				string lastName;
				cout << "Enter the passenger's firsts name: ";
				cin >> firstName;
				cout << endl << "Enter the passenger's last name: ";
				cin >> lastName;
				cout << endl << endl;

				bool foundPassenger = false;
				for(std::map<size_t,Passenger>::iterator itt = passengers.begin(); itt != passengers.end(); ++itt)
				{
					//if we find someone with the same first and last name as the user inputted
					if(caseInsensitiveStringEquals(itt->second.getFirstName(),firstName)
						&& caseInsensitiveStringEquals(itt->second.getLastName(),lastName))
					{
						if(!foundPassenger)
							cout << "Passengers with the name " << firstName << " " << lastName << ": " << endl << endl;
						
						cout << itt->second << endl;
						foundPassenger = true;
					}
				}

				if(!foundPassenger)
					cout << "No passengers found with the name " << firstName << " " << lastName << endl;

				cout << endl;
			}
			break;
			//search by first name
			case '3':
			{
				string firstName;
				cout << "Enter the first name of a passenger to find: ";
				cin >> firstName;
				cout << endl;

				bool foundPassenger = false;
				for(std::map<size_t,Passenger>::iterator itt = passengers.begin(); itt != passengers.end(); ++itt)
				{
					//if we find someone with the same first and last name as the user inputted
					if(caseInsensitiveStringEquals(itt->second.getFirstName(),firstName))
					{
						if(!foundPassenger)
							cout << "Passengers with first name " << firstName << ": " << endl << endl;
						
						cout << itt->second << endl;
						foundPassenger = true;
					}
				}

				if(!foundPassenger)
					cout << "No passengers found with the first name " << firstName << endl;

				cout << endl;
			}
			break;
				//search by last name
			case '4':
			{
				string lastName;
				cout << "Enter the last name of a passenger to find: ";
				cin >> lastName;
				cout << endl;

				bool foundPassenger = false;
				for(std::map<size_t,Passenger>::iterator itt = passengers.begin(); itt != passengers.end(); ++itt)
				{
					//if we find someone with the same first and last name as the user inputted
					if(caseInsensitiveStringEquals(itt->second.getLastName(),lastName))
					{
						if(!foundPassenger)
							cout << "Passengers with last name " << lastName << ": " << endl << endl;
						
						cout << itt->second << endl;
						foundPassenger = true;
					}
				}

				if(!foundPassenger)
					cout << "No passengers found with the last name " << lastName << endl;

				cout << endl;
			}
			break;
		}
	}
	while(selection != '0');
}

bool ReservationSystem::caseInsensitiveStringEquals(const string& a, const string& b)
{
    size_t sz = a.size();
    if (b.size() != sz)
        return false;
    for (unsigned int i = 0; i < sz; ++i)
        if (tolower(a[i]) != tolower(b[i]))
            return false;
    return true;
}

// # 10
void ReservationSystem::flightSearch_Option()
{
	bool displayPassengers = false;
	char selection;
	do
	{
		cout << "Search Flight - Options" << endl;
		cout << "-----------------------" << endl;
		cout << "1. Search flight by city to city" << endl;
		cout << "2. Search flight by flight number" << endl;
		cout << "3. Toggle displaying passengers for a Flight " << (displayPassengers ? "(ON)" : "(OFF)") << endl;
		cout << "0. [B]ack" << endl;

		//Get selection 
		selection = getSubMenuChoice(3);

		switch (selection)
		{
			case '1':	
			{
				string city1;
				string city2;

				cout << "What is the departure city?" << endl;
				cin >> city1;
				uppercaseString(city1);

				while(cities.find(city1) == cities.end()) //not a valid city
				{
					cout << "Invalid departure city entered" << endl;
					cout << "What is the departure city?" << endl;
					cin >> city1;
					uppercaseString(city1);
				}

				cout << "What is the destination city?" << endl;
				cin >> city2;
				uppercaseString(city2);

				while(cities.find(city2) == cities.end()) //not a valid city
				{
					cout << "Invalid departure city entered" << endl;
					cout << "What is the destination city?" << endl;
					cin >> city2;
					uppercaseString(city2);
				}
				cout << endl;

				cout << "List of flights with the following requirements:" << endl;
				bool foundAFlight = false;
				for (f_map::iterator iter = flights.begin(); iter != flights.end(); ++iter)
				{
					if (iter->second.getOrigin() == city1 && iter->second.getDestination() == city2)
					{
						cout << iter->second << endl;
						if(displayPassengers)
						{
							cout << "Passengers: " << endl;
							vector<size_t> manifest = iter->second.getManifest();
							for(int i = 0; i < manifest.size(); ++i)
								cout << passengers[manifest[i]] << endl;
							cout << endl;
						}
						foundAFlight = true;
					}
				}
				if(!foundAFlight)
					cout << "No flights found from " << city1 << " to " << city2 << endl;
				cout << endl;
			}
			break;

		case '2':
			{
			size_t flightNum;
			cout << "Enter flight number: ";
			cin >> flightNum;

			while(flightNum > flights.size())
			{
				cout << "Invalid flight number" << endl;
				cout << "Enter flight number: ";
				cin >> flightNum;
			}
			
			Flight fOb = flights[flightNum];
			cout <<  endl << fOb << endl << endl;
			if(displayPassengers)
			{
				cout << "Passengers: " << endl;
				vector<size_t> manifest = fOb.getManifest();
				for(int i = 0; i < manifest.size(); ++i)
					cout << passengers[manifest[i]] << endl;
				cout << endl;
			}
			}
			break;
		case '3':
			{
				displayPassengers = !displayPassengers;
				cout << "Displaying passengers with Flight data is now: " << (displayPassengers ?  "(ON)" : "(OFF)") << endl << endl;
			}
			break;
		}
	} while (selection != '0');
}

// # 1
void ReservationSystem::allPassengers_Option()
{
	p_map::iterator it;
	for (it = passengers.begin(); it != passengers.end(); it++)
	{
		cout << it->second << endl;
	}
	cout << endl;
}

// # 2
void ReservationSystem::pasByDestination_Option()
{
	string city;

	cout << "Please enter the destination city to check flights of: ";
	cin >> city;
	uppercaseString(city);
	cout << endl;

	while(cities.find(city) == cities.end())
	{
		cout << "Invalid city entered" << endl;
		cout << "Please enter the destination city to check flights of: ";
		cin >> city;
		uppercaseString(city);
		cout << endl;
	}

	cout << "All passengers going to " << city << ": " << endl;
	for (f_map::iterator it = flights.begin(); it != flights.end(); ++it)
	{
		if (it->second.getDestination() == city)
		{
			vector<size_t> manifest = it->second.getManifest();
			for (size_t i = 0; i < manifest.size(); ++i)
			{
				cout << passengers[manifest[i]] << endl;
			}
		}
	}
	cout << endl;
}

void ReservationSystem::pasByDeparture_Option()
{
	string city;

	cout << "Please enter the departure city to check flights of: ";
	cin >> city;
	uppercaseString(city);
	cout << endl;

	while(cities.find(city) == cities.end())
	{
		cout << "Invalid city entered" << endl;
		cout << "Please enter the departure city to check flights of: ";
		cin >> city;
		uppercaseString(city);
		cout << endl;
	}
	
	cout << "All passengers leaving " << city << ": " << endl;
	for (f_map::iterator it = flights.begin(); it != flights.end(); ++it)
	{
		if (it->second.getOrigin() == city)
		{
			vector<size_t> manifest = it->second.getManifest();
			for (size_t i = 0; i < manifest.size(); ++i)
			{
				cout << passengers[manifest[i]] << endl;
			}
		}
	}
	cout << endl;
}

// # 4
void ReservationSystem::pasByFlight_Option()
{
	typedef p_map::iterator it_type;
	size_t flight_num;
	cout << "Enter the flight number to view it's passengers: ";
	cin >> flight_num;
	cout << endl;

	while(flight_num > flights.size() || flight_num < 0)
	{
		cout << "Invalid flight number" << endl;
		cout << "Enter the flight number to view it's passengers: ";
		cin >> flight_num;
		cout << endl;
	}

	for (it_type iterator = passengers.begin(); iterator != passengers.end(); ++iterator)
	{
		if (iterator->second.getFlightNumber() == flight_num)
		{
			cout << iterator->second << endl;
		}
		//else
		//Not On This Flight
	}
	cout << endl;
}

void ReservationSystem::pasOverbook_Option()
{
	bool foundFlight = false;
	for (size_t curFlight = 0; curFlight < flights.size(); curFlight++)
	{
		// Check if flight has left [reference time - departure time]
		if ( flights[curFlight].hasFlightLeft(input_time))
		{
			foundFlight = true;
			// Flight departed, get list of overbooked passengers
			deque<size_t> p_vec = flights[curFlight].getOverBooked();

			if(p_vec.empty())
				cout << "No overbooked passengers on flight " << curFlight << endl;

			for (size_t iterator = 0; iterator < p_vec.size(); iterator++)
				cout << passengers[p_vec[iterator]] << endl;
		}
	}

	if(!foundFlight)
		cout << "No flights have departed yet" << endl;

	cout << endl;
}

void ReservationSystem::pasWaiting_Option()
{
	bool foundFlight = false;
	for (size_t curFlight = 0; curFlight < flights.size(); curFlight++)
	{
		// Check if flight has left [reference time - departure time]
		if ( ! flights[curFlight].hasFlightLeft(input_time) )
		{
			foundFlight = true;

			// Flight hasn't left, get list of passengers waiting to board
			deque<size_t> p_vec = flights[curFlight].getOverBooked();

			if(p_vec.empty())
				cout << "No passengers waiting for flight " << curFlight << endl;

			for (size_t iterator = 0; iterator < p_vec.size(); iterator++)
				cout << passengers[p_vec[iterator]] << endl;
		}
	}

	if(!foundFlight)
		cout << "All flights have already left for the day" << endl;
	cout << endl;
}

// # 7
void ReservationSystem::allFlights_Option()
{
	for (f_map::iterator it = flights.begin(); it != flights.end(); ++it)
	{
		cout << it->second << endl << endl;
	}

	cout << endl;
}

// # 8
void ReservationSystem::flightsAndPas_Option()
{
	for (f_map::iterator it = flights.begin(); it != flights.end(); ++it)
	{
		cout << it->second << endl;
		cout << "Passengers: " << endl;

		vector<size_t> manifest = it->second.getManifest();
		for (size_t i = 0; i < manifest.size(); ++i)
		{
			cout << " " << passengers[manifest[i]] << endl;
		}
		cout << endl;
	}
	cout << endl;
}

void ReservationSystem::uppercaseString(std::string& s)
{
	std::transform(s.begin(), s.end(), s.begin(), toupper);
}

