/*

   Author			: Simar Mann Singh
   Reference		: Intro to Computer Science (Link : https://www.youtube.com/watch?v=kwxCP_LLZJ4&t=328s)

*/

#include<GLFW/glfw3.h>
#include"linmath.h"
#include <stdlib.h>				
#include <stdio.h>

/*constants used in the program*/
const float DEG2RAD = 3.14159 / 180;
float radius = 0.96;

float r = 0.31;
float g = 0.52;
float b = 0.2;
float centerX = 320.0;
float centerY = 240.0;
float M_PI = 3.14159;
float PI_180 = M_PI / 180.0f;

void backgroundDisplay(void) {
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

void ppi_display(void){
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

		glColor3f(r, g, b);
		for (int i = -10; i <11; i++) {
			glLineWidth(0.2);
			glBegin(GL_LINE_STRIP);
			glVertex2f(-0.96, (i * 0.1));
			glVertex2f(0.96, (i * 0.1));
			glEnd();
		}
		for (int i = -10; i <11; i++) {
			glLineWidth(0.2);
			glBegin(GL_LINE_STRIP);
			glVertex2f((i * 0.1), -0.96);
			glVertex2f((i * 0.1), 0.96);
			glEnd();
		}
		glColor3f(1, 1, 1);
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
		backgroundDisplay();							// to display the checked background

														/* Swap front and back buffers */
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();													// glfwTerminate() should always be used before exiting the code to release the resources the program is using
	exit(EXIT_SUCCESS);
}


int main(int argc, char *argv[])
{
	
	ppi_display();

}