#shader vertex
#version 330 core

layout (location = 0) in vec3 aPos;

void main()
{
   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 0.7);
};

#shader fragment
#version 330 core

layout(location=0) out vec4 FragColor;

void main()
{
   FragColor = vec4(0.5f, 0.9f, 0.0f, 1.0f);
};