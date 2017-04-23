#pragma once

#include <filesystem>
#include <fstream>
#include <string>
#include <iostream>
#include <vector>

//Manages file IO

class FileManager
{
public:
	//Used by ImageLoader to read image files
	static bool readFile(std::string filePath, std::vector<unsigned char>& buffer);

	//Used by Shader to read shader files
	static bool readFile(std::string filePath, std::string& buffer);

private:
	static void log(const std::string text);
};