#pragma once
#include<string>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "Renderer.h"
#include <unordered_map>

struct shaderProgramSource
{
	std::string vertexSource1;
	std::string vertexSource2;
	std::string fragSource;
};

class Shader
{
private:
	std::string m_FilePath;
	unsigned int m_RendererID;
	std::unordered_map<std::string, int> m_UniformLocationCache;

public:
	Shader(const std::string& filepath, int vertexshdr_n);
	~Shader();

	void bind() const;
	void unBind() const;

	// Set Uniforms
	void setUniform1i(const std::string& name, int value);
	void setUniform1f(const std::string& name, float value);
	void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3);

private:
	unsigned int compileShader(unsigned int type, const std::string& source);
	int GetUniformLocation(const std::string& name);
	shaderProgramSource ParseShader(const std::string& filepath);
	GLuint createShader(const std::string& vertexShadersrc_n, const std::string& fragmentShadersrc);

};