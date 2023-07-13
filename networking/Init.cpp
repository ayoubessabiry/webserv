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
	client.rqst.is_reading_chunked = false;
	client.body_file_opened = false;
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
		std::string host = clients[i].rqst.headers["Host"];
		size_t size = host.size();
		size_t find_dot = host.find(":");
		std::string port = host.substr(find_dot + 1, host.size());
		for (int j = 0 ; j < server.size(); j++)
		{
			if (server[j]._server.port == port)
			{
				clients[i].configuration = server[j]._server;
				break ;
			}
		}
		FD_CLR(ready_client, &masterRead);
		FD_SET(ready_client, &masterWrite);
		(ready_client > max_Wsocket) ? max_Wsocket = ready_client : max_Wsocket;
	}
}

void	Init::send_rqst(int ready_client){
	int i =0;
	while(i < clients.size() && ready_client != clients[i].socket)
		++i;

	clients[i].desired_location = clients[i].match_location();

	// std::cout << clients[i].desired_location.methods[0];
	// std::cout << clients[i].desired_location.root + clients[i].rqst.uri << std::endl;

	clients[i].get.setfileName(clients[i].desired_location.root + clients[i].rqst.uri);
	
	/////////////////////////////
	std::vector<std::string>	allowedMethods;

	allowedMethods.push_back("GET");

	clients[i].get.setBufferSize(10000000);
	clients[i].get.setAutoIndex(true);
	clients[i].get.setAllowedMethods(allowedMethods);
	clients[i].get.initGetMethod();

	// send_response(clients[i].socket);
	std::string	responseHeader(clients[i].get.getResponseHeaders());
	send(clients[i].socket, responseHeader.c_str(), strlen(responseHeader.c_str()), 0);
	size_t bytes_sent = 0;
	std::string	responseBody(clients[i].get.getResponseBody());
	bytes_sent = send(clients[i].socket, responseBody.c_str(), strlen(responseBody.c_str()), 0);
	/////////////////////////////
	FD_CLR(clients[i].socket, &masterWrite);
	close(clients[i].socket);
	clients.erase(clients.begin()+i);
}

void	Init::write_socket(int ready_client){
	send_rqst(ready_client);
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