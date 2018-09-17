#include"VertexBuffer.h"
#include"Renderer.h"

//VertexBuffer::VertexBuffer()

VertexBuffer::VertexBuffer(const void * data, unsigned int size)
{
	//buffer vertex 
	glGenBuffers(1, &vb_RendererID);									//we can generate a vertex buffer object(VBO) or a vertex array object(VAO)
	glBindBuffer(GL_ARRAY_BUFFER, vb_RendererID);						//bind that generated buffer to the type GL_ARRAY_BUFFER
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);  //Copies vertex data into buffer's memory

}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &vb_RendererID);
}

void VertexBuffer::bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, vb_RendererID);			//bind that generated buffer to the type GL_ARRAY_BUFFER
}

void VertexBuffer::unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);						//bind that generated buffer to the type GL_ARRAY_BUFFER
}
