# include "../headers/Init.hpp"

void	Init::read_socket(int ready_client){
	int i = -1;
	while(++i < server.size()){
		if (ready_client == server[i]._socket){
			add_client(ready_client);
			break ;
		}
	}
	if (i == server.size())
		get_rqst(ready_client);
}

int		Init::wait_clients(fd_set	ready, int max_sock){
	int i = 1;
	while (i <= max_sock && !FD_ISSET(i, &ready))
		++i;
	return ((i > max_sock) ? -1 : i);
}

void	Init::add_client(int new_client){
	int		new_socket;
	Client	client;

	new_socket = accept(new_client, (sockaddr *)&client.client_add, &client.addr_size);

	// client.configuration = server_block[new_client];
	client.is_reading_body = false;
	client.rqst.body = "";
	client.request_collector = "";
	client.socket = new_socket;
	client.recv_byte = 0;

	clients.push_back(client);
	FD_SET(new_socket, &masterRead);
	(new_socket > max_Rsocket) ? max_Rsocket = new_socket : max_Rsocket;
}

void	Init::get_rqst(int ready_client){
	int				i = 0;

	while (i < clients.size() && ready_client != clients[i].socket)
		++i;
	memset(&clients[i].buff, 0, MAX_REQUEST_SIZE);
	clients[i].recv_byte += recv(ready_client, &clients[i].buff, MAX_REQUEST_SIZE, 0);
	if(send_request(clients[i])){
		// move ready_client to send();
		FD_CLR(ready_client, &masterRead);
		FD_SET(ready_client, &masterWrite);
		(ready_client > max_Wsocket) ? max_Wsocket = ready_client : max_Wsocket;
	}
}

void	Init::send_rqst(int ready_client){
	char buff[] = "HTTP/1.1 HTTP/1.1 200 OK\nDate: Sun, 18 Oct 2012 10:36:20 GMT\nServer: Apache/2.2.14 (Win32)\nContent-Length: 88\nConnection: Closed\nContent-Type: text/html;\ncharset=iso-8859-1\n\r\n\r<html><body><h1>Hello, World!</h1></body></html>";
	send(ready_client, buff, sizeof buff, 0);
	FD_CLR(ready_client, &masterWrite);
	close(ready_client);
	// exit(1);
}

void	Init::write_socket(int ready_client){
	// send_rqst(ready_client);
}

void	Init::start_listening(){
	while (1){
		timeval timeout = {10, 0}; // waiting for 10sec
		int		read_client;
		int		write_client;
		int		i;

		read = masterRead;
		write = masterWrite;
		if (select(max_Rsocket +  max_Wsocket + 1, &read, &write, 0, &timeout) < 0){
			// response with time out;
			std::cout << "timeout" << std::endl;
		}
		read_client = wait_clients(read, max_Rsocket);
		write_client = wait_clients(write, max_Wsocket);
		if (read_client !=  -1)
			read_socket(read_client);
		if (write_client !=  -1)
			write_socket(write_client);
	}
}