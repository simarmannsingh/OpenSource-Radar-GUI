/*
 -------------------------------------------------------------------------------------------------------------------------------------------

 Author			:	Simar Mann Singh
 Department		:	High Frequency Department, TF
 Date			:	28/08/2018

 -------------------------------------------------------------------------------------------------------------------------------------------
*/

/*
CODE
(search for "[SEGMENT]" in the code to find more about them )

// [SEGMENT] VARIABLE DECLARATIONS
// [SEGMENT] FUNCTION DECLARATIONS
// [SEGMENT] GLFW AND OPENGL INITIALIZATION
// [SEGMENT] CONTEXT CREATION AND GLAD INITIALIZATION
// [SEGMENT] MAIN RENDER LOOP
// [SEGMENT] REGISTERING KEY PRESS INTERRUPT USING CALLBACKS
// [SEGMENT] REGISTERING ERROR INTERRUPT USING CALLBACKS
// [SEGMENT] CHANGING SCREENSIZE USING CALLBACK
// [SEGMENT] DESIGNING SCREENOVERLAY TO RENDER INFORMATION TAB ON SCREEN
// [SEGMENT] ANIMATING SIGNAL ON INFORMATION TAB
*/

#include "graphics.h"

//-------------------------------------------------------------------------------------------------------------------------------------------------
// [SEGMENT] VARIABLE DECLARATIONS
//-------------------------------------------------------------------------------------------------------------------------------------------------

static bool flag_coherent	= FALSE;
static bool flag_connect	= FALSE;
static bool flag_CFAR		= FALSE;
static bool Close_App		= FALSE;
static bool Background		= TRUE;
static bool InfoButton		= FALSE;
static bool ScreenOverlayFlag = TRUE;
static bool infoFlag        = FALSE;
static bool animate         = FALSE;
bool show_demo_window		= TRUE;																			


int width = 200;
int height = 200;

int Screen_width = 800;
int Screen_height = 800;
								
// Variables for Radar PPI display
float centerX = 320.0;
float centerY = 240.0;
float PI_180 = M_PI / 180.0f;
float radius = (centerX < centerY) ? centerX : centerY;
float rad_64 = radius / 64;
float rad_3 = radius / 3;

// Variables for ImGui
const char* glsl_version = "#version 130";																
static int counter = 0;																					
																		
const float DISTANCE = 20.0f;
const float x_DISTANCE = 20.0f;
const float y_DISTANCE = 220.0f;
const float NetOvrlayDistance = 420.0f;
static int ScreenOverlay_n = 0;
double elapsedTime = 0;
static int corner = 1;

static float Slider1 = 0.0f;																					
static float Slider2 = 0.0f;																					
static float Slider3 = 0.0f;																					



// Variables for FreeType fonts
FontRenderer FR;
Shader_s font_shader;
GLuint va_font, vb_font;

const int v_corners = 80;


//-------------------------------------------------------------------------------------------------------------------------------------------------
// [SEGMENT] FUNCTION DECLARATIONS
//-------------------------------------------------------------------------------------------------------------------------------------------------

void glInitialize();																	// To initialize GLFW and OpenGL library
GLFWwindow* glCreateWindow();															// To create an empty window of size 1080 x 720 and initialize Glad library 
int glRenderLoop(GLFWwindow* window);													// To render every graphical entity in the window created
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);		// To enable Keyboard Input to system using system callbacks
void error_callback(int error, const char* description);								// To enable error reporting using error callbacks
void framebuffer_size_callback(GLFWwindow* window, int width, int height);				// Callback for any change in the position/size of the generated window
void screenOverlay(bool* p_open, int corner);											// Enabling screen Overlay to design the information tab. Funciton called from Render Loop
void renderGraph();																		// Animating signal, represented on the Information Tab. Function invoked from Render Loop


