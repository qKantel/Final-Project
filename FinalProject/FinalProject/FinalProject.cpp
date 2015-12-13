
/*
*	group		Left Group
*	class		CS M20 - Chetlen
*	project		Final Group Project - Airplane Reservation System
*	file		FinalProject.cpp
*	status		Complete
*/

/* Includes */
#include <fstream>
#include <iostream>
#include <ctime>
#include "ReservationSystem.h"

int main()
{
	bool valid_time = false;
	string time_str;
	// Create time variables
	time_t currentTime;
	tm timeReference;

	// Cleans the struct to have empty values
	time(&currentTime);
	localtime_s(&timeReference, &currentTime);

	timeReference.tm_sec = 0;

	while (!valid_time) // Wait for user to enter proper time
	{
		cout << "Please input the time at which you wish to check the state of the flights." << endl;
		cout << "The time format is (HH:MM) (Type E or Q to quit): ";

		getline(cin, time_str);

		// Exit variables
		if (time_str == "e" || time_str == "E" || time_str == "q" || time_str == "Q")
			exit(0);
		if (time_str.size() == 5)
		{
			try
			{
				// Attempt to take user time
				timeReference.tm_hour = stoi(time_str.substr(0, 2));
				timeReference.tm_min = stoi(time_str.substr(3, 2));
				valid_time = true;
			}
			catch (invalid_argument)
			{
				cout << "That was an invalid time." << endl;
			}
		}
		else if (time_str == "united")
		{
			string input;

			while (true)
			{
				cout << endl << "Welcome to the United Airlines Flight Delay Query System!" << endl;
				cout << "Please enter a flight number to check if it has been delayed or enter \'E\' to exit: ";
				cin >> input;
				cout << endl;
				if (input == "E" || input == "e")
					break;

				cout << "Flight " << input << " has been INDEFINTELY delayed due to: {INSERT_REASON}" << endl;

			}
		}
		else // invalid time input
		{
			cout << "That was an invalid time." << endl;
		}
	}

	ReservationSystem system(timeReference);

	//Run the main menu until it tells us to quit
	while (system.runMenu());

	return 0;
}
