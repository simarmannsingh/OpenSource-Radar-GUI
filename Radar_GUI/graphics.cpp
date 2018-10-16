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
#include "Shader.h"
#include "Shader_s.h"
#include "Texture.h"

bool flag_coherent = FALSE;
bool flag_CFAR = FALSE;

int width = 200;
int height = 200;


int Screen_width = 800;
int Screen_height = 800;


float centerX = 320.0;
float centerY = 240.0;
float PI_180 = M_PI / 180.0f;
float radius = (centerX < centerY) ? centerX : centerY;
float rad_64 = radius / 64;
float rad_3 = radius / 3;

const char* glsl_version = "#version 130";																
bool show_demo_window = true;																			
static int counter = 0;																					
static float f = 0.0f;																					

// Initialize the FreeType fonts
FontRenderer FR;
Shader_s font_shader;
GLuint va_font, vb_font;

const int v_corners = 80;


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

int glRenderLoop(GLFWwindow* window)
{
	/*
	 -------------------------------------------------------------------------------------------------------------------------------------------

														-->>  RENDER LOOP  <<--

	 -------------------------------------------------------------------------------------------------------------------------------------------
	*/
	
		static float vertices[] = {											// vertices - vertex data for rectangle
			-1.0f,		-1.0f,   0.0f,   0.0f,		// vetex 1
			 1.0f,		-1.0f,   1.0f,   0.0f,		// vetex 2 
			 1.0f,	    1.0f,    1.0f,   1.0f,		// vetex 3
			-1.0f,		1.0f,    0.0f,   1.0f		// vetex 4
		};

		static float vertices_tri[] = {											// vertices - vertex data for rectangle
			-1.0f,		-1.0f,   0.0f,   0.0f,		// vetex 1
			 1.0f,		-1.0f,   1.0f,   0.0f,		// vetex 2 
			 0.0f,	    1.0f,    1.0f,   1.0f,		// vetex 3		
		};

		GLfloat vertices_PPI[v_corners][2];									//vertices_PPI - vertex data for Circle		
		GLfloat angle = 0.0;
		GLfloat radius = 1.0;

		for (int i = 0; i < v_corners; i++)									// Initializing the vertex_PPI vector 
		{
			angle = i * 2 * 3.14 / v_corners;
			vertices_PPI[i][0] = radius * cos(angle);
			vertices_PPI[i][1] = radius * sin(angle);

		}

		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		unsigned int indices_tri[] = {
		0, 1, 2
		};

		unsigned int indices_PPI[] = {
			0, 1, 2,
			0, 2, 3,
			0, 3, 4,
			0, 4, 5,
			0, 5, 6,
			0, 6, 7,
			0, 7, 8,
			0, 8, 9,
			0, 9, 10,
			0, 10, 11,
			0, 11, 12,
			0, 12, 1,
		};
		/*
		float texCoord[] = {
			0.0f, 0.0f,		//Lower-left corner
			1.0f, 0.0f,		//Lower-Right corner
			0.5f, 1.0f,		//top-center corner
		};
		*/
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//Generating CIRCLE's Buffers and adding data
		VertexArray va_circ;
		VertexBuffer vb_circ(vertices_PPI, sizeof(vertices_PPI));
		VertexBufferLayout layout_circ;
		layout_circ.Push<float>(2);
		layout_circ.Push<float>(2);
		va_circ.AddBuffer(vb_circ, layout_circ);
		IndexBuffer ib_circ(indices_PPI, 12);
		va_circ.unBind();
		vb_circ.unbind();
		ib_circ.unbind();
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

		// Generating RECTANGLE's buffer and adding data 
		VertexArray va_rec;
		VertexBuffer vb_rec(vertices, sizeof(vertices));
		VertexBufferLayout layout_rec;
		layout_rec.Push<float>(2);
		layout_rec.Push<float>(2);
		va_rec.AddBuffer(vb_rec, layout_rec);
		IndexBuffer ib_rec(indices, 6);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertices), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);
		va_rec.unBind();
		vb_rec.unbind();
		ib_rec.unbind();
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);


		// Generating TRIANGLE's buffer and adding data 
		VertexArray va_tri;
		VertexBuffer vb_tri(vertices_tri, sizeof(vertices_tri));
		VertexBufferLayout layout_tri;
		layout_tri.Push<float>(2);
		layout_tri.Push<float>(2);
		va_tri.AddBuffer(vb_tri, layout_tri);
		IndexBuffer ib_tri(indices_tri, 3);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertices_tri), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		va_tri.unBind();
		vb_tri.unbind();
		ib_tri.unbind();
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);


		Shader shader1("res/shaders/Basic.shader", 1);
		//Shader shader2("res/shaders/Basic.shader", 2);
		shader1.bind();
		//shader2.bind();

		shader1.setUniform4f("u_Color", 0.9f, 0.0f, 0.0f, 1.0f);
		//shader2.setUniform4f("u_Color", 0.9f, 0.0f, 0.0f, 1.0f);
		shader1.setUniform1i("u_Texture", 0);
		//shader2.setUniform1i("u_Texture", 0);

		//creating texture
		Texture texture("res/images/diamond.png");
		texture.bind();
		//shader.unBind();
		

	/*
	-------------------------------------------------------------------------------------------------------------------------------------------

												FONT rendering using Freetype library

	-------------------------------------------------------------------------------------------------------------------------------------------
	*/
			
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

														-->>  ImGUI Setup  <<--

	 -------------------------------------------------------------------------------------------------------------------------------------------
	*/
	 
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	//ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	ImGui::StyleColorsDark();
		

	/*
	 -------------------------------------------------------------------------------------------------------------------------------------------

														-->>  Translation Matrix  <<--

	 -------------------------------------------------------------------------------------------------------------------------------------------
	*/
	{
		/*
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
	}
	/*
	 -------------------------------------------------------------------------------------------------------------------------------------------

														-->>  OpenGL Loop  <<--

	 -------------------------------------------------------------------------------------------------------------------------------------------
	*/
	while (!glfwWindowShouldClose(window))
	{	
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shader1.bind();
		glViewport(0, 0, 800, 800);
		shader1.setUniform4f("u_Color", 0.8f, 0.8f, 0.1f, 1.0f);
				
		{
			// RECTANGLES
			// Rendering object number 1		-  OUTER RECT
			// Defining Properties 
			va_rec.Bind();
			ib_rec.bind();			
			glViewport(540, 640, 260, 160);
			glLineWidth(10.0f);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			shader1.setUniform4f("u_Color", 0.0f, 0.1f, 0.6f, 1.0f);
			
			//Drawing the object
			GLClearError();
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
			GLCheckError();			
			
			glViewport(545, 645, 250, 150);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			shader1.setUniform4f("u_Color", 0.4f, 0.4f, 0.4f, 1.0f);
			
			//Drawing the object			
			GLClearError();
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
			GLCheckError();

			va_rec.unBind();
		}
		
		{
			//CIRCELS
			// Outer Circle
			va_circ.Bind();
			ib_circ.bind();
			glViewport(60, 100, 600, 600);
			shader1.setUniform4f("u_Color", 0.2f, 0.2f, 0.6f, 1.0f);			
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glDrawArrays(GL_TRIANGLE_FAN, 0, 80);
			//Inner Circle
			glViewport(70, 110, 580, 580);
			shader1.setUniform4f("u_Color", 0.2f, 0.2f, 0.3f, 1.0f);
			glDrawArrays(GL_TRIANGLE_FAN, 0, 80);

			//first ring
			glViewport(210, 250, 300, 300);
			shader1.setUniform4f("u_Color", 0.15f, 0.15f, 0.25f, 1.0f);
			glDrawArrays(GL_TRIANGLE_FAN, 0, 80);

			glViewport(215, 255, 290, 290);
			shader1.setUniform4f("u_Color",0.2f, 0.2f, 0.3f, 1.0f);
			glDrawArrays(GL_TRIANGLE_FAN, 0, 80);

			//Center Dome
			glViewport(355, 395, 10, 10);
			shader1.setUniform4f("u_Color",0.15f, 0.15f, 0.25f, 1.0f);
			glDrawArrays(GL_TRIANGLE_FAN, 0, 80);
			va_circ.unBind();
			ib_circ.unbind();
		}
		/*
		{
		//Scanning Beam
			va_tri.Bind();
			glViewport(360, 400, 270, 270);
			shader1.setUniform4f("u_Color", 0.4f, 0.4f, 0.4f, 1.0f);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			//Drawing the object			
			GLClearError();
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
			GLCheckError();

			va_tri.unBind();
		}
		*/
		glViewport(550, 650, 240, 140);
		shader1.setUniform4f("u_Color", 0.8f, 0.8f, 0.8f, 1.0f);
		FR.RenderText(font_shader, "Status :", 20.0f, 640.0f, 1.0f, va_font, vb_font);
		if (flag_coherent)
		{
			FR.RenderText(font_shader, "COHERENT", 320.0f, 640.0f, 1.0f, va_font, vb_font);
		}
		else
		{
			FR.RenderText(font_shader, "INCOHERENT", 320.0f, 640.0f, 1.0f, va_font, vb_font);
		}

		FR.RenderText(font_shader, "CFAR   :", 20.0f, 580.0f, 1.0f, va_font, vb_font);
		if (flag_CFAR)
		{
			FR.RenderText(font_shader, "ON", 320.0f, 580.0f, 1.0f, va_font, vb_font);
		}
		else
		{
			FR.RenderText(font_shader, "OFF", 320.0f, 580.0f, 1.0f, va_font, vb_font);
		}		
		
		
		
		// ImGUI
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		
		{
			ImGui::Begin("Hello, world!");
			if (show_demo_window)
				ImGui::ShowDemoWindow(&show_demo_window);

			ImGui::Text("RADAR Controls");               // Display some text (you can use a format strings too)
			ImGui::Checkbox("CFAR", &show_demo_window);      // Edit bools storing our window open/close state
			ImGui::Checkbox("COHERENT", &show_demo_window);      // Edit bools storing our window open/close state
			ImGui::Text("ZOOM");
			ImGui::SameLine();
			ImGui::SliderFloat("", &f, 0.0f, 1.0f);

			ImGui::Text("Andgular Offset :");
			ImGui::SameLine();
			ImGui::SliderFloat("", &f, 0.0f, 1.0f);

			ImGui::Text("ZeroPoint Offset :");
			ImGui::SameLine();
			ImGui::SliderFloat("", &f, 0.0f, 1.0f);

			if (ImGui::Button("Button"))
				counter++;
			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End(); 
		}


		ImGui::Render();
		glfwMakeContextCurrent(window);
		glfwGetFramebufferSize(window, &Screen_width, &Screen_height);
		glViewport(0, 0, Screen_width, Screen_height);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
					
		glFlush();
			   
		// glfw: swap buffers and poll IO events (key pressed/release, mouse moved etc)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}	
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	
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
	if (key == GLFW_KEY_O && action == GLFW_PRESS)
	{
		if (!flag_CFAR)
		{
			printf("CFAR ON\n");
			flag_CFAR = TRUE;

		}
		else
		{
			printf("CFAR OFF\n");
			flag_CFAR = FALSE;
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


void framebuffer_size_callback(GLFWwindow* window, int width, int height)			// Callback for any change in the position/size of the generated window
{
	glViewport(0, 0, width, height);
}
