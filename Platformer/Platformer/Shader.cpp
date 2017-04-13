#include "Shader.h"

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include "fileManager.h"

//The : mNumAttributes(0) ect. is an initialization list. It is a better way to initialize variables, since it avoids an extra copy. 
Shader::Shader() : mNumAttributes(0), mProgramID(0), mVertShaderID(0), mFragShaderID(0)
{
}

Shader::~Shader()
{
}

//Compiles the shaders into a form that your GPU can understand
void Shader::compileShaders(const std::string& vertShaderPath, const std::string& fragShaderPath) 
{	
	std::string vertSource;
	std::string fragSource;

	fileManager::readFileToBuffer(vertShaderPath, vertSource);
	fileManager::readFileToBuffer(fragShaderPath, fragSource);

	compileShadersFromSource(vertSource.c_str(), fragSource.c_str());
}

void Shader::compileShadersFromSource(const char* vertexSource, const char* fragmentSource) 
{
	//Vertex and fragment shaders are successfully compiled.
	//Now time to link them together into a program.
	//Get a program object.
	mProgramID = glCreateProgram();

	//Create the vertex shader object, and store its ID
	mVertShaderID = glCreateShader(GL_VERTEX_SHADER);
	if (mVertShaderID == 0) {
		log("Vertex shader failed to be created!");
	}

	//Create the fragment shader object, and store its ID
	mFragShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	if (mFragShaderID == 0) {
		log("Fragment shader failed to be created!");
	}

	//Compile each shader
	compileShader(vertexSource, "Vertex Shader", mVertShaderID);
	compileShader(fragmentSource, "Fragment Shader", mFragShaderID);
}

void Shader::linkShaders()
{

	//Attach our shaders to our program
	glAttachShader(mProgramID, mVertShaderID);
	glAttachShader(mProgramID, mFragShaderID);

	//Link our program
	glLinkProgram(mProgramID);

	//Note the different functions here: glGetProgram* instead of glGetShader*.
	GLint isLinked = 0;
	glGetProgramiv(mProgramID, GL_LINK_STATUS, (int *)&isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(mProgramID, GL_INFO_LOG_LENGTH, &maxLength);

		//The maxLength includes the NULL character
		std::vector<char> errorLog(maxLength);
		glGetProgramInfoLog(mProgramID, maxLength, &maxLength, &errorLog[0]);

		//We don't need the program anymore.
		glDeleteProgram(mProgramID);
		//Don't leak shaders either.
		glDeleteShader(mVertShaderID);
		glDeleteShader(mFragShaderID);

		//print the error log and quit
		const std::string error = &(errorLog[0]);
		const std::string text = "Shaders failed to link: " + error;
		log(text);
	}

	//detach shaders after a successful link
	glDetachShader(mProgramID, mVertShaderID);
	glDetachShader(mProgramID, mFragShaderID);
	glDeleteShader(mVertShaderID);
	glDeleteShader(mFragShaderID);
}

//Adds an attribute to our shader
void Shader::addAttribute(const std::string& attributeName)
{
	glBindAttribLocation(mProgramID, mNumAttributes++, attributeName.c_str());
}

GLint Shader::getUniformLocation(const std::string& uniformName) 
{
	GLint location = glGetUniformLocation(mProgramID, uniformName.c_str());
	if (location == GL_INVALID_INDEX) {
		log("Uniform " + uniformName + " not found in shader!");
	}
	return location;
}

//enable the shader, and all its attributes
void Shader::use() 
{
	glUseProgram(mProgramID);
	//enable all the attributes we added with addAttribute
	for (int i = 0; i < mNumAttributes; i++) {
		glEnableVertexAttribArray(i);
	}
}

//disable the shader
void Shader::unuse() 
{
	glUseProgram(0);
	for (int i = 0; i < mNumAttributes; i++) {
		glDisableVertexAttribArray(i);
	}
}

void Shader::dispose() {
	if (mProgramID) glDeleteProgram(mProgramID);
}

//Compiles a single shader file
void Shader::compileShader(const char* source, const std::string& name, GLuint id) {

	//tell opengl that we want to use fileContents as the contents of the shader file
	glShaderSource(id, 1, &source, nullptr);

	//compile the shader
	glCompileShader(id);

	//check for errors
	GLint success = 0;
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);

	if (success == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<char> errorLog(maxLength);
		glGetShaderInfoLog(id, maxLength, &maxLength, &errorLog[0]);

		glDeleteShader(id); //Don't leak the shader.

		//Print error log and quit
		const std::string error = &(errorLog[0]);
		log("Shader " + name + " failed to compile");
		log(error);
	}
}

void Shader::log(const std::string text)
{
	std::cout << "[Shader] " << text << std::endl;
}