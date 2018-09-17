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

bool flag_coherent;
unsigned int fs;
unsigned int vs;


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

	int width, height;
	width = 200;
	height = 200;

	static float vertices[] = {									//vertex data is a collection of vertices in 3D space
		 0.0f,	   -0.5f,    		// 0
		 0.125f,   -0.175f,
		 0.5f,		0.0f,    		// 1
		 0.125f,    0.175f,
		 0.0f,		0.5f,    		// 2
		-0.125f,    0.175f,
	    -0.5f,		0.0f,    		// 3		
		-0.125f,   -0.175f,

	};
	

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 4,
		4, 5, 6,
		6, 7, 0
	};
   
	unsigned int VAO, circleVao, circleVbo, circleEbo;
	
	//vertex array
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	

	VertexArray va;
	VertexBuffer vb(vertices, sizeof(vertices));

	VertexBufferLayout layout;
	layout.Push<float>(2);
	va.AddBuffer(vb, layout);	
	
	IndexBuffer ib(indices, 12);
	glUseProgram(shdrprgm);
				
	int location = glGetUniformLocation(shdrprgm, "u_Color");
	if (location != -1)
	{
		std::cout << "Uniform Location not found" << std::endl;
	}
	glUniform4f(location, 0.9f, 0.0f, 0.0f, 1.0f);

	//to DRAW in WIREFRAME mode
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//unbinding the Vertex array and Vertex Buffers
	glBindVertexArray(0);
	glUseProgram(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
	/*
	// Initialize the FreeType fonts
	FontInit();
	// Set OpenGL options
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
				
		va.Bind();
		ib.bind();
		
		// Rendering object number 1
		// Defining Properties 
		glViewport(0, 0, width, height);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glUniform4f(location, 0.9f, 0.0f, 0.0f, 1.0f);
		//Drawing the object
		GLclearError();
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, NULL);
		GLCheckError();
		
		
		// Rendering object number 2
		// Defining Properties 
		glViewport(600, 0, width, height);
		glUniform4f(location, 0.0f, 0.9f, 0.0f, 1.0f);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		//Drawing the object
		GLclearError();
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, NULL);
		GLCheckError();
			
				

		// Rendering object number 3
		// Defining Properties 
		glViewport(0, 600, width, height);
		void FontRender();
		glUniform4f(location, 0.0f, 0.0f, 0.9f, 1.0f);
		
		GLclearError();
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, NULL);
		GLCheckError();



		// Rendering object number 4
		// Defining Properties 
		glViewport(600, 600, width, height);
		void FontRender();
		glUniform4f(location, 0.9f, 0.9f, 0.9f, 1.0f);

		GLclearError();
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, NULL);
		GLCheckError();

		// Rendering object number 5
		// Defining Properties 
		glViewport(100, 100, 600, 600);
		void FontRender();
		glUniform4f(location, 0.9f, 0.9f, 0.0f, 1.0f);

		GLclearError();
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, NULL);
		GLCheckError();



		/*
		// Buffers for CIRCLE
		glGenVertexArrays(1, &circleVao);
		glBindVertexArray(VAO);

		glGenBuffers(1, &circleVbo);
		glBindBuffer(GL_ARRAY_BUFFER, circleVbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glGenBuffers(1, &circleEbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, circleEbo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
				
		GLclearError();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
		GLCheckError();
		*/			
		glFlush();


		/*
		int number = 80;
		float radius = 0.4f;
		float twopi = 2.0 * 3.1452423;
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glBegin(GL_POLYGON);
		glUseProgram(shp);
		glUniform4f(location, 0.9f, 0.0f, 0.0f, 1.0f);
		glVertex2f(0.0f, 0.0f);
		for (int i = 0; i <= 80; i++)
			glVertex2f(radius*cosf(i*twopi / number), radius*sinf(i*twopi / number));
		glEnd();

		*/
			   
		// glfw: swap buffers and poll IO events (key pressed/release, mouse moved etc)
		glfwSwapBuffers(window);
		glfwPollEvents();
		//glFlush();
	}

	return 0;
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	std::cout << key << "\n";
	switch (key)
	{
	case GLFW_KEY_C:	printf("Connecting... \n");
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
	glShaderSource(id, 1, &src, NULL);
	glCompileShader(id);

	int result;
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
	unsigned int program = glCreateProgram();
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
	glDeleteShader(vs);
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
	std::cout << "SHADER PROGRAM" << shaderprog;
		
	return shaderprog;
}



void framebuffer_size_callback(GLFWwindow* window, int width, int height)			// Callback for any change in the position/size of the generated window
{
	glViewport(0, 0, width, height);
}
