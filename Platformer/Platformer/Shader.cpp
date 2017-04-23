#include "Shader.h"

#include "fileManager.h"

Shader::Shader()
{
}

Shader::~Shader()
{
}

//Compile the shaders
void Shader::initShaders(const std::string& vertPath, const std::string& fragPath,
	const std::string& geomPath, const std::string& tcsPath, const std::string& tesPath)
{

	//Create the shader program
	mProgram = glCreateProgram();

	//Create the vertex shader object
	mVertShader = glCreateShader(GL_VERTEX_SHADER);
	if (mVertShader == 0)
	{
		log("Vertex shader failed to be created!");
	}

	//Create the fragment shader object
	mFragShader = glCreateShader(GL_FRAGMENT_SHADER);
	if (mFragShader == 0)
	{
		log("Fragment shader failed to be created!");
	}

	//Shader source code buffer strings
	std::string vertSource;
	std::string fragSource;

	//Read the shader files into strings
	FileManager::readFile(vertPath, vertSource);
	FileManager::readFile(fragPath, fragSource);

	//Convert source to char* and compile each shader
	compileShader(vertSource, "Vertex Shader", mVertShader);
	compileShader(fragSource, "Fragment Shader", mFragShader);

	//Attach the shaders to the program
	glAttachShader(mProgram, mVertShader);
	glAttachShader(mProgram, mFragShader);
}

//Link the shaders together into a program
void Shader::linkShaders()
{
	//Link the program
	glLinkProgram(mProgram);

	GLint isLinked = 0;
	glGetProgramiv(mProgram, GL_LINK_STATUS, (int*)&isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(mProgram, GL_INFO_LOG_LENGTH, &maxLength);

		//Max length includes the NULL character
		std::vector<char> errorLog(maxLength);
		glGetProgramInfoLog(mProgram, maxLength, &maxLength, &errorLog[0]);

		//Delete the program as it's no longer needed
		glDeleteProgram(mProgram);

		//Don't leak the shaders
		glDeleteShader(mVertShader);
		glDeleteShader(mFragShader);

		//Print the error log
		const std::string text = "Shaders failed to link: " + std::string(&(errorLog[0]));
		log(text);
	}

	//Detach and delete shaders after linking, whether it fails or not
	glDetachShader(mProgram, mVertShader);
	glDetachShader(mProgram, mFragShader);
	glDeleteShader(mVertShader);
	glDeleteShader(mFragShader);
}

//Adds an attribute to the shader
void Shader::addAttribute(const std::string& attributeName)
{
	glBindAttribLocation(mProgram, mNumAttributes++, attributeName.c_str());
}

GLint Shader::getUniformLocation(const std::string& uniformName) 
{
	GLint location = glGetUniformLocation(mProgram, uniformName.c_str());
	if (location == GL_INVALID_INDEX) 
	{
		log("Uniform " + uniformName + " not found in shader!");
	}
	return location;
}

//Enable the shader attributes
void Shader::enableAttributes() 
{
	glUseProgram(mProgram);
	//Enable all of the attributes that were added with addAttribute()
	for (int i = 0; i < mNumAttributes; i++) 
	{
		glEnableVertexAttribArray(i);
	}
}

//Disable the shader attributes
void Shader::disableAttributes()
{
	glUseProgram(0);
	//Disable all the attributes that were added with addAttribute()
	for (int i = 0; i < mNumAttributes; i++) 
	{
		glDisableVertexAttribArray(i);
	}
}

void Shader::dispose() 
{
	if (mProgram)
	{
		//Delete the shader program if it exists
		glDeleteProgram(mProgram);
	}
}

//Compiles a single shader file
void Shader::compileShader(const std::string sourceString, const std::string& name, GLuint& shader)
{
	//Pass the shader source code to GL
	//1 = the number of individual strings being used
	const GLchar* source = (const GLchar*)sourceString.c_str();
	glShaderSource(shader, 1, &source, 0);

	//Compile the shader
	glCompileShader(shader);

	//Error check
	GLint isCompiled = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);

	//if success is GL_FALSE then the most recent compilation failed, otherwise it succeeded
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

		//Max length includes the NULL character
		std::vector<char> errorLog(maxLength);
		glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);

		//Print the error
		log("Shader " + name + " failed to compile with error: " + &(errorLog[0]));

		//Don't leak the shader
		glDeleteShader(shader);
	}
}

void Shader::log(const std::string text)
{
	std::cout << "[Shader] " << text << std::endl;
}