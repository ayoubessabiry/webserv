# include "../headers/Init.hpp"


void	Init::read_socket(Server& s){
	fd_set	read;
	timeval timeout = {60, 0}; // waiting for 10sec
	int		ready_client;

	read = s.masterRead;
	printf("hani\n");
	if (select(s.max_Rsocket + 1, &read, 0, 0, &timeout) < 0){
		// response with time out;
		std::cout << "timeout" << std::endl;
	}
	ready_client = s.wait_clients(read, s.max_Rsocket);
	printf("%d hi from client %d\n", s._socket ,ready_client);
	if (ready_client == s._socket)
		s.add_client(ready_client);
	else  
		s.get_rqst(ready_client);
}

void	Init::write_socket(Server& s){
	fd_set	write;
	timeval	timeout = {0, 0}; // waiting for 10sec
	int		ready_client;

	write = s.masterWrite;
	if (select(s.max_Rsocket + 1, 0, &write, 0, &timeout) < 0){
		// response with time out;
		std::cout << "timeout" << std::endl;
	}
	ready_client = s.wait_clients(write, s.max_Wsocket);
	s.send_rqst(ready_client);
}

void	Init::start_listening(){
	while (1){
		for (int i = 0; i < server.size(); ++i){
			read_socket(server[i]); // reading request from ready clients
			if (server[i].max_Wsocket)
				write_socket(server[i]); // wirting responsr for ready clients
		}
	}
}