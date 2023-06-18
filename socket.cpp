#include "headers/Client.hpp"


// Create out server socket 
int	create_socket(const char *host, const char *port){
	addrinfo hints, *bind_addr;
	int		s, socket_fd;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	if ((s = getaddrinfo(host, port, &hints, &bind_addr)) != 0){
		std::cerr << "getaddrinfo() error: " << gai_strerror(s) << std::endl;
		return (-1);
	}
	socket_fd = socket(bind_addr->ai_family, bind_addr->ai_socktype, bind_addr->ai_protocol);
	if (socket_fd == -1){
		std::cerr << "socket() error: " << std::strerror(errno);
		return (-1);
	}
	if (bind(socket_fd, bind_addr->ai_addr, bind_addr->ai_addrlen) == -1){
		std::cerr << "bind() error: " << std::strerror(errno);
		return (-1);
	}
	freeaddrinfo(bind_addr);
	if (listen(socket_fd, BACKLOG) == -1) {
		std::cerr << "listen() error: " << std::strerror(errno);
		return (-1);
	}
	return (socket_fd);
}

void timeout(){}

int	add_client(std::vector<Client> &clients, int socket, fd_set& main)
{
	int new_c;
	Client client;

	new_c = accept(socket, (sockaddr *)&client.client_add, &client.addr_size);
	client.socket = new_c;
	clients.push_back(client);
	FD_SET(new_c, &main);
	return (new_c);
}

int	wait_client(int max_socket, fd_set reads){
	for (int i = 1; i <= max_socket; ++i){
		if (FD_ISSET(i, &reads))
			return (i);
	}
	return (-1);
}

void	get_rqst(std::vector<Client> &clients, int socket){
	int i = 0;
	std::cout << "client " << socket << std::endl;
	char reqst[MAX_REQUEST_SIZE];
	while (socket != clients[i].socket)
		++i;
	clients[i].recv_byte = recv(socket, reqst, MAX_REQUEST_SIZE, 0);
	strcat(clients[i].request, reqst);
	if (!strstr(clients[i].request, "\n\r\n\r")){
		// sending the request to mounir
		std::cout << "new request \n" <<clients[i].request << "end of request" << std::endl;
		clients[i].request[0] = '\0';
	}
}

int main(){
	std::vector<Client> clients;
	int max_socket, socket, new_c;

	socket = create_socket(0, PORT);
	fd_set main;
	FD_ZERO(&main);
	FD_SET(socket, &main);
	max_socket = socket;
	while (1){
		fd_set reads;
		reads = main;
		std::cout << "here" << std::endl;
		if (select(max_socket + 1, &reads, 0, 0, 0) < 0){
			timeout();
		}
		int ready_client = wait_client(max_socket, reads);
		if (ready_client == socket){
			new_c = add_client(clients, ready_client, main);
			(max_socket < new_c) ? max_socket = new_c : max_socket;
		}
		else
			get_rqst(clients, ready_client);
	}
	
}