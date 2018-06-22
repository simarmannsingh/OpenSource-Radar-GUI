#pragma once
#include <winsock2.h>
#include <Windows.h>
#include "ClientNetwork.h"
#include "NetworkData.h"

class RemoteClient
{

public:

	RemoteClient();
	~RemoteClient(void);

	ClientNetwork* network;
};

