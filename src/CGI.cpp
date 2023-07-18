/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aessabir <aessabir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/16 11:37:15 by aessabir          #+#    #+#             */
/*   Updated: 2023/07/18 15:26:04 by aessabir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/CGI.hpp"
# include "../headers/header.hpp"


void	CGI::get_uri_info(std::string& uri){
	ENV["QUERY_STRING"] = uri.substr(uri.find("?") + 1,uri.size());
}

std::string	CGI::get_file_name(std::string &uri){
	ENV["SCRIPT_NAME"] = uri.substr(0,uri.find("?"));
	return uri.substr(0,uri.find("?"));
}

void	CGI::set_env(request& request){
	ENV["AUTH_TYPE"] = "";
	std::map<std::string, std::string>::iterator it = request.headers.find("Content-Length");
	ENV["CONTENT_LENGTH"] = (it != request.headers.end())? it->second : "";
	it = request.headers.find("Content-Type");
	ENV["CONTENT_TYPE"] = (it != request.headers.end())? it->second : "";
	ENV["SERVER_PROTOCOL"] = "HTTP/1.1";
	ENV["REQUEST_METHOD"] = request.method;
	ENV["REMOTE_HOST"] = "";
	get_uri_info(request.uri);
}

bool	CGI::check_cgi(Client& client){
	std::string filename = client.desired_location.root + get_file_name(client.rqst.uri);
	if (get_ext(filename) != "php")
		return false;
	ENV["SERVER_PORT"] = client.configuration.port;
	ENV["SERVER_NAME"] = client.configuration.host;
	ENV["REMOTE_ADDR"] = client.configuration.host;
	ENV["REDIRECT_STATUS"] = "true";
	set_env(client.rqst);
	exec_cgi(filename);
	return true;
}

void	CGI::convert_map_to_char(){
	env = new char*[ENV.size() + 1];

	std::map<std::string, std::string>::iterator it;
	int i = 0;
	for(it = ENV.begin(); it != ENV.end(); ++it){
		env[i] = new char[it->first.size() + it->second.size() + 2];
		env[i] = const_cast<char *>((it->first + "=" + it->second).c_str());
		env[i][it->first.size() + it->second.size()] = '\0';
		// std::cout << env[i] << std::endl;
		++i;
	}
	env[i] = 0;
}

std::string	random_name()
{
	std::string alphas = "abcdefghijklmnopqrstuvwxyz123456789";
	std::string	file_name = "/tmp/";

	srand(time(0));

	for	(int i = 0 ; i < 13; i++)
		file_name += alphas[rand() % alphas.size()];

	return file_name;
}

size_t calculateFile(const std::string &fileName)
{
	std::ifstream file;

	file.open(fileName, std::ios::binary);
	file.seekg(0, file.end);
	size_t fileLength = file.tellg();
	file.seekg(0, file.beg);
	file.close();
	return fileLength;
}

bool	CGI::send_cgi_response(Client& client){
	std::fstream file;
	std::string	 s;
	char		r[MAX_REQUEST_SIZE];

	file.open(cgi_file_name, std::ios_base::in | std::ios_base::out);
	file.seekg(client.bytes_sent);
	file.read(r, MAX_REQUEST_SIZE);
	std::string read(r, file.gcount());
	if (client.header){
		client.fileSize = calculateFile(cgi_file_name);
		s = read;
		s = s.substr(0, s.find("\r\n\r\n"));
		std::size_t find = s.find("Status: ");
		if(find == std::string::npos){
			status = 200;
			std::string response = "HTTP/1.1 200 OK\r\nContent-Length: " + std::to_string(client.fileSize - s.size()) + "\r\n";
			send(client.socket, response.c_str(), response.size(), 0);
		}
		else{
			status = stoi(s.substr(8, 3));
		}
		client.header = false;
	}
	if (client.bytes_sent >= client.fileSize){
		unlink(cgi_file_name.c_str());
		return false;
	}
	client.bytes_sent += send(client.socket, read.c_str(), read.size(), 0);
	unlink(cgi_file_name.c_str());
	return true;
}

void CGI::exec_cgi(std::string filename) {
	char **args = new char*[3];
	cgi_file_name = random_name();
	int	fd = open(cgi_file_name.c_str(), O_CREAT | O_RDWR, 0644);

	args[0] = strdup("/Users/aessabir/Desktop/webserv/cgi/php-cgi");
	args[1] = strdup(filename.c_str());
	args[2] = 0;
    int id = fork();
    if (id == 0) {
		convert_map_to_char();
		dup2(fd, 1);
        if (execve(args[0], args, env) == -1)
			std::cerr << "execve() error: " << std::strerror(errno);
    }
	wait(NULL);
	close(fd);
}
