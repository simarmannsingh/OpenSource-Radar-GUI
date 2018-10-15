#shader vertex 1			// VERTEX SHADER 1					
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;

layout(location = 2) in vec2 texCoord;


uniform vec3 u_Color;
out vec2 v_texCoord;

void main()
{
	gl_Position = vec4(aPos, 1.0);	
	u_Color = aColor;
	v_texCoord = texCoord;
}

#shader vertex 2						// VERTEX SHADER 2		
#version 330 core
layout(location = 0) in vec3 aPos;			
layout(location = 1) in vec2 texCoord;		

out vec2 v_texCoord;							

void main()
{
	gl_Position = vec4(aPos, 1.0);
	v_texCoord = texCoord;
}

#shader fragment					// FRAGMENT SHADER
#version 330 core
layout(location = 0) out vec4 FragColor;

in vec3 u_Color;
in vec2 v_texCoord;


uniform sampler2D u_Texture;


void main()
{
	FragColor = texture(u_Texture, v_texCoord) * vec4(u_Color, 1.0f);
};