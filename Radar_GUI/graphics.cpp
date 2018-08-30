/*
 -------------------------------------------------------------------------------------------------------------------------------------------

 Author			:	Simar Mann Singh 
 Department		:	High Frequency Department, TF
 Date			:	28/08/2018

 -------------------------------------------------------------------------------------------------------------------------------------------
*/
#include "graphics.h"
#include <stdlib.h>
#include <stdio.h>

void glInitialize()
{
	/*
	 -------------------------------------------------------------------------------------------------------------------------------------------
	
					Initializing all the required graphics libraries 
	
	 -------------------------------------------------------------------------------------------------------------------------------------------
	*/
	
	// GLFW Initialization
	int glfw_Init = glfwInit();
	if (!glfw_Init)
	{
		printf("Failed at GLFW Initialization\n");
		Sleep(5000);
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);										//defines the veriosn of the openGL used 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		
}

GLFWwindow* glCreateWindow()
{
	glfwSetErrorCallback(error_callback);
	GLFWwindow* window = glfwCreateWindow(1080, 720, "Radar - HF dept TF", NULL, NULL);
	if (window == NULL)
	{
		printf("Failed at creating window\n");
		Sleep(2000);
		glfwTerminate();
		exit(EXIT_FAILURE);		
	}
	printf("creating window done\n");
	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	//glViewport(0, 0, 640, 480);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	//glRenderLoop(window);

	//  GLAD Initialization
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf("Failed at GLAD Initialization\n");
		Sleep(5000);
		exit(EXIT_FAILURE);
	}
	printf("GLAD Initialization done\n");
	Sleep(200);
	return window;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)			// Callback for any change in the position/size of the generated window
{
	glViewport(0, 0, width, height);
}


void glRenderLoop(GLFWwindow* window)
{

	processInput(window);
	glfwSwapBuffers(window);
	glfwPollEvents();
	

	

	int nn = 0;
	while (!glfwWindowShouldClose(window))
	{	
		glfwSwapInterval(1);
		//setup view
		int width, height;

		glfwGetFramebufferSize(window, &width, &height);
		
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT);

		float r = 0.31;
		float g = 0.52;
		float b = 0.2;

			glColor3f(r, g, b);
			//for (int i = -10; i < 11; i++) {
			glLineWidth(0.8);
			glBegin(GL_LINE_STRIP);
			glVertex2f(0, (height/2));
			glVertex2f(width, (height / 2));
			glEnd();
		

		printf("loop count : %d \n", nn);
		nn++;
	}
	
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		printf("Escape Key pressed/Aborted by User. \n");
		Sleep(2000);
		glfwSetWindowShouldClose(window, true);

	}

}


void error_callback(int error, const char* description)
{
	puts(description);
	Sleep(2000);
}