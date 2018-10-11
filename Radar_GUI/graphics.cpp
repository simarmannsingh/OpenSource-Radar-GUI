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
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include <math.h>
#include "FontRenderer.h"
#include <corecrt_math_defines.h>
#include "shader_s.h"

bool flag_coherent = FALSE;
unsigned int fs;
unsigned int vs;

int width = 200;
int height = 200;

float centerX = 320.0;
float centerY = 240.0;
float PI_180 = M_PI / 180.0f;
float radius = (centerX < centerY) ? centerX : centerY;
float rad_64 = radius / 64;
float rad_3 = radius / 3;


// Initialize the FreeType fonts
FontRenderer FR;
Shader font_shader;
GLuint va_font, vb_font;

const int v_corners = 80;

static void GLclearError()
{
	while (glGetError() != GL_NO_ERROR);
}

static void  GLCheckError()
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] Error Code ->" << error << std::endl;
	}
}

struct shaderProgramSource
{
	std::string vertexSource;
	std::string fragSource;
};

static shaderProgramSource ParseShader(const std::string& filepath)
{
	std::ifstream stream(filepath);

	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	ShaderType  type = ShaderType::NONE;
	std::stringstream ss[2];

	std::string line;
	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				type = ShaderType::VERTEX;
			}
				
			else if (line.find("fragment") != std::string::npos)
			{
				type = ShaderType::FRAGMENT;
			}
		}
		else
		{
			ss[(int)type] << line << '\n';
		}
	}

	return { ss[0].str(), ss[1].str() };
}

void glInitialize()
{
	/*
	 -------------------------------------------------------------------------------------------------------------------------------------------

					Initializing GLFW Library and setting minimum required version to OpenGL 3.3

	 -------------------------------------------------------------------------------------------------------------------------------------------
	*/

	// GLFW Initialization
	int glfw_Init = glfwInit();
	if (!glfw_Init)
	{
		printf("Failed at GLFW Initialization\n");
		Sleep(3000);
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);										// defines the veriosn of the openGL used 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);						// We are using <<---OPENGL_CORE--->> Profile
	printf("GLFW initialization completed successfully\n");

	
}

GLFWwindow* glCreateWindow()
{
	
	glfwSetErrorCallback(error_callback);
	GLFWwindow* window = glfwCreateWindow(800, 800, "Radar - HF dept TF", NULL, NULL);	// creating a window 800x800 in size			--->> 800 x 600
	if (window == NULL)
	{
		printf("Failed at creating window\n");
		Sleep(2000);
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	printf("Window Creation completed successfully\n");
	
	/* Make the window's context current */
	glfwMakeContextCurrent(window);														//making context of our window the main context on the current thread
	glfwSwapInterval(1);

	//  GLAD Initialization
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf("Failed at GLAD Initialization\n");
		Sleep(3000);
		exit(EXIT_FAILURE);
	}
	printf("GLAD Initialization completed successfully\n");
	glfwSetKeyCallback(window, key_callback);
	//glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);

	return window;
}

