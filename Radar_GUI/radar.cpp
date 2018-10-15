/*
-------------------------------------------------------------------------------------------------------------------------------------------
 Author			:	Simar Mann Singh
 Department		:	High Frequency Department, TF
 Date			:	14/06/2018
 References		:		1. Intro to Computer Science (Link : https://www.youtube.com/watch?v=kwxCP_LLZJ4&t=328s)
						2. TCP socket programming (Link : https://msdn.microsoft.com/en-us/library/windows/desktop/ms737591(v=vs.85).aspx )

-------------------------------------------------------------------------------------------------------------------------------------------
*/


//#include "stdafx.h
#include "RemoteServer.h"
#include <iostream>
#include <sstream>
#include "Logger.h"
#include "RemoteClient.h"
#include <process.h>
#include "linmath.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include "graphics.h"


using namespace std;
using namespace CPlusPlusLogging;

#pragma once
#define WIN32_LEAN_AND_MEAN



/*constants used in the program*/
#define MAXRECVSTRING 4294967296
#define DEFAULT_PORT "27015"
#define DEFAULT_BUFLEN 64

struct sockaddr_in ServAddr;
struct sockaddr_in broadcastAddr; /* Broadcast Address */
void serverLoop();
void clientLoop(void);


RemoteServer * server;
RemoteClient * client;

