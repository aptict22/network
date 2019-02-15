#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib,"ws2_32.lib")
#include <WinSock2.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#define PORT 27015
#define BUFSIZE 32768
#define SERVER_IP "192.168.0.112"

enum Packet
{
	P_ChatMessage,
	P_File,
	P_Test
};


class Server
{
public:
	Server(int port, bool BroadcastPublically = false);
	bool ListenForNewConnection();
	bool SendString(int ID, std::string & _string);
	int getTotalConnections() const;
	bool SendFile(int ID, std::string _string);
private:
	bool SendInt(int ID, int _int);
	bool GetInt(int ID, int & _int);

	bool SendPacketType(int ID, Packet _packettype);
	bool GetPacketType(int ID, Packet & _packettype);

	//bool SendString(int ID, std::string & _string);
	bool GetString(int ID, std::string & _string);

	
	static void ClientHandlerThread(int ID);

	bool ProcessPacket(int ID, Packet _packettype);

private:
	SOCKET Connections[100];
	int TotalConnections = 0;
	SOCKADDR_IN addr; //Address that we will bind our listening socket to
	int addrlen = sizeof(addr); //length of the address (required for accept call)

	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);
};

static Server * serverptr;