
#include "Passenger.h"

Passenger::Passenger()
{
	firstName = "";
	lastName = "";
	Membership = "";
	reservationNumber = 0;
	flightNumber = -1;
}

Passenger::Passenger(string afirstName, string alastName, string aMembership, size_t areservationNumber, int aflightNumber) :
	firstName(afirstName), lastName(alastName), Membership(aMembership), reservationNumber(areservationNumber), flightNumber(aflightNumber)
{
}

ostream& operator<<(ostream& out , const Passenger& aPassenger)
{
	out << aPassenger.getFirstName() << "   " << aPassenger.getLastName() << "   " << aPassenger.getMembership()
		<< "   " << aPassenger.getFlightNumber() << endl;
	return out;

}

istream& operator>>(istream& in, Passenger& aPassenger)
{
	int iBuf;
	string buffer, temp;
	string delimiter = " ";  // Remove trailing whitespace

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

	return in;
}
