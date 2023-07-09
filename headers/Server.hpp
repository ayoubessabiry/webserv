#ifndef SERVER_HPP_GARD
# define SERVER_HPP_GARD
# include "webserv.hpp"

class Server  
{
public :
	std::string			_server_name;
	int					_socket;
	const char*			_port;
	const char*			_host;

	Server(const char* port, const char* host, std::string server_name) : _port(port), _host(host), _server_name(server_name){}
	void	create_socket();
};



#endif