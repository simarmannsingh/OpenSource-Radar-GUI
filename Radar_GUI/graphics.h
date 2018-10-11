/*
 -------------------------------------------------------------------------------------------------------------------------------------------

 Author			:	Simar Mann Singh
 Department		:	High Frequency Department, TF
 Date			:	28/08/2018

 -------------------------------------------------------------------------------------------------------------------------------------------
*/
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Logger.h"



// Initialization
void glInitialize();
GLFWwindow* glCreateWindow();




// Callbacks
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void error_callback(int error, const char* description);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

// Shader Setup and Rendering
static unsigned int createShader(const std::string& vertexShadersrc, const std::string& fragmentShadersrc);
static unsigned int compileShader(unsigned int type, const std::string& source);
unsigned int setupShaderProgram();
int glRenderLoop(GLFWwindow* window, unsigned int shp);


