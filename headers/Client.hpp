#ifndef CLIENT_HPP_GARD
# define CLIENT_HPP_GARD
# include "webserv.hpp"
# define MAX_REQUEST_SIZE 2047


class Client  
{
public : 	
	socklen_t			addr_size;
	sockaddr_storage	client_add;
	std::string			request;
	ssize_t				recv_byte;
	ssize_t				header_byte;
	ssize_t				body_byte;
	int					socket;
};

#endif