#ifndef SERVER_HPP_GARD
# define SERVER_HPP_GARD
# include "Client.hpp"

class Server  
{
public :
	std::vector<Client>	clients;
	int					max_Rsocket;
	int					max_Wsocket;
	int					_socket;
	const char*			_port;
	const char*			_host;
	fd_set				masterRead;
	fd_set				masterWrite;


	Server(const char* port, const char* host) : _port(port), _host(host){
		create_socket();
		start_listening();
	}
	void	create_socket();
	void	start_listening();
	void	add_client(int new_client);
	void	get_rqst(int ready_client);
	void	send_rqst(int ready_client);
	int		wait_clients(fd_set ready, int max_sock);
	void	read_socket();
	void	write_socket();
};



#endif