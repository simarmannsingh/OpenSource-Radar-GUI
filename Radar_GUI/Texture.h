#pragma once

#include "Renderer.h"
#include "stb_image.h"
class Texture
{
private : 
	unsigned int m_RendererID;
	std::string m_filePath;
	unsigned char* m_localBuffer;
	int m_width, m_height, m_BPP;
	//unsigned int slot = 0;

public:
	Texture(const std::string &path);
	~Texture();

	void bind(unsigned int slot = 0) const;
	void unBind();

	inline int GetWidth() const { return m_width; }
	inline int GetHeight() const { return m_height; }
};