#pragma once

#include <WinSock2.h>
#include <Windows.h>


class NetworkServices
{
public:
	static int sendMessage(SOCKET cursocket, char * message, int messageSize);
	static int receiveMessage(SOCKET cursocket, char * buffer, int bufSize);
	
};