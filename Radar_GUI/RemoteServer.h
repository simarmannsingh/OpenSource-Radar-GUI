/*
 -------------------------------------------------------------------------------------------------------------------------------------------
 Author			:	Simar Mann Singh
 Department		:	High Frequency Department, TF
 Date			:	28/08/2018
 -------------------------------------------------------------------------------------------------------------------------------------------
*/

// Please refer to RemoteServer.cpp for documentation

#include "ServerNetwork.h"
#include "NetworkData.h"

class RemoteServer{
public:
	// id's to assign clients for our table
	unsigned int client_id;
	RemoteServer(void);
	~RemoteServer(void);

	void receiveFromClients();
	void update();
	void sendActionPackets();
	// IDs for the clients connecting for table in ServerNetwork 
	//static unsigned int client_id;

	// The ServerNetwork object 
	ServerNetwork* svnetwork;
	// data buffer
	char network_data[MAX_PACKET_SIZE];
};

