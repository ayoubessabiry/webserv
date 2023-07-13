
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <iostream>
#include <stack>
#include <fstream>
#include <string>
#include <vector>
#include <fcntl.h>
#include <map>
#include <sstream>
#include <netdb.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#define RED_COLOR "\033[1;31m"
#define GREEN_COLOR "\033[1;32m"
#define BLUE_COLOR "\033[1;34m"
#define END_COLOR "\033[0m"
#define MY_SOCK_PATH "/Users/adbaich/Desktop/test"
#define ISVALIDSOCKET(s) ((s) >= 0)
#define CLOSESOCKET(s) close(s)
#define SOCKET int
#define GETSOCKETERRNO() (errno)
typedef std::map<std::string, std::string> _MAP;
void ft_get(const char *file_name, SOCKET &socket_client, _MAP types_map, std::string &ext);
bool ft_check_file(std::string file_name);
std::string get_ext(std::string &file_path);
std::string ft_get_path(std::string request, std::string &method);
void string_toupper(std::string &x);
char *get_time();
void ft_upload(const char *file_name, const char *body, int i, _MAP types_map);
void ft_error(const char *file_err, int i, _MAP types_map);
std::string getContentTypeOfExt(_MAP &types_map, std::string &ext);
int isDirectory(const char *path);
bool isIndexWithinDirectory(std::string &fileName, std::vector<std::string> indexes);
std::string getAutoIndex(const char *dir);
std::vector<std::string> getFileWithinDirectory(const char *dir);
bool methodIsAllowed(std::string method, std::vector<std::string> &allowedMethods);
void uploadFile(std::string &filePath, std::string &contentFile);
bool pathDoesNotExist(const char *path);
bool canNotOpenDirectoryOrSubDirectories(const char *dir);
void deleteDirectoryContent(std::string& dir);
std::string getRangeFromFile(const char* path, int start, size_t buffer_size);
std::string	intToString(int	num);

_MAP get_map();