//-------------------------------------------------------------------------------------------------------------------------------------------------
// [SEGMENT] GLFW AND OPENGL INITIALIZATION 
//-------------------------------------------------------------------------------------------------------------------------------------------------
void glInitialize()
{
	
	// Initializing GLFW Library and setting minimum required version to OpenGL 3.3
	
	// GLFW Initialization
	int glfw_Init = glfwInit();
	if (!glfw_Init)
	{
		printf("Failed at GLFW Initialization\n");
		Sleep(3000);
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);										// defines the version of the openGL used 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);						// We are using <<---OPENGL_CORE--->> Profile
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	printf("GLFW initialization completed successfully\n");

	
}

//-------------------------------------------------------------------------------------------------------------------------------------------------
// [SEGMENT] CONTEXT CREATION AND GLAD INITIALIZATION 
//-------------------------------------------------------------------------------------------------------------------------------------------------
GLFWwindow* glCreateWindow()
{
	// To create an empty window of size 1080 x 720 and initialize Glad library 
	// Registering Interrupt function calls using callbacks
	glfwSetErrorCallback(error_callback);	
	GLFWwindow* window = glfwCreateWindow(1080, 720, "Radar - HF dept TF", NULL, NULL);	// creating a window 1080x720 in size			--->> 800 x 600
	if (window == NULL)
	{
		printf("Failed at creating window\n");
		Sleep(2000);
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	printf("Window Creation completed successfully\n");
	
	// Make the window's context current 
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

//-------------------------------------------------------------------------------------------------------------------------------------------------
// [SEGMENT] MAIN RENDER LOOP 
//-------------------------------------------------------------------------------------------------------------------------------------------------
int glRenderLoop(GLFWwindow* window)
{
	/*
	 -------------------------------------------------------------------------------------------------------------------------------------------
					To render every graphical entity in the window created
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
			angle = (GLfloat)(i * 2 * 3.14 / v_corners);
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
		 


	//	FONT RENDERING USING FREETYPE LIBRARY
			
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
		
	//  ImGUI Setup  
	 
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	ImGui::StyleColorsDark();
		

	//  TRANSLATION MATRIX

	{
		
		glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
		glm::mat4 trans(1);
		std::cout << "Before  :::  \nvec.x  : " << vec.x << std::endl << "vec.y : " << vec.y << std::endl << "vec.z : " << vec.z << std::endl;
		trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
		vec = trans * vec;
		//trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
		//trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
		std::cout << "After  :::   ---> vec.x  : " << vec.x << std::endl << "vec.y : " << vec.y << std::endl << "vec.z : " << vec.z << std::endl;


		glm::mat4 transform;
		transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));
		transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
		//glUseProgram(shdrprgm);

		//unsigned int transformLoc = glGetUniformLocation(shdrprgm, "transform");
		//glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

		//unsigned int location = glGetUniformLocation(shdrprgm, "transform");
		//glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(trans));
		
	}

	// OPENGL LOOP
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
	
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Radar Beam
		shader1.bind();
		shader1.setUniform4f("u_Color", 0.1f, 0.21f, 0.09f, 0.12f);
		va_circ.Bind();
		ib_circ.bind();
		if (Background)
		{
			glViewport(-160, -250, 1280, 1200);
			elapsedTime = glfwGetTime();
			if (elapsedTime > 10)
			{
				elapsedTime = 0;
			}
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			for (int i = 0; i < 10; i++)
			{
				glDrawArrays(GL_TRIANGLE_FAN, i, i + 15);
			}

			glViewport(650, -100, 1280, 1200);
			for (int i = 0; i < 8; i++)
			{
				shader1.setUniform4f("u_Color", 0.12f, 0.63f, 0.02f, 0.15f);
				glDrawArrays(GL_TRIANGLE_FAN, i, i + 15);
			}
		}

		glViewport(0, 0, 800, 800);
		shader1.setUniform4f("u_Color", 0.8f, 0.8f, 0.1f, 1.0f);		
		{
			//CIRCELS
			// Outer Circle
		
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
			
		}
		
		
		va_circ.unBind();
		ib_circ.unbind();
		
		const char* item[] = { "2x", "4x" ,"8x" };
		{
			// ImGUI
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			//ImGui::ShowDemoWindow(&show_demo_window);
			// Screen Overlay
			if (ScreenOverlayFlag)
			// Funtion Calls for rendering Different GUI sections on Screen
				screenOverlay(&ScreenOverlayFlag, corner);
				screenOverlayTwo(&ScreenOverlayFlag, corner);
				SmartHUD(&ScreenOverlayFlag, 2);


				ImVec2 window_pos = ImVec2((corner & 1) ? ImGui::GetIO().DisplaySize.x - x_DISTANCE : x_DISTANCE, (corner & 2) ? ImGui::GetIO().DisplaySize.y - NetOvrlayDistance : NetOvrlayDistance);
				ImVec2 window_pos_pivot = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
				ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
				ImGui::SetNextWindowBgAlpha(0.3f); // Transparent background
								
				ImGui::Begin("Network Controls", &ScreenOverlayFlag, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);

				ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "NETWORK SETTINGS");
				ImGui::Separator();
				
				static char buf2[64] = ""; ImGui::InputText("decimal", buf2, 64, ImGuiInputTextFlags_CharsDecimal);
				ImGui::SameLine(250);
				if (ImGui::Button("Connect", ImVec2(80, 20)))
				{
					char key[] = "192.168.1.100";
					int res = strcmp(buf2, key);
					if (res )
					{
						Beep(400, 500);
					}
				}
				ImGui::Checkbox("Plot Graph",&animate);
				renderGraph();

				if (ImGui::Button("Info", ImVec2(80, 20)))
					ImGui::OpenPopup("Credits");
				if (ImGui::BeginPopupModal("Credits", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove))
				{
					ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "Development Credits ");
					ImGui::Text("Departmet Head   :  Prof. Dr.-Ing. Michael Hoft");
					ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.7f, 0.8f), "(Email)          :  mh@tf.uni-kiel.de ");
					ImGui::Text("Maintainer       :  Dr.-Ing. Alexander Tyuplik");
					ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.7f, 0.8f), "(Email)          :  alt@tf.uni-kiel.de ");
					ImGui::Text("Source Code      :  Simar Mann Singh");
					ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "\n\nCopyrights ");
					ImGui::Text("(c) High Frequency (HF) Department \nChristian-Albrechts-Universitat zu Kiel (CAU) \n\n");
					ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "All rights reserved!");
					ImGui::Text("Modification, sublicensing, or distribution of this Software Program \nis strictly prohibited!!\n\n");
					ImGui::Separator();
					ImGui::Text("");
					ImGui::SameLine(200);
					if (ImGui::Button("OK", ImVec2(80, 20))) { ImGui::CloseCurrentPopup(); }
					ImGui::SetItemDefaultFocus();
					ImGui::EndPopup();
				}
				
				ImGui::SameLine();
				if (ImGui::Button("Exit", ImVec2(80, 20)))
				{
					ImGui::End();
					ImGui_ImplOpenGL3_Shutdown();
					ImGui_ImplGlfw_Shutdown();
					ImGui::DestroyContext();
					return 0;
				}
				ImGui::SameLine();
				ImGui::End();			
			
			ImGui::Render();
			glfwMakeContextCurrent(window);
			glfwGetFramebufferSize(window, &Screen_width, &Screen_height);
			glViewport(0, 0, Screen_width, Screen_height);
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}
		glFlush();
			   
		// glfw: swap buffers and poll IO events (key pressed/release, mouse moved etc)
		glfwMakeContextCurrent(window);
		glfwSwapBuffers(window);		
	}	
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	
	return 0;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------
// [SEGMENT] REGISTERING KEY PRESS INTERRUPT USING CALLBACKS
//-------------------------------------------------------------------------------------------------------------------------------------------------
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


//-------------------------------------------------------------------------------------------------------------------------------------------------
// [SEGMENT] REGISTERING ERROR INTERRUPT USING CALLBACKS
//-------------------------------------------------------------------------------------------------------------------------------------------------
void error_callback(int error, const char* description)
{
	puts(description);
	Sleep(2000);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------
// [SEGMENT] CHANGING SCREENSIZE USING CALLBACK
//-------------------------------------------------------------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)			
{
	glViewport(0, 0, width, height);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------
// [SEGMENT] DESIGNING SCREENOVERLAY TO RENDER INFORMATION TAB ON SCREEN
//-------------------------------------------------------------------------------------------------------------------------------------------------
void screenOverlay(bool* p_open, int corner)
{
	ImVec2 window_pos = ImVec2((corner & 1) ? ImGui::GetIO().DisplaySize.x - DISTANCE : DISTANCE, (corner & 2) ? ImGui::GetIO().DisplaySize.y - DISTANCE : DISTANCE);
	ImVec2 window_pos_pivot = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
	if (corner != -1)
	
	ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
	ImGui::SetNextWindowBgAlpha(0.3f); // Transparent background
	if (ImGui::Begin("Example: Simple Overlay", p_open, (corner != -1 ? ImGuiWindowFlags_NoMove : 0) | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav))
	{
		ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "STATUS");
		ImGui::Separator();
		if (!flag_connect)
		{
			ImGui::Text("Connection Status  : Disconnected    \n");
		}
		else
		{
			ImGui::Text("Connection Status  : Connected   \n");
		}
		
		
		if (!flag_coherent)
		{
			ImGui::Text("Coherent Status    : Incoherent\n");
		}
		else
		{
			ImGui::Text("Coherent Status    : Coherent\n");
		}
		
		if (!flag_CFAR)
		{
			ImGui::Text("CFAR Mode	      : OFF       \n");
		}
		else
		{
			ImGui::Text("CFAR Mode	      : ON       \n");
		}
		ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "Signal Reception");
		ImGui::Separator();
		

		ImGui::Text("Amplitude : ");
		if (flag_connect)
		{
			ImGui::SameLine(100);
			ImGui::Text(" %0.3f", (2 * ImGui::GetIO().Framerate));
		}
		ImGui::Text("Direction : ");
		if (flag_connect)
		{
			ImGui::SameLine(100);
			ImGui::Text(" %d", flag_CFAR ? 1 : 0);
		}

		ImGui::Text("Angle     : ");
		if (flag_connect)
		{
			ImGui::SameLine(100);
			ImGui::Text(" %0.3f", (ImGui::GetIO().Framerate * 3));
		}

		ImGui::Text("Range     : ");
		if (flag_connect)
		{
			ImGui::SameLine(100);
			ImGui::Text(" %0.3f", (ImGui::GetTime() / 5) * 2 * ImGui::GetIO().Framerate);
		}

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)   ", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
	ImGui::End();
}

void screenOverlayTwo(bool* p_open, int corner)
{
	ImVec2 window_pos = ImVec2((corner & 1) ? ImGui::GetIO().DisplaySize.x - x_DISTANCE : x_DISTANCE, (corner & 2) ? ImGui::GetIO().DisplaySize.y - y_DISTANCE : y_DISTANCE);
	ImVec2 window_pos_pivot = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
	ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
	ImGui::SetNextWindowBgAlpha(0.3f); // Transparent background
	
	ImGui::Begin("RADAR Controls", &ScreenOverlayFlag, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);

	ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "RADAR SETTINGS");
	ImGui::Separator();
	if (ImGui::Checkbox("Connect", &flag_connect))      // Edit bools storing our window open/close state
	{
		;// animate = FALSE;
	}
	ImGui::SameLine(150);
	ImGui::RadioButton("2x Zoom", &counter, 0);
	ImGui::Checkbox("Coherent", &flag_coherent);      // Edit bools storing our window open/close state
	ImGui::SameLine(150);
	ImGui::RadioButton("4x Zoom", &counter, 1);
	ImGui::Checkbox("CFAR", &flag_CFAR);      // Edit bools storing our window open/close state

	ImGui::Text("ZOOM            :");
	ImGui::SameLine();
	ImGui::SliderFloat("Slider1", &Slider1, 0.0f, 1.0f);
	ImGui::Text("Andgular Offset :");
	ImGui::SameLine();
	ImGui::SliderFloat("Slider2", &Slider2, 0.0f, 1.0f);
	ImGui::Text("ZeroPoint Offset:");
	ImGui::SameLine();
	ImGui::SliderFloat("Slider3", &Slider3, 0.0f, 1.0f);
	ImGui::Text("   ");

	ImGui::End();
}

void SmartHUD(bool* p_open, int corner)
{
	ImVec2 window_pos = ImVec2((corner & 1) ? ImGui::GetIO().DisplaySize.x - x_DISTANCE : x_DISTANCE, (corner & 2) ? ImGui::GetIO().DisplaySize.y - x_DISTANCE : x_DISTANCE);
	ImVec2 window_pos_pivot = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
	ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
	ImGui::SetNextWindowBgAlpha(0.3f); // Transparent background

	ImGui::Begin("Display", &ScreenOverlayFlag, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);

	ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "Display Options");
	ImGui::Separator();
	ImGui::Checkbox("Background", &Background);      // Edit bools storing our window open/close state
	
	ImGui::End();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------
// [SEGMENT] ANIMATING SIGNAL FLOW ON INFORMATION TAB 
//-------------------------------------------------------------------------------------------------------------------------------------------------
void renderGraph()
{			
		// Tip: If your float aren't contiguous but part of a structure, you can pass a pointer to your first float and the sizeof() of your structure in the Stride parameter.
		static float values[90] = { 0 };
		static int values_offset = 0;
		static double refresh_time = 0.0;
		if (!animate || refresh_time == 0.0f)
			refresh_time = ImGui::GetTime();
		while (refresh_time < ImGui::GetTime()) // Create dummy data at fixed 60 hz rate for the demo
		{
			static float phase = 0.0f;
			values[values_offset] = cosf(phase);
			values_offset = (values_offset + 1) % IM_ARRAYSIZE(values);
			phase += 0.5f*values_offset;
			refresh_time += 1.0f / 60.0f;				// 60.0f represents plotting frequency which is 60Hz
		}
	if (flag_connect && animate)
	{
		// Create a dummy array of contiguous float values to plot
		ImGui::PlotLines("Lines", values, IM_ARRAYSIZE(values), values_offset, "avg 0.0", -1.0f, 1.0f, ImVec2(340, 100));
	}
	
	else if ((!flag_connect && animate))
	{
		ImGui::OpenPopup("Error");
		ImGui::PlotLines("Lines", NULL, NULL, NULL, "avg 0.0", -1.0f, 1.0f, ImVec2(340, 100));
		if (ImGui::BeginPopupModal("Error", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove))
		{
			ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "Radar Disconnected\n\n");
			ImGui::Text("Please connect the Radar to the computer\nand click on Connect Checkbox.");
			ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.7f, 0.8f), "(Connection will establish automatically)\n\n\n");
			//ImGui::Text("Data unavailable for plotting graph");
			ImGui::Separator();
			ImGui::Text("");
			ImGui::SameLine(105);
			if (ImGui::Button("OK", ImVec2(80, 20)))
			{
				animate = FALSE;
				flag_connect = FALSE;
				ImGui::PlotLines("Lines", NULL, NULL, NULL, "avg 0.0", -1.0f, 1.0f, ImVec2(340, 100));
				ImGui::CloseCurrentPopup();				
			}
			ImGui::SetItemDefaultFocus();
			ImGui::EndPopup();
		}

	}
	else 
	{
		ImGui::PlotLines("Lines", NULL, NULL, NULL, "avg 0.0", -1.0f, 1.0f, ImVec2(340, 100));
	}
}






/*
There are two flags:-
1. Bool "Connect"		---->			CHECKBOX    //This flag is raised when Checkbox "Connect" is clicked 
2. Bool "Animate"		---->			CHECKBOX    //This flag is raised when Checkbox "Plot Graph" is clicked 

Situation:
If (!Connect)
	then PLOT GRAPH should not work which means 
	Animate = FALSE;

			If(Connect)
			and then if PLOT GRAPH is clicked, graph can be plotted which means
				Animate = TRUE;
			else
			This should show an error that without Connect enabled, PLOT GRAPH doesn't work
			For this, I created the below logic
			//function for displaying  error()
							IF





*/











