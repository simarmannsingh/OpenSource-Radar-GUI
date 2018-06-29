#pragma once
#include <string.h>

#define MAX_PACKET_SIZE 12   //Use this with caution, This is number of 'Bytes'


/*create bitfields for custom bit length data type
Our payload consists of 
1. Amplitude  : 26 bits
2. Rangle cell: 16 bits
3. Angle      :  9 bits
4. Direction  :  3 bits

*/


struct Packet {

	//total size of the variables = 9 bytes
	unsigned int amplitude;				//size : 4 bytes
	unsigned short int range;			//size : 2 bytes
	unsigned short int Angle;			//size : 2 bytes
	bool Direc;							//size : 1 byte


	void serialize(char * data) {
		memcpy(data, this, sizeof(Packet));
	}

	void deserialize(char * data) {
		memcpy(this, data, sizeof(Packet));
	}
};

