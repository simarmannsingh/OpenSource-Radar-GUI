#include"VertexArray.h"
#include"Renderer.h"

VertexArray::VertexArray()
{
	//vertex array
	glGenVertexArrays(1, &va_RendererID);	
}


VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &va_RendererID);
}

void VertexArray::AddBuffer(const VertexBuffer & vb, const VertexBufferLayout & layout)
{
	Bind();													// Binding Vertex array
	vb.bind();												// Binding Vertex buffer
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset);
		offset += element.count *VertexBufferElement::GetSizeOfType(element.type);
	}
}

void VertexArray::Bind() const
{
	glBindVertexArray(va_RendererID);
}

void VertexArray::unBind() const
{
	glBindVertexArray(0);
}
