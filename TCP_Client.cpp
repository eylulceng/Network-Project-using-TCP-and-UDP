

#include "pch.h"
#include <iostream>
#include <WS2tcpip.h>
#include <string>
#pragma comment (lib, "ws2_32.lib")
using namespace std;

int main()
{
	const char* IPaddress = "127.0.0.1"; // localhost, IP address of the server
	int portNumber = 54000;			// listening server, listening port number on the server

	// start WinSock
	WSADATA data;
	WORD version = MAKEWORD(2, 2);
	int winsockResult = WSAStartup(version, &data); // if it returns 0, have no error
	if (winsockResult != 0) // 
	{
		cout << " Winsock cannot be started !" << endl;
		return -1; // program ends unsuccessfully
	}

	// create a socket
	SOCKET soc = socket(AF_INET, SOCK_STREAM, 0);
	if (soc == INVALID_SOCKET)
	{
		cout << "socket cannot be created !" << WSAGetLastError() << endl;
		WSACleanup();
		return -1; // program ends unsuccessfully
	}
	// fill in a hint structure
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(portNumber); // host to network
	inet_pton(AF_INET, IPaddress, &hint.sin_addr);


	// connect to server
	int connectResult = connect(soc, (sockaddr*)&hint, sizeof(hint));
	if (connectResult == SOCKET_ERROR)
	{
		cout << "Client CANNOT connect to server" << WSAGetLastError() << endl;
		closesocket(soc);	// close the socket	
		WSACleanup();		// close the winsock
		return -1;
	}
	// loop for send and recv data
	char message[4096];
	string userMessage;

	do
	{
		// make the user input a message
		cout << " > ";
		getline(cin, userMessage);

		if (userMessage.size() > 0)		// check user type anything
		{
			// send a message
			int sendResult = send(soc, userMessage.c_str(), userMessage.size() + 1, 0); // size() + 1 because of '\0'
			if (sendResult != SOCKET_ERROR)
			{
				// wait response
				ZeroMemory(message, 4096);
				int bytesReceived = recv(soc, message, 4096, 0);
				if (bytesReceived > 0) // echo
				{
					cout << "Server Responsing >" << string(message, 0, bytesReceived) << endl; // displaying message
				}
			}
		}


	} while (userMessage.size() > 0);

	closesocket(soc);	// close socket
	WSACleanup();		// close winsock

	return 0;			// program ends successfully
}

