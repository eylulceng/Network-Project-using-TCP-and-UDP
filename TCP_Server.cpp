

#include "pch.h"
#include <iostream>
#include <string>
#include <WS2tcpip.h>
#pragma comment (lib, "ws2_32.lib")
using namespace std;

int main(void)
{

	// start WinSock
	WSADATA data;
	WORD version = MAKEWORD(2, 2); // to get the version 2.2

	int isWsOK = WSAStartup(version, &data);
	
	if (isWsOK != 0)
	{
		cout << "WinSock cannot be started !" << endl;
		return -1; // program ends unsuccessfully
	}

	// create the socket
	SOCKET socketListening = socket(AF_INET, SOCK_STREAM, 0);
	if (socketListening == INVALID_SOCKET)
	{
		cout << "Socket cannot be created !" << endl;
		return -1;
	}
	//

	// bind the IP and port
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(54000); // host to network short
	// 54000 is listening port
	hint.sin_addr.S_un.S_addr = INADDR_ANY; // to get any address

	bind(socketListening, (sockaddr*)&hint, sizeof(hint)); // bind our socket 'listen'
	//

	// socket listening
	listen(socketListening, SOMAXCONN);

	// wait for connection
	sockaddr_in client;
	int clientSize = sizeof(client);

	SOCKET clientSocket = accept(socketListening, (sockaddr*)&client, &clientSize);// listening for connection
	if (clientSocket == INVALID_SOCKET)
	{
		cout << "Client ERROR!" << endl;
	}
	
	
	char host[NI_MAXHOST];				// Client's remote name
	char service[NI_MAXHOST];			// Service the client is connected on

	ZeroMemory(host, NI_MAXHOST);
	ZeroMemory(service, NI_MAXHOST);
	
	// check if we can get the host name information
	if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0) // DNS lookup for the client info
	{
		cout << host << "	connected on port " << service << endl;
	}
	else
	{
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		cout << host << "	connected on port " << ntohs(client.sin_port) << endl;
	}

	closesocket(socketListening); 	// close listening socket

	// while loop : recieve and echo the message to the client
	char message[4096];

	while (true)
	{
		ZeroMemory(message, 4096);
		
		// wait client to send a data
		int bytesReceived = recv(clientSocket, message, 4096, 0);
		if (bytesReceived == SOCKET_ERROR)
		{
			cout << "ERROR in recv() !" << endl;
			break;
		}
		if (bytesReceived == 0)
		{
			cout << "client is disconnected ! " << endl;
			break;
		}
		cout << string(message, 0, bytesReceived) << endl;


		// echo the message
		send(clientSocket, message, bytesReceived + 1, 0);	//otherwise send our info back

	}

	closesocket(clientSocket);	// close a socket
	WSACleanup();				// close WinSock

	
	return 0;					// program ends successfully
}

