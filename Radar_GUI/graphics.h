/*
 -------------------------------------------------------------------------------------------------------------------------------------------

 Author			:	Simar Mann Singh
 Department		:	High Frequency Department, TF
 Date			:	28/08/2018

 -------------------------------------------------------------------------------------------------------------------------------------------
*/
#include <glad/glad.h>
#include <GLFW/glfw3.h>


void glInitialize();
GLFWwindow* glCreateWindow();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void error_callback(int error, const char* description);
void glRenderLoop(GLFWwindow* window);
void processInput(GLFWwindow* window);