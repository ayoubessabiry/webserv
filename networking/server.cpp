# include "../headers/Server.hpp"


void	Server::create_socket(){
	addrinfo	hints, *bind_addr;
	int			s;
	int			a;

	a = 1;
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	if ((s = getaddrinfo(_host, _port, &hints, &bind_addr)) != 0){
		std::cerr << "getaddrinfo() error: " << gai_strerror(s) << std::endl;
		exit(1);
	}
	_socket = socket(bind_addr->ai_family, bind_addr->ai_socktype, bind_addr->ai_protocol);
	if (_socket == -1){
		std::cerr << "socket() error: " << std::strerror(errno);
		exit(1);
	}
	if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &a, sizeof(int)) < 0){
		std::cerr << "setsockopt(SO_REUSEADDR) failed" << std::endl;
		exit(1);
	}
	if (setsockopt(_socket, SOL_SOCKET, SO_NOSIGPIPE, &a, sizeof(int)) < 0){
		std::cerr << "setsockopt(SO_REUSEADDR) failed" << std::endl;
		exit(1);
	}
	if (bind(_socket, bind_addr->ai_addr, bind_addr->ai_addrlen) == -1){
		std::cerr << "bind() error: " << std::strerror(errno);
		exit(1);
	}
	freeaddrinfo(bind_addr);
	if (listen(_socket, BACKLOG) == -1) {
		std::cerr << "listen() error: " << std::strerror(errno);
		exit(1);
	}
}
