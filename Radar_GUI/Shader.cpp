#include "Shader.h"

unsigned int fs;
unsigned int vs;




Shader::Shader(const std::string & filepath, int vertexshdr_n)
	:m_FilePath(filepath), m_RendererID(0)
{
	shaderProgramSource spsource = ParseShader(filepath);
	if (vertexshdr_n == 1)
	{
		std::cout << "VERTEX Shader 1  :-\n" << spsource.vertexSource1 << std::endl;
		std::cout << "FRAGMENT Shader  :-\n" << spsource.fragSource;
		m_RendererID = createShader(spsource.vertexSource1, spsource.fragSource);
	}
	else if (vertexshdr_n == 2)
	{
		std::cout << "VERTEX Shader 2  :-\n" << spsource.vertexSource2 << std::endl;
		std::cout << "FRAGMENT Shader  :-\n" << spsource.fragSource;
		m_RendererID = createShader(spsource.vertexSource2, spsource.fragSource);
	}
	std::cout << "Object Shader Program Compilation Successfull..." << std::endl;

}

Shader::~Shader()
{
	glDeleteProgram(m_RendererID);
}

void Shader::bind() const
{
	glUseProgram(m_RendererID);
}

void Shader::unBind() const
{
	glUseProgram(0);
}

void Shader::setUniform1i(const std::string & name, int value)
{
	glUniform1i(GetUniformLocation(name), value);
}

void Shader::setUniform1f(const std::string & name, float value)
{
	glUniform1f(GetUniformLocation(name), value);
}

void Shader::setUniform4f(const std::string & name, float v0, float v1, float v2, float v3)
{
	glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}


unsigned int Shader::compileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, 0);
	glCompileShader(id);

	GLint result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		printf("-->> COMPILATION UNSUCCESSFULL \n");
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));

		glGetShaderInfoLog(id, length, &length, message);

		std::cout << "COMPILATION FAILED \n" << std::endl;
		//if (type == GL_VERTEX_SHADER && "vertex" : "fragment") << "shader" << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}
	return id;
}

 int Shader::GetUniformLocation(const std::string & name)
 {
	 if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
		 m_UniformLocationCache[name];


	 int location = glGetUniformLocation(m_RendererID, name.c_str());
	 if (location == -1)
	 {
		 std::cout << "Warning : Uniform " << name << " not found !!" << std::endl;
		 std::cout << "color value :  " << location << std::endl;
	 }
	  m_UniformLocationCache[name] = location;
	 return location;
 }

 GLuint Shader::createShader(const std::string& vertexShadersrc_n, const std::string& fragmentShadersrc)
{
	//creating vertex and fragment shaders
	vs = compileShader(GL_VERTEX_SHADER, vertexShadersrc_n);
	fs = compileShader(GL_FRAGMENT_SHADER, fragmentShadersrc);

	//Linking Shaders to the shader Program
	GLuint program = glCreateProgram();
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	//checking for Linking Errors
	int progLinkRes;													// to store the result of the program linking
	glGetProgramiv(program, GL_LINK_STATUS, &progLinkRes);
	if (!progLinkRes)
	{
		printf("-->> SHADER PROGRAM LINKING UNSUCCESSFULL \n");
		int length;
		glGetShaderiv(program, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(program, length, &length, message);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << std::endl;
		std::cout << message << std::endl;
		return 0;
	}
	return program;
}


 shaderProgramSource Shader::ParseShader(const std::string& filepath)
{
	std::ifstream stream(filepath);

	enum class ShaderType
	{
		NONE = -1, VERTEX1 = 0, VERTEX2 = 1, FRAGMENT = 2,
	};

	ShaderType  type = ShaderType::NONE;

	std::stringstream ss[3];

	std::string line;
	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				if (line.find("1") != std::string::npos)
				{
					type = ShaderType::VERTEX1;
				}
				else if (line.find("2") != std::string::npos)
				{
					type = ShaderType::VERTEX2;
				}
			}

			else if (line.find("fragment") != std::string::npos)
			{
				type = ShaderType::FRAGMENT;
			}
		}
		else
		{
			ss[(int)type] << line << '\n';
		}
	}
	return { ss[0].str(), ss[1].str(), ss[2].str() };
}