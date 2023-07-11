/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbaich <adbaich@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/01 13:46:24 by adbaich           #+#    #+#             */
/*   Updated: 2023/07/11 13:21:55 by adbaich          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/header.hpp"

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

// void ft_error(const char *file_err, int i, _MAP types_map)
// {

// 	std::string root("/Users/adbaich/Desktop/test/errors/");
// 	std::string tmp(root);

// 	tmp.append(file_err);
// 	std::string extension = get_ext(tmp);
// 	ft_get(tmp.c_str(), i, types_map, extension);
// }

int isDirectory(const char *path)
{

	struct stat statbuf;
	if (stat(path, &statbuf) != 0)
		return 0;
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
	autoIndexHtml.append("<!DOCTYPE html> <html> <head><h1>Aut oIndex</h1><title>Webserv</title></head><body>\n");
	for (size_t i = 0; i < filesWithinDirectory.size(); i++)
	{
		const char *fileWithinDirectory = filesWithinDirectory[i].c_str();

		autoIndexHtml.append("<a href=\"").append(fileWithinDirectory);
		autoIndexHtml.append("\" >").append(fileWithinDirectory).append("</a><br>\n");
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

void uploadFile(std::string &filePath, std::string &contentFile)
{
	std::ofstream file;

	file.open(filePath.c_str(), std::ios_base::binary);
	file << contentFile;
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

void deleteDirectoryContent(std::string& dir)
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

std::string	getRangeFromFile(const char* path, int start, size_t buffer_size)
{
	std::ifstream	file;
	std::string		body;

	file.open(path, std::ifstream::binary);
	if (file.is_open())
	{
		char* buffer = new char[buffer_size];
		if (start)
			file.seekg(start-1);
		file.read(buffer, buffer_size -1);
		buffer[file.gcount()] = '\0';
		body = buffer;
		delete [] buffer;
	}
	file.close();
	return body;
}

std::string	intToString(int	num) {

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
