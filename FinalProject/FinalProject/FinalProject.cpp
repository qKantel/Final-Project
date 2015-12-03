
#include "ReservationSystem.h"

int main()
{
	while (ReservationSystem::getInstance().runMenu()) {}
	ReservationSystem::getInstance().closeMenu();

	return 0;
}
