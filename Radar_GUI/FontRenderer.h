#pragma once
#include <ft2build.h>
#include "graphics.h"
#include <iostream>
#include <map>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader_s.h"

#include FT_FREETYPE_H

struct Character {
	GLuint TextureID;		// ID handle of the glyph texture
	glm::ivec2 Size;		// Size of glyph
	glm::ivec2 Bearing;		// Offset from baseline to left/top of glyph
	GLuint Advance;			// Horizontal offset to advance to next glyph
};


class FontRenderer
{
private:
	//Global variables
	FT_Library ft;
	FT_Face face;

public:
	void RenderText(Shader_s fshader, std::string text, GLfloat x, GLfloat y, GLfloat scale, GLuint va_font, GLuint vb_font);
	Shader_s FontInit();
	void FontSetup();
};
