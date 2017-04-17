#include "Shader.h"

#include "fileManager.h"

Shader::Shader()
{
}

Shader::~Shader()
{
}

//Compile the shaders
void Shader::compileShaders(const std::string& vertPath, const std::string& fragPath, 
	const std::string& geomPath, const std::string& tcsPath, const std::string& tesPath)
{	
	std::string vertSource;
	std::string fragSource;

	FileManager::readFile(vertPath, vertSource);
	FileManager::readFile(fragPath, fragSource);

	compileShadersFromSource(vertSource.c_str(), fragSource.c_str());
}

void Shader::compileShadersFromSource(const char* vertPath, const char* fragPath) 
{
	//Create the program
	mProgramID = glCreateProgram();

	//Create the vertex shader object and store its ID
	mVertShaderID = glCreateShader(GL_VERTEX_SHADER);
	if (mVertShaderID == 0) 
	{
		log("Vertex shader failed to be created!");
	}

	//Create the fragment shader object and store its ID
	mFragShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	if (mFragShaderID == 0) 
	{
		log("Fragment shader failed to be created!");
	}

	//Compile each shader
	compileShader(vertPath, "Vertex Shader", mVertShaderID);
	compileShader(fragPath, "Fragment Shader", mFragShaderID);
}

void Shader::linkShaders()
{
	//Attach the shaders to the program
	glAttachShader(mProgramID, mVertShaderID);
	glAttachShader(mProgramID, mFragShaderID);

	//Link the program
	glLinkProgram(mProgramID);

	GLint isLinked = 0;
	glGetProgramiv(mProgramID, GL_LINK_STATUS, (int*)&isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(mProgramID, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<char> errorLog(maxLength);
		glGetProgramInfoLog(mProgramID, maxLength, &maxLength, &errorLog[0]);

		//Delete the program
		glDeleteProgram(mProgramID);

		//Delete the shaders
		glDeleteShader(mVertShaderID);
		glDeleteShader(mFragShaderID);

		//print the error log and quit
		const std::string error = &(errorLog[0]);
		const std::string text = "Shaders failed to link: " + error;
		log(text);
	}

	//Detach shaders after a successful link
	glDetachShader(mProgramID, mVertShaderID);
	glDetachShader(mProgramID, mFragShaderID);
	glDeleteShader(mVertShaderID);
	glDeleteShader(mFragShaderID);
}

//Adds an attribute to the shader
void Shader::addAttribute(const std::string& attributeName)
{
	glBindAttribLocation(mProgramID, mNumAttributes++, attributeName.c_str());
}

GLint Shader::getUniformLocation(const std::string& uniformName) 
{
	GLint location = glGetUniformLocation(mProgramID, uniformName.c_str());
	if (location == GL_INVALID_INDEX) 
	{
		log("Uniform " + uniformName + " not found in shader!");
	}
	return location;
}

//Enable the shader its attributes
void Shader::use() 
{
	glUseProgram(mProgramID);
	//Enable all the attributes we added with addAttribute
	for (int i = 0; i < mNumAttributes; i++) 
	{
		glEnableVertexAttribArray(i);
	}
}

//disable the shader
void Shader::unuse() 
{
	glUseProgram(0);
	for (int i = 0; i < mNumAttributes; i++) 
	{
		glDisableVertexAttribArray(i);
	}
}

void Shader::dispose() 
{
	if (mProgramID) glDeleteProgram(mProgramID);
}

//Compiles a single shader file
void Shader::compileShader(const char* source, const std::string& name, GLuint id) 
{
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