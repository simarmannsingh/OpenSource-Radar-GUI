
#include "RemoteServer.h"
#include "NetworkData.h"

// id's to assign clients for our table
unsigned int RemoteServer::client_id;


RemoteServer::RemoteServer(void)
{
	
	// id's to assign clients for our table
	client_id = 0;

	// set up the server network to listen 
	network = new ServerNetwork();

}

RemoteServer::~RemoteServer(void)
{
}

void RemoteServer::receiveFromClients()
{
	Packet Rxpacket;

	// go through all clients
	
	std::map<unsigned int, SOCKET>::iterator iter;
	
	for (iter = network->sessions.begin(); iter != network->sessions.end(); iter++)
	{
		// get data for that client. 'data_length' contains the number of bytes received from the incomming socket.
		int data_length = network->receiveData(iter->first, network_data);
		
															// network_data is a pointer to the buffer to receive incoming data
		if (data_length <= 0)
		{
			//no data recieved
			continue;
		}
		else
		{ 
			printf("\nSize of Data_length : %d\n", data_length);
			Rxpacket.deserialize(network_data);
			
			printf("----------------------------------------------------------\n");
			printf("Data RECEIVED is :-\n");
			printf("Amplitude : %d\n", Rxpacket.amplitude);
			printf("Angle     : %d\n", Rxpacket.Angle);
			printf("Range     : %d\n", Rxpacket.range);
			printf("Direction : %d\n", Rxpacket.Direc);
			printf("----------------------------------------------------------\n");
			
		  
		}
 
	}

}

void RemoteServer::update()
{

	// get new clients
	if (network->acceptNewClient(client_id))
	{
		printf("client %d has been connected to the server\n", client_id);
		
		//moving on to the next client
		//client_id++;   
	}
	receiveFromClients();

}

void RemoteServer::sendActionPackets()
{
	// send action packet
	const unsigned int packet_size = sizeof(Packet);
	char packet_data[packet_size];

	Packet packet;
	packet.amplitude = 0;
	packet.serialize(packet_data);

	network->sendToAll(packet_data, packet_size);
}