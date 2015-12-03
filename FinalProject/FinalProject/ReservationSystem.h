#pragma once

class ReservationSystem
{
public:
	static ReservationSystem& getInstance()
	{
		static ReservationSystem instance;	// Guaranteed to be destroyed.
											// Instantiated on first use.

		return instance;
	}

private:
	ReservationSystem() {}
	~ReservationSystem() {}
	ReservationSystem(const ReservationSystem &)	= delete;
	void operator= (const ReservationSystem &)		= delete;

};
