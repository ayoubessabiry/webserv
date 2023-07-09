#ifndef CLIENT_HPP_GARD
# define CLIENT_HPP_GARD
# include "webserv.hpp"
# define MAX_REQUEST_SIZE 65535


class Client  
{
public : 	
	socklen_t			addr_size;
	sockaddr_storage	client_add;
	// std::string			request;
	// request				_request;
	ssize_t				recv_byte;
	ssize_t				header_byte;
	ssize_t				body_byte;
	char				buff[MAX_REQUEST_SIZE];
	int					socket;
};

#endif