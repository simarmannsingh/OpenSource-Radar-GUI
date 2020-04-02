/*
 -------------------------------------------------------------------------------------------------------------------------------------------
 Author			:	Simar Mann Singh
Creation  Date	:	28/08/2018
 -------------------------------------------------------------------------------------------------------------------------------------------
*/
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <math.h>
#include <corecrt_math_defines.h>
#include <string>
#include <iostream>
#include <sstream>
#include "FontRenderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Shader_s.h"
#include "Texture.h"

// Initialization
void glInitialize();
GLFWwindow* glCreateWindow();

// Callbacks
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void error_callback(int error, const char* description);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

// Rendering
int glRenderLoop(GLFWwindow* window);

//ImGui
void screenOverlay(bool* p_open, int corner);
void screenOverlayTwo(bool* p_open, int corner);
void SmartHUD(bool* p_open, int corner);
void renderGraph();