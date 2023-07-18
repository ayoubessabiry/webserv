/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbaich <adbaich@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/01 13:46:24 by adbaich           #+#    #+#             */
/*   Updated: 2023/07/15 21:55:03 by adbaich          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/header.hpp"

std::string ft_get_path(std::string request, std::string &method)
{
	char *token;
	char *line_tmp;

	line_tmp = strdup(request.c_str());
	token = strtok(line_tmp, " ");
	if (!token)
	{
		return std::string();
	}
	method = token;
	token = strtok(NULL, " ");
	if (!token)
	{
		return std::string();
	}
	return (token);
}

bool ft_check_file(std::string file_name)
{

	std::ifstream file(file_name);

	if (!file.is_open())
	{
		return true;
	}
	file.close();
	return false;
}

std::string get_ext(std::string &file_path)
{

	std::string::size_type pos;

	pos = file_path.rfind('.');

	if (pos != std::string::npos)
	{
		std::string extension = file_path.substr(pos + 1);
		return extension;
	}
	return std::string();
}

char *get_time()
{

	time_t timer;
	char *time_msg;

	time(&timer);
	time_msg = ctime(&timer);
	time_msg[strlen(time_msg) - 1] = '\0';
	return time_msg;
}

void string_toupper(std::string &x)
{
	for (size_t i = 0; i < x.size(); i++)
		x[i] = toupper(x[i]);
}

int isDirectory(const char *path)
{
	struct stat statbuf;
	if (stat(path, &statbuf) != 0)
		return 0;
	std::cout << path << std::endl;
	return S_ISDIR(statbuf.st_mode);
}

std::vector<std::string> getFileWithinDirectory(const char *dir)
{
	std::vector<std::string> filesWithinDirectory;
	DIR *dirp = opendir(dir);
	if (!dirp)
	{
		perror("opendir");
		exit(1);
	}
	for (struct dirent *dent; (dent = readdir(dirp)) != NULL;)
	{
		std::string fileWithinDirectory(dent->d_name);
		if (strcmp(fileWithinDirectory.c_str(), ".") == 0 || strcmp(fileWithinDirectory.c_str(), "..") == 0)
			continue;
		if (isDirectory(std::string(dir).append(fileWithinDirectory).c_str()))
		{
			fileWithinDirectory.append("/");
		}
		filesWithinDirectory.push_back(fileWithinDirectory);
	}
	return filesWithinDirectory;
}

std::string getAutoIndex(const char *dir)
{
	std::vector<std::string> filesWithinDirectory = getFileWithinDirectory(dir);

	std::string autoIndexHtml;
	autoIndexHtml.append("<!DOCTYPE html> <html> <head><h1>Auto Index</h1>"
	"<title>Webserv</title>"
	"<link href='https://unpkg.com/boxicons@2.1.4/css/boxicons.min.css' rel='stylesheet'></head><body>\n");
	for (size_t i = 0; i < filesWithinDirectory.size(); i++)
	{
		const char *fileWithinDirectory = filesWithinDirectory[i].c_str();

		if (isDirectory(fileWithinDirectory))
		{
			autoIndexHtml.append("<i class=\'bx bxs-folder\'></i>");
		}
		else
		{
			autoIndexHtml.append("<i class=\'bx bxs-file\'></i>");
		}
		autoIndexHtml.append("<a href=\"").append(fileWithinDirectory);
		autoIndexHtml.append("\" >");
		autoIndexHtml.append(fileWithinDirectory).append("</a><br>\n");
	}
	autoIndexHtml.append("</body></html>\n");
	return autoIndexHtml;
}

bool isIndexWithinDirectory(std::string &fileName, std::vector<std::string> indexes)
{
	std::vector<std::string> filesWithinDirectory = getFileWithinDirectory(fileName.c_str());
	for (size_t i = 0; i < indexes.size(); i++)
	{
		for (size_t j = 0; j < filesWithinDirectory.size(); j++)
		{
			if (!filesWithinDirectory[j].compare(indexes[i]))
			{
				fileName.append(indexes[i]);
				return true;
			}
		}
	}
	return false;
}

bool methodIsAllowed(std::string method, std::vector<std::string> &allowedMethods)
{
	for (size_t i = 0; i < allowedMethods.size(); i++)
	{
		if (!method.compare(allowedMethods[i]))
		{
			return true;
		}
	}
	return false;
}

void uploadFile(std::string &fileToWrite, std::string &fileToRead)
{
	size_t sizeOfFileToRead = calculateSizeOfFile(fileToRead);
	size_t BytesRead = 0;
	std::ofstream file;
	std::string rangeFromFile;

	file.open(fileToWrite.c_str(), std::ios_base::binary);
	while (true)
	{
		rangeFromFile = getRangeFromFile(fileToRead.c_str(), BytesRead, 1000);
		if (BytesRead >= sizeOfFileToRead)
		{
			break;
		}
		file << rangeFromFile;
		BytesRead += rangeFromFile.size();
	}
	file.close();
}

bool pathDoesNotExist(const char *path)
{
	if (access(path, F_OK) < 0)
	{
		return true;
	}
	return false;
}

bool canNotOpenDirectoryOrSubDirectories(const char *dir)
{
	DIR *dirp = opendir(dir);
	if (!dirp)
	{
		return true;
	}
	std::vector<std::string> filesWithinDirectory = getFileWithinDirectory(dir);
	for (size_t i = 0; i < filesWithinDirectory.size(); i++)
	{
		std::string dirAsString(dir);
		std::string subDir((dirAsString + "/" + filesWithinDirectory[i]));
		if (isDirectory(subDir.c_str()))
		{
			if (canNotOpenDirectoryOrSubDirectories(subDir.c_str()))
				return true;
		}
	}
	return false;
}

void deleteDirectoryContent(std::string &dir)
{
	std::vector<std::string> filesWithinDirectory = getFileWithinDirectory(dir.c_str());

	for (size_t i = 0; i < filesWithinDirectory.size(); i++)
	{

		std::string subDir((dir + "/" + filesWithinDirectory[i]));
		if (isDirectory(subDir.c_str()))
		{
			deleteDirectoryContent(subDir);
		}
		remove(subDir.c_str());
	}
	remove(dir.c_str());
}

std::string getRangeFromFile(const char *path, int start, size_t buffer_size)
{
	std::ifstream file;

	file.open(path, std::ifstream::binary);
	char *buffer = new char[buffer_size + 1];
	file.seekg(start);
	file.read(buffer, buffer_size);
	buffer[file.gcount()] = '\0';
	std::string body(buffer, file.gcount());
	delete[] buffer;
	file.close();
	return body;
}

std::string intToString(int num)
{

	std::string string;

	if (!num)
	{
		string.append("0");
	}
	while (num)
	{
		char c;
		c = (num % 10) + '0';
		string += c;
		num = num / 10;
	}
	string = std::string(string.rbegin(), string.rend());
	return string;
}

size_t calculateSizeOfFile(const std::string &fileName)
{
	std::ifstream file;

	file.open(fileName, std::ios::binary);
	file.seekg(0, file.end);
	size_t fileLength = file.tellg();
	file.seekg(0, file.beg);
	file.close();
	return fileLength;
}
