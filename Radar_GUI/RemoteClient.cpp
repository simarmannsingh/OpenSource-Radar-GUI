//#include "stdafx.h"
#include "RemoteClient.h"
#include <time.h>

RemoteClient::RemoteClient()
{
	network = new ClientNetwork();
	Packet Txpacket;

	// To send data packets, declare their parameters
	const unsigned int packet_size = sizeof(Txpacket);
	
	char packet_data[packet_size];
	
	printf("Packet structure details :\n");
	printf("size of unsigned int : %d \n",sizeof(unsigned int));
	printf("size of unsigned short int : %d \n", sizeof(unsigned short int));
	printf("size of bool : %d \n", sizeof(bool));


	//initialize the data members 
	/*
	srand(time(NULL));
	packet.packet_type	= rand() % 4294967295;
	packet.Angle		= rand() % 65535;
	packet.range		= rand() % 65535;
	packet.Direc = 0;
	*/

	Txpacket.amplitude = 526;
	Txpacket.Angle = 45;
	Txpacket.range = 9;
	Txpacket.Direc = 0;
	
	printf("Size of packet structure is : %d\n", sizeof(Txpacket));
	printf("packet structure consists of : %d \n", packet_data);

	Txpacket.serialize(packet_data);			//Serialize is used to convert packet type data into byte that can be send over Network.
	
	NetworkServices::sendMessage(network->ClientSocket, packet_data, packet_size);
	printf("----------------------------------------------------------\n");
	printf("Data SENT is :-\n");
	printf("Amplitude : %d\n", Txpacket.amplitude);
	printf("Angle     : %d\n", Txpacket.Angle);
	printf("Range     : %d\n", Txpacket.range);
	printf("Direction : %d\n", Txpacket.Direc);
	printf("----------------------------------------------------------\n");


}
