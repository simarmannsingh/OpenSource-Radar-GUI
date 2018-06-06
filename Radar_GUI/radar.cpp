/*

   Author			: Simar Mann Singh
   Reference		: Intro to Computer Science (Link : https://www.youtube.com/watch?v=kwxCP_LLZJ4&t=328s)

*/

#include<GLFW/glfw3.h>
#include"linmath.h"
#include <stdlib.h>				
#include <stdio.h>


int main(int argc, char *argv[])
{
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);										//defines the veriosn of the openGL used 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	GLFWwindow* window = glfwCreateWindow(480, 480, "OpenGL using GLFW", NULL, NULL);
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	/*constants used in the program*/
	const float DEG2RAD = 3.14159 / 180;
	float radius = 0.96;
	float r = 0.31;
	float g = 0.2;
	float b = 0.75;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
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
		*/

		//Drawing
		//glColor3f(r, g, b);
		glBegin(GL_LINE_STRIP);											// GL_LINE_STRIP or GL_POLYGON defines whether the shape drawn would be a line shape or a filled polygon
		for (int i = 0; i < 360; i++) {
			float degInRad = i * DEG2RAD;
			glVertex2f(cos(degInRad)*radius, sin(degInRad)*radius);		
		}
		glEnd();


		glBegin(GL_LINE_STRIP);											// GL_LINE_STRIP or GL_POLYGON defines whether the shape drawn would be a line shape or a filled polygon
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

		glBegin(GL_POLYGON);											// GL_LINE_STRIP or GL_POLYGON defines whether the shape drawn would be a line shape or a filled polygon
		for (int i = 0; i < 360; i++) {
			float degInRad = i * DEG2RAD;
			glVertex2f(cos(degInRad)*(0.01), sin(degInRad)*(0.01));
		}
		glEnd();

		/*glBegin(GL_POLYGON);											// GL_LINE_STRIP or GL_POLYGON defines whether the shape drawn would be a line shape or a filled polygon
		for (int i = 0; i < 360; i++) {
			float degInRad = i * DEG2RAD;
			glVertex2f(cos(degInRad)*(0.01), radius));
		}															//this need to be updated with code for drawing a line connecting center of radar to last circle of radar
		glEnd();

		*/


		/* Swap front and back buffers */
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();													// glfwTerminate() should always be used before exiting the code to release the resources the program is using
	exit(EXIT_SUCCESS);


}