
#include "Renderer.h"


void  GLCheckError()
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] Error Code ->" << error << std::endl;
	}
}

void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}
