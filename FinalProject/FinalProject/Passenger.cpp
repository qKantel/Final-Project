/*
*	file	Passenger.cpp
*	status	Complete
*/
#include "Passenger.h"

Passenger::Passenger()
{
	firstName = "";
	lastName = "";
	Membership = "";
	reservationNumber = -1;
	flightNumber = -1;
}

Passenger::Passenger(string afirstName, string alastName, string aMembership, size_t areservationNumber, int aflightNumber) :
	firstName(afirstName), lastName(alastName), Membership(aMembership), reservationNumber(areservationNumber), flightNumber(aflightNumber)
{
}

ostream& operator<<(ostream& out, const Passenger& aPassenger)
{
	//old way of displaying a passenger. Left in here so it doesnt get repeated
	//	out << aPassenger.getReservationNumber() << "   " << aPassenger.getFirstName() << "   " << aPassenger.getLastName() << "   " << aPassenger.getMembership()
	//		<< "   " << aPassenger.getFlightNumber();


	out << aPassenger.getFirstName() << " " << aPassenger.getLastName() << endl;
	out << "   Reservation #" << aPassenger.getReservationNumber() << endl;
	out << "   " << aPassenger.getMembership() << " on Flight #" << aPassenger.getFlightNumber();
	return out;

}