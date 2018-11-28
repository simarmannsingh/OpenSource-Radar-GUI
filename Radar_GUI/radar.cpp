/*
-------------------------------------------------------------------------------------------------------------------------------------------
 Author			:	Simar Mann Singh
 Department		:	High Frequency Department, TF
 Date			:	14/06/2018
 References		:		1. Intro to Computer Science (Link : https://www.youtube.com/watch?v=kwxCP_LLZJ4&t=328s)
						2. TCP socket programming (Link : https://msdn.microsoft.com/en-us/library/windows/desktop/ms737591(v=vs.85).aspx )

-------------------------------------------------------------------------------------------------------------------------------------------
*/


// RadarGUI, v1.0 
// (main code and documentation)

// Please read 'Programmer guide' below for notes on how to use/alter RadarGUI.
// Developed by Simar Mann Singh under expert supervision by Dr. -Ing Alexender Teplyuk.
// This is a propriety software to be used exclusively by the High Frequency Departmenent, Univeristy of Kiel. 
// Under no circumstances can this software be used, reproduced or redistributed without prior permission from the concerned person in High Frequency Department, Univeristy of Kiel.

/*
Index:

DOCUMENTATION

- FEATURES
- END-USER GUIDE
- PROGRAMMER GUIDE (read me!)
  - Read first
  - How to update to a newer version of Dear ImGui
  - Getting started with integrating Dear ImGui in your code/engine
  - This is how a simple application may look like (2 variations)
  - This is how a simple rendering function may look like
  - Using gamepad/keyboard navigation controls
- API BREAKING CHANGES (read me when you update!)
- FREQUENTLY ASKED QUESTIONS (FAQ), TIPS
  - How can I tell whether to dispatch mouse/keyboard to imgui or to my application?
  - How can I display an image? What is ImTextureID, how does it works?
  - How can I have multiple widgets with the same label or without a label? A primer on labels and the ID Stack.
  - How can I use my own math types instead of ImVec2/ImVec4?
  - How can I load a different font than the default?
  - How can I easily use icons in my application?
  - How can I load multiple fonts?
  - How can I display and input non-latin characters such as Chinese, Japanese, Korean, Cyrillic?
  - How can I use the drawing facilities without an ImGui window? (using ImDrawList API)
  - I integrated Dear ImGui in my engine and the text or lines are blurry..
  - I integrated Dear ImGui in my engine and some elements are clipping or disappearing when I move windows around..
  - How can I help?

CODE
(search for "[SEGMENT]" in the code to find them)

// [SEGMENT] FUNCTION DECLARATIONS
// [SEGMENT] CONTEXT AND MEMORY ALLOCATORS
// [SEGMENT] MAIN USER FACING STRUCTURES (ImGuiStyle, ImGuiIO)
// [SEGMENT] MISC HELPER/UTILITIES (Maths, String, Format, Hash, File functions)
// [SEGMENT] MISC HELPER/UTILITIES (ImText* functions)
// [SEGMENT] MISC HELPER/UTILITIES (Color functions)
// [SEGMENT] ImGuiStorage
// [SEGMENT] ImGuiTextFilter
// [SEGMENT] ImGuiTextBuffer
// [SEGMENT] ImGuiListClipper
// [SEGMENT] RENDER HELPERS
// [SEGMENT] MAIN CODE (most of the code! lots of stuff, needs tidying up!)
// [SEGMENT] TOOLTIPS
// [SEGMENT] POPUPS
// [SEGMENT] KEYBOARD/GAMEPAD NAVIGATION
// [SEGMENT] COLUMNS
// [SEGMENT] DRAG AND DROP
// [SEGMENT] LOGGING/CAPTURING
// [SEGMENT] SETTINGS
// [SEGMENT] PLATFORM DEPENDENT HELPERS
// [SEGMENT] METRICS/DEBUG WINDOW
*/


//-----------------------------------------------------------------------------
// DOCUMENTATION
//-----------------------------------------------------------------------------

