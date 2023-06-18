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
	FD_ZERO(&masterRead);
	FD_ZERO(&masterWrite);
	FD_SET(_socket, &masterRead);
	max_Rsocket = _socket;
	max_Wsocket = 0;
}

void	Server::read_socket(){
	fd_set	read;
	timeval timeout = {10, 0}; // waiting for 10sec
	int		ready_client;

	read = masterRead;
	if (select(max_Rsocket + 1, &read, 0, 0, &timeout) < 0){
		// response with time out;
		std::cout << "timeout" << std::endl;
	}
	ready_client = wait_clients(read, max_Rsocket);
	if (ready_client == _socket)
		add_client(ready_client);
	else  
		get_rqst(ready_client);
}

void	Server::write_socket(){
	fd_set	write;
	timeval	timeout = {10, 0}; // waiting for 10sec
	int		ready_client;

	write = masterWrite;
	if (select(max_Rsocket + 1, 0, &write, 0, &timeout) < 0){
		// response with time out;
		std::cout << "timeout" << std::endl;
	}
	ready_client = wait_clients(write, max_Wsocket);
	send_rqst(ready_client);
}

void	Server::start_listening(){
	while (1){
		read_socket(); // reading request from ready clients
		if (max_Wsocket)
			write_socket(); // wirting responsr for ready clients
	}
}

void	Server::add_client(int new_client){
	int		new_socket;
	Client	client;

	new_socket = accept(new_client, (sockaddr *)&client.client_add, &client.addr_size);
	client.socket = new_socket;
	client.recv_byte = 0;
	clients.push_back(client);
	FD_SET(new_socket, &masterRead);
	(new_socket > max_Rsocket) ? max_Rsocket = new_socket : max_Rsocket; 
}

int		Server::wait_clients(fd_set	ready, int max_sock){
	int i = 1;
	while (i <= max_sock && !FD_ISSET(i, &ready))
		++i;
	return ((i > max_sock) ? -1 : i);
}

void	Server::get_rqst(int ready_client){
	int				i = 0;
	char			buff[MAX_REQUEST_SIZE + 1];

	memset(&buff, 0, MAX_REQUEST_SIZE + 1);
	while (ready_client != clients[i].socket)
		++i;
	clients[i].recv_byte += recv(ready_client, buff, MAX_REQUEST_SIZE, 0);

	if(send_request(buff)){
		// move ready_client to send();
		std::cout << "am here" << std::endl;
		FD_SET(ready_client, &masterWrite);
		(ready_client > max_Wsocket) ? max_Wsocket = ready_client : max_Wsocket;
	}
}

void	Server::send_rqst(int ready_client){

	char buff[] = "HTTP/1.1 HTTP/1.1 200 OK\nDate: Sun, 18 Oct 2012 10:36:20 GMT\nServer: Apache/2.2.14 (Win32)\nContent-Length: 88\nConnection: Closed\nContent-Type: text/html;\ncharset=iso-8859-1\n\r\n\r<html><body><h1>Hello, World!</h1></body></html>";

	send(ready_client, buff, sizeof buff, 0);
	exit(1);
}
