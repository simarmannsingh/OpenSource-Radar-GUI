
#include "Renderer.h"


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