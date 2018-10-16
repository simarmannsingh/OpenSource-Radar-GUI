/*
 -------------------------------------------------------------------------------------------------------------------------------------------

 Author			:	Simar Mann Singh
 Department		:	High Frequency Department, TF
 Date			:	28/08/2018

 -------------------------------------------------------------------------------------------------------------------------------------------
*/
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Renderer.h"

// Initialization
void glInitialize();
GLFWwindow* glCreateWindow();

// Callbacks
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void error_callback(int error, const char* description);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

// Rendering
int glRenderLoop(GLFWwindow* window);


