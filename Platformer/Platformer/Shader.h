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

	void compileShaders(const std::string& vertPath, const std::string& fragPath,
		const std::string& geomPath = "", const std::string& tcsPath = "", const std::string& tesPath = "");

	void compileShadersFromSource(const char* vertexSource, const char* fragmentSource);

	void linkShaders();

	void addAttribute(const std::string& attributeName);

	GLint getUniformLocation(const std::string& uniformName);

	void use();
	void unuse();

	void dispose();
private:
	int mNumAttributes = 0;

	GLuint mProgramID = 0;

	GLuint mVertShaderID = 0;
	GLuint mFragShaderID = 0;

	void compileShader(const char* source, const std::string& name, GLuint id);

	void log(const std::string text);
};
