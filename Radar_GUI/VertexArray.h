#pragma once
#include"VertexBuffer.h"
#include "VertexBufferLayout.h"


class VertexArray 
{
private:
	unsigned int va_RendererID;

public:
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

	void Bind() const;
	void unBind() const;
};