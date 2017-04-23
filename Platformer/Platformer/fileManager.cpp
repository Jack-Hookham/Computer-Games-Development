#include "fileManager.h"

//Used by ImageLoader to read image files
bool FileManager::readFile(std::string filePath, std::vector<unsigned char>& buffer) 
{
	bool success = true;

	std::ifstream file(filePath, std::ios::binary);
	if (file.fail()) 
	{
		perror(filePath.c_str());
		success = false;
	}
	else
	{
		//Seek to the end
		file.seekg(0, std::ios::end);

		//Get the file size
		unsigned int fileSize = (unsigned int)file.tellg();
		file.seekg(0, std::ios::beg);

		//Reduce the file size by any header bytes that might be present
		fileSize -= (unsigned int)file.tellg();

		buffer.resize(fileSize);
		file.read((char *)&(buffer[0]), fileSize);
		file.close();
	}

	return success;
}

//Used by Shader to read shader files
bool FileManager::readFile(std::string filePath, std::string& buffer)
{
	bool success = true;

	std::ifstream file(filePath, std::ios::binary);
	if (file.fail())
	{
		perror(filePath.c_str());
		return false;
	}
	else
	{
		//seek to the end
		file.seekg(0, std::ios::end);

		//Get the file size
		unsigned int fileSize = (unsigned int)file.tellg();
		file.seekg(0, std::ios::beg);

		//Reduce the file size by any header bytes that might be present
		fileSize -= (unsigned int)file.tellg();

		buffer.resize(fileSize);
		file.read((char *)&(buffer[0]), fileSize);
		file.close();
	}

	return success;
}

void FileManager::log(const std::string text)
{
	std::cout << "[FileManager] " << text << std::endl;
}
