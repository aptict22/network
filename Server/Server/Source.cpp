#include "Server.h"

void command();

Server server(PORT);

int main()
{

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)command, NULL, NULL, NULL);
	for (int i  = 0; i < 100; i++)
	{
		server.ListenForNewConnection();
	}
	system("pause");
	return 0;
}

void command()
{
	std::string com;
	std::string Path;
	while (true)
	{
		std::getline(std::cin, com);
		if (com == "test")
		{
			std::cout << "Set File path: " << std::endl;
			std::getline(std::cin, Path);
			int con = server.getTotalConnections();
			for (int i = 0; i < con; i++)
			{
				server.SendFile(i, Path);
			}
		}
	}
}