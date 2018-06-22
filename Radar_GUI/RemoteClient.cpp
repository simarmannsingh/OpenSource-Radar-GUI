//#include "stdafx.h"
#include "RemoteClient.h"


RemoteClient::RemoteClient()
{
	network = new ClientNetwork();
	
	// send init packet
	const unsigned int packet_size = sizeof(Packet);
	char packet_data[packet_size];

	Packet packet;
	packet.packet_type = INIT_CONNECTION;

	packet.serialize(packet_data);						//Serialize is used to convert packet type data into byte tht can be send over Network.

	NetworkServices::sendMessage(network->connectSocket, packet_data, packet_size);

}
