//#include "stdafx.h"
#include "RemoteClient.h"
#include <time.h>
#include <cstdlib>

RemoteClient::RemoteClient()
{	
	Clnetwork = new ClientNetwork();

	printf("Packet structure details :\n");
	printf("size of Amplitude (unsigned int)   : %d \n", sizeof(unsigned int));
	printf("size of Range (unsigned int)       : %d \n", sizeof(unsigned int));
	printf("size of Angle (unsigned short int) : %d \n", sizeof(unsigned short int));
	printf("size of Direction (bool)           : %d \n", sizeof(bool));
	printf("Size of packet structure is : %d\n", sizeof(Packet));
}

RemoteClient::~RemoteClient(void)
{

}

void RemoteClient::sendData(ClientNetwork* network)
{
	const int packet_size = sizeof(Txpacket);
	char packet_data[packet_size];

	Txpacket.amplitude = rand() % 2000;
	Txpacket.range = rand() % 35;
	Txpacket.Angle = rand() % 10;
	Txpacket.Direc = false;
	   
	Txpacket.serialize(packet_data);			//Serialize is used to convert packet type data into byte that can be send over Network.

	NetworkServices::sendMessage(network->ClientSocket, packet_data, packet_size);
	printf("----------------------------------------------------------\n");
	printf("Data SENT is :-\n");
	printf("Amplitude : %d\n", Txpacket.amplitude);
	printf("Angle     : %d\n", Txpacket.Angle);
	printf("Range     : %d\n", Txpacket.range);
	printf("Direction : %s\n", Txpacket.Direc ? "true" : "false");
	printf("----------------------------------------------------------\n");

}

/*
// To send data packets, declare their parameters
static const int packet_size = sizeof(Txpacket);
static char* packet_data[packet_size];

*/