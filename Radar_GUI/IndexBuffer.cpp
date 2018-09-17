#include"IndexBuffer.h"
#include"Renderer.h"

//IndexBuffer::IndexBuffer()

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
	:m_Count(count)
{
	_ASSERT(sizeof(unsigned int) == sizeof(GLuint)); 

	glGenBuffers(1, &m_RendererID);												//we can generate a vertex buffer object(VBO) or a vertex array object(VAO)
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);						//bind that generated buffer to the type GL_ARRAY_BUFFER
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count*sizeof(unsigned int), data, GL_STATIC_DRAW);  //Copies vertex data into buffer's memory

}

IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &m_RendererID);
}

void IndexBuffer::bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);						//bind that generated buffer to the type GL_ARRAY_BUFFER
}

void IndexBuffer::unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);						//bind that generated buffer to the type GL_ARRAY_BUFFER
}
