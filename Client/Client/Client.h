#pragma once
#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <stdlib.h>

enum Packet
{
	P_ChatMessage,
	P_File,
	P_Test
};

class Client
{
public:
	Client(std::string IP, int PORT);
	bool Connect();
	bool CloseConnection();
	SOCKET getSocket();
	bool SendString(std::string & _string);
private:
	bool ProcessPacket(Packet _packettype);
	static void ClientThread();

	bool SendInt(int _int);
	bool SendPacketType(Packet _packettype);

	bool GetFile();
	void startFileRecv();

	bool GetInt(int & _int);
	bool GetPacketType(Packet & _packettype);
	bool GetString(std::string & _string);

private:
	SOCKET Connection;
	SOCKADDR_IN addr;
	int sizeofaddr = sizeof(addr);
	std::ofstream fout;
};

static Client * clientptr;