#shader vertex 1			// VERTEX SHADER 1					
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 texCoord;
out vec2 v_texCoord;

void main()
{
	gl_Position = vec4(aPos, 1.0);	
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

in vec2 v_texCoord;

uniform vec4 u_Color;
uniform sampler2D u_Texture;


void main()
{
	vec4 texColor = texture(u_Texture, v_texCoord);
	FragColor = u_Color;
};