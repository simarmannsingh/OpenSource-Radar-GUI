#include "NetworkServices.h"

int NetworkServices::sendMessage(SOCKET cursocket, char * message, int messageSize)
{
	return send(cursocket, message, messageSize, 0);
}

int NetworkServices::receiveMessage(SOCKET cursocket, char * buffer, int bufSize)
{
	return recv(cursocket, buffer, bufSize, 0);

}
