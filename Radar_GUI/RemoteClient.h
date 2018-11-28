#pragma once
#include <winsock2.h>
#include <Windows.h>
#include "ClientNetwork.h"
#include "NetworkData.h"

static Packet Txpacket;

static ClientNetwork* Clnetwork;	

class RemoteClient
{
public:	
	
	// Constructor / Destructor
	RemoteClient();
	~RemoteClient(void);	

	void sendData(ClientNetwork* network);
};

