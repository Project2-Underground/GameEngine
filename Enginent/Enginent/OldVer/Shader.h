#pragma once

#include <string>
#include <GL\glew.h>
using namespace std;

class Shader
{
	GLuint shaderId;
	string fileName;
	GLenum shaderType;

public:
	GLuint getShaderId();
	bool LoadSource();
	void PrintShaderLog();
	Shader(string fileName, GLenum shaderType);
	~Shader();
};