# include "../headers/Init.hpp"


void	Init::read_socket(Server& s){
	fd_set	read;
	timeval timeout = {0, 0}; // waiting for 10sec
	int		ready_client;

	read = masterRead;
	if (select(max_Rsocket + 1, &read, 0, 0, NULL) < 0){
		// response with time out;
		std::cout << "timeout" << std::endl;
	}
	ready_client = s.wait_clients(read, max_Rsocket);
	// printf("ready %d\n", ready_client);
	if (ready_client == s._socket)
		s.add_client(ready_client, masterRead, max_Rsocket);
	else  
		s.get_rqst(ready_client, masterWrite, max_Wsocket);
}

void	Init::write_socket(Server& s){
	fd_set	write;
	timeval	timeout = {10, 0}; // waiting for 10sec
	int		ready_client;

	write = masterWrite;
	if (select(max_Rsocket + 1, 0, &write, 0, &timeout) < 0){
		// response with time out;
		std::cout << "timeout" << std::endl;
	}
	ready_client = s.wait_clients(write, max_Wsocket);
	s.send_rqst(ready_client);
}

void	Init::start_listening(){
	while (1){
		for (int i = 0; i < server.size(); ++i){
			read_socket(server[i]); // reading request from ready clients
			if (max_Wsocket)
				write_socket(server[i]); // wirting responsr for ready clients
		}
	}
}