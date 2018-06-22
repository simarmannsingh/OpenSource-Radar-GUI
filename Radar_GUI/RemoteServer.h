
#include "ServerNetwork.h"
#include "NetworkData.h"

class RemoteServer
{

public:
	// id's to assign clients for our table
	static unsigned int client_id; 
	RemoteServer(void);
	~RemoteServer(void);

	void receiveFromClients();
	void update();
	void sendActionPackets();


private:

	// IDs for the clients connecting for table in ServerNetwork 
	//static unsigned int client_id;

	// The ServerNetwork object 
	ServerNetwork* network;
	// data buffer
	char network_data[MAX_PACKET_SIZE];
};