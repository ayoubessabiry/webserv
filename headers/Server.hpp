#ifndef SERVER_HPP_GARD
# define SERVER_HPP_GARD
# include "webserv.hpp"
# include "parse_config.hpp"

class Server  
{
public :
	std::string			_server_name;
	server_block		_server;
	int					_socket;
	const char*			_port;
	const char*			_host;

	Server(server_block &server) : _server_name(server.server_name.c_str()),
								 _server(server),
								 _port(server.port.c_str()),
								 _host(server.host.c_str())
								{
	}
	void	create_socket();
};



#endif