
#include "ReservationSystem.h"

ReservationSystem * ReservationSystem::p_rsvs_instance_ = nullptr;

bool ReservationSystem::runMenu()
{
	bool valid_time = false;
	string time_str;
	while (!valid_time)
	{
		cout << "Please input your time reference (format HH:MM): ";
		getline(cin, time_str);
		// TODO, allow user to input 'e' to end
		if (time_str.size() == 5)
		{
			try
			{
				input_time.tm_hour = stoi(time_str.substr(0, 2));
				input_time.tm_min = stoi(time_str.substr(3, 2));
				valid_time = true;
			}
			catch (invalid_argument)
			{
				cout << "That was an invalid time." << endl;
			}
		}
		else // invalid time input
		{
			cout << "That was an invalid time." << endl;
		}
	}

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
	}

	if (selection == '0')
		return false;

	return true;
}

void ReservationSystem::closeMenu()
{
	// Checks if nullptr
	delete p_rsvs_instance_;
	p_rsvs_instance_ = nullptr;

	//endProgram(0);
}

void ReservationSystem::initialize()
{
	// TODO
	// Is this function all we need to initialize graphs?
	createGraph();


	cities.push_back("A");
	cities.push_back("B");
	cities.push_back("C");
	cities.push_back("D");
	cities.push_back("E");
	cities.push_back("F");
	cities.push_back("G");
	cities.push_back("I");
	cities.push_back("J");
	cities.push_back("L");
	cities.push_back("M");
	cities.push_back("N");
	cities.push_back("O");
	cities.push_back("P");

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

void ReservationSystem::createGraph()
{
	flightMap = new Graph<char, int>(14);

	flightMap->add(Edge<char, int>('A', 'B', 142));
	flightMap->add(Edge<char, int>('A', 'C', 170));
	flightMap->add(Edge<char, int>('C', 'D', 114));
	flightMap->add(Edge<char, int>('D', 'E', 93));
	flightMap->add(Edge<char, int>('D', 'M', 209));
	flightMap->add(Edge<char, int>('M', 'N', 208));
	flightMap->add(Edge<char, int>('N', 'P', 134));
	flightMap->add(Edge<char, int>('P', 'O', 193));
	flightMap->add(Edge<char, int>('E', 'F', 155));
	flightMap->add(Edge<char, int>('F', 'G', 184));
	flightMap->add(Edge<char, int>('F', 'I', 160));
	flightMap->add(Edge<char, int>('I', 'G', 83));
	flightMap->add(Edge<char, int>('I', 'L', 88));
	flightMap->add(Edge<char, int>('I', 'J', 73));
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
	cout << "0. [Q]uit" << endl;
	cout << endl;

}

char ReservationSystem::getMainMenuChoice()
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


// # 9, 11
// TODO
void ReservationSystem::passengerSearch_Option()
{

}

// # 10
void ReservationSystem::flightSearch_Option()
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
	// TODO
	// Input check this is a city
	string city;
	cout << "Please enter the destination city to check flights of: ";
	cin >> city;

	cout << "All passengers going to " << city << ": " << endl;
	for (f_map::iterator it = flights.begin(); it != flights.end(); ++it)
	{
		if (it->second.getDestination() == city)
		{
			vector<size_t> manifest = it->second.getManifest();
			for (size_t i = 0; i < manifest.size(); ++i)
			{
				cout << manifest[i] << endl;
			}
		}
	}
	cout << endl;
}

// # 3
void ReservationSystem::pasByDeparture_Option()
{
	// TODO
	// Input check this is a city
	string city;
	cout << "Please enter the departure city to check flights of: ";
	cin >> city;

	cout << "All passengers leaving " << city << ": " << endl;
	for (f_map::iterator it = flights.begin(); it != flights.end(); ++it)
	{
		if (it->second.getOrigin() == city)
		{
			vector<size_t> manifest = it->second.getManifest();
			for (size_t i = 0; i < manifest.size(); ++i)
			{
				cout << manifest[i] << endl;
			}
		}
	}
	cout << endl;
}

// # 4
// TODO
// Input check flight number (is size_t)
// Check if flight number exists
void ReservationSystem::pasByFlight_Option()
{
	typedef p_map::iterator it_type;
	size_t flight_num;
	cout << "Enter the flight number to view it's passengers: ";
	cin >> flight_num;
	for (it_type iterator = passengers.begin(); iterator != passengers.end(); iterator++)
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


// Difference between passengers overbooked
// and those waiting for a flight:
// passengers overbooked did NOT make the flight and the flight has already taken off/begun
// passengers waiting CAN make the flight because the time of the flight is in the future
// check ReservationSystem.h to see the structure the time is stored in

// # 5
// TODO
// Redo to check time according to flight class
void ReservationSystem::pasOverbook_Option()
{
	for (size_t curFlight = 0; curFlight != flights.size(); curFlight++)
	{
		vector<size_t> p_vec = flights[curFlight].getManifest();
		for (size_t iterator = 40; iterator >= p_vec.size(); iterator++)
			passengers[p_vec[iterator]];
	}
	cout << endl;
}

// # 6
// TODO
// Redo time check according to new Flight class
void ReservationSystem::pasWaiting_Option()
{
	for (size_t curFlight = 0; curFlight != flights.size(); curFlight++)
	{
		/*
		if (flights[curFlight].getDepartTime() == input_time)//current time
		{
			vector<size_t> passengers = flights[curFlight].getManifest();
			for (size_t iterator = 0; iterator >= passengers.size(); iterator++)
				passengers[passengers[iterator]];
		}
		*/
	}
	cout << endl;
}

// # 7
void ReservationSystem::allFlights_Option()
{
	for (f_map::iterator it = flights.begin(); it != flights.end(); ++it)
	{
		cout << it->second << endl;
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
