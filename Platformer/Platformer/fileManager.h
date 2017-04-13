#pragma once

#include <filesystem>
#include <fstream>
#include <string>
#include <iostream>
#include <vector>

struct DirEntry 
{
	std::string path;
	bool isDirectory;
};

class FileManager
{
public:
	static bool readFile(std::string filePath, std::vector<unsigned char>& buffer);
	static bool readFile(std::string filePath, std::string& buffer);

	static bool getDirectoryEntries(const char* path, std::vector<DirEntry>& rvEntries);
	static bool makeDirectory(const char* path);

private:
	static void log(const std::string text);
};