#pragma once
#include "header.hpp"

class server
{
	struct addrinfo *bind_address;
	fd_set current_sockets, ready_sockets;
	SOCKET socket_listen;
	std::string		method;
	int i;
	bool	forbidden;
    _MAP types_map;

	void local_add_conf(char **av);
	void socket_cr();
	void sock_add_bind();
	void sock_listen();
	void sock_set();
	void sock_ser_up();
	void sock_cl_up();

public:

	std::string data;
	server();
	~server();
	void start(char **av);
};