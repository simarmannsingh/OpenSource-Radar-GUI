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
