/*
 -------------------------------------------------------------------------------------------------------------------------------------------
	Author			:	Simar Mann Singh
	Department		:	High Frequency Department, TF
	Date			:	28/08/2018
 -------------------------------------------------------------------------------------------------------------------------------------------
*/

#include "RemoteServer.h"

RemoteServer::RemoteServer(void)
{	
	// id's to assign clients for our table
	 client_id = 0;

	 // set up the server network to listen 
	 ServerNetwork*  svnetwork = new ServerNetwork();
	
}

RemoteServer::~RemoteServer(void)
{
}

void RemoteServer::receiveFromClients()
{
	
	// go through all clients
	Packet Rxpacket;
	std::map<unsigned int, SOCKET>::iterator iter;
	
	for (iter = svnetwork->sessions.begin(); iter != svnetwork->sessions.end(); iter++)
	{
		// get data for that client. 'data_length' contains the number of bytes received from the incomming socket.
		int data_length = svnetwork->receiveData(iter->first, network_data);
		
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
			printf("Direction : %s\n", Rxpacket.Direc ? "true" : "false");
			printf("----------------------------------------------------------\n");
			
		  
		}
 
	}

}

void RemoteServer::update()
{

	// get new clients
	if (svnetwork->acceptNewClient(client_id))
	{
		printf("client %d has been connected to the server\n", client_id);
		//moving on to the next client
		//client_id++;   
	}
	else
	{
		printf("No client connected\n");
	}
	//receiveFromClients();

}

void RemoteServer::sendActionPackets()
{
	// send action packet
	const unsigned int packet_size = sizeof(Packet);
	char packet_data[packet_size];

	
	Packet Rxpacket;
	Rxpacket.amplitude = 0;
	Rxpacket.serialize(packet_data);

	svnetwork->sendToAll(packet_data, packet_size);
}