int glRenderLoop(GLFWwindow* window, unsigned int shdrprgm)
{
	/*
	 -------------------------------------------------------------------------------------------------------------------------------------------

														-->>  RENDER LOOP  <<--

	 -------------------------------------------------------------------------------------------------------------------------------------------
	*/

	static float vertices[] = {									//vertex data is a collection of vertices in 3D space
		  0.5f,	    0.3f,    		// 0		
		 -0.5f,		0.3f,    		// 1
		 -0.5f,		-0.3f,    		// 2
		  0.5f,		-0.3f,    		// 3		
		
	};
	
	GLfloat vertices_PPI[v_corners][2];									//vertex data is a collection of vertices in 3D space		
	GLfloat angle = 0.0;
	GLfloat radius = 1.0;
	
	for (int i = 0; i < v_corners; i++)
	{
		angle = i * 2 * 3.14 / v_corners;
		vertices_PPI[i][0] = radius * cos(angle);
		vertices_PPI[i][1] = radius * sin(angle);
		//std::cout << r * cos(angle) << ", ";
		//std::cout << r * sin(angle) << std::endl;
	}

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0,
	};

	unsigned int indices_PPI[] = {
		0, 1, 2,
		0, 2, 3,
		0, 3, 4,
		0, 4, 5,
		0, 5, 6,
		0, 6, 7,
		0, 7, 8,
		0, 1, 8,
	};   

	float texCoord[] = {
		0.0f, 0.0f,		//Lower-left corner
		1.0f, 0.0f,		//Lower-Right corner
		0.5f, 1.0f,		//top-center corner
	};

	unsigned int VAO, circleVao, circleVbo, circleEbo;
	
	//vertex array
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	
	// Test code 

	glGenBuffers(1, &circleVbo);
	glBindBuffer(GL_ARRAY_BUFFER, circleVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_PPI), vertices_PPI, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);	
	glGenVertexArrays(1, &circleVao);
	glBindVertexArray(circleVao);
	glBindBuffer(GL_ARRAY_BUFFER, circleVbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);


	// *test code ends


	// vertex array and buffer for rendering squares on the screen
	VertexArray va;
	VertexBuffer vb(vertices, sizeof(vertices));

	VertexBufferLayout layout;
	layout.Push<float>(2);
	va.AddBuffer(vb, layout);	
	
	IndexBuffer ib(indices, 6);
	glUseProgram(shdrprgm);
		

	int color = glGetUniformLocation(shdrprgm, "u_Color");
	if (color == NULL)
	{
		std::cout << "Uniform color not found" << std::endl;
		std::cout << "color value :  " << color << std::endl;
	}
	glUniform4f(color, 0.9f, 0.0f, 0.0f, 1.0f);
	if (color == NULL)
	{
		std::cout << "Uniform color AGAIN not found" << std::endl;
		std::cout << "color value :  " << color << std::endl;
	}
	//to DRAW in WIREFRAME mode
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//unbinding the Vertex array and Vertex Buffers
	glBindVertexArray(0);
	glUseProgram(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
	//-------------------------------------------------------------------------------------------------------------------------------------------
	//
	//											FONT rendering using Freetype library
	//
	//-------------------------------------------------------------------------------------------------------------------------------------------
	
	// Initialize the FreeType fonts
	font_shader = FR.FontInit();
	FR.FontSetup();

	// Configure va_font/vb_font for texture quads
	glGenVertexArrays(1, &va_font);
	glGenBuffers(1, &vb_font);
	glBindVertexArray(va_font);
	glBindBuffer(GL_ARRAY_BUFFER, vb_font);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	

	/*
	 -------------------------------------------------------------------------------------------------------------------------------------------

														-->>  Translation Matrix  <<--

	 -------------------------------------------------------------------------------------------------------------------------------------------
	
	glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
	glm::mat4 trans(1);
	std::cout << "Before  :::   ---> vec.x  : " << vec.x << std::endl << "vec.y : " << vec.y << std::endl << "vec.z : " << vec.z << std::endl;
	trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
	vec = trans * vec;
	//trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
	//trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));	
	std::cout << "After  :::   ---> vec.x  : " << vec.x << std::endl << "vec.y : " << vec.y << std::endl << "vec.z : " << vec.z << std::endl;
	
	
	glm::mat4 transform;
	transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));
	transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
	glUseProgram(shdrprgm);
	unsigned int transformLoc = glGetUniformLocation(shdrprgm, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
		
	//unsigned int location = glGetUniformLocation(shdrprgm, "transform");
	//glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(trans));
	*/
	/*
	 -------------------------------------------------------------------------------------------------------------------------------------------

														-->>  OpenGL Loop  <<--

	 -------------------------------------------------------------------------------------------------------------------------------------------
	*/

	while (!glfwWindowShouldClose(window))
	{	
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		glUseProgram(shdrprgm);
				
		glViewport(0, 0, 800, 800);
		glUniform4f(color, 0.8f, 0.8f, 0.1f, 1.0f);
				
		//glDrawArrays(GL_TRIANGLE_FAN, circleVao, 8);

		va.Bind();
		ib.bind();
		
		{
			// RECTANGLES
			// Rendering object number 1		-  OUTER RECT
			// Defining Properties 
			glViewport(600, 600, width, height);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glUniform4f(color, 0.4f, 0.4f, 0.4f, 1.0f);
			
			//Drawing the object
			GLclearError();
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
			GLCheckError();

			// Rendering object number 1.1		-  INNER RECT
			// Defining Properties 
			glViewport(625, 625, 150, 150);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glUniform4f(color, 0.3f, 0.3f, 0.3f, 1.0f);

			//Drawing the object
			GLclearError();
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
			GLCheckError();
					


			// Rendering object number 2		
			// Defining Properties 
			glViewport(600, 500, width, height);
			glUniform4f(color, 0.4f, 0.4f, 0.4f, 1.0f);
			//Drawing the object
			GLclearError();
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
			GLCheckError();

			// Rendering object number 2.2		
			// Defining Properties 
			glViewport(625, 525, 150, 150);
			glUniform4f(color, 0.3f, 0.3f, 0.3f, 1.0f);
			//Drawing the object
			GLclearError();
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
			GLCheckError();
		}
		
		{
			//CIRCELS
			// Outer Circle
			glViewport(60, 100, 600, 600);
			glUniform4f(color, 0.2f, 0.2f, 0.6f, 1.0f);
			glBindVertexArray(circleVao);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glDrawArrays(GL_TRIANGLE_FAN, 0, 80);
			//Inner Circle
			glViewport(70, 110, 580, 580);
			glUniform4f(color, 0.2f, 0.2f, 0.3f, 1.0f);
			glDrawArrays(GL_TRIANGLE_FAN, 0, 80);

			//first ring
			glViewport(210, 250, 300, 300);
			glUniform4f(color, 0.15f, 0.15f, 0.25f, 1.0f);
			glDrawArrays(GL_TRIANGLE_FAN, 0, 80);

			glViewport(215, 255, 290, 290);
			glUniform4f(color, 0.2f, 0.2f, 0.3f, 1.0f);
			glDrawArrays(GL_TRIANGLE_FAN, 0, 80);

			//Center Dome
			glViewport(355, 395, 10, 10);
			glUniform4f(color, 0.15f, 0.15f, 0.25f, 1.0f);
			glDrawArrays(GL_TRIANGLE_FAN, 0, 80);

		}
		
		glViewport(600, 600, width, height);
		if (flag_coherent)
		{
			FR.RenderText(font_shader, "Status : ON", 280.0f, 0.0f, 1.0f, va_font, vb_font);			
		}
		else
		{
			FR.RenderText(font_shader, "Status : OFF", 280.0f, 0.0f, 1.0f, va_font, vb_font);
		}
		FR.RenderText(font_shader, "(C) HF Department, TF lala", 0.0f, 570.0f, 1.0f, va_font, vb_font);
		
		
		
		
		//Color
		GLfloat r = 0; 
		GLfloat g = 0;
		GLfloat b = 0;

		r = fmod(r + 0.01, 1);
		g = fmod(g + 0.02, 1);
		b = fmod(b + 0.03, 1);
		
		
		//Horizontal lines
		glUniform4f(color, 0.7f, 0.7f, 0.7f, 1.0f);
		for (int i = -10; i < 11; i++) {
			glLineWidth(0.2);
			glBegin(GL_LINE_STRIP);
			glVertex2f(-0.96, (i * 0.1));
			glVertex2f(0.96, (i * 0.1));
			glEnd();
		}

		//Vertical lines
		for (int i = -10; i < 11; i++) {
			glLineWidth(0.2);
			glBegin(GL_LINE_STRIP);
			glVertex2f((i * 0.1), -0.96);
			glVertex2f((i * 0.1), 0.96);
			glEnd();
		}

		
		
					
		glFlush();
			   
		// glfw: swap buffers and poll IO events (key pressed/release, mouse moved etc)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	return 0;
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	std::cout << key << "\n";

	if (key == GLFW_KEY_C && action == GLFW_PRESS)
	{
		if (!flag_coherent)
		{
			printf("Connecting... \n");			
			flag_coherent = TRUE;

		}
		else
		{
			printf("Disconnecting... \n");
			flag_coherent = FALSE;
		}
	}

	switch (key)
	{
	case GLFW_KEY_0:	printf("key zero pressed. \n"); 
			break;
	
	case GLFW_KEY_D:	printf("Coherent Activated. \n");
		break;
	case GLFW_KEY_A:	printf("Incoherent Activated. \n");
		break;
	case GLFW_KEY_O:	if (flag_coherent)
		printf("CFAR activated \n");
						else
		printf("CFAR deactivated");
		break;
	case GLFW_KEY_U: printf("Key D pressed. \n");
		break;
	case GLFW_KEY_E: printf("Key E pressed. \n");
		break;
	case GLFW_KEY_F: printf("Key F pressed. \n");
		break;
	case GLFW_KEY_ESCAPE: printf("ESCAPE Key pressed. \n");
		glfwSetWindowShouldClose(window, GLFW_TRUE);
		break;
	default: printf("OUT OF SYLLABUS Key pressed. \n");
		break;
	}

}

void error_callback(int error, const char* description)
{
	puts(description);
	Sleep(2000);
}

static unsigned int compileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, 0);
	glCompileShader(id);

	GLint result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		printf("-->> COMPILATION UNSUCCESSFULL \n");
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));

		glGetShaderInfoLog(id, length, &length, message);

		std::cout << "COMPILATION FAILED \n" << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader" << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}

	return id;
}

