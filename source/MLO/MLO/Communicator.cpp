/**
* 2020.4.5 eastroot
*/

#include "Communicator.h"

Communicator* Communicator::Instance = nullptr;

Communicator* Communicator::GetInstance()
{
	if (Instance == nullptr)
	{
		Instance = new Communicator();
	}

	return Instance;
}

void Communicator::ReleaseInstance()
{
	if (Instance != nullptr)
	{
		delete Instance;
		Instance = nullptr;
	}
}

Communicator::~Communicator()
{

}

void Communicator::Alert(int Type)
{
	// make packet

	// send

}

void Communicator::SendRecord()
{
	// make packet

	// upload
	// note : can open multi file? how about make file manager?

}
