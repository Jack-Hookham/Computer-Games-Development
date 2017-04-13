#include "fileManager.h"

//Namespace alias
namespace fs = std::tr2::sys;

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

bool FileManager::getDirectoryEntries(const char* path, std::vector<DirEntry>& rvEntries)
{
	auto dpath = fs::path(path);
	// Must be directory
	if (!fs::is_directory(dpath)) return false;

	for (auto it = fs::directory_iterator(dpath); it != fs::directory_iterator(); ++it) 
	{
		rvEntries.emplace_back();
		rvEntries.back().path = it->path().string();
		if (is_directory(it->path())) {
			rvEntries.back().isDirectory = true;
		}
		else {
			rvEntries.back().isDirectory = false;
		}
	}
	return true;
}

bool FileManager::makeDirectory(const char* path)
{
	return fs::create_directory(fs::path(path));
}

void FileManager::log(const std::string text)
{
	std::cout << "[FileManager] " << text << std::endl;
}
