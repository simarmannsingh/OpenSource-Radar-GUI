#include "ClientNetwork.h"


ClientNetwork::ClientNetwork(void)
{
	//create WSA data object
	WSADATA clientwsaData;

	//socket 
	ClientSocket = INVALID_SOCKET;

	//holds address information for the socket which is to be connected to
	struct addrinfo *result = NULL,
		*ptr = NULL,
		hints;
	
	// Initialise winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &clientwsaData);
	if (iResult != 0)
	{
		printf("WSAStartup failed with error : %d \n", iResult);
		exit(1);
	}

	//set address information
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;									// toggle between UDP and TCP
	

	iResult = getaddrinfo("127.0.0.1", DEFAULT_PORT, &hints, &result);
	if (iResult != 0)
	{
		printf("getaddrinfo failed with error : %d \n", iResult);
		WSACleanup();
		exit(1);
	}

	//attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next)
	{
		// Connect a SCOKET for connecting to server
		ClientSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

		if (ClientSocket == INVALID_SOCKET) {
			printf("socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			exit(1);
		}

		// Connect to server.
		iResult = connect(ClientSocket, ptr->ai_addr, (int)ptr->ai_addrlen);

		if (iResult == SOCKET_ERROR)
		{
			closesocket(ClientSocket);
			ClientSocket = INVALID_SOCKET;
			printf("The server is down... did not connect");
		}
	}


	// no longer need address info for server
	freeaddrinfo(result);



	// if connection failed
	if (ClientSocket == INVALID_SOCKET)
	{
		printf("Unable to connect to server!\n");
		WSACleanup();
		exit(1);
	}

	// Set the mode of the socket to be nonblocking
	u_long iMode = 1;

	iResult = ioctlsocket(ClientSocket, FIONBIO, &iMode);
	if (iResult == SOCKET_ERROR)
	{
		printf("ioctlsocket failed with error: %d\n", WSAGetLastError());
		closesocket(ClientSocket);
		WSACleanup();
		exit(1);
	}

	//disable nagle
	char value = 1;
	setsockopt(ClientSocket, IPPROTO_TCP, TCP_NODELAY, &value, sizeof(value));


}

ClientNetwork::~ClientNetwork(void)
{
}
