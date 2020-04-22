/** 
* 2020.4.7 eastroot
*
* Communicator
* send alert packet to server and send recorded file to server
*/

#pragma once

// #include <SD.h>

struct Alert
{
	char MLOID[8];
	char PacketID;
	char AlertType;
};

class Communicator
{
public:
	~Communicator();

	static Communicator* GetInstance();
	static void ReleaseInstance();

	bool Initialize(const char* Id, const char* Password);

	void Alert(int Type);
	void SendRecord();

private:
	Communicator();

	static Communicator* Instance;

};