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
	client.rqst.chunk_part = "";
	client.rqst.chunk_saver = "";
	client.rqst.found_next_hexa = false;
	client.rqst.b_size = 0;
	client.rqst.is_reading_chunked = false;
	client.body_file_opened = false;
	client.is_reading_body = false;
	client.rqst.next_hex_saver = "";
	client.rqst.body = "";
	client.request_collector = "";
	client.socket = new_socket;
	client.bytes_sent = 0;
	client.recv_byte = 0;
	client.header = true;

	clients.push_back(client);
	FD_SET(new_socket, &masterRead);
	(new_socket > max_Rsocket) ? max_Rsocket = new_socket : max_Rsocket;
}

void	Init::get_rqst(int ready_client){
	int				i = 0;
	int				recB = 0;

	while (i < clients.size() && ready_client != clients[i].socket)
		++i;
	memset(&clients[i].buffer, 0, MAX_REQUEST_SIZE);
	recB = recv(ready_client, &clients[i].buffer, MAX_REQUEST_SIZE, 0);
	clients[i].recv_byte += recB;
	std::string	buff(clients[i].buffer, recB);
	if(send_request(clients[i], buff)){
		std::cout << "sending request ...\n";
		// move ready_client to send();
		std::cout << "done" << std::endl;
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

void	Init::send_response(int ready_client){
	int i =0;
	while(i < clients.size() && ready_client != clients[i].socket)
		++i;

	clients[i].desired_location = clients[i].match_location();

	////////////////////////////
	if (clients[i].rqst.method == "GET")
	{
		clients[i].get.setfileName(clients[i].desired_location.root + clients[i].rqst.uri);
		clients[i].get.setBufferSize(MAX_REQUEST_SIZE);
		bool	auto_index;
		if (clients[i].desired_location.auto_index == "on")
			auto_index = true;
		if (clients[i].desired_location.auto_index == "off")
			auto_index = false;
		clients[i].get.setAutoIndex(auto_index);
		clients[i].get.setIndexes(clients[i].desired_location.indexes);
		clients[i].get.setAllowedMethods(clients[i].desired_location.methods);
		// clients[i].get.setfileName(clients[i].rqst.file_name);
		clients[i].get.initGetMethod();
		/////////////////////////////
		if (clients[i].header){
			std::string	responseHeader(clients[i].get.getResponseHeaders());
			send(clients[i].socket, responseHeader.c_str(), strlen(responseHeader.c_str()), 0);
			clients[i].header = false;	
		}
		std::string	responseBody = clients[i].get.getResponseBody();
		if (!responseBody.empty()){
			int  s = send(clients[i].socket, responseBody.c_str(), responseBody.size(),  0);
			//std::cout << responseBody;
			clients[i].bytes_sent += s;
			clients[i].get.setBytesSent(clients[i].bytes_sent);
			if (s == -1){
				std::cout << std::strerror(errno) << std::endl;
				exit(1);
			}
		}
		if (responseBody.empty()){
			FD_CLR(clients[i].socket, &masterWrite);
			close(clients[i].socket);
			clients.erase(clients.begin()+i);
		}
	}
	else if (clients[i].rqst.method == "POST")
	{
		clients[i].post.setBufferSize(MAX_REQUEST_SIZE);

		clients[i].post.setAllowedMethods(clients[i].desired_location.methods);
		clients[i].post.setfileName(clients[i].rqst.uri);
		clients[i].post.setUploadDirectory(clients[i].desired_location.upload);
		clients[i].post.setBodyRequestFile(clients[i].rqst.file_name);
		clients[i].post.postProcess();
		/////////////////////////////
		if (clients[i].header){
			std::string	responseHeader(clients[i].post.getResponseHeaders());
			send(clients[i].socket, responseHeader.c_str(), strlen(responseHeader.c_str()), 0);
			clients[i].header = false;	
		}
		std::string	responseBody = clients[i].post.getResponseBody();
		if (!responseBody.empty()){
			int  s = send(clients[i].socket, responseBody.c_str(), responseBody.size(),  0);
			//std::cout << responseBody;
			clients[i].bytes_sent += s;
			clients[i].post.setBytesSent(clients[i].bytes_sent);
			if (s == -1){
				std::cout << std::strerror(errno) << std::endl;
				exit(1);
			}
		}
		if (responseBody.empty()){
			FD_CLR(clients[i].socket, &masterWrite);
			close(clients[i].socket);
			clients.erase(clients.begin()+i);
		}
	}
	else if (clients[i].rqst.method == "DELETE")
	{
		clients[i].delete_method.setBufferSize(MAX_REQUEST_SIZE);

		clients[i].delete_method.setAllowedMethods(clients[i].desired_location.methods);
		clients[i].delete_method.setfileName(clients[i].desired_location.root + clients[i].rqst.uri);
		clients[i].delete_method.deleteProcess();
		/////////////////////////////
		if (clients[i].header){
			std::string	responseHeader(clients[i].delete_method.getResponseHeaders());
			send(clients[i].socket, responseHeader.c_str(), strlen(responseHeader.c_str()), 0);
			clients[i].header = false;	
		}
		std::string	responseBody = clients[i].delete_method.getResponseBody();
		if (!responseBody.empty()){
			int  s = send(clients[i].socket, responseBody.c_str(), responseBody.size(),  0);
			//std::cout << responseBody;
			clients[i].bytes_sent += s;
			clients[i].delete_method.setBytesSent(clients[i].bytes_sent);
			if (s == -1){
				std::cout << std::strerror(errno) << std::endl;
				exit(1);
			}
		}
		if (responseBody.empty()){
			FD_CLR(clients[i].socket, &masterWrite);
			close(clients[i].socket);
			clients.erase(clients.begin()+i);
		}
	}
}

void	Init::write_socket(int ready_client){
	send_response(ready_client);
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