GLFWwindow* window;
/*
void backgroundDisplay(void) 
{
	//glClear(GL_COLOR_BUFFER_BIT); // | GL_DEPTH_BUFFER_BIT);
	//glColor3f(0.712, 0.767, 0.431);
	glBegin(GL_LINES);
	for (int angle = 0; angle <= 360; angle += 1)
	{
		
		if (angle % 30 == 0)
		{
			glLineWidth(2.5);

			glVertex2f(0, 0);
			glVertex2f(0 / 10 + (0 / 200 * angle), 0 / 10 + (0 / 200 * angle));

		}

	}
	glEnd();
}

//____________________________________________________________________________________________________________________________

void ppi_display(void){
	
	
	glfwSwapInterval(1);

	//setup view
	float ratio;
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	ratio = width / (float)height;
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT);

	/*
	//Color
	r = fmod(r + 0.01, 1);
	g = fmod(g + 0.02, 1);
	b = fmod(b + 0.03, 1);
	//
	//Horizontal lines
	glColor3f(r, g, b);
	for (int i = -10; i < 11; i++) {
		glLineWidth(0.2);
		glBegin(GL_LINE_STRIP);
		glVertex2f(-0.96, (i * 0.1));
		glVertex2f(0.96, (i * 0.1));
		glEnd();
	}

	//Vertical lines
	for (int i = -10; i < 11; i++) {
		glLineWidth(0.2);
		glBegin(GL_LINE_STRIP);
		glVertex2f((i * 0.1), -0.96);
		glVertex2f((i * 0.1), 0.96);
		glEnd();
	}
	glColor3f(1, 1, 1);
	//Drawing
	//glColor3f(r, g, b);
	//glBegin(GL_LINE_STRIP);											// GL_LINE_STRIP or GL_POLYGON defines whether the shape drawn would be a line shape or a filled polygon
	for (int i = 0; i < 360; i++) {
		float degInRad = i * DEG2RAD;
		//glVertex2f(cos(degInRad)*radius, sin(degInRad)*radius);
	}
	glEnd();


	glBegin(GL_LINE_STRIP);												// GL_LINE_STRIP or GL_POLYGON defines whether the shape drawn would be a line shape or a filled polygon
	for (int i = 0; i < 360; i++) {
		float degInRad = i * DEG2RAD;
		glVertex2f(cos(degInRad)*(radius / 2), sin(degInRad)*(radius / 2));
	}
	glEnd();

	glBegin(GL_LINE_STRIP);											// GL_LINE_STRIP or GL_POLYGON defines whether the shape drawn would be a line shape or a filled polygon
	for (int i = 0; i < 360; i++) {
		float degInRad = i * DEG2RAD;
		glVertex2f(cos(degInRad)*(0.72), sin(degInRad)*(0.72));
	}
	glEnd();

	glBegin(GL_LINE_STRIP);											// GL_LINE_STRIP or GL_POLYGON defines whether the shape drawn would be a line shape or a filled polygon
	for (int i = 0; i < 360; i++) {
		float degInRad = i * DEG2RAD;
		glVertex2f(cos(degInRad)*(radius / 4), sin(degInRad)*(radius / 4));
	}
	glEnd();

	glBegin(GL_POLYGON);
	for (int i = 0; i < 360; i++) {
		float degInRad = i * DEG2RAD;
		glVertex2f(cos(degInRad)*(0.01), sin(degInRad)*(0.01));
	}
	glEnd();



	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glBegin(GL_LINE_STRIP);
	glVertex2f(0, -0.96);
	glVertex2f(0, 0.96);
	glEnd();

	glBegin(GL_LINE_STRIP);
	glVertex2f(-0.96, 0);
	glVertex2f(0.96, 0);
	glEnd();
	//backgroundDisplay();							// to display the checked background

													// Swap front and back buffers 
	glfwSwapBuffers(window);
	glfwPollEvents();

	// Loop until the user closes the window 
	
	glfwDestroyWindow(window);
	glfwTerminate();									// glfwTerminate() should always be used before exiting the code to release the resources the program is using
	exit(EXIT_SUCCESS);
}

//____________________________________________________________________________________________________________________________


void readRadarData(int argc, char **argv[])
{
	WSADATA wsaData;									//Structure for WinSock setup communication 
	SOCKET connectSocket = INVALID_SOCKET;

	struct addrinfo *result = NULL,
		*ptr = NULL,
		hints;

	const char *sendbuf = "this is a test";
	char recvbuf[DEFAULT_BUFLEN];
	int iResult;
	int recvbuflen = DEFAULT_BUFLEN;					// Length of received string 

	

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		//return 1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	// Resolve the server address and port
	iResult = getaddrinfo(*argv[2], DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		//return 1;
	}

	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

		// Create a SOCKET for connecting to server
		connectSocket = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);
		if (connectSocket == INVALID_SOCKET) {
			printf("socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			//return 1;
		}

		// Connect to server.
		iResult = connect(connectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(connectSocket);
			connectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (connectSocket == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		WSACleanup();
		//return 1;
	}

	// Send an initial buffer
	iResult = send(connectSocket, sendbuf, (int)strlen(sendbuf), 0);
	if (iResult == SOCKET_ERROR) {
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(connectSocket);
		WSACleanup();
		//return 1;
	}

	printf("Bytes Sent: %ld\n", iResult);

	// shutdown the connection since no more data will be sent
	iResult = shutdown(connectSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(connectSocket);
		WSACleanup();
		//return 1;
	}

	// Receive until the peer closes the connection
	do {

		iResult = recv(connectSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0)
			printf("Bytes received: %d\n", iResult);
		else if (iResult == 0)
			printf("Connection closed\n");
		else
			printf("recv failed with error: %d\n", WSAGetLastError());

	} while (iResult > 0);

	// cleanup
	closesocket(connectSocket);
	WSACleanup();







	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {												// Load Winsock 2.0 DLL 
		printf("WSAStartup failed: %d\n", iResult);
		//return 1;
	}

	

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	char abcd[5];

	//Resolve the server address and port
	iResult = getaddrinfo(abcd, DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed : %d\n", iResult);
		WSACleanup();
		//return 1;
	}

	
	
	// Attempt to connect to the first address returned by
	// the call to getaddrinfo
	ptr = result;

	// Create a SOCKET for connecting to server
	connectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

	if (connectSocket == INVALID_SOCKET) {
		printf("Error at socket(): %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		//return 1;

	}

	//TO CONNECT TO A SOCKET
	// Connect to server.
	iResult = connect(connectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		closesocket(connectSocket);
		connectSocket = INVALID_SOCKET;
	}

	// Should really try the next address returned by getaddrinfo
	// if the connect call failed
	// But for this simple example we just free the resources
	// returned by getaddrinfo and print an error message

	freeaddrinfo(result);

	if (connectSocket == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		WSACleanup();
		//return 1;
	}


	// Send an initial buffer
	iResult = send(connectSocket, sendbuf, (int)strlen(sendbuf), 0);
	if (iResult == SOCKET_ERROR) {
		printf("send failed: %d\n", WSAGetLastError());
		closesocket(connectSocket);
		WSACleanup();
		//return 1;
	}

	printf("Bytes Sent: %ld\n", iResult);

	// shutdown the connection for sending since no more data will be sent
	// the client can still use the ConnectSocket for receiving data
	iResult = shutdown(connectSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed: %d\n", WSAGetLastError());
		closesocket(connectSocket);
		WSACleanup();
		//return 1;
	}

	// Receive data until the server closes the connection
	do {
		iResult = recv(connectSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0)
			printf("Bytes received: %d\n", iResult);
		else if (iResult == 0)
			printf("Connection closed\n");
		else
			printf("recv failed: %d\n", WSAGetLastError());
	} while (iResult > 0);

	// cleanup
	closesocket(connectSocket);
	WSACleanup();

	//return 0;

}

*/


int  main(int argc, char **argv[])
{
	glInitialize();
	GLFWwindow* glHandle = glCreateWindow();	
	glRenderLoop(glHandle);
	
	glfwDestroyWindow(window);		
	glfwTerminate();
	
	//---------------------------------------------------------------------------------------------------------------------------------------
	/*
	// initialize the server ( define a socket at server, bind it to address and start listening)
	server = new RemoteServer();
	Sleep(500);
	serverLoop();

	// initialize the client
	client = new RemoteClient();
	
	//ppi_display();

	/*
	for(int i= 0; i<10;i++)
	{ 
	// initialize the server ( define a socket at server, bind it to address and start listening)
	
	server = new RemoteServer();
	Sleep(5000);
	// create thread with arbitrary argument for the run function
	_beginthread(serverLoop, 0, (void*)12);

	// initialize the client
	client = new RemoteClient();

	//printf("total Number of argc: %d \n", argc);
	//readRadarData(argc, argv);
	//clientLoop();
	}
	*/
	//---------------------------------------------------------------------------------------------------------------------------------------
	
	return 0;
}

void serverLoop()
{
	while (true)
	{
		server->update();
	}

}

void clientLoop(void)
{
	while (true)
	{
		//client->update();
	}
}
