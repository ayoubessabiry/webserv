#ifndef SERVER_HPP_GARD
# define SERVER_HPP_GARD
# include "Client.hpp"

class Server  
{
public :
	std::vector<Client>	clients;
	int					max_socket;
	int					_socket;
	const char*			_port;
	const char*			_host;
	fd_set				main;

	Server(const char* port, const char* host) : _port(port), _host(host){
		create_socket();
	}
	void	create_socket();
	void	add_client(int new_client);
	void	get_rqst(int ready_client);
	int		wait_clients(fd_set ready);
};



#endif