/* 

 FEATURES
 =========
 - Easy to use Graphical User Interface for the Pulse Doppler Radar designed by High Frequency Department, Univeristy of Kiel.
 - Easy to update when IP address changes.
 - % Ability to record the data and store in a file.
 - % Ability to playback the recorded data from the file.
 - Easy to change screen size.
 - Easy to Zoom-In/Out 
 - Reliable Communication
 - Keyboard Shortcuts available
 - Efficient runtime and memory consumption (NB- we do allocate when "growing" content e.g. creating a window,
   opening a tree node for the first time, etc. but a typical frame should not allocate anything)

 Weaknesses includes:
 - Limited layout features, intricate layouts are typically crafted in code
 
 END-USER GUIDE
 ==============
 - Click upper right corner to close a window, available when 'bool* p_open' is passed to ImGui::Begin().
 - Click and drag on lower right corner to resize window (double-click to auto fit window to its contents).
 - Click and drag on any empty space to move window.
 - TAB/SHIFT+TAB to cycle through keyboard editable fields.
 - CTRL+Click on a slider or drag box to input value as text.
 - Use mouse wheel to scroll.
 - KEY COMBINATIONS TO OPERATE RADAR:-
   -> c		-	Toggle Conenction to Hardware
   -> d		-	Turn Coherency ON
   -> a		-	Turn Coherency OFF
   -> o		-	Toggle CFAR Implementation in the radar signal processing 
   -> q		-	Clear Screen 
   -> q		-	Linear Scale/log Scale
   -> Esc	-	Exit program

 - Mouse cursor can also be used to toggle the state of Different options on the RADAR GUI Control Panel
 - Features like Zoom are provided by mouse cursor only. Hover the mouse cursor over the region to be zoomed-in and press and hold left-click to enable zoom window.
  

 PROGRAMMER GUIDE
 ================

 - READ ME
	Detailed documentaion of the code has been provided. Double Click on the file "Index.html" kept in the project Directory.
	Documentation and class maps have been generated using the C++ Documentation tool "Doxygen" coupled with graph generator tool "Graphviz" which can be downloaded from the following Hyperlinks
	1. Doxygen :	http://www.doxygen.org/download.html 
	2. GraphViz:	https://graphviz.gitlab.io/download/
	All class maps, functions, variables have been included in the detialed documentation.
 
 - Read the FAQ below this section!
 - Call and read ImGui::Demonstrate() for Demo code demonstrating most features that the Radar GUI provides.
  

  GETTING STARTED WITH USING RadarGUI code and different APIs it provides
   It is recommended you build the .cpp files as part of your project and not as a library.
 - You can later customize the imconfig.h file to tweak some compilation time behavior, such as integrating imgui types with your own maths types.
 - You may be able to grab and copy a ready made imgui_impl_*** file from the examples/ folder.
 - When using RadarGUI, follow the declaration of variables, functions and types to find comments about them.
 
 */


#include <iostream>
#include <sstream>
#include "Logger.h"
#include <process.h>
#include "linmath.h"
#include "graphics.h"

using namespace std;
using namespace CPlusPlusLogging;

#pragma once
#define WIN32_LEAN_AND_MEAN

/*constants used in the program*/
#define MAXRECVSTRING 128
#define DEFAULT_PORT "27015"
#define DEFAULT_BUFLEN 64

struct sockaddr_in ServAddr;
struct sockaddr_in broadcastAddr; /* Broadcast Address */
void serverLoop();
void clientLoop(void);


RemoteServer * server;
RemoteClient * client;

GLFWwindow* window;

//-----------------------------------------------------------------------------
// [SEGMENT] FUNCTION DECLARATIONS
//-----------------------------------------------------------------------------
//Since radar.cpp contains main() function, it has only the main function using which all other functions are invoked.


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
	// initialize the server ( define a socket at server, bind it to address and start listening)
	/*
	server = new RemoteServer();
	Sleep(500);
	client = new RemoteClient();
	server->update();
	*/
	glInitialize();
	GLFWwindow* glHandle = glCreateWindow();	
	glRenderLoop(glHandle);
	
	glfwDestroyWindow(window);		
	glfwTerminate();
	
	//---------------------------------------------------------------------------------------------------------------------------------------
	/*
	
	
	//ppi_display();

	/*
	for(int i= 0; i<10;i++)
	{ 
	// initialize the server (define a socket at server, bind it to address and start listening)
	
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

void clientLoop(void)
{
	while (true)
	{
		//client->update();
	}
}
