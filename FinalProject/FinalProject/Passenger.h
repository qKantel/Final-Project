#include <string>

using namespace std;

class passenger
{
	friend ostream &operator<<(ostream &, const passenger &);
	friend istream &operator>>(istream&, passenger& );
private:
	string firstName;
	string lastName;
	string Membership;
	int reservationNumber;
	int flightNumber;
public:
	
	passenger();
	passenger(string afirstName, string alastName, string aMembership, int areservationNumber, int aflightNumber);

	string getFirstName() const { return firstName; }
	string getLastName() const { return lastName; }
	string getMembership() const { return Membership; }
	int getReservationNumber() const { return reservationNumber; }
	int getFlightNumber() const { return flightNumber; }

	void setFirstName(string afirstName) { firstName = afirstName; }
	void setLastName(string alastName) { lastName = alastName; }
	void setMembership(string aMembership) { Membership = aMembership; }
	void setReservationNumber(int areservationNumber) { reservationNumber = areservationNumber; }
	void setflightNumber(int aflightNumber) { flightNumber = aflightNumber; }


};
