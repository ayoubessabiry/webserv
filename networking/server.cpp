# include "../headers/Server.hpp"


void	Server::create_socket(){
	addrinfo	hints, *bind_addr;
	int			s;

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
	if (bind(_socket, bind_addr->ai_addr, bind_addr->ai_addrlen) == -1){
		std::cerr << "bind() error: " << std::strerror(errno);
		exit(1);
	}
	freeaddrinfo(bind_addr);
	if (listen(_socket, BACKLOG) == -1) {
		std::cerr << "listen() error: " << std::strerror(errno);
		exit(1);
	}
	FD_ZERO(&main);
	FD_SET(_socket, &main);
	max_socket = _socket;
}

void	Server::add_client(int new_client){
	int		new_socket;
	Client	client;

	new_socket = accept(new_client, (sockaddr *)&client.client_add, &client.addr_size);
	client.socket = new_socket;
	client.recv_byte = 0;
	clients.push_back(client);
	FD_SET(new_socket, &main);
	(new_socket > max_socket) ? max_socket = new_socket : max_socket; 
}

int		Server::wait_clients(fd_set	ready){
	int i = 1;
	while (i <= max_socket && !FD_ISSET(i, &ready))
		++i;
	return ((i > max_socket) ? -1 : i);
}

void	Server::get_rqst(int ready_client){
	int				i = 0;
	char			buff[MAX_REQUEST_SIZE + 1];

	memset(&buff, 0, MAX_REQUEST_SIZE + 1);
	while (ready_client != clients[i].socket)
		++i;
	clients[i].recv_byte += recv(ready_client, buff, MAX_REQUEST_SIZE, 0);
	if(send_request(buff)){
		std::cout << "req done!" << std::endl;
	}
}
