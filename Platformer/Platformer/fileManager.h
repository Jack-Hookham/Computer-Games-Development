#pragma once

#include <vector>

struct DirEntry 
{
	std::string path;
	bool isDirectory;
};

class fileManager
{
public:
	static bool readFile(std::string filePath, std::vector<unsigned char>& buffer);
	static bool readFile(std::string filePath, std::string& buffer);

	static bool getDirectoryEntries(const char* path, std::vector<DirEntry>& rvEntries);
	static bool makeDirectory(const char* path);
};