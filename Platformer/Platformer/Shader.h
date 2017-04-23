#pragma once

#include <GL/glew.h>
#include <string>
#include <iostream>
#include <vector>
#include <fstream>

//Based on https://www.khronos.org/opengl/wiki/Shader_Compilation

class Shader
{
public:
	Shader();
	~Shader();

	void initShaders(const std::string& vertPath, const std::string& fragPath,
		const std::string& geomPath = "", const std::string& tcsPath = "", const std::string& tesPath = "");

	void linkShaders();

	void addAttribute(const std::string& attributeName);

	GLint getUniformLocation(const std::string& uniformName);

	void enableAttributes();
	void disableAttributes();

	void dispose();
private:
	int mNumAttributes = 0;

	GLuint mProgram = 0;

	GLuint mVertShader = 0;
	GLuint mFragShader = 0;

	void compileShader(const std::string sourceString, const std::string& name, GLuint& shader);

	void log(const std::string text);
};