static unsigned int createShader(const std::string& vertexShadersrc, const std::string& fragmentShadersrc)
{
	//creating vertex and fragment shaders
	vs = compileShader(GL_VERTEX_SHADER, vertexShadersrc);
	fs = compileShader(GL_FRAGMENT_SHADER, fragmentShadersrc);
	
	//Linking Shaders to the shader Program
	GLuint program = glCreateProgram();
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);
	
	//checking for Linking Errors
	int progLinkRes;													// to store the result of the program linking
	glGetProgramiv(program, GL_LINK_STATUS, &progLinkRes);
	if(!progLinkRes)
	{
		printf("-->> SHADER PROGRAM LINKING UNSUCCESSFULL \n");
		int length;
		glGetShaderiv(program, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(program, length, &length, message);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << std::endl;
		std::cout << message << std::endl;
		return 0;
	}
	//glDeleteShader(vs);
	//glDeleteShader(fs);
	return program;
}

unsigned int setupShaderProgram()
{
	// Prsring Shader file
	shaderProgramSource spsource = ParseShader("res/shaders/Basic.shader");
	std::cout << "VERTEX data" << spsource.vertexSource;
	std::cout << "FRAGMENT data" << spsource.fragSource;

	unsigned int shaderprog = createShader(spsource.vertexSource, spsource.fragSource);
	std::cout << "Object Shader Program Compilation Successfull..." << std::endl;
		
	return shaderprog;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)			// Callback for any change in the position/size of the generated window
{
	glViewport(0, 0, width, height);
}
