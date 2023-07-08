#ifndef SERVER_HPP_GARD
# define SERVER_HPP_GARD
# include "Client.hpp"

class Server  
{
public :
	std::vector<Client>	clients;
	std::string			_server_name;
	fd_set				masterRead;
	fd_set				masterWrite;
	int					max_Rsocket;
	int					max_Wsocket;
	int					_socket;
	const char*			_port;
	const char*			_host;

	Server(const char* port, const char* host, std::string server_name) : _port(port), _host(host), _server_name(server_name){
		create_socket();
	}
	void	create_socket();
	void	add_client(int new_client);
	void	get_rqst(int ready_clien);
	void	send_rqst(int ready_client);
	int		wait_clients(fd_set ready, int max_sock);
};



#endif