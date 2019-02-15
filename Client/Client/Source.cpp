/*
Author: Pindrought
Date: 2/21/2016
This is the solution for the client that you should have at the end of tutorial 5.
*/
#include "Client.h"

#define SERVER_IP "192.168.0.112"
#define SERVER_PORT 27015

//File file;
Client myClient(SERVER_IP, SERVER_PORT);

int main()
{
	if (!myClient.Connect())
	{
		std::cout << "Failed to connect to server" << std::endl;
		system("pause");
		return 1;
	}

	std::string userinput; //holds the user's chat message
	while (1)
	{
		std::getline(std::cin, userinput); //Get line if user presses enter and fill the buffer
		if (!myClient.SendString(userinput)) //Send string: userinput, If string fails to send... (Connection issue)
			break;
		Sleep(10);
	}
	//startFileRecv();

	return 0;
}
