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

bool Communicator::Initialize(const char* Id, const char* Password)
{
	int Try = 0;

	// need #include <ESP8266WiFi.h>
	// WiFi is member of Communicator
	WiFi.begin(Id, Password);
	
	while (WiFi.status() != WL_CONNECTED)
	{
		Try++;

		delay(500);

		if (Try > 5)
		{
			return false;
		}
	}

	return true;
}

void Communicator::Alert(int Type)
{
	// make packet


	// send
	// nead #include <HTTPClient.h>
	if (WiFi.status() == WL_CONNECTED)
	{
		HTTPClient Client;

		Client.begin("server url");
		Client.addHeader("Content-Type", "text/plain");	// can be json?

		int RequestCode = Client.POST("body content. this case alert packet");

		if (RequestCode > 0)
		{
			// post success
		}

		Client.end();
	}
	else
	{
		// lost wifi connection.
		// TODO : retry
	}
}

void Communicator::SendRecord()
{
	// make packet

	// upload
	// note : can open multi file? how about make file manager?

}
