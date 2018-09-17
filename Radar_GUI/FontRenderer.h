#pragma once
#include <ft2build.h>
#include "graphics.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include FT_FREETYPE_H

void RenderText( std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);
void FontInit();

