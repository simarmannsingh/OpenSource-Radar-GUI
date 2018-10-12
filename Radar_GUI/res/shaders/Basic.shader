#shader vertex 1								//Vertex shader - 1
#version 330 core
layout(location = 0) in vec3 aPos;				// the position variable has attribute position 0
//layout(location = 1) in vec3 aColor;			// the color variable has attribute position 1

//out vec4 u_Color;								// output a color to the fragment shader

void main()
{
	gl_Position = vec4(aPos, 1.0);
	//u_Color = aColor;							// set ourColor to the input color we got from the vertex data
}


#shader vertex 2								//Vertex shader - 2
#version 330 core
layout(location = 0) in vec3 aPos;			
layout(location = 1) in vec3 aColor;		

out vec4 u_Color;							

void main()
{
	gl_Position = vec4(aPos, 1.0);
	u_Color = aColor;							
}


#shader fragment
#version 330 core

out vec4 FragColor;   //layout(location = 0) out vec4 FragColor;

uniform vec4 u_Color;

void main()
{
	FragColor = u_Color;
};