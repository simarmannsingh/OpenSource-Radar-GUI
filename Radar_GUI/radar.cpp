/*

   Author			: Simar Mann Singh
   Reference		: Intro to Computer Science (Link : https://www.youtube.com/watch?v=kwxCP_LLZJ4&t=328s)

*/

#include<GLFW/glfw3.h>
#include"linmath.h"
#include <stdlib.h>				
#include <stdio.h>

float centerX = 320.0;
float centerY = 240.0;
float PI_180 = M_PI / 180.0f;

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

		glLineWidth(2.5);
		glBegin(GL_LINES);
		for (int i = 0; i < 360; i++) {
			float degInRad = i * DEG2RAD;
			glVertex2f(0.01, 0.01);
			//glVertex2f(0.72, 0.72);
			glVertex2f(cos(degInRad)*(radius), sin(degInRad)*(radius));
		}
		
		//glVertex2f(cos(degInRad)*(0.01), sin(degInRad)*(0.01));
			
		glEnd();











		glPopMatrix();

		//Display 45 deg crosshair

		glDisable(GL_TEXTURE_2D);
		glEnable(GL_LINE_SMOOTH);

		glLineWidth(1.5);
		glPushMatrix();
		glColor3f(0.712, 0.767, 0.431);
		glBegin(GL_LINES);
		for (float angle = 0.0; angle <= 360.0; angle += 45.0)
		{
			float n_cos = cos(angle * PI_180);
			float n_sin = sin(angle * PI_180);
			glVertex2f(centerX + n_cos * radius / 8, centerY + n_sin * radius / 8);
			glVertex2f(centerX + n_cos * radius, centerY + n_sin * radius);

		}

		//Display Degrees

		for (int angle = 0; angle <= 360; angle += 1)
		{
			float n_cos = cos(angle * PI_180);
			float n_sin = sin(angle * PI_180);
			if (angle % 5 == 0)
			{
				glLineWidth(2.5);
				glVertex2f(centerX + n_cos * (radius + (rad_64)), centerY + n_sin * (radius + (rad_64)));
				glVertex2f(centerX + n_cos * (radius + (radius / 20)), centerY + n_sin * (radius + (radius / 20)));
			}
			else
			{
				glLineWidth(1.5);
				glVertex2f(centerX + n_cos * (radius + (rad_64)), centerY + n_sin * (radius + (rad_64)));
				glVertex2f(centerX + n_cos * (radius + (radius / 30)), centerY + n_sin * (radius + (radius / 30)));
			}
			if ((angle + 22) % 45 == 0)
			{
				glVertex2f(centerX + n_cos * (rad_3 - rad_64), centerY + n_sin * (rad_3 - rad_64));
				glVertex2f(centerX + n_cos * (rad_3 + rad_64), centerY + n_sin * (rad_3 + rad_64));

				glVertex2f(centerX + n_cos * (2 * rad_3 - rad_64), centerY + n_sin * (2 * rad_3 - rad_64));
				glVertex2f(centerX + n_cos * (2 * rad_3 + rad_64), centerY + n_sin * (2 * rad_3 + rad_64));
			}
		}
		glEnd();
		glLineWidth(2.5);
		glBegin(GL_LINE_LOOP);

		//Display outer circle

		for (int angle = 0; angle <= 360; angle += 1)
		{
			glVertex2f(centerX + cos(angle * PI_180) * (radius + (rad_64)), centerY + sin(angle * PI_180) * (radius + (rad_64)));
		}

		glEnd();

		glLineWidth(2.0);
		glBegin(GL_LINE_LOOP);

		//Display inner circle

		for (int angle = 0; angle <= 360; angle += 1)
		{
			glVertex2f(centerX + cos(angle * PI_180) * (radius + 1), centerY + sin(angle * PI_180) * (radius + 1));
		}

		glEnd();

		glLineWidth(2.0);
		glBegin(GL_LINE_STRIP);

		//Display center circles

		for (int angle = 0; angle <= 360; angle += 1)
		{
			glVertex2f(centerX + cos(angle * PI_180) * (rad_3), centerY + sin(angle * PI_180) * (rad_3));
		}

		for (int angle = 0; angle <= 360; angle += 1)
		{
			glVertex2f(centerX + cos(angle * PI_180) * (2 * rad_3), centerY + sin(angle * PI_180) * (2 * rad_3));
		}

		glEnd();






		/* Swap front and back buffers */
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();													// glfwTerminate() should always be used before exiting the code to release the resources the program is using
	exit(EXIT_SUCCESS);


}