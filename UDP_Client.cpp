/*

			UDP - Client - Test Program

												*/
#include "pch.h"
#include <iostream>
#include <WS2tcpip.h> // necessary for winSock
#pragma comment (lib, "ws2_32.lib")
#include <string>
using namespace std;

int main(int argc, char* argv[]) // we can pass in a command line option

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

	// create a hint structure for server
	sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(54000);

	inet_pton(AF_INET, "127.0.0.1", &server.sin_addr);

	// socket creation
	SOCKET out = socket(AF_INET, SOCK_DGRAM, 0);

	// write out to that socket
	string arg(argv[1]);
	int isSendOk = sendto(out, arg.c_str(), arg.size() + 1, 0, (sockaddr*)&server, sizeof(server));

	if (isSendOk == SOCKET_ERROR)
	{
		cout << "Socket cannot be opened !" << WSAGetLastError() << endl;
		return -1; // program ends unsuccessfully
	}

	closesocket(out); 	// close the socket
	WSACleanup(); // close winsock

	return 0; // program ends successfully
}

