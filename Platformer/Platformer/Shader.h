#pragma once

#include <GL/glew.h>
#include <string>
#include <iostream>
#include <vector>
#include <fstream>

//http://www.opengl.org/wiki/Shader_Compilation

class Shader
{
public:
	Shader();
	~Shader();

	void compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);

	void compileShadersFromSource(const char* vertexSource, const char* fragmentSource);

	void linkShaders();

	void addAttribute(const std::string& attributeName);

	GLint getUniformLocation(const std::string& uniformName);

	void use();
	void unuse();

	void dispose();
private:

	int mNumAttributes;

	void compileShader(const char* source, const std::string& name, GLuint id);

	GLuint mProgramID;

	GLuint mVertShaderID;
	GLuint mFragShaderID;

	void log(const std::string text);
};
