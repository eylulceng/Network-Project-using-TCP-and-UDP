/*
						
			UDP - Server - Test Program				
						
												*/




#include "pch.h"
#include <iostream>
#include <WS2tcpip.h> // necessary for winSock
#pragma comment (lib, "ws2_32.lib")
using namespace std;

int main(void)

{

	// Start WinSock
	WSADATA data; // winsock data
	WORD word = MAKEWORD(2, 2);
	int isWsOk = WSAStartup(word, &data);
	if (isWsOk != 0)
	{
		cout << "WinSock cannot be started !" << endl;
		return -1; // it is fatal so exit
	}
	
	// socket creation
	// Bind socket to ip adress and port
	SOCKET in = socket(AF_INET, SOCK_DGRAM, 0); // creating socket
	// AF_INET is used for version 4 (IPv4)
	// SOCK_DGRAM is used, because it is UDP
	sockaddr_in serverHint;
	serverHint.sin_addr.S_un.S_addr = ADDR_ANY; // give me any address
	serverHint.sin_family = AF_INET;
	serverHint.sin_port = htons(54000); // little to big endian
	//host to network short


	if (bind(in, (sockaddr*)&serverHint, sizeof(serverHint)) == SOCKET_ERROR)
	{
		cout << "Socket cannot be binded !" << WSAGetLastError() << endl;
		return -1; // program ends unsuccessfully
	}

	// check out is client okey
	// client connects to the server
	sockaddr_in client;
	int lenghtOfClient = sizeof(client);
	ZeroMemory(&client, sizeof(client));

	// where the message received into
	char message[1024];
	
	// program waits for client
	while (true)
	{
		ZeroMemory(message, 1024); // we want to message's memory is 0 every time

		// wait message
		int bytesIn = recvfrom(in, message, 1024, 0, (sockaddr*)&client, &lenghtOfClient);
		if (bytesIn == SOCKET_ERROR)
			cout << "Error received from client " << WSAGetLastError() << endl;

		// display message and information of client
		char clientIP[256];
		ZeroMemory(clientIP, 256);

		inet_ntop(AF_INET, &client.sin_addr, clientIP, 256); // to get client information
		cout << "Message is received from " << clientIP << " : " << message << endl;
	}

	closesocket(in); // close socket
	WSACleanup(); // close winsock

	return 0;

}

