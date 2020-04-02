/*
-------------------------------------------------------------------------------------------------------------------------------------------
 Author			:	Simar Mann Singh
 Creation Date	:	14/06/2018
-------------------------------------------------------------------------------------------------------------------------------------------
*/

// Open Source Radar GUI (v0.1)
// (main code and documentation)


//-----------------------------------------------------------------------------
// DOCUMENTATION
//-----------------------------------------------------------------------------

/* 
 PROGRAMMER GUIDE
 ================
 - READ ME
	Detailed documentaion of the code has been provided. Double Click on the file "Index.html" kept in the project Directory.
	Documentation and class maps have been generated using the C++ Documentation tool "Doxygen" coupled with graph generator tool "Graphviz" which can be downloaded from the following Hyperlinks
	1. Doxygen :	http://www.doxygen.org/download.html 
	2. GraphViz:	https://graphviz.gitlab.io/download/
	All class maps, functions, variables have been included in the detialed documentation.
 */


#include <iostream>
#include "graphics.h"

GLFWwindow* window;

int main(int argc, char **argv[])
{	
	glInitialize();
	GLFWwindow* glHandle = glCreateWindow();
	glRenderLoop(glHandle);

	glfwDestroyWindow(window);		
	glfwTerminate();
	return 0